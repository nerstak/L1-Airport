#include <stdio.h>
#include <stdlib.h>
#include "actions.h"
#include "data_access.h"
#include "log_read.h"

int main()
{
    int line=0;
    events_reading(&line,"1531");
    // initializing variables
    int time,numscheduled;
    Planes_list Landing=NULL,Takeoff_wait=NULL;
    Takeoff_list * Takeoff_now;
    Takeoff_now=malloc(sizeof(Takeoff_list));
    initTakeoff(Takeoff_now);

    //      ALL INITIAL FILE READING SHIT HERE

    for(time=0;time<=1440;time++)
    {
        if(time%5==0) //Player input every 5 min
        {
            //assign_takeoff()


            // ALL PLAYER INPUT AND INTERACTIONS HERE




            movetoqueue(Takeoff_wait,Takeoff_now,time);
            sortwaitinglist(Takeoff_wait);
        }


                //EVENTS
        if(Emergency(Landing))//Emergency landing
        {

            // REGISTER EVENT

            Landing=Landing->next_waiting;
        }
        else if(Takingoff(Takeoff_now,time))//Take offs
        {

            // REGISTER EVENT

            Takeoff_now->first=Takeoff_now->first->next_waiting;
        }
        else if(Landing!=NULL)//Landings & Blacklisted landings
        {

            // REGISTER EVENT

            Landing=Landing->next_waiting;
        }
        sortwaitinglist(Landing);
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
