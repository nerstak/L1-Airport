#ifndef DATA_ACCESS_H_INCLUDED
#define DATA_ACCESS_H_INCLUDED

#include <string.h>

//Structure of a plane
typedef struct Plane{
    char id[7];
    int fuel;
    int comsumption;
    char takeoff_time[4];
    struct Company* company;
}Plane;

//Structure of the cell of a plane
typedef struct Cell_plane{
    Plane plane;
    struct Cell_plane* next_plane_company;// Pointer on the next plane of the company
    struct Cell_plane* previous_plane_company;// Pointer on the previous plane of the company
    struct Cell_plane* next_waiting;// Pointer on the next plane waiting (take off or landing)
}Cell_plane;

//lists takeoff_wait and landing_wait will be of type Plane_list
typedef Cell_plane *Planes_list;


//Structure of a company
typedef struct Company{
    char name[15];
    char acronym[4];
    Planes_list planes_company;
}Company;



//Structure of the cell of a company
typedef struct Cell_company{
    Company company;
    struct cell_company* next_company;
    struct cell_company* previous_company;
}Cell_company;

typedef Cell_company *Companies_list;


/* definition of the immediate takeoff queue list */
typedef struct Takeoff_list{
    Planes_list first;
    Planes_list last;
}Takeoff_list;

typedef struct list_present_planes{
    Takeoff_list * takeoff;
    Planes_list boarding;
    Planes_list emergency;
    Planes_list blacklist;
    Planes_list landing;
} lists_present_planes;

Company * new_company(char *name,char *acronym); //Create a new company
void new_cell_company(Companies_list * list_company,char *name,char *acronym); //Create the new cell of a company
Plane * new_plane(char *id, int comsumption, int fuel, char *takeoff_time, Company *ptr_comp);
void new_cell_plane(char *id,int comsumption, int fuel, char *takeoff_time, Company *ptr_comp);

Company * search_company(Companies_list * list_company, char * acronym); //Search a company through an acronym
Cell_plane * search_cell_plane(Planes_list  planes_company, char * name); //Search a plane through an ID

void init_present_planes(lists_present_planes ** present_planes);
#endif // DATA_ACCESS_H_INCLUDED
