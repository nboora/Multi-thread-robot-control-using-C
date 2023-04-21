
/* File:   client.c */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <unistd.h>

/**************************************************** Client Server *****************************************************/

int main(int argc, char** argv) {
        int socket_desc;                                      // defining a socket as int
        struct sockaddr_in server_addr;                       // adding the address information in a struct
        char server_message[2000], client_message[2000];      // defining the server message and client message
        
        // Cleaning the Buffers
        memset(server_message,'\0',sizeof(server_message));   // memory reset - server message
        memset(client_message,'\0',sizeof(client_message));   // memory reset - client message
        
        // creating a TCP (Transmission Control Protocol) socket. 
        // The socket would be used to connect to the server and start communication.
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);        // Creating a socket that returns a socket descripton
        if(socket_desc < 0)
        {
                printf("\nCould Not Create Socket. Error!!!!!\n");
                return -1;
        } 
        printf("\nSocket Created\n");
        
        /***************************************************************************************************************/

        // specifying the required IP and Port of the server to connect.
        printf("Hello");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        /***************************************************************************************************************/

        // connection request using connect() from client side to the server to accept()
        printf("Hello");
        if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
                printf("Hello");
                printf("\nConnection Failed. Error!!!!!");
                return -1;
        }
        printf("\nConnected\n");
        
        /***************************************************************************************************************/

        // Receive the message back from the server
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("\nReceive Failed. Error!!!!!\n");
                return -1;
        }
        printf("\nServer Message: \n%s\n",server_message);
        
        //Get Input from the User
        printf("\nEnter Message: ");
        gets(client_message);
        
        //Send the message to Server
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("\nSend Failed. Error!!!!\n");
                return -1;
        }
        
        //Receive the message back from the server
        memset(server_message,'\0',sizeof(server_message));
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("\nReceive Failed. Error!!!!!\n");
                return -1;
        }
        printf("\nServer info Message: %s\n",server_message);
        char port[10],ip[20],name[20];
        int i=0;
        int j=0;

        // adding different parts of the server message to arrays
        for(i=0;server_message[i]!=',';i++)
        {
            name[i]=server_message[i];
        }
        i++;
        for(j=0;server_message[i]!=',';i++,j++)
        {
            ip[j]=server_message[i];
        }
        i++;
        for(j=0;server_message[i]!='\0';i++,j++)
        {
            port[j]=server_message[i];
        }
        // Removing any extra trailing character in the IP address
      	// ip[strlen(ip)-1] = '\0'; 
        int sub_server_port=atoi(port);                        // defining port as int by using atoi.
        printf("\nName:%s\n",name);
        printf("IP:%s\n",ip);
        printf("port: %i\n",sub_server_port);
        memset(server_message,'\0',sizeof(server_message));    // memory reset - server message
        memset(client_message,'\0',sizeof(client_message));    // memory reset - client message
        close(socket_desc);                                    //Closing the Socket
        socket_desc = -1;                                      //Creating Socket
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_desc < 0)
        {
                printf("\nCould Not Create Socket. Error!!!!!\n");
                return -1;
        }
        printf("\nSocket Created\n");
        
        /***************************************************************************************************************/

        //Specifying the IP and Port of the server to connect
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(sub_server_port);
        server_addr.sin_addr.s_addr = inet_addr(ip);
        
        /***************************************************************************************************************/

        // connecting to the server accept() using connect() from client side
        if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
                printf("\nConnection Failed with Sub Server. Error!!!!!");
                return -1;
        }
        printf("\nConnected to Sub Server\n");
        memset(server_message,'\0',sizeof(server_message));       //Cleaning the Buffers
        memset(client_message,'\0',sizeof(client_message));
        
        //Receive the message back from the server
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("\nReceive Failed. Error!!!!!\n");
                return -1;
        }
        printf("\nSub Server Message: \n%s\n",server_message);
        
        //Get Input from the User
        printf("\nEnter Message: ");
        gets(client_message);
        
        //Send the message to Server
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("\nSub Server Send Failed. Error!!!!\n");
                return -1;
        }
        
        //Receive the message back from the server
        memset(server_message,'\0',sizeof(server_message));
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("\nSub Server Receive Failed. Error!!!!!\n");
                return -1;
        }
        printf("\nSub Server Message: %s\n",server_message);
        close(socket_desc);             // closing socket
    return (EXIT_SUCCESS);
}

