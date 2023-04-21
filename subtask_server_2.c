/* File:   Sub_Server_1.c */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/**************************************************** Sub Server -1 *****************************************************/

// Function sending message to the main server.
int send_msg(char msg[])
{

    int socket_desc;                                       // defining a socket as int
    struct sockaddr_in server_addr;                        // adding the address information in a struct
    char server_message[2000], client_message[2000];       // defining the server message and client message
    
    //Cleaning the Buffers
    memset(server_message,'\0',sizeof(server_message));    // memory reset - server message
    memset(client_message,'\0',sizeof(client_message));    // memory reset - client message
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);         // Creating a socket that returns a socket descripton
    if(socket_desc < 0)
    {
        printf("Could Not Create Socket. Error!!!!!\n");
        return -1;
    }
    printf("Socket Created\n");
    
    // Specifying the IP and Port of the server to connect
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // connecting to the server accept() using connect() from client side
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Connection Failed. Error!!!!!");
        return -1;
    }
    printf("Connected\n");
    
    //Send the message to my information to Main Server
    strcpy(server_message,msg);
    printf("%s",server_message);
    if(send(socket_desc, server_message, strlen(server_message),0) < 0)   // server message to socket description
    {
            printf("Send Failed. Error!!!!\n");
            return -1;
    }
    memset(server_message,'\0',sizeof(server_message));                // memory reset - server message
    memset(client_message,'\0',sizeof(client_message));                // memory reset - client message
    close(socket_desc);     //Closing the Socket
    return 0;
}

/**********************************************************************************************************************/

// creating the struct for the client information.
struct client_info           /* client info struture */
{
    int socket;              /* socket */
    int port;                /* port */
    char ip[10];             /* ip */
    struct client_info* loc;
};

/**********************************************************************************************************************/

// Client thread function
void *client_thread (void* c_info)
{
    printf("starting client_thread\n");
    char server_message[2000], client_message[2000],port_buffer[7];
    struct client_info *info=(struct client_info*) c_info;
    int client_sock=info->socket;
    printf("\nSocket: %i",client_sock);
    printf("\nPort: %i",info->port);
    printf("\nIP: %s",info->ip);
    printf("\nmalloc: %s ",info->loc);
    
    // Defining the test types to show on the client server
    char test_type[]="Enter your test type:\ns2_task1\n";
    memset(server_message,'\0',sizeof(server_message));    //Cleaning the Buffers
    memset(client_message,'\0',sizeof(client_message));
    strcpy(server_message,test_type);
    if (send(client_sock, server_message, strlen(server_message),0)<0)  // server message to client socket
    {
        printf("client_thread Send Failed. Error!!!!!\n");
        return 0;
    }
    
    //Receive the message from the client
        if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
        {
            printf("client_thread Receive Failed. Error!!!!!\n");
            return 0;
        }
        printf("client_thread Client Sock: %i\n",client_sock);
        printf("client_thread Client Message: %s\n",client_message);
        memset(server_message,'\0',sizeof(server_message));             //Cleaning the Buffers
        memset(port_buffer,'\0',sizeof(port_buffer));
        
        // checking for the task input from the client
        if(strcmp(client_message,"s2_task1")==0)
        {
            sprintf(port_buffer, "%d", info->port);            // sends formatted output
            strcpy(server_message,info->ip);                   // String copy
            strcat(server_message,",");                        // string concatenation
            strcat(server_message,port_buffer);
            strcat(server_message,",pathplanning,");
            strcat(server_message,client_message);
            strcat(server_message," \n");
            strcat(server_message, "***************************************************************************\n\n");
            strcat(server_message, "The robot started to perform the path planning task from the sub server-2\n\n");
            strcat(server_message, "***************************************************************************\n\n");
        }
        else
            strcpy(server_message, "Invalid Input!!!");
        
        //Send the message back to client
        if (send(client_sock, server_message, strlen(server_message),0)<0)
        {
            printf("client_thread Send Failed. Error!!!!!\n");
            return 0;
        }
        if(send_msg(server_message)==-1)
            return -1;
    close(client_sock);              // closing client socket
    free(info->loc);                 // free the information
    pthread_exit(NULL);              // exiting the thread
}

/**********************************************************************************************************************/

// Function to set the connection for client.
int client_con(char my_ip[], char my_port[])
{   
    int port=atoi(my_port);                             // defining port ab int by using atoi.
    int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;    // adding the address information in a struct
        pthread_t thread;                               // initiating a thread
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);  // Creating a socket that returns a socket descripton
        if(socket_desc < 0)
        {
            printf("Could Not Create Socket. Error!!!!!\n");
            return -1;
        }
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(my_ip);
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        {
            printf("Bind Failed. Error!!!!!\n");
            return -1;
        }        
        printf("Bind Done\n");
        
        //Put the socket into Listening State
        if(listen(socket_desc, 1) < 0)
        {
            printf("Listening Failed. Error!!!!!\n");
            return -1;
        }
        printf("Listening for Incoming Connections.....\n");
        while(1)    // infinite loop
        {
            //Accept the incoming Connections
            client_size = sizeof(client_addr);
            client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
            if (client_sock < 0)
            {
                printf("Accept Failed. Error!!!!!!\n");
                return -1;
            }
            else
            {
                struct client_info *info;
                info=malloc(sizeof(struct client_info));
                strcpy(info->ip,inet_ntoa(client_addr.sin_addr));
                info->port=ntohs(client_addr.sin_port);
                info->socket=client_sock;
                info->loc=info;
                int ret = pthread_create(&thread, NULL, client_thread, (void *)info);
                if (ret!=0)
                {
                    printf("Error In Creating Thread\n");
                }
            }
            //printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        }
        close(socket_desc);    // Closing the Socket
        pthread_exit(NULL);    // Terminates the parent thread    
    return 0;
}

/***************************************************** MAIN *****************************************************************/

int main(int argc, char** argv) {
    char msg[100];
    char my_test_name[]="pathplanning";     // defining the name of the testing
    char my_ip[]="127.0.0.1";
    char my_port[]="2020";
    memset(msg,'\0',sizeof(msg));           // cleaning the buffer 
    strcpy(msg,my_test_name);               // string copy
    strcat(msg,",");                        // string concatenate
    strcat(msg,my_ip);
    strcat(msg,",");
    strcat(msg,my_port); 
    if(send_msg(msg)==-1)
        return -1;
    client_con(my_ip,my_port);              // client connection
    return (EXIT_SUCCESS);
}
