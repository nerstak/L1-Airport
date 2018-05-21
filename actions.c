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


void sort_all_lists(lists_present_planes * present_planes,Companies_list all_companies,Companies_list blacklisted_companies,int time)
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

void extract_blacklisted(Planes_list * blacklist_landings,Planes_list * normal_landings,Companies_list blacklisted_companies)
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
        Planes_list PlaneL=*Planes;
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
        return Fueltime(plane);
    }
}

int Fueltime(Plane * plane) //calculates number of minutes remaining in the air
{
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


void move_plane_lists(Planes_list  *ini,Planes_list  *dest)
{
    Planes_list  *cur=dest;
    Planes_list lmao;
    lmao=*ini;
    if(*dest==NULL)
    {
        (*dest)=*ini;
        (*ini)=(*ini)->next_waiting;
        (*dest)->next_waiting=NULL;
    }
    else
    {
        while((*cur)->next_waiting!=NULL)
        {
            (*cur)=(*cur)->next_waiting;
        }
        (*cur)->next_waiting=(*ini);
        (*ini)=(*ini)->next_waiting;
        (*cur)->next_waiting->next_waiting=NULL;
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
        Planes_list wait=*waiting;
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
                if(search_company(blacklisted_companies,wait->plane.company->acronym)!=NULL) //if blacklisted, ignore
                {
                    wait=wait->next_waiting;
                }
                else
                {
                    if(immediate->last==NULL)
                    {
                        immediate->last=wait;
                        immediate->first=wait;
                    }

                    else
                        immediate->last->next_waiting=wait;
                    immediate->last=wait;
                    wait=wait->next_waiting;
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


void all_fuel_Use(lists_present_planes * present_planes)
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
        cur=cur->next_waiting;
    }
}


void user_interaction(lists_present_planes * present_planes, Companies_list * all_companies,Companies_list * blacklisted_companies, int time)
{
    int select=0;
    while(select!=32)
    {
        system("cls");
        printf("\n       What would you like to do?\n\n  1. Add airplane to takeoff\n  2. Add airplane to landing\n  3. Remove an airplane at launch\n  4. Declare a landing airplane as emergency\n  5. Put a company on the blacklist\n  6. Display all companies and their aircrafts\n  7. Display status of a company's planes\n  8. Display airplanes awaiting takeoff\n  9. Display airplanes waiting to land\n  0. Display history\n\n Press Spacebar to quit menu and continue simulation...");
        select=getch();
        system("cls");
        switch(select)
        {
        case(48):
            read_log(0);
            break;
        case(49):

            break;
        case(50):

            break;
        case(51):

            break;
        case(52):

            break;
        case(53):

            break;
        case(54):
            print_companies(*all_companies);
            break;
        case(55):

            break;
        case(56):

            break;
        case(57):

            break;
        }
        if(select!=32)
        {
            printf("\n\n   Press any key to go back to interaction menu...");
            getch();
        }
    }
}


void print_companies(Companies_list companies)
{
    Cell_company * cur=companies;
    Cell_plane * curplane;
    while(cur!=NULL)
    {
        printf("\n\n Company: %s\n  Acronym %s\n",cur->company.name,cur->company.acronym);
        curplane=cur->company.planes_company;
        while(curplane!=NULL)
        {
            printf("\n    Plane ID:%s",curplane->plane.id);
            curplane=curplane->next_plane_company;
        }
        cur=cur->next_company;
    }
}




