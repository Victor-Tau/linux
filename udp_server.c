#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int port=8000;

int main( ) {

    int sin_len;
    char message[256];

    int socket_fd;
    struct sockaddr_in my_addr;
    printf("Waiting for data form sender \n");

    bzero(&my_addr,sizeof(my_addr));
//    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    my_addr.sin_port=htons(port);
    sin_len=sizeof(my_addr);
    socket_fd=socket(AF_INET,SOCK_DGRAM,0);
    bind(socket_fd,(struct sockaddr *)&my_addr,sizeof(my_addr));

    while(1)
    {
        recvfrom(socket_fd,message,sizeof(message),0,(struct sockaddr *)&my_addr,&sin_len);
        printf("Response from server:%s\n",message);
        if(strncmp(message,"stop",4) == 0)//接受到的消息为 “stop”
        {

            printf("Sender has stopped\n");
            break;
        }
    }

    close(socket_fd);
    exit(0);

    return (0);
}
