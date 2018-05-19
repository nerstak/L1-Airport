#ifndef LOG_READ_H_INCLUDED
#define LOG_READ_H_INCLUDED
#include "data_access.h"

void seperate(int beginning, int ending, char * words, char * result);

char * events_reading(int *num_line, char *time, Companies_list * list_company, Companies_list * blacklisted_company, lists_present_planes *list_planes_used);

void events_execution(char *event, Companies_list * list_company, Companies_list * blacklisted_company, lists_present_planes *list_planes_used);

Companies_list setup_companies();

void read_log(int lines_to_read)

#endif // LOG_READ_H_INCLUDED
