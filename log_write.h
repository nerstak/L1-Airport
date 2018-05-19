#ifndef LOG_WRITE_H_INCLUDED
#define LOG_WRITE_H_INCLUDED
#include "data_access.h"

char * generate_action_log(char * type_action,Plane * plane_selected,char *stime);

void create_log();

void write_action(char * type_action,Plane * plane_selected,char *stime);

#endif // LOG_WRITE_H_INCLUDED
