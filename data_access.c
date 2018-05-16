#include <stdio.h>
#include <stdlib.h>
#include "data_access.h"

Company * new_company(char *name,char *acronym)
{
    Company * newComp=malloc(sizeof(Company));
    strcpy(newComp->name,name);
    strcpy(newComp->acronym,acronym);
    newComp->planes_company=NULL;
    return newComp;
}

void new_cell_company(Companies_list * list_company,char *name,char *acronym)
{
    Companies_list cursor_company;
    cursor_company=*list_company;
    Cell_company * newCellComp=malloc(sizeof(Cell_company));
    Company *temp=new_company(name,acronym);
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

Plane * new_plane(char *id, int comsumption, int fuel, char *takeoff_time, Company *ptr_comp)
{
    Plane * newPlane=malloc(sizeof(Plane));
    strcpy(newPlane->id,id);
    newPlane->comsumption=comsumption;
    newPlane->fuel=fuel;
    strcpy(newPlane->takeoff_time,takeoff_time);
    newPlane->company=ptr_comp;
    return newPlane;
}

void new_cell_plane(char *id,int comsumption, int fuel, char *takeoff_time, Company *ptr_comp)
{
    Planes_list cursor_plane;
    cursor_plane=ptr_comp->planes_company;
    Cell_plane * newCellPlane=malloc(sizeof(Cell_plane));
    Plane * temp=new_plane(id,comsumption,fuel,takeoff_time,ptr_comp);
    newCellPlane->plane=*temp;
    newCellPlane->next_plane_company=NULL;
    newCellPlane->next_waiting=NULL;
    newCellPlane->previous_plane_company=NULL;

    if(ptr_comp->planes_company==NULL)
    {
        ptr_comp->planes_company=newCellPlane;
    }
    else
    {
        while(cursor_plane->next_plane_company != NULL)
        {
            cursor_plane=cursor_plane->next_plane_company;
        }
        cursor_plane->next_plane_company=newCellPlane;
    }



}

Company * search_company(Companies_list * list_company, char * acronym)
{
    Companies_list cursor_company;
    cursor_company=*list_company;
    while(cursor_company != NULL && strcmp(acronym,cursor_company->company.acronym)!=0)
    {
        cursor_company=cursor_company->next_company;
    }
    if(cursor_company!=NULL && strcmp(acronym,cursor_company->company.acronym)==0)
    {
        return cursor_company;
    }
    return NULL;
}

Cell_plane * search_cell_plane(Planes_list * planes_company, char * name)
{
    Planes_list cursor_plane;
    cursor_plane=*planes_company;
    while(cursor_plane != NULL&& strcmp(name,cursor_plane->plane.id)!=0)
    {
        cursor_plane=cursor_plane->next_plane_company;
    }
    if(cursor_plane != NULL && strcmp(name,cursor_plane->plane.id)==0)
    {
        return cursor_plane;
    }
    return NULL;
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
