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

char * events_reading(int *num_line, char *time, Companies_list * list_company, lists_present_planes *list_planes_used)
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
            events_execution(line,list_company,list_planes_used);
        }
    }
    else
    {
        printf("Nope\n");
    }

}

void events_execution(char *event,Companies_list * list_company, lists_present_planes *list_planes_used)
{
    //First decomposition
    char temp[2],type_event;
    if(event[7]=='A' || event[7]=='D' || event[7]=='U') //Checking if
    {
        char id[7],acro_comp[4];
        Companies_list ptr_comp;
        seperate(0,3,event,acro_comp);
        seperate(0,6,event,id);
        printf("Acr: %s\nName: %s\n",acro_comp,id);
        ptr_comp=search_company(list_company,acro_comp);
        if(ptr_comp==NULL)//Creation of the company if it doesn't exist
        {
            char new_name_comp[15];
            printf("Name of the company corresponding to \"%s\"?",acro_comp);
            scanf("%s",new_name_comp);
            new_cell_company(list_company,new_name_comp,acro_comp);
            ptr_comp=search_company(list_company,acro_comp);
        }
        if(presence_in_lists(list_planes_used->boarding,id)==0 && presence_in_lists(list_planes_used->landing,id)==0 && presence_in_lists(list_planes_used->emergency,id)==0 && presence_in_lists(list_planes_used->takeoff->first,id)==0) //Checking if the plane is not currently used
        {
            char takeoff_time[5],fuel[3],consumption[3];
            Cell_plane * ptr_plane;
            ptr_plane=search_cell_plane(&ptr_comp->company.planes_company,id);
            seperate(9,13,event,takeoff_time);
            seperate(14,16,event,fuel);
            seperate(17,19,event,consumption);
            if(ptr_plane==NULL) //Creating the plane if it doesn't exists
            {
                new_cell_plane(id,atoi(consumption),atoi(fuel),takeoff_time,ptr_comp);
                ptr_plane=search_cell_plane(&ptr_comp->company.planes_company,id);
            }
            else //Or updating the parameters
            {
                ptr_plane->plane.fuel=atoi(fuel);
                ptr_plane->plane.comsumption=atoi(fuel);
                strcpy(ptr_plane->plane.takeoff_time,takeoff_time);
            }
            switch(event[7])
            {
            case 'A':
                add_to_list(&list_planes_used->landing,ptr_plane);
                break;
            case 'D':
                add_to_list(&(list_planes_used->boarding),ptr_plane);
                break;
            case 'U':
                add_to_list(&list_planes_used->emergency,ptr_plane);
                break;
            }
        }
    }

    char keyword[10];
    seperate(0,9,event,keyword);
    if(strcmp("BLACKLIST",keyword)==0)
    {
        //Add to the dictionnary of blacklisted companies. Then, each turn verifies if the company's plane isn't on the list during sort, however go to emergency
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

void add_to_list(Planes_list * list,Cell_plane *newCellPlane)
{
    Planes_list cursor;
    cursor=*list;
    while(cursor!=NULL && cursor->next_waiting!=NULL)
    {
        printf("zz %s\n",cursor->plane.id);
        cursor=cursor->next_waiting;
    }
    if(cursor==NULL)
        *list=newCellPlane;
    else
        cursor->next_waiting=newCellPlane;
    printf("a%d",newCellPlane);
}
