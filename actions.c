#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_access.h"
#include "actions.h"

void time2string(int intime, char * stime) //converts system time that goes from 0 to 1440 to a string of clock time 0000 to 2400
{
    sprintf(stime,"%d%d%d%d",(intime/60)/10,(intime/60)%10,(intime%60)/10,(intime%60)%10);
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


void sort_all_lists(lists_present_planes * present_planes,Companies_list all_companies,Companies_list blacklisted_companies,int time)//Sort list according to the remaining fuel
{
    //order landing and take off list
    if(present_planes->landing!=NULL)
    {
        sortwaitinglist(&(present_planes->landing));
    }
    if(present_planes->boarding!=NULL)
        sortwaitinglist(&(present_planes->boarding));
    //boarding to take off unless blacklisted
    move2queue(&(present_planes->boarding),present_planes->takeoff,time,&blacklisted_companies);
    //extract landing to emergency
    while(Emergency(present_planes->landing))
    {
        move_plane_lists(&(present_planes->landing),&(present_planes->emergency));
    }
    //extract landing to blacklisted
    extract_blacklisted(&(present_planes->blacklist),&(present_planes->landing),blacklisted_companies);
    //sort blacklisted
    sortwaitinglist(&(present_planes->blacklist));
    //extract blacklisted to emergency
    while(Emergency(present_planes->blacklist))
    {
        move_plane_lists(&(present_planes->blacklist),&(present_planes->emergency));
    }
    //sort emergency
    sortwaitinglist(&(present_planes->emergency));
}

void extract_blacklisted(Planes_list * blacklist_landings,Planes_list * normal_landings,Companies_list blacklisted_companies) //Add a plane of a blacklisted company in a list of blacklisted airplans
{
    Planes_list prev,cur,landings=*normal_landings;
    int empty=0;
    while(empty==0)
    {
        if(landings==NULL)
            empty=1;
        else if(search_company(&blacklisted_companies,landings->plane.company->acronym)!=NULL)
        {
            move_plane_lists(normal_landings,blacklist_landings);
        }
        else
            empty=1;
        landings=*normal_landings;
    }
    if(landings!=NULL)
    {
        prev=landings;
        cur=prev;
        while(cur!=NULL)
        {
            if(search_company(&blacklisted_companies,landings->plane.company->acronym)!=NULL)
            {
                move_plane_lists(&cur,blacklist_landings);
                prev->next_waiting=cur;
            }
            prev=prev->next_waiting;
            if(cur!=NULL)
                cur=cur->next_waiting;
        }
    }
}

        //search_company(blacklisted_companies,Takeoff->first->plane.company->acronym)!=NULL


void sortwaitinglist(Planes_list * Planes) //Sorts the landing and take off waiting lists in order of time or fuel time
{
    if(*Planes!=NULL)
    {
        int unsorted=1;
        Cell_plane *cur,*prev,*follow;
        while(unsorted)
        {
            unsorted=0;
            prev=(*Planes);
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
                    (*Planes)=cur;
                    prev=(*Planes);
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
        return Fueltime(plane);
    }
}

int Fueltime(Plane * plane) //calculates number of minutes remaining in the air
{
    if(plane->fuel==0)
        return 0;
    return (plane->fuel)/(plane->comsumption);
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


void move_plane_lists(Planes_list  *ini,Planes_list  *dest)//Changes a plane of list
{
    Cell_plane * cur=*dest;
    Cell_plane * pla=*ini;
    if(*dest==NULL)
    {
        (*dest)=pla;
        (*ini)=(*ini)->next_waiting;
        (*dest)->next_waiting=NULL;
    }
    else
    {
        while(cur->next_waiting!=NULL)
        {
            cur=cur->next_waiting;
        }
        cur->next_waiting=pla;
        (*ini)=(*ini)->next_waiting;
        cur->next_waiting->next_waiting=NULL;
    }
}

int Takingoff(Takeoff_list * Takeoff,int time,Companies_list blacklisted_companies) //Sees if a plane is scheduled for takeoff
{
    if(Takeoff->first!=NULL)
    {
        int blacklistcheck=0;
        while(blacklistcheck==0)
        {
            if(Takeoff->first==NULL)
                blacklistcheck=1;
            else if(search_company(&blacklisted_companies,Takeoff->first->plane.company->acronym)!=NULL)
                Takeoff->first=Takeoff->first->next_waiting;
                if(Takeoff->first==NULL)
                    Takeoff->last=NULL;
            else
                blacklistcheck=1;
        }

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

void move2queue(Planes_list * waiting,Takeoff_list * immediate,int time,Companies_list * blacklisted_companies)// Moves plane from the takeoff wait list to the taking off queue.
{
    if(*waiting!=NULL)
    {
        char acro[4];
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
        while(numscheduled<5 && (*waiting)!=NULL)
        {
            if(time2int((*waiting)->plane.takeoff_time)<time+5)
            {
                sprintf(acro,"%c%c%c",(*waiting)->plane.id[0],(*waiting)->plane.id[1],(*waiting)->plane.id[2]);

                if(search_company(blacklisted_companies,acro)) //if blacklisted, ignore
                {
                    (*waiting)=(*waiting)->next_waiting;
                }
                else
                {
                    if(immediate->last==NULL)
                    {
                        immediate->last=(*waiting);
                        immediate->first=(*waiting);
                    }
                    else
                        immediate->last->next_waiting=(*waiting);
                    immediate->last=(*waiting);
                    (*waiting)=(*waiting)->next_waiting;
                    immediate->last->next_waiting=NULL;
                    numscheduled++;
                }
            }
            else
            {
                numscheduled=5;
            }
        }
    }
}


void all_fuel_Use(lists_present_planes * present_planes)//Update the fuel remaining
{
    use_fuel(&(present_planes->landing));
    use_fuel(&(present_planes->blacklist));
    use_fuel(&(present_planes->emergency));

}


void use_fuel(Planes_list * planesL)
{
    Planes_list PlanesL=*planesL;
    Cell_plane * cur=PlanesL;
    while(cur!=NULL)
    {
        cur->plane.fuel-=cur->plane.comsumption;
        if(cur->plane.fuel<0)
            cur->plane.fuel=0;
        cur=cur->next_waiting;
    }
}
