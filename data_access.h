#ifndef DATA_ACCESS_H_INCLUDED
#define DATA_ACCESS_H_INCLUDED

//Structure of a company
typedef struct company{
    char* name;
    char acronym[3];
    Planes_list planes_company;
}Company;

//Structure of a plane
typedef struct plane{
    char id[7];
    int fuel;
    int comsumption;
    char hour_takeoff[4];
    Company* company;
}Plane;

//Structure of the cell of a plane
typedef struct cell_plane{
    Plane plane;
    struct cell_plane* next_plane_company;// Pointer on the next plane of the company
    struct cell_plane* previous_plane_company;// Pointer on the previous plane of the company
    struct cell_plane* next_waiting;// Pointer on the next plane wanting (take off or landing)
}Cell_plane;

typedef Cell_plane *Planes_list;


//Structure of the cell of a company
typedef struct cell_company{
    Company company;
    struct cell_company* next_company;
}Cell_company;

typedef Cell_company *Companies_list;




/* definition of the landing queue list */
typedef struct {
    Planes_list * first;
    Planes_list * last;
}Takeoff_list;

void initTakeoff(Takeoff* Que);
void pushTakeoff(Takeoff* Que, Cell_plane *nElt);
Cell_plane* popTakeoff(Takeoff* Que);
void displayTakeoff(Takeoff *Que);




#endif // DATA_ACCESS_H_INCLUDED
