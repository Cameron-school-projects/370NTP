#include <sys/time.h>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "packets.h"
int main()
{
    time_t test = time(0);
    char *data = new char[1024];
    packets *test4 = new packets();
    packets *test5 = new packets();
    strcpy(test4->dstaddr, "testing");
    strcpy(test4->srcaddr, "testin4");
    test4->xmt = static_cast<int>(test);
    test4->org = 5;
    serialize(test4, data);
    printf("%d", test4->xmt);
    deSerialize(data, test5);
}