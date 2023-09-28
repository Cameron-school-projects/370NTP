/*
Opens a socket on Port 8080, and attempts to connect to 10.0.0.2
and attempts tosend a collection of ntp packets
which are used to determine the network connection time delta
This program was testing using mininet on an Ububntu box
*/
//using specifically string.h to get access to bzero()
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <chrono>
#include <iostream>
#include "packets.h"
//used to easily access timestamp functions
using namespace std::chrono;
#define PORT 8080
#define CONNECT_ERR 0
void setUpConnection(sockaddr_in &serverInfo);
int64_t connectToServer(int socketLocation, sockaddr_in &serverInfo,packets *ntpPacket);
void writeToServer(int &connectionSocket,packets* ntpPacket,char* serverResponse);
char* readFromServer(int &connectionSocket,packets* ntpPacket);
void calculateDelay(packets* ntpPacket,int64_t connectionStartTime);
int main()
{
    int64_t connectionStartTime;
    char *serverResponseData = new char[1024];
    int obj_socket = 0;
    //struct holding connection information of C++ server
    struct sockaddr_in serv_addr;
    //defined in packets.h file, contains fields for addresses, and network timestamp fields
    packets *ntpPacket = new packets();
    //create socket, and  test for connection error
    if ((obj_socket = socket(AF_INET, SOCK_STREAM, 0)) < CONNECT_ERR)
    {
        printf("Socket creation error!");
        return -1;
    }
    try{
    connectionStartTime = connectToServer(obj_socket,serv_addr,ntpPacket);
    writeToServer(obj_socket,ntpPacket,serverResponseData);
    serverResponseData = readFromServer(obj_socket,ntpPacket);
    calculateDelay(ntpPacket,connectionStartTime);
    }
    catch(char* exceptionReason){
        std::cout<<exceptionReason<<std::endl;
        return -1;
    }

}
/*
Sets up the values within the sockaddr_in struct to successfully connect to the server
params: serverInfo - sockaddr_in struct to modify (passed by reference)
*/
void setUpConnection(sockaddr_in &serverInfo){
    serverInfo.sin_family = AF_INET;
    //convert port to network byte order
    serverInfo.sin_port = htons(PORT);
    //get server address in binary network format, which the connect function will recognize
    inet_pton(AF_INET, "10.0.0.2", &serverInfo.sin_addr);
}
/*

*/
int64_t connectToServer(int socketLocation, sockaddr_in &serverInfo,packets *ntpPacket){
    int64_t connectionStartTime=0;
    if (connect(socketLocation, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
     {
        throw("Connection Failed : Can't establish a connection over this socket!");
     }
    //set up start time for network delay measurement
    connectionStartTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    // fill out ntpPacket
    strcpy(ntpPacket->dstaddr, inet_ntoa(serverInfo.sin_addr));
    //set up packet values, used later in the calculateDelay function
    ntpPacket->xmt = connectionStartTime;
    ntpPacket->leap = 0;
    return connectionStartTime;
}
/*

*/
void writeToServer(int &connectionSocket,packets* ntpPacket,char* serverResponse){
 serialize(ntpPacket, serverResponse);
    if (write(connectionSocket, serverResponse, 1024) < 0)
    {
        throw("error writing to socket\n");
    }
}
/*

*/
char* readFromServer(int &connectionSocket,packets* ntpPacket){
    char* dataFromServer;
    //make sure data buffer is zero, to avoid data corruption
    bzero(dataFromServer, 1024);
    if (read(connectionSocket, dataFromServer, 1024) < 0)
    {
        throw("error reading");
    }
    deSerialize(dataFromServer, ntpPacket);
    //final timestamp, after read and write are executed
    ntpPacket->ref= duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return dataFromServer;
}
/*

*/
void calculateDelay(packets* ntpPacket,int64_t connectionStartTime){
        
    int64_t timeDelta;
    //calculate total delay
    timeDelta = ((ntpPacket->servrec - connectionStartTime) + (ntpPacket->org - ntpPacket->ref));
    timeDelta/=2;
    std::cout<<"Time delta is: "<<timeDelta<<std::endl;
}