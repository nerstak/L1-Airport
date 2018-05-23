#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_access.h"
#include "user.h"
#include "log_read.h"

void user_interaction(lists_present_planes * present_planes, Companies_list * all_companies,Companies_list * blacklisted_companies, int time)//Menu of interaction for the user
{
    int select=0,fuel,consumption;
    char input[4], name[7],event[20],n_time[5],curtime[5];
    time2string(time,curtime);
    Cell_plane * plane_cell;
    while(select!=32)
    {
        system("cls");
        char stime[5];
        time2string(time,stime);
        printf("Time - %c%c:%c%c",stime[0],stime[1],stime[2],stime[3]);
        printf("\n\n       What would you like to do?\n\n  1. Add airplane to takeoff\n  2. Add airplane to landing\n  3. Remove an airplane at launch\n  4. Declare a landing airplane as emergency\n  5. Put a company on the blacklist\n  6. Display all companies and their aircrafts\n  7. Display status of a company's planes\n  8. Display airplanes awaiting takeoff\n  9. Display airplanes waiting to land\n  0. Display history\n\n Press Spacebar to quit menu and continue simulation...");
        //printf("\n takeoff:%s boarding:%s",present_planes->takeoff->first->plane.id,present_planes->boarding->plane.id);
        int yeha=rand()%5;
        if(yeha)
            printf("Yeha %d",yeha);
        select=getch();
        system("cls");
        switch(select)
        {
        case(48): //Print the history
            read_log(0);
            break;

        case(49): //Add an airplane to takeoff
            print_unused(*all_companies,present_planes);
            printf("\nName of the plane? (Acronym of company + 3 numbers) ");
            scanf("%s",name);
            printf("At what time do you want to launch it (HHMM)? ");
            scanf("%s",n_time);
            sprintf(event,"%s-D-%s------",name,n_time);
            events_execution(event,all_companies, blacklisted_companies,present_planes,curtime);
            break;

        case(50): //Add an airplane to landing
            print_unused(*all_companies,present_planes);
            printf("\nName of the plane? (Acronym of company + 3 numbers) ");
            scanf("%s",name);
            printf("Consumption of the plane? ");
            scanf("%d",&consumption);
            printf("Fuel of the plane? ");
            scanf("%d",&fuel);
            sprintf(event,"%s-A------%d%d-%d%d",name,fuel/10,fuel%10,consumption/10,consumption%10);
            events_execution(event,all_companies, blacklisted_companies,present_planes,curtime);
            break;

        case(51): //Remove an airplane at launch
            if(present_planes->boarding!=NULL)
            {
                printf("\n       Planes awaiting takeoff:\n");
                print_planes(present_planes->boarding,0);
                 printf("\n ID of the plane?: ");
                scanf("%s",name);
                plane_cell=pop_plane(&(present_planes->boarding),name);
                if(plane_cell==NULL)
                    printf("\n       No plane found\n");
            }
            else
                printf("\n       No planes are awaiting takeoff\n");
            break;

        case(52): //Declare an airplane as emergency
            if(present_planes->landing!=NULL || present_planes->blacklist!=NULL)
            {
                printf("\n       Select a plane to move to emergency:\n");
                print_planes(present_planes->landing,1);
                print_planes(present_planes->blacklist,1);
                printf("\n ID of the plane?: ");
                scanf("%s",name);
                plane_cell=pop_plane(&(present_planes->landing),name);
                if(plane_cell==NULL)
                    plane_cell=pop_plane(&(present_planes->blacklist),name);
                if(plane_cell==NULL)
                    printf("\n       No plane found\n");
                else
                {
                    move_plane_lists(&plane_cell,&(present_planes->emergency));
                }
            }
            else
            {
                printf("\n       No planes are waiting to land:\n");
            }



            break;

        case(53): //Put a company on the blacklist
            print_companies(*all_companies);
            printf("\n\n   What company would you like to Blacklist(enter acronym: ");
            scanf("%s",input);
            if(search_company(blacklisted_companies,input)==NULL)
                blacklist_company(input,all_companies,blacklisted_companies);
            break;

        case(54): //Display all companies and their aircrafts
            print_companies(*all_companies);
            if(*blacklisted_companies!=NULL)
            {
                Cell_company * cur=*blacklisted_companies;
                printf("\n\n    The Following companies are blacklisted:");
                while(cur!=NULL)
                {
                    printf("\n - %s",cur->company.name);

                    cur=cur->next_company;
                }
            }
            break;

        case(55): //Display status of a company's aircrafts
            print_companies(*all_companies);
            printf("\n\n   What company would you like to see the planes of? (enter acronym)\n");
            scanf("%s",input);
            if(search_company(all_companies,input)==NULL)
                printf("\n    That company doesn't exist");
            else
                planes_status(search_company(all_companies,input),0,present_planes);
            break;

        case(56): //Display planes waiting to takeoff
            if(present_planes->boarding!=NULL || present_planes->takeoff->first!=NULL)
            {
                printf("\n       Planes awaiting takeoff:\n");
                print_planes(present_planes->boarding,0);
                print_planes(present_planes->takeoff->first,0);
            }
            else
                printf("\n       No planes are awaiting takeoff\n");
            break;

        case(57): //Display planes waiting for landing
            if(present_planes->emergency!=NULL)
            {
                printf("\n       Planes in emergency:\n");
                print_planes(present_planes->emergency,1);
            }
            if(present_planes->blacklist!=NULL)
            {
                printf("\n       Blacklisted Planes:\n");
                print_planes(present_planes->blacklist,1);
            }
            if(present_planes->landing!=NULL)
            {
                printf("\n       Planes waiting to land:\n");
                print_planes(present_planes->landing,1);
            }
            if(present_planes->landing==NULL && present_planes->blacklist==NULL && present_planes->emergency==NULL)
                printf("\n       No planes are currently waiting to land");
            break;

        }
        if(select!=32)
        {
            printf("\n\n   Press any key to go back to interaction menu...");
            getch();
        }
    }
}

void print_planes(Planes_list cur,int landing) //Print data of a plane
{
    while(cur!=NULL)
    {
        printf("\n  Plane ID: %s\n",cur->plane.id);
        if(landing)
            printf("    Fuel: %d\n    Consumption: %d\n",cur->plane.fuel,cur->plane.comsumption);
        else
            printf("    Scheduled Takeoff: %c%c:%c%c",cur->plane.takeoff_time[0],cur->plane.takeoff_time[1],cur->plane.takeoff_time[2],cur->plane.takeoff_time[3]);
        cur=cur->next_waiting;
    }
}


void print_companies(Companies_list companies) //Print all planes of a company
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

void planes_status(Company * Company,int unused,lists_present_planes * present_planes) //Print the list where the airplane is
{
    Cell_plane * curplane;
    curplane=Company->planes_company;
    if(curplane==NULL && unused==0)
        printf("\n    This company has no active plane");
    else
    {
        if(unused==1)
            printf("\n  %s -=- Acronym: %s",Company->name,Company->acronym);
        while(curplane!=NULL)
        {
            if(unused==0)
                printf("\n    Plane ID:%s",curplane->plane.id);
            if ((search_planes_list(&(curplane->plane),present_planes->boarding) || search_planes_list(&(curplane->plane),present_planes->takeoff->first)) && unused==0)
                printf(" - Waiting to take off\n       Scheduled time:%c%c:%c%c",curplane->plane.takeoff_time[0],curplane->plane.takeoff_time[1],curplane->plane.takeoff_time[2],curplane->plane.takeoff_time[3]);
            else if((search_planes_list(&(curplane->plane),present_planes->landing) || search_planes_list(&(curplane->plane),present_planes->emergency) || search_planes_list(&(curplane->plane),present_planes->blacklist)) && unused==0)
                printf(" - Waiting to land\n       Fuel: %d\n       Consumption: %d",curplane->plane.fuel,curplane->plane.comsumption);
            else if(search_planes_list(&(curplane->plane),present_planes->boarding)==0 && search_planes_list(&(curplane->plane),present_planes->takeoff->first)==0 && search_planes_list(&(curplane->plane),present_planes->landing)==0 && search_planes_list(&(curplane->plane),present_planes->emergency)==0 && search_planes_list(&(curplane->plane),present_planes->blacklist)==0 )
            {
                if(unused)
                    printf("\n    Plane ID:%s",curplane->plane.id);
                printf(" - Unused");
            }

            curplane=curplane->next_plane_company;
        }
    }
}

int search_planes_list(Plane * plane,Planes_list cur)
{
    while(cur!=NULL)
    {
        if(strcmp(plane->id,cur->plane.id)==0)
            return 1;
        else
            cur=cur->next_waiting;
    }
    return 0;
}


void print_unused(Companies_list cur,lists_present_planes * present_planes)
{
    while(cur!=NULL)
    {
        planes_status(&(cur->company),1,present_planes);
        cur=cur->next_company;
    }
}


Cell_plane * pop_plane(Planes_list * Plane_cell,char * ID)
{
    Cell_plane * prev=*Plane_cell,*cur;
    if(prev!=NULL)
    {
        if(strcmp(ID,prev->plane.id)==0)
        {
            (*Plane_cell)=(*Plane_cell)->next_waiting;
            prev->next_waiting=NULL;
            return prev;
        }
        cur=prev->next_waiting;
        while(cur!=NULL)
        {
            if(strcmp(ID,cur->plane.id)==0)
            {
                prev->next_waiting=cur->next_waiting;
                cur->next_waiting=NULL;
                return cur;
            }
            cur=cur->next_waiting;
            prev=prev->next_waiting;
        }
    }
    return NULL;
}


void random_gen(lists_present_planes * present_planes, Companies_list * all_companies,Companies_list * blacklisted_companies, int time)
{
    if(rand()%12==0)
    {
        char name[4],stime[5],event[20],curtime[5];
        time2string(time,curtime);
        int ID;
        Companies_list randc;
        Cell_plane * plane_cell;
        switch(rand()%3)
        {
        case 0:
            // add takeoff
            randc=randcomp(*all_companies);
            if(randc!=NULL)
            {
                ID=rand()%1000;
                time2string(time+6+(rand()%120),stime);
                sprintf(event,"%s%d%d%d-D-%s------",randc->company.acronym,ID/100,(ID%100)/10,ID%10,stime);
                events_execution(event,all_companies, blacklisted_companies,present_planes,curtime);
            }
            break;
        case 1:
            randc=randcomp(*all_companies);
            if(randc!=NULL)
            {
                int fuel,consumption;
                ID=rand()%1000;
                consumption=(rand()%5)+1;
                fuel=consumption*((rand()%15)+1);
                sprintf(event,"%s%d%d%d-A------%d%d-%d%d",randc->company.acronym,ID/100,(ID%100)/10,ID%10,fuel/10,fuel%10,consumption/10,consumption%10);
                events_execution(event,all_companies, blacklisted_companies,present_planes,curtime);
            }
            break;
        case 2:
            // remove launch or decide landing in an emergency or blacklist
            if(rand()%5==0)
            {
                switch(rand()%3)
                {
                case 0:
                    plane_cell=randplane(present_planes->boarding);
                    if(plane_cell!=NULL)
                        pop_plane(&(present_planes->boarding),plane_cell->plane.id);
                    // remove launch
                    break;
                case 1:
                    plane_cell=randplane(present_planes->landing);
                    if(plane_cell!=NULL)
                    {
                        pop_plane(&(present_planes->landing),plane_cell->plane.id);
                        move_plane_lists(&plane_cell,&(present_planes->emergency));
                    }
                    // emergency land
                    break;
                case 2:
                    randc=randcomp(*all_companies);
                    if(randc!=NULL)
                    {
                        if(search_company(blacklisted_companies,randc->company.acronym)==NULL)
                            blacklist_company(randc->company.acronym,all_companies,blacklisted_companies);
                    }
                    // blacklist
                    break;
                }
            }
            break;
        }
    }
}



Planes_list randplane(Planes_list Planes)
{
    int n=0,i;
    if(Planes!=NULL)
    {
        Planes_list cur=Planes;
        while(cur!=NULL)
        {
            cur=cur->next_waiting;
            n++;
        }
        cur=Planes;

        n=rand()%n;
        for(i=0;i<n;i++)
        {
            cur=cur->next_waiting;
        }
        return cur;
    }
    return NULL;
}


Companies_list randcomp(Companies_list all_companies)
{
    if(all_companies==NULL)
        return NULL;
    else
    {
        int i,n=0;
        Companies_list cur=all_companies;
        while(cur!=NULL)
        {
            cur=cur->next_company;
            n++;
        }
        cur=all_companies;
        n=rand()%n;
        for(i=0;i<n;i++)
        {
            cur=cur->next_company;
        }
        return cur;

    }
}









