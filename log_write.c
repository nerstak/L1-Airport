#include <stdio.h>
#include <stdlib.h>
#include "log_write.h"

void generate_action_log(char action,Plane * plane_selected,char *stime,char * event)
{
    if(action=='D')
    {
        sprintf(event,"%s-%c-%s------",strupr(&(plane_selected->id)),action,stime);
    }
    else
    {
        sprintf(event,"%s-%c------%d%d-%d%d",strupr(&(plane_selected->id)),action,(plane_selected->fuel)/10,(plane_selected->fuel)%10,(plane_selected->comsumption)/10,(plane_selected->comsumption)%10);
    }
}

void create_log()
{
    FILE * log_file = NULL;
    log_file = fopen("cfg/report.log","w+");
    fclose(log_file);
}

void write_action(char * event)
{
    FILE * log_file = NULL; //Opening the log file
    char tmp[1];
    log_file = fopen("cfg/report.log","a");
    if (log_file==NULL)
    {
        return NULL;
    }
    fputs(event,log_file);
    fclose(log_file);
}
