#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_access.h"
#include "actions.h"

void time2string(int intime, char * stime) //converts system time that goes from 0 to 1440 to a string of clock time 0000 to 2400
{
    stime[0]=((intime/60)/10)+48;
    stime[1]=((intime/60)%10)+48;
    stime[2]=((intime%60)/10)+48;
    stime[3]=((intime%60)%10)+48;
}

int time2int(char * stime) //converts a clock time string to system time
{
    int hours;
    char h[3],m[3];
    h[0]=stime[0];
    h[1]=stime[1];
    m[0]=stime[2];
    m[1]=stime[3];
    return (atoi(h)*60)+atoi(m);
}
