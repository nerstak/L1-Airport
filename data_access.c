#include <stdio.h>
#include <stdlib.h>
#include "data_access.h"

Company * new_company()
{
    Company * newComp=malloc(sizeof(Company));
    printf("Name of the company: ");
    scanf("%s",newComp->name);
    printf("Acronym: ");
    scanf("%s",newComp->acronym);
    newComp->planes_company=NULL;
    return newComp;
}

void new_cell_company(Companies_list * list_company)
{
    Companies_list cursor_company;
    cursor_company=*list_company;
    Cell_company * newCellComp=malloc(sizeof(Cell_company));
    Company *temp=new_company();
    newCellComp->company=*temp;
    newCellComp->next_company=NULL;

    if(*list_company==NULL)
    {
        *list_company=newCellComp;
    }
    else
    {
        while(cursor_company->next_company != NULL)
        {
            cursor_company=cursor_company->next_company;
        }
        cursor_company->next_company=newCellComp;
    }
}

Plane * newPlane()
{
    Plane * newPlane=malloc(sizeof(Plane));
    printf("ID of the plane: ");
    scanf("%s",newPlane->id);

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

/*void displayTakeoff(Takeoff_list *Que)
{
    Cell_plane * cur=Que->first;
    while(cur!=NULL)
    {
        printf("%s - %s\n",cur->plane->takeoff_time,cur->plane->id);
        cur=cur->next_waiting;
    }
}*/
