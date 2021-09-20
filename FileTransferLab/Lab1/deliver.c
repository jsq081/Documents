#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

int main(int argc, char** argv){

    if (argc != 3) {
        printf("Error: wrong agument format.\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);

    //create the socket
    int socketfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    if (socketfd < 0){
        printf("Error: unsuccessful creation of socket");
        exit (EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memset (serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    inet_pton(AF_INET,argv[1],&(serv_addr.sin_addr));

    char input[100]; //= {'0'};
    char filename[100]; //= {'0'};

    //get user input 
    printf("Input a message of the following form\n\t ftp <file name>\n");

    scanf("%s %s", input, filename);

    //printf("1111\n");

    if(strcmp(input, "ftp") != 0){
        printf("Error: Need to type ftp.\n");
        exit(EXIT_FAILURE);
    }

    // check if the file exists
    if( access( filename, F_OK ) == -1) {
        printf("File does not exists.\n");
        exit(EXIT_FAILURE);
    } 

    socklen_t serv_addr_len = sizeof(serv_addr);

    //check if msg send succsessful
    if (sendto( socketfd, "ftp", sizeof("ftp") , 0 , (struct sockaddr *) &serv_addr, serv_addr_len) == -1) {
        printf("Failed send msg\n");
        exit(EXIT_FAILURE);
    }
    

    //recv msg 
    char buf[100];
    if (recvfrom(socketfd, (char*) buf, sizeof(buf), 0, (struct sockaddr*) &serv_addr, &serv_addr_len) == -1) {
        printf("Error: Unseccessful recieve\n");
        exit(EXIT_FAILURE);
    }

    if(strcmp(buf, "yes")==0){
        printf("A file transfer can start\n");
    }else{
        exit(EXIT_FAILURE);
    }

    return 0;
}
