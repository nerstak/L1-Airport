#ifndef LOG_WRITE_H_INCLUDED
#define LOG_WRITE_H_INCLUDED
#include "data_access.h"

void generate_action_log(char action,Plane * plane_selected,char *stime,char * event);

void create_log();

void write_action(char * event);

#endif // LOG_WRITE_H_INCLUDED
