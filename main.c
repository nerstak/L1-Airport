#include <stdio.h>
#include <stdlib.h>
#include "actions.h"
#include "data_access.h"
#include "log_read.h"

int main()
{
    int line=0;
    char stime[5];
    //events_reading(&line,"1531");
    // initializing variables
    int time;
    lists_present_planes * present_planes;
    init_present_planes(present_planes);
    Companies_list all_companies, blacklisted_companies;
    all_companies=setup_companies();
    blacklisted_companies=NULL;
    //      ALL INITIAL FILE READING SHIT HERE  (done?)

    for(time=0;time<=1440;time++)
    {
        time2string(time,stime);
        events_reading(&line,stime,&all_companies,present_planes);
        sort_all_lists(present_planes,all_companies,blacklisted_companies,time);
        if(time%5==0) //Player input every 5 min
        {



            // ALL PLAYER INPUT AND INTERACTIONS HERE




            sort_all_lists(present_planes,all_companies,blacklisted_companies,time);
        }


                //EVENTS
        if(present_planes->emergency!=NULL)//Emergency landing
        {

            // REGISTER EVENT

            present_planes->emergency=present_planes->emergency->next_waiting;
        }
        else if(Takingoff(present_planes->takeoff,time,blacklisted_companies))//Take offs
        {

            // REGISTER EVENT

            present_planes->takeoff->first=present_planes->takeoff->first->next_waiting;
        }
        else if(present_planes->blacklist!=NULL)//Blacklisted landings
        {

            // REGISTER EVENT

            present_planes->blacklist=present_planes->blacklist->next_waiting;
        }
        else if(present_planes->landing!=NULL)//Landings
        {

            // REGISTER EVENT

            present_planes->landing=present_planes->landing->next_waiting;
        }
    }/*

    //Actually to test the viability of functions
    int lm=0;
    Takeoff_list tempa;
    tempa.first=NULL;
    lists_present_planes list_planes_used;
    list_planes_used.boarding=NULL;
    list_planes_used.emergency=NULL;
    list_planes_used.landing=NULL;
    list_planes_used.takeoff=&tempa;
    Companies_list list_company,tmp;
    list_company = setup_companies();
    events_reading(&lm,"1531",&list_company,&list_planes_used);
    printf("%s\n",list_company->company.acronym);
    tmp=list_company->next_company;
    printf("%s\n",tmp->company.acronym);
    new_cell_plane("DCT042",2,50,"1337",list_company);
    new_cell_plane("DCT056",2,50,"1338",list_company);
    new_cell_plane("DCT059",2,50,"1339",list_company);
    list_planes_used.boarding=list_company->company.planes_company;
    printf("%d\n",presence_in_lists(list_planes_used.boarding,"DCT043"));
    printf("--%s\n",list_planes_used.landing->plane.id);
    //new_cell_company(&list_company);*/
}
