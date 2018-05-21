#include <stdio.h>
#include <stdlib.h>
#include "data_access.h"

Company * new_company(char *name,char *acronym) //creates a new company from name and acronym
{
    Company * newComp=malloc(sizeof(Company));
    strcpy(newComp->name,name);
    strcpy(newComp->acronym,acronym);
    newComp->planes_company=NULL;
    return newComp;
}

void new_cell_company(Companies_list * list_company,char *name,char *acronym)//adds a cell company to the list of companies
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

Plane * new_plane(char *id, int comsumption, int fuel, char *takeoff_time, Company *ptr_comp)//Creates a new Plane element
{
    Plane * newPlane=malloc(sizeof(Plane));
    strcpy(newPlane->id,id);
    newPlane->comsumption=comsumption;
    newPlane->fuel=fuel;
    strcpy(newPlane->takeoff_time,takeoff_time);
    newPlane->company=ptr_comp;
    return newPlane;
}

void new_cell_plane(char *id,int comsumption, int fuel, char *takeoff_time, Company *ptr_comp) // Creates a plane in a company's list
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
        newCellPlane->previous_plane_company=cursor_plane;                          //Theo added this line, remove it if I goofed up
    }



}

Company * search_company(Companies_list * list_company, char * acronym) //Goes through a list of companies to find a company from an acronym
{
    if(list_company!=NULL)
    {
        Companies_list cursor_company;
        cursor_company=*list_company;
        while(cursor_company != NULL)
        {
            if(strcmp(acronym,cursor_company->company.acronym)==0)
                return cursor_company;
            else
                cursor_company=cursor_company->next_company;
        }
    }
    return NULL;
}

Cell_plane * search_cell_plane(Planes_list planes_company, char * id) //Search a plane in the list of company, using the ID
{
    Planes_list cursor_plane=planes_company;
    while(cursor_plane != NULL)
    {
        if(strcmp(id,cursor_plane->plane.id)==0)
            return cursor_plane;
        else
            cursor_plane=cursor_plane->next_plane_company;
    }
    return NULL;
}

int presence_in_lists(Planes_list list_planes, char * id) //If a plane is in a list, it returns 1, else 0
{
    Planes_list cur;
    cur=list_planes;
    while(cur!=NULL && strcmp(cur->plane.id,id)!=0)
        cur=cur->next_waiting;
    if(cur!=NULL && strcmp(cur->plane.id,id)==0)
        return 1;
    return 0;
}


void init_present_planes(lists_present_planes ** present_planes)
{
    *present_planes=malloc(sizeof(lists_present_planes));
    (*present_planes)->blacklist=NULL;
    (*present_planes)->boarding=NULL;
    (*present_planes)->emergency=NULL;
    (*present_planes)->landing=NULL;
    (*present_planes)->takeoff=malloc(sizeof(Takeoff_list));
    (*present_planes)->takeoff->first=NULL;
    (*present_planes)->takeoff->last=NULL;
}


void initTakeoff(Takeoff_list* Que)
{
    Que->first=NULL;
    Que->last=NULL;
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
