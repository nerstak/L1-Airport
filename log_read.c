#include <stdio.h>
#include <stdlib.h>

#include "log_read.h"
#include "data_access.h"

///Line should be "HHMM TYP STUFF_EXEC", ONLY one is executed at the time

int events_reading(int *num_line, char *time)
{
    FILE * events_file = NULL; //Opening the file events.log
    char line[100],time_event[5],temp[2];
    strcpy(time_event,"");
    strcpy(line,"");
    events_file = fopen("cfg/events.log","r");
    if (events_file==NULL)
    {
        return NULL;
    }
    printf("%d",*num_line);
    for(int i=0;i<*num_line;i++) //Going to the last line
    {
        fgets(line,100,events_file);
    }
    ++*num_line;
    fgets(line,100,events_file);
    for(int i=0;i<4;i++) //Checking if the event have to be triggered
    {
        temp[0]=line[i]; //Converting a caracter pickup into a string
        temp[1]='\0';
        strcat(time_event,temp);
    }
    if(strcmp(time,time_event)==0)
    {
        printf("THTH\n"); //Replace with events_execution()
    }
    else
    {
        printf("Nope\n");
    }

}

void events_execution(char *event)
{
    //First decomposition
    char temp[2],type_event[4];
    for(int i=5;i<8;i++) //Checking if the event have to be triggered
    {
        temp[0]=event[i]; //Converting a caracter pickup into a string
        temp[1]='\0';
        strcat(type_event,temp);
    }
    if(strcmp(type_event,"ADD")==0)
    {
        char id_plane[7], list_to_add;
        for(int i=9;i<15;i++) //Checking if the event have to be triggered
        {
            temp[0]=event[i]; //Converting a caracter pickup into a string
            temp[1]='\0';
            strcat(id_plane,temp);
        }
        list_to_add=event[16];
        ///To Finish
    }

}

Companies_list  setup_companies()
{
    Companies_list * list_company=malloc(sizeof(Companies_list));
    *list_company=NULL;
    FILE * companies_file = NULL; //Opening the file events.log
    char line[100],company[15],acronym[4],temp[2];
    strcpy(line,"");
    companies_file = fopen("cfg/companies.cfg","r");
    if(companies_file==NULL)
    {
        return NULL;
    }
    while (fgets(line,100,companies_file)!=NULL)
    {
        strcpy(company,"");
        strcpy(acronym,"");
        int i=0;
        while(i<14 && line[i]!='=')
        {
            temp[0]=line[i]; //Converting a caracter pickup into a string
            temp[1]='\0';
            strcat(company,temp);
            i++;
        }
        for(++i;i<strlen(company)+4;i++)
        {
            temp[0]=line[i]; //Converting a caracter pickup into a string
            temp[1]='\0';
            strcat(acronym,temp);
        }
        new_cell_company(list_company,company,acronym);
    }
    return *list_company;
}
