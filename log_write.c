#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_write.h"

void generate_action_log(char action,Plane * plane_selected,char *stime,char * event)
{
    if(action=='D')
    {
        sprintf(event,"%s-%c-%s------",&(plane_selected->id),action,stime);
    }
    else
    {
        sprintf(event,"%s-%c------%d%d-%d%d",&(plane_selected->id),action,(plane_selected->fuel)/10,(plane_selected->fuel)%10,(plane_selected->comsumption)/10,(plane_selected->comsumption)%10);
    }
}

void create_log()
{
    FILE * log_file = NULL;
    log_file = fopen("report.log","w+");
    fclose(log_file);
}

void write_action(char * event,char * stime,int runway)
{
    FILE * log_file = NULL; //Opening the log file
    char tmp[1],history[30];
    sprintf(history,"%d_%c%c:%c%c - %s",runway+1,stime[0],stime[1],stime[2],stime[3],event);
    log_file = fopen("report.log","a");
    if (log_file==NULL)
    {
        return NULL;
    }
    fputs(history,log_file);
    fputs("\n",log_file);
    fclose(log_file);
}
