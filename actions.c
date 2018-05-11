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
    char h[3],m[3];
    h[0]=stime[0];
    h[1]=stime[1];
    m[0]=stime[2];
    m[1]=stime[3];
    return (atoi(h)*60)+atoi(m);
}

void sortwaitinglist(Planes_list PlaneL) //Sorts the landing and take off waiting lists in order of time or fuel time
{
    if(PlaneL!=NULL)
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
}

int PlanePriority(Plane * plane) //Used in sorting function to determine priority of plane
{
    if(plane->comsumption==NULL)
    {
        return time2int(plane->takeoff_time);
    }
    else
    {
        /*
        if(Fueltime(plane)>5 && PLANE IS BLACKLISTED  ) //This is to put blacklisted planes as less important than emergency but more than normal. It is also a very shitty system
        {
            return 6;
        }
        */
        return Fueltime(plane);
    }
}

int Fueltime(Plane * plane) //calculates number of minutes remaining in the air
{
    return plane->fuel/plane->comsumption;
}

int Emergency(Planes_list Landing) //sees if a plane has less than 5 mins remaining in the tank
{
    if(Landing!=NULL)
    {
        if(Fueltime(&(Landing->plane))<6)
        {
            return 1;
        }
    }
    return 0;
}

int Takingoff(Takeoff_list * Takeoff,int time) //Sees if a plane is scheduled for takeoff
{
    if(Takeoff->first!=NULL)
    {
        /*
        while( PLANE IS BLACKLISTED )
        {
            Takeoff->first=Takeoff->first->next_waiting;
        }
        */
        if(Takeoff->first!=NULL)
        {
            if(time2int(Takeoff->first->plane.takeoff_time)<=time)
            {
                return 1;
            }
        }
    }
    return 0;
}

void movetoqueue(Planes_list wait,Takeoff_list * immediate,int time)// MOves plane from the takeoff wait list to the taking off queue.
{
    if(wait!=NULL)
    {
        int numscheduled=0;
        Planes_list count=immediate->first;
        if(count!=NULL)
        {
            numscheduled++;
            while(count!=immediate->last)
            {
                numscheduled++;
                count=count->next_waiting;
            }
        }
        while(numscheduled<5 && wait!=NULL)
        {
            if(time2int(wait->plane.takeoff_time)<time+5)
            {
                immediate->last->next_waiting=wait;
                immediate->last=wait;
                wait=wait->next_waiting;
                immediate->last->next_waiting=NULL;
                numscheduled++;
            }
            else
            {
                numscheduled=5;
            }
        }
    }
}






