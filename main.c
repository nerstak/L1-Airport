#include <stdio.h>
#include <stdlib.h>
#include "data_access.h"
#include "log_read.h"

int main()
{
    //Actually to test the viability of functions
    Companies_list list_company=NULL;
    new_cell_company(&list_company,"Doctor","DCT");
    printf("%s\n",list_company->company.acronym);
    new_cell_plane("DCT042",2,50,"1337",list_company);
    new_cell_plane("DCT056",2,50,"1338",list_company);
    new_cell_plane("DCT059",2,50,"1339",list_company);
    printf("%s",list_company->company.planes_company->next_plane_company->next_plane_company->plane.id);
    //new_cell_company(&list_company);
}
