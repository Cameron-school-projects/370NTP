struct packets
{
    int dstaddr; /* destination (remote) address */
    int srcaddr; /* source (local) address */
    char leap;      /* leap indicator */
    int org;     /* origin timestamp (t2)*/
    int servrec;     /* receive timestamp (t1)*/
    int xmt;     /* transmit timestamp (t3)*/
    int ref; /*t0 client recive timestamp*/
};
