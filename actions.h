#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "data_access.h"

void time2string(int intime, char * stime);
int time2int(char * stime);
void sortwaitinglist(Planes_list PlaneL);
int PlanePriority(Plane * plane);
int Fueltime(Plane * plane);
int Emergency(Planes_list Landing);
int Takingoff(Takeoff_list * Takeoff,int time);
void movetoqueue(Planes_list wait,Takeoff_list * immediate,int time);

#endif // ACTIONS_H_INCLUDED
