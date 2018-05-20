#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_read.h"
#include "data_access.h"

void seperate(int beginning, int ending, char * words, char * result) //Takes a string (words) and the range of chars it should take from that string and puts them in result
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

char * events_reading(int *num_line, char *time, Companies_list * list_company, Companies_list * blacklisted_company, lists_present_planes *list_planes_used) //reads a line and sees if it is time to run that script, if so it runs that script
{
    FILE * events_file = NULL; //Opening the file events.log
    char line[100],time_event[5],temp[2];
    int follow=1;
    events_file = fopen("cfg/events.log","r");
    if (events_file==NULL)
    {
        return NULL;
    }
    while(fgets(line,5,events_file)!=NULL && follow)
    {
        seperate(0,4,line,time_event);
        if(strcmp(time,time_event)==0)
            follow=0;
    }
    if(strcmp(time,time_event)==0)
    {
        printf("Time_file=%s\n",time_event);
        while(fgets(line,25,events_file)!=NULL && line[4]!=':')
        {
            events_execution(line,list_company, blacklisted_company,list_planes_used);
        }
    }
    else
    {
        //printf("Nope\n");
    }
    fclose(events_file);
}

void events_execution(char *event,Companies_list * list_company, Companies_list * blacklisted_company,  lists_present_planes *list_planes_used)
{
    //First decomposition
    char temp[2],type_event;
    if((event[7]=='A' || event[7]=='D' || event[7]=='U')&& secure_entry(event)==1) //Checking if
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
        if(presence_in_lists(list_planes_used->boarding,id)==0 && presence_in_lists(list_planes_used->landing,id)==0 && presence_in_lists(list_planes_used->emergency,id)==0 && presence_in_lists(list_planes_used->takeoff->first,id)==0 && presence_in_lists(list_planes_used->blacklist,id)==0) //Checking if the plane is not currently used
        {
            char takeoff_time[5],fuel[3],consumption[3];
            Cell_plane * ptr_plane;
            printf("how bout this %s",id);
            ptr_plane=search_cell_plane(ptr_comp->company.planes_company,id);
            seperate(9,13,event,takeoff_time);
            seperate(14,16,event,fuel);
            seperate(17,19,event,consumption);
            if(ptr_plane==NULL) //Creating the plane if it doesn't exists
            {
                new_cell_plane(id,atoi(consumption),atoi(fuel),takeoff_time,ptr_comp);
                ptr_plane=search_cell_plane(ptr_comp->company.planes_company,id);
            }
            else //Or updating the parameters
            {
                strcpy(ptr_plane->plane.takeoff_time,takeoff_time);
                ptr_plane->plane.fuel=atoi(fuel);
                ptr_plane->plane.comsumption=atoi(fuel);

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
    if(strcmp("BLACKLIST",keyword)==0) ///BLACKLIST
    {
        char acro_comp[4];
        Companies_list ptr_comp;
        seperate(10,13,event,acro_comp);
        printf("%s!",acro_comp);
        ptr_comp=search_company(list_company,acro_comp);
        if(ptr_comp!=NULL)
        {
            new_cell_company(blacklisted_company,ptr_comp->company.name,ptr_comp->company.acronym);
            printf("\nDONE\n");
        }
        //Add to the dictionnary of blacklisted companies.
    }
}

Companies_list setup_companies()
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

void read_log(int lines_to_read)
{
    FILE * log_file = NULL; //Opening the file events.log
    char line[100]={0,};
    int i=0;
    log_file = fopen("cfg/report.log","r");
    if (log_file==NULL)
    {
        return NULL;
    }
    while(fgets(line,100,log_file)!=NULL && i<lines_to_read)
    {
        printf("%s\n",line);
        if(i!=0 && i%9==0)
            getchar();
        ++i;
    }
    fclose(log_file);
}

int secure_entry(char * line) //Function to verify that everything the user enters is conform to what we want
{
    int secure=1;
    int i=0;
    for(i;i<3;i++)//Check that 3 first characters are letters
    {
        if(line[i]>90 || line[i]<65)
            secure=0;
    }
    for(i;i<6;i++)//Check that following 3 characters are numbers
    {
        if(line[i]>'9')
            secure=0;
    }
    if(line[6]!='-' || line[8]!='-' || line[13]!='-' || line[16]!='-')//Check that the union--sign are at the good place
        secure=0;
    if(line[7]=='D')
    {
        for(i=9;i<13;i++)
        {
            if(line[i]>90 || line[i]<65)
                secure=0;
        }
    }
    else if(line[7]=='U' || line[7]=='A')
    {
        i=14;
        while(i<19)
        {
            if(i!=16)
            {
                if(line[i]>'9')
                    secure=0;
            }
            ++i;
        }
    }
    return secure;
}
