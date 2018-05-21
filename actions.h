#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "data_access.h"

void time2string(int intime, char * stime);
int time2int(char * stime);
void sort_all_lists(lists_present_planes * present_planes,Companies_list all_companies,Companies_list blacklisted_companies,int time);
void extract_blacklisted(Planes_list * black_landings,Planes_list * landings,Companies_list blacklisted_companies);
void sortwaitinglist(Planes_list * PlaneL);
int PlanePriority(Plane * plane);
int Fueltime(Plane * plane);
int Emergency(Planes_list Landing);
void move_plane_lists(Planes_list * ini,Planes_list * dest);
int Takingoff(Takeoff_list * Takeoff,int time,Companies_list  blacklisted_companies);
void move2queue(Planes_list * wait,Takeoff_list * immediate,int time,Companies_list * blacklisted_companies);
void all_fuel_Use(lists_present_planes * present_planes);
void use_fuel(Planes_list * planesL);

void user_interaction(lists_present_planes * present_planes, Companies_list * all_companies,Companies_list * blacklisted_companies, int time);
void print_companies(Companies_list companies);

#endif // ACTIONS_H_INCLUDED
