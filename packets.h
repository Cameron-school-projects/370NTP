#include <string>
class packets
{
public:
    char dstaddr[50]; /* destination (remote) address, needs to be at least 46 long for ip addresses*/
    char srcaddr[50]; /* source (local) address, needs to be at least 46 long for ip addresses*/
    char leap;         /* leap indicator */
    uint64_t org;           /* origin timestamp (t2)*/
    uint64_t servrec;       /* receive timestamp (t1)*/
    uint64_t xmt;           /* transmit timestamp (t0)*/
    uint64_t ref;           /*t3 client recive timestamp*/
};
void serialize(packets *packetToserialzie,  char *data)
{
    // turn object into char* that is spaceS delimited
    // if I had more time/this was for a real enviroment, i would not use a string and just convert the class directly to a byte array
    // but for this case preformance is not a huge deal, and this is easier to implement
    std::string test;
    test = packetToserialzie->dstaddr;
    test.append(" ");
    test.append(packetToserialzie->srcaddr);
    test.append(" ");
    test.append(std::to_string(packetToserialzie->leap));
    test.append(" ");
    test.append(std::to_string(packetToserialzie->org));
    test.append(" ");
    test.append(std::to_string(packetToserialzie->servrec));
    test.append(" ");
    test.append(std::to_string(packetToserialzie->servrec));
    test.append(" ");
    test.append(std::to_string(packetToserialzie->ref));
    const char* help = test.c_str();
    strcpy(data,help);
}
void deSerialize( char *data, packets *NtpPacket)
{
    //tokenize the data and turn it back into the class
    char message[1024];
    char *p;
    strcpy(message, data);
    p = strtok(message, " ");
    strcpy(NtpPacket->dstaddr, p);
    p = strtok(0, " \t");
    strcpy(NtpPacket->srcaddr, p);
    p = strtok(0, " \t");
    NtpPacket->leap = std::stoull(p);
    p = strtok(0, " \t");
    NtpPacket->org = std::stoull(p);
    p = strtok(0, " \t");
    NtpPacket->servrec = std::stoull(p);
    p = strtok(0, " \t");
    NtpPacket->xmt = std::stoull(p);
    p = strtok(0, " \t");
    NtpPacket->ref = std::stoull(p);
}
