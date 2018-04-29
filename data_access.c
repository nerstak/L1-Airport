#include <stdio.h>
#include <stdlib.h>
#include "data_access.h"

void new_company(Company * newcomp)
{
    printf("Name of the company: ");
    scanf("%s",newcomp->name);
    printf("Acronym: ");
    scanf("%s",newcomp->acronym);
    newcomp->planes_company=NULL;
}

void initTakeoff(Takeoff_list* Que)
{
    Que->first=NULL;
    Que->last=NULL;
}

void pushTakeoff(Takeoff_list* Que, Cell_plane *nElt)
{
    if (Que->last==NULL)
    {
        Que->last=nElt;
        Que->first=nElt;
    }
    else
    {
        Que->last->next_waiting=nElt;
        Que->last=nElt;
    }
}
Cell_plane* popTakeoff(Takeoff_list* Que)
{
    if(Que->first==NULL)
    {
        return NULL;
    }
    else
    {
        Cell_plane * cur;
        cur=Que->first;
        Que->first=cur->next_waiting;
        if(Que->first==NULL)
            Que->last=NULL;
        return cur;
    }
}
void displayTakeoff(Takeoff_list *Que)
{
    Cell_plane * cur=Que->first;
    while(cur!=NULL)
    {
        printf("%s - %s\n",cur->plane->takeoff_time,cur->plane->id);
        cur=cur->next_waiting;
    }
}
