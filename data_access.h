#ifndef DATA_ACCESS_H_INCLUDED
#define DATA_ACCESS_H_INCLUDED


//Structure of a company
typedef struct company{
    char name[15];
    char acronym[3];
    Plane planes_company;
}Company;

//Structure of a plane
typedef struct plane{
    char id[7];
    int fuel;
    int comsumption;
    char takeoff_time[4];
    Company* company;
}Plane;

//Structure of the cell of a plane
typedef struct cell_plane{
    Plane plane;
    struct cell_plane* next_plane_company;// Pointer on the next plane of the company
    struct cell_plane* previous_plane_company;// Pointer on the previous plane of the company
    struct cell_plane* next_waiting;// Pointer on the next plane wanting (take off or landing)
}Cell_plane;

//lists takeoff_wait and landing_wait will be of type Plane_list
typedef Cell_plane *Planes_list;


//Structure of the cell of a company
typedef struct cell_company{
    Company company;
    struct cell_company* next_company;
    struct cell_company* previous_company;
}Cell_company;

typedef Cell_company *Companies_list;


/* definition of the immediate takeoff queue list */
typedef struct {
    Cell_plane * first;
    Cell_plane * last;
}Takeoff_list;


void initTakeoff(Takeoff_list* Que);
void pushTakeoff(Takeoff_list* Que, Cell_plane *nElt);
Cell_plane* popTakeoff(Takeoff_list* Que);
void displayTakeoff(Takeoff_list *Que);

void new_company(Company * newcomp);







char * time2string(int intime);



#endif // DATA_ACCESS_H_INCLUDED
