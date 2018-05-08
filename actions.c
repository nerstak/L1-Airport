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

void sortwaitinglist(Planes_list PlaneL)
{
    int unsorted=1;
    Cell_plane *cur,*prev,*follow;
    while(unsorted)
    {
        unsorted=0;
        prev=PlaneL;
        cur=prev->next_waiting;
        if(cur!=NULL)
            follow=cur->next_waiting;
        else
            follow=NULL;
        if(cur!=NULL)
        {
            if(PlanePriority(&(prev->plane))>PlanePriority(&(cur->plane)))
            {
                prev->next_waiting=follow;
                cur->next_waiting=prev;
                PlaneL=cur;
                prev=PlaneL;
                cur=prev->next_waiting;
                unsorted=1;
            }
        }
        while(follow!=NULL)
        {
            if(PlanePriority(&(cur->plane))>PlanePriority(&(follow->plane)))
            {
                prev->next_waiting=follow;
                cur->next_waiting=follow->next_waiting;
                follow->next_waiting=cur;
                unsorted=1;
            }
            prev=prev->next_waiting;
            cur=prev->next_waiting;
            follow=cur->next_waiting;
        }
    }
}

int PlanePriority(Plane * plane)
{
    if(plane->comsumption=NULL)
    {
        return time2int(&(plane->takeoff_time));
    }
    else
    {
        return Fueltime(plane);
    }
}

int Fueltime(Plane * plane)
{
    return plane->fuel/plane->comsumption;
}
