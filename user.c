#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_access.h"
#include "user.h"

void user_interaction(lists_present_planes * present_planes, Companies_list * all_companies,Companies_list * blacklisted_companies, int time)//Menu of interaction for the user
{
    int select=0;
    char input[10];
    while(select!=32)
    {
        system("cls");
        char stime[5];
        time2string(time,stime);
        printf("Time - %c%c:%c%c",stime[0],stime[1],stime[2],stime[3]);
        printf("\n\n       What would you like to do?\n\n  1. Add airplane to takeoff\n  2. Add airplane to landing\n  3. Remove an airplane at launch\n  4. Declare a landing airplane as emergency\n  5. Put a company on the blacklist\n  6. Display all companies and their aircrafts\n  7. Display status of a company's planes\n  8. Display airplanes awaiting takeoff\n  9. Display airplanes waiting to land\n  0. Display history\n\n Press Spacebar to quit menu and continue simulation...");
        select=getchar();
        system("cls");
        switch(select)
        {
        case(48): //Print the history
            read_log(0);
            break;
        case(49): //Add an airplane to takeoff

            break;
        case(50): //Add an airplane to landing

            break;
        case(51): //Remove an airplane at launch

            break;
        case(52): //Declare an airplane as emergency

            break;
        case(53):

            break;
        case(54):
            print_companies(*all_companies);
            break;
        case(55):
            print_companies(*all_companies);
            printf("\n\n   What company would you like to see the planes of? (enter acronym)\n");
            scanf("%s",input);
            if(search_company(all_companies,input)==NULL)
                printf("\n    That company doesn't exist");
            else
                planes_status(search_company(all_companies,input),0,present_planes);
            break;
        case(56):
            if(present_planes->boarding!=NULL || present_planes->takeoff->first!=NULL)
            {
                printf("\n       Planes awaiting takeoff:\n");
                print_planes(present_planes->boarding,0);
                print_planes(present_planes->takeoff->first,0);
            }
            else
                printf("\n       No planes are awaiting takeoff\n");
            break;
        case(57):
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
            printf("\n\n   Press any key to go back to interaction menu...   %d",select);
            getchar();
        }
    }
}

void print_planes(Planes_list cur,int landing) //Print data of a plane
{
    while(cur!=NULL)
    {
        printf("\n  Plane ID: %s\n",cur->plane.id,cur->plane.fuel,cur->plane.comsumption);
        if(landing=1)
            printf("    Fuel: %d\n    Consumption: %d\n",cur->plane.fuel,cur->plane.comsumption);
        else
            printf("    Scheduled Takeoff: %s",cur->plane.takeoff_time[0],cur->plane.takeoff_time[1],cur->plane.takeoff_time[2],cur->plane.takeoff_time[3]);
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
    if(curplane==NULL)
        printf("\n    This company has no active plane");
    else
    {
        while(curplane!=NULL)
        {
            if(unused==0)
                printf("\n    Plane ID:%s",curplane->plane.id);
            if (search_planes_list(&(curplane->plane),present_planes->boarding) || search_planes_list(&(curplane->plane),present_planes->takeoff->first) && unused==0)
                printf(" - Waiting to take off\n       Scheduled time:%c%c:%c%c",curplane->plane.takeoff_time[0],curplane->plane.takeoff_time[1],curplane->plane.takeoff_time[2],curplane->plane.takeoff_time[3]);
            else if(search_planes_list(&(curplane->plane),present_planes->landing) || search_planes_list(&(curplane->plane),present_planes->emergency) || search_planes_list(&(curplane->plane),present_planes->blacklist) && unused==0)
                printf(" - Waiting to land\n       Fuel: %d\n       Consumption: %d",curplane->plane.fuel,curplane->plane.comsumption);
            else if(search_planes_list(&(curplane->plane),present_planes->boarding)==0 || search_planes_list(&(curplane->plane),present_planes->takeoff->first)==0 ||search_planes_list(&(curplane->plane),present_planes->landing)==0 || search_planes_list(&(curplane->plane),present_planes->emergency)==0 || search_planes_list(&(curplane->plane),present_planes->blacklist)==0 )
                printf(" - Unused");
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
