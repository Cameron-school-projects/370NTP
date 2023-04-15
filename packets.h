struct packets
{
    char dstaddr[512]; /* destination (remote) address, needs to be 512 long */
    char srcaddr[512]; /* source (local) address, needs to be 512 long */
    char leap;      /* leap indicator */
    int org;     /* origin timestamp (t2)*/
    int servrec;     /* receive timestamp (t1)*/
    int xmt;     /* transmit timestamp (t0)*/
    int ref; /*t3 client recive timestamp*/
};
void serialize(packets *packetToserialzie, char *data)
{
    int BUFSIZE=512;
    int *q = (int *)data;
    char *p = (char *)q;
    int i = 0;
    while (i < BUFSIZE)
    {
        *p = packetToserialzie->dstaddr[i];
        p++;
        i++;
    }
    p++;
    i = 0;
    while (i < BUFSIZE)
    {
        *p = packetToserialzie->dstaddr[i];
        p++;
        i++;
    }
    p++;
    q = (int *)p;
    *q = packetToserialzie->leap;
    q++;
    *q = packetToserialzie->org;
    q++;
    *q = packetToserialzie->servrec;
    q++;
    *q = packetToserialzie->xmt;
    q++;
    *q = packetToserialzie->ref;
    q++;

    char *p = (char *)q;
}
void deSerialize(char *data, packets *NtpPacket)
{
    int BUFSIZE=512;
   char* p=data;
    int i=0;
    while(i<BUFSIZE){
        NtpPacket->dstaddr[i]=*p;
        p++;
        i++;
    }
    p++;
    i=0;
    while(i<BUFSIZE){
        NtpPacket->srcaddr[i]=*p;
        p++;
        i++;
    }
    p++;
    int *q = (int *)data;
    NtpPacket->leap = *q;
    q++;
    NtpPacket->org = *q;
    q++;
    NtpPacket->servrec = *q;
    q++;
    NtpPacket->xmt = *q;
    q++;
    NtpPacket->ref = *q;
    q++;
}
