#include <stdio.h>
#include <stdlib.h>
#include "log_write.h"

char * generate_action_log(char * type_action,Plane * plane_selected,char *stime)
{
    char line[20];
    strcpy(line,plane_selected->id);
    strcat(line,"-");
    if(strcmp(type_action,"A_LANDING")==0 || strcmp(type_action,"U_LANDING")==0 || strcmp(type_action,"N_LANDING")==0 || strcmp(type_action,"CRASH"))
    {
        line[7]=type_action[0];
        strcat(line,"-");
        strcat(line,stime);
        strcat(line,"-");
        strcat(line,plane_selected->fuel);
        strcat(line,"-");
        strcat(line,plane_selected->comsumption);
    }
    else if (strcmp(type_action,"TAKE_OFF"))
    {
        line[7]='D';
        strcat(line,"-");
        strcat(line,stime);
    }
    return line;
}

void create_log()
{
    FILE * log_file = NULL;
    log_file = fopen("cfg/report.log","w+");
    fclose(log_file);
}

void write_action(char * type_action,Plane * plane_selected,char *stime)
{
    FILE * log_file = NULL; //Opening the log file
    char tmp[1];
    log_file = fopen("cfg/report.log","a");
    if (log_file==NULL)
    {
        return NULL;
    }
    fputs(generate(type_action,plane_selected,stime));
    fclose(log_file);
}
