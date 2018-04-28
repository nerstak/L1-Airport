#include <stdio.h>
#include <stdlib.h>
#include "data_access.h"

void new_company()
{
    char name[10];
    Company *temp=malloc(sizeof(Company));
    printf("Name of the company?");
    scanf("%s",temp->name);
}

