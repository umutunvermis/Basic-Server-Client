#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char MESSAGE[] = "Hello from Simple Server!\n";
char MESSAGE1[] = "IP address:           ";
char MESSAGE2[] = "\nPort number:          ";
char MESSAGE3[] = "\nServer socket number: ";
char MESSAGE4[] = "\nYour socket number:   ";
char MESSAGE5[] = "\nGoodbye!\n";


int main(int argc, char *argv[]){

    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;


    if (2 != argc){
        fprintf (stderr, "Usage %s <port \n>", argv[0]);
        exit(1);
    }

    // SOCKET

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (simpleSocket == -1){
        fprintf(stderr, "Could not create a socket! \n");
        exit(1);
    }
    else
        fprintf(stderr, "Socket created!\n");
        printf("Socket no: %d\n", simpleSocket);   
    
    // BIND    

    simplePort = atoi(argv[1]);
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);

    printf("Port no: %d\n", simplePort);


    returnStatus = bind(simpleSocket,(struct sockaddr *)&simpleServer, sizeof(simpleServer));

    if (returnStatus == 0)
        fprintf(stderr, "Bind completed\n");
    else{
        fprintf(stderr, "Bind error\n");
        close(simpleSocket);
        exit(1);
    }

    // LISTEN

    returnStatus = listen(simpleSocket, 5);

    if (returnStatus == -1){
        fprintf(stderr, "Socket error\n");
        close(simpleSocket);
        exit(1);
    }
    
    // ACCEPT

    while(1){

        struct sockaddr_in clientName = { 0 };
        int simpleClient = 0;
        int clientNameLength = sizeof(clientName);

        int simpleChildSocket = accept(simpleSocket, (struct sockaddr *)&clientName, &clientNameLength);

        if (simpleChildSocket == -1){
            fprintf(stderr, "Accept Error");
            close(simpleSocket);
            exit(1);
        }

        char ip_client[BUFSIZ];
        sprintf(ip_client, "%d", (int)clientName.sin_addr.s_addr);
        //write(simpleChildSocket, port, strlen(port));

        char port[BUFSIZ];
        sprintf(port, "%d", simplePort);
        //write(simpleChildSocket, port, strlen(port));

        char socket[BUFSIZ];
        sprintf(socket, "%d", simpleSocket);
        //write(simpleChildSocket, socket, strlen(socket));

        char childSocket[BUFSIZ];
        sprintf(childSocket, "%d", simpleChildSocket);
        //write(simpleChildSocket, childSocket, strlen(childSocket));

        write(simpleChildSocket, MESSAGE, strlen(MESSAGE));

        write(simpleChildSocket, MESSAGE1, strlen(MESSAGE1));
        write(simpleChildSocket, ip_client, strlen(ip_client));


        write(simpleChildSocket, MESSAGE2, strlen(MESSAGE2));
        write(simpleChildSocket, port, strlen(port));

        write(simpleChildSocket, MESSAGE3, strlen(MESSAGE3));
        write(simpleChildSocket, socket, strlen(socket));

        write(simpleChildSocket, MESSAGE4, strlen(MESSAGE4));
        write(simpleChildSocket, childSocket, strlen(childSocket));
        
        write(simpleChildSocket, MESSAGE5, strlen(MESSAGE5));
        close(simpleChildSocket);

    }

}