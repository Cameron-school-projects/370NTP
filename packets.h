#include <string>
class packets
{
public:
    char dstaddr[512]; /* destination (remote) address, needs to be 512 long */
    char srcaddr[512]; /* source (local) address, needs to be 512 long */
    char leap;         /* leap indicator */
    int org;           /* origin timestamp (t2)*/
    int servrec;       /* receive timestamp (t1)*/
    int xmt;           /* transmit timestamp (t0)*/
    int ref;           /*t3 client recive timestamp*/
};
void serialize(packets *packetToserialzie,  char *data)
{
    // turn object into char* that is spaceS delimited
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
    char message[1024];
    char *p;
    // I wanted to create an iterator so i could just loop through my class and
    strcpy(message, data);
    p = strtok(message, " ");
    strcpy(NtpPacket->dstaddr, p);
    p = strtok(0, " \t");
    strcpy(NtpPacket->srcaddr, p);
    p = strtok(0, " \t");
    NtpPacket->leap = atoi(p);
    p = strtok(0, " \t");
    NtpPacket->org = atoi(p);
    p = strtok(0, " \t");
    NtpPacket->servrec = atoi(p);
    p = strtok(0, " \t");
    NtpPacket->xmt = atoi(p);
    p = strtok(0, " \t");
    NtpPacket->ref = atoi(p);
}
