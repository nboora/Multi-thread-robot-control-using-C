/* File:   Main_Server.c */

#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**************************************************** Main Server *****************************************************/

// function for checking the format of the sub server message
int format_check(char str[])
{
    int i=0;
    int count=0;
    for(i=0;i<strlen(str);i++)
    {
        if(str[i]==',')
            count++;
    }
    return count;                                           // count the comma
}

/**********************************************************************************************************************/

// function pointer for the sub server thread function
void *ssthread_func (int *client_socket)
{
    printf("starting the sub server thread func\n");
    char server_message[2000], client_message[2000];        // defining the server message and client message

    // Cleaning the Buffers -> setting 0
    memset(server_message,'\0',sizeof(server_message));     // memory reset - server message
    memset(client_message,'\0',sizeof(client_message));     // memory reset - client message

    //Receive the message from the client and checking the format
    if (recv(client_socket, client_message, sizeof(client_message),0) < 0)    // sanity check
    {
        printf("Recieve Failed for ssthread_func. Error!!!!!\n");
        return 0;
    }
    printf("\nssthread_func Client Socket: %i\n",client_socket);   // print the client socket
    printf("\nssthread_func Client Message: %s\n",client_message); // print the client message
    if(format_check(client_message)==2)                            // format check
    {
        FILE *sub_server_info_File;                                // creating the file
        sub_server_info_File = fopen("sub_server_info.txt", "a");  // append to txt file
        char entry[200];
        strcpy(entry, client_message);                             // appending the client mesaage logging
        strcat(entry, "\n");                                       // concatinating the entry string
        printf("Sub Server info Received: %s",client_message);
        fputs(entry, sub_server_info_File);
        fclose(sub_server_info_File);
    }
    else if(format_check(client_message)==3)
    {
        FILE *clients_records_File;
        clients_records_File = fopen("clients_records.txt", "a");
        char entry[200];
        strcpy(entry, client_message);
        strcat(entry, "\n");
        printf("Sub Server Message Recieved\nClient Record: %s",client_message);
        fputs(entry, clients_records_File); // file  write - enrty
        fclose(clients_records_File); 
    }
    else
        printf("\nSub_server with socket %i msg format incorrect!!!\n",client_socket);
    memset(server_message,'\0',sizeof(server_message));          // memory reset - server message
    memset(client_message,'\0',sizeof(client_message));          // memory reset - client message

    //Closing the Socket
    close(client_socket);
    pthread_exit(NULL);       
}

/**********************************************************************************************************************/

// function to get the sub server information
int ss_info(char test_name[],char* info)
{
    memset(info,'\0',sizeof(info));                      // memory reset - server message
    FILE *fp;
    char str[100];
    fp = fopen("sub_server_info.txt", "r");              // read the txt file
    if (fp == NULL){
        printf("Could not open file!!\n");
        return 1;
    }
    int flag=0;                                          
    while (fgets(str, sizeof(str), fp) != NULL)
    {
        int i=0;
        flag=1;
        while(i<strlen(test_name))
        {
            if(str[i]!=test_name[i])
            {
                flag=0;
                break;
            }
            i++;
        }
        if(flag==1)
        {    
            strcpy(info,str);
            char *newline,*carriage_return;
            newline = strchr(info,'\n');
            if(newline != NULL)
            *newline = '\0';
            carriage_return = strchr(info,'\r');
            if(carriage_return != NULL)
            *carriage_return = '\0';
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

/**********************************************************************************************************************/

// Thread function for the client.
void *clientthread_func (int *client_socket)
{
    printf("starting clientthread_func\n");
    char server_message[2000], client_message[2000];
    char str[]="Please Enter your Test option\nkinematics\npathplanning\npick&place\n\n";

    // Cleaning the Buffers
    memset(server_message,'\0',sizeof(server_message));                     // memory reset - server message
    memset(client_message,'\0',sizeof(client_message));                     // memory reset - client message
    strcpy(server_message,str);
    if (send(client_socket, server_message, strlen(server_message),0)<0)    // send to client
    {
        printf("clientthread_func Send Failed. Error!!!!!\n");
        return 0;
    }
        // Receive the message from the client
        if (recv(client_socket, client_message, sizeof(client_message),0) < 0)  // receive the message from client
        {
            printf("clientthread_func Receive Failed. Error!!!!!\n");
            return 0;
        }
        printf("clientthread_func Client Socket: %i\n",client_socket);
        printf("clientthread_func Client Message: %s\n",client_message);
        if(ss_info(client_message,server_message)==1)
            strcpy(server_message, "Invalid Input!!!");

        // Send the message back to client
        if (send(client_socket, server_message, strlen(server_message),0)<0)
        {
            printf("clientthread_func Send Failed. Error!!!!!\n");
            return 0;
        }
        memset(server_message,'\0',sizeof(server_message));          // memory reset - server message
        memset(client_message,'\0',sizeof(client_message));          // memory reset - client message

    //Closing the Socket
    close(client_socket);    
    pthread_exit(NULL);       
}

/**********************************************************************************************************************/

// Function for sub server
void *ss_func (int *subsocket_desc)
{
    printf("starting the sub server ss_func\n");
    int client_socket, client_size; 
    struct sockaddr_in client_addr;
    char server_message[2000], client_message[2000];
    pthread_t thread4;                                       // defining a new thread variable
    
    //Cleaning the Buffers
    memset(server_message,'\0',sizeof(server_message));      // memory reset - server message
    memset(client_message,'\0',sizeof(client_message));      // memory reset - client message
    
    while(1){                                                // infinite loop
        //Accept the incoming Connections
        client_size = sizeof(client_addr);
        client_socket = accept(subsocket_desc, (struct sockaddr*)&client_addr, &client_size);   //establish the connection 
        if (client_socket < 0)
        {
            printf("Accept Failed. Error!!!!!!\n");
            return 0;
        }
        else
        {
            int ret = pthread_create(&thread4, NULL, ssthread_func, (int*)client_socket);       // creating a thread
            if (ret!=0)
            {
                printf("Error In Creating Thread\n");
            }
        }
        printf("1-Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    }
    close(subsocket_desc);
    pthread_exit(NULL);    
}

/**********************************************************************************************************************/

// The client function
void *client_func (int *socket_desc)
{
    printf("starting client_func\n");
    int client_socket, client_size; 
    struct sockaddr_in client_addr;
    char server_message[2000], client_message[2000];
    pthread_t thread3;
    
    //Cleaning the Buffers
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    while(1){
        //Accept the incoming Connections
        client_size = sizeof(client_addr);
        client_socket = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);  // establish the connection to client
        if (client_socket < 0) 
        {
            printf("Accept Failed. Error!!!!!!\n");
            return 0;
        }
        else
        {
            int ret = pthread_create(&thread3, NULL, clientthread_func, (int*)client_socket); //establish tthe connection
            if (ret!=0)
            {
                printf("Error In Creating Thread\n");
            }
        }
        printf("1-Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    }
    close(socket_desc);
    pthread_exit(NULL);   
}

/**********************************************************************************************************************/

// Main 
int main(int argc, char** argv) {
    int socket_desc, subsocket_desc, client_socket, client_size; 
    struct sockaddr_in server_addr, sub_server_addr, client_addr;
    pthread_t thread1,thread2;

    // ******* crete - variable assign, bind the socket, listen, accept, send ****** //

    // Creating Socket
    // Create a Transmission Control Protocol socket that returns a socket descriptor. 
    // This socket descriptor would be used to communicate with the client.
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    subsocket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc < 0)
    {
         printf("Could Not Create Socket. Error!!!!!\n");
        return -1;
    }
    printf("Socket Created\n");
    if(subsocket_desc < 0)
    {
        printf("Could Not Create Sub Socket. Error!!!!!\n");
        return -1;
    }
    printf("Sub Socket Created\n");

    // ********************************************************************** //

    
    // initialize the server address by providing the required IP and port number. 
    // The server keeps all the address information for both the server and the client in the sockaddr_in struct.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sub_server_addr.sin_family = AF_INET;
    sub_server_addr.sin_port = htons(2001);
    sub_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Binding IP and Port to socket.
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
    {
        printf("Bind Failed. Error!!!!!\n");
        return -1;
    }        
    printf("Bind Done\n");
    
    if(bind(subsocket_desc, (struct sockaddr*)&sub_server_addr, sizeof(sub_server_addr))<0)
    {
        printf("Sub Bind Failed. Error!!!!!\n");
        return -1;
    }        
    printf("Sub Bind Done\n");

    // ********************************************************************** //
    
    // socket in Listening State for incoming message from client.
    if(listen(socket_desc, 1) < 0)
    {
        printf("Listening Failed. Error!!!!!\n");
        return -1;
    }
    printf("Listening for Incoming Connections.....\n");
    
    if(listen(subsocket_desc, 1) < 0)
    {
        printf("Sub Listening Failed. Error!!!!!\n");
        return -1;
    }
    printf("Sub Listening for Incoming Connections.....\n");
    
    // ********************************************************************** //
    
    // creating the thread for subsocket and socket
    int ret1 = pthread_create(&thread1, NULL, ss_func, (int*)subsocket_desc);
    if (ret1!=0)
    {
        printf("Error In Creating Thread1\n");
    }
    int ret2 = pthread_create(&thread2, NULL, client_func, (int*)socket_desc);
    if (ret2!=0)
    {
        printf("Error In Creating Thread2\n");
    }
    
    pthread_join(thread1,NULL);    // joining the thread
    pthread_join(thread2,NULL);
    close(socket_desc);            // closing the Socket
    pthread_exit(NULL);            // terminates the parent thread
    return (EXIT_SUCCESS);
}
