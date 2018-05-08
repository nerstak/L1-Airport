#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED


void time2string(int intime, char * stime);
int time2int(char * stime);
void sortwaitinglist(Planes_list PlaneL);
int PlanePriority(Plane * plane);
int Fueltime(Plane * plane);

#endif // ACTIONS_H_INCLUDED
