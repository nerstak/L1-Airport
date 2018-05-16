#include <stdio.h>
#include <stdlib.h>

#include "log_read.h"
#include "data_access.h"

void seperate(int beginning, int ending, char * words, char * result)
{
    char temp[2];
    strcpy(result,"");
    for(int i=beginning;i<ending;i++) //Checking if the event have to be triggered
    {
        temp[0]=words[i]; //Converting a caracter pickup into a string
        temp[1]='\0';
        strcat(result,temp);
    }
}

char * events_reading(int *num_line, char *time, Companies_list * list_company)
{
    FILE * events_file = NULL; //Opening the file events.log
    char line[100],time_event[5],temp[2];
    events_file = fopen("cfg/events.log","r");
    if (events_file==NULL)
    {
        return NULL;
    }
    fgets(line,5,events_file);
    seperate(0,4,line,time_event);
    ++*num_line;
    printf("Time_file=%s\n",time_event);
    if(strcmp(time,time_event)==0)
    {
        while(fgets(line,25,events_file)!=NULL && line[4]!=":")
        {
            printf("%s\n",line);
            events_execution(line,list_company);
        }
    }
    else
    {
        printf("Nope\n");
    }

}

void events_execution(char *event,Companies_list * list_company)
{
    //First decomposition
    char temp[2],type_event;
    if(event[7]=='A' || event[7]=='D')
    {
        char name[7],acro_comp[4];
        Companies_list ptr_comp;
        seperate(0,3,event,acro_comp);
        seperate(0,6,event,name);
        printf("Acr: %s\nName: %s\n",acro_comp,name);
        ptr_comp=search_company(list_company,acro_comp);
        if(ptr_comp==NULL)
        {
            char new_name_comp[15];
            printf("Name of the company corresponding to \"%s\"?",acro_comp);
            scanf("%s",new_name_comp);
            new_cell_company(list_company,new_name_comp,acro_comp);
            ptr_comp=search_company(list_company,acro_comp);
        }
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
    fclose(companies_file);
    return *list_company;
}
