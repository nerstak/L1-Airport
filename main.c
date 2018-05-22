#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"
#include "data_access.h"
#include "log_read.h"
#include "log_write.h"
#include "user.h"

int main()
{
    // initializing variables
    int time;
    char stime[5],event[25];
    //ALL INITIAL FILE READING
    lists_present_planes * present_planes;
    init_present_planes(&present_planes);
    Companies_list all_companies, blacklisted_companies;
    all_companies=setup_companies();
    blacklisted_companies=NULL;

    create_log();

    for(time=0;time<=1440;time++) //Big main loop
    {
        time2string(time,stime);
        events_reading(stime,&all_companies,&blacklisted_companies,present_planes);
        sort_all_lists(present_planes,all_companies,blacklisted_companies,time);//After adding new events, we sort
        if(time%5==0) //Player input every 5 min
        {
            user_interaction(present_planes,&all_companies,&blacklisted_companies,time);
            sort_all_lists(present_planes,all_companies,blacklisted_companies,time);//After adding new events, we sort
        }
        //EVENTS
        if(present_planes->emergency!=NULL)//Emergency landing
        {
            // REGISTER EVENT
            if(present_planes->emergency->plane.fuel<=0)
                generate_action_log('C',&(present_planes->emergency->plane),stime,event);
            else
                generate_action_log('U',&(present_planes->emergency->plane),stime,event);
            write_action(event,stime);
            // EXECUTION EVENT
            present_planes->emergency=present_planes->emergency->next_waiting;
        }
        else if(Takingoff(present_planes->takeoff,time,blacklisted_companies))//Take offs
        {
            // REGISTER EVENT
            generate_action_log('D',&(present_planes->takeoff->first->plane),stime,event);
            write_action(event,stime);
            // EXECUTION EVENT
            present_planes->takeoff->first=present_planes->takeoff->first->next_waiting;
            if(present_planes->takeoff->first==NULL)
                present_planes->takeoff->last=NULL;
        }
        else if(present_planes->blacklist!=NULL)//Blacklisted landings
        {
            // REGISTER EVENT
            generate_action_log('N',&(present_planes->blacklist->plane),stime,event);
            write_action(event,stime);
            // EXECUTION EVENT
            present_planes->blacklist=present_planes->blacklist->next_waiting;
        }
        else if(present_planes->landing!=NULL)//Landings
        {

            // REGISTER EVENT
            generate_action_log('A',&(present_planes->landing->plane),stime,event);
            printf("EVENT:        %s\n",event);
            write_action(event,stime);
            // EXECUTION EVENT
            present_planes->landing=present_planes->landing->next_waiting;
        }
        all_fuel_Use(present_planes);
    }
    return 0;
}
