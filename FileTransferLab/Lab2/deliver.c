#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>     
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>

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

    //converts an Internet address in its standard text format 
    //into its numeric binary form
    inet_pton(AF_INET,argv[1],&(serv_addr.sin_addr));

    char input[100]; 
    //char filename[100]; 
    char filename[100] = {'\0'};
    //get user input 
    printf("Input a message of the following form\n\t ftp <file name>\n");

    scanf("%s %s", input,filename);


    /*no input for filename case
    if (filename[0] == '\0') {
      printf("Error: Need enter filename\n");
      return 1;
    }*/


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

    //send msg
    clock_t start,end; //set up time vars

    start = clock();

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

    end = clock();
    printf("RTT=%f us.\n",((double)(end-start)*1000000/CLOCKS_PER_SEC));
    printf("kakakak");

    if(strcmp(buf, "yes")==0){
        printf("A file transfer can start\n");
    }else{
        exit(EXIT_FAILURE);
    }

    close(socketfd);

    
    return 0;
}
