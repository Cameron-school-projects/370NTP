#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <ctime>
#include "packets.h"

#define TRUE 1
#define FALSE 0
#define PORT 8080
#define BUFSIZE 512
int main()
{
    time_t t0, t3=time(0);
    int timeDelta; 
    char *data;
    int obj_socket = 0, reader;
    struct sockaddr_in serv_addr;
    packets *ntpPacket;
    char buffer[1024] = {0};
    if ((obj_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error !");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (connect(obj_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed : Can't establish a connection over this socket !");
        return -1;
    }
    t0 = time(0);
    // fill out ntpPacket
    strcpy(ntpPacket->dstaddr,inet_ntoa(serv_addr.sin_addr));
    ntpPacket->xmt=t0;
    ntpPacket->leap=0;
    //srcdst is local ip
    serialize(ntpPacket,data);
    if(write(obj_socket,data,1024)<0){
        perror("error writing to socket\n");
    }
    bzero(data,1024);
    if(read(obj_socket,data,1024)<0){
        perror("error reading\n");
    }
    deSerialize(data,ntpPacket);
    t3=time(0);
    timeDelta=((ntpPacket->servrec-t0)+(ntpPacket->org-t3))/2;
    printf("time delta is %d\n",timeDelta);


}