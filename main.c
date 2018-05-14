#include <stdio.h>
#include <stdlib.h>
#include "actions.h"
#include "data_access.h"
#include "log_read.h"

int main()
{/*
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
    }*/
    //Actually to test the viability of functions
    Companies_list list_company,tmp;
    list_company = setup_companies();
    printf("%s\n",list_company->company.acronym);
    tmp=list_company->next_company;
    printf("%s\n",tmp->company.acronym);
    new_cell_plane("DCT042",2,50,"1337",list_company);
    new_cell_plane("DCT056",2,50,"1338",list_company);
    new_cell_plane("DCT059",2,50,"1339",list_company);
    printf("%s",list_company->company.planes_company->next_plane_company->next_plane_company->plane.id);
    //new_cell_company(&list_company);


}
