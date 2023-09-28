#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>
#include <string>
#include <chrono>
#include <cstdint>
#include <sys/time.h> 
#include "packets.h"
//used so we an get time in milliseconds 
using namespace std::chrono;
#define TRUE 1
#define FALSE 0
#define PORT 8080
int main(int argc, char *argv[])
{
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[30];
    int max_clients = 30;
    int activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;
    int64_t t1, t2;

    char buffer[1025]={0}; 

    fd_set readfds;

    // class of ntp info
    packets *timeInfo=new packets();

    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    // create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set master socket to allow multiple connections ,
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    // try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while (TRUE)
    {
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        // wait for an activity on one of the sockets , timeout is NULL 
        // so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            t1=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            if (read(new_socket, buffer, 1024) ==0)
            {
                t2=duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                //located in packets.h
                deSerialize(buffer, timeInfo);
                strcpy(timeInfo->dstaddr,inet_ntoa(address.sin_addr));
                timeInfo->servrec = t1;
                timeInfo->org = t2;
                serialize(timeInfo, buffer);
                printf("writing\n");
                send(new_socket, buffer, sizeof(buffer), 0);
            }
            // add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }

        // else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if ((valread = read(sd, buffer, 1024)) == 0)
            {
                getpeername(sd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);
                printf("Host disconnected , ip %s , port %d \n",
                       inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                
                close(sd);
                client_socket[i] = 0;
                break;
            }

        }
    }

    return 0;
}
