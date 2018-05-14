#ifndef LOG_READ_H_INCLUDED
#define LOG_READ_H_INCLUDED
#include "data_access.h"

int events_reading(int *num_line, char *time);

void events_execution(char *event);

Companies_list setup_companies();

#endif // LOG_READ_H_INCLUDED
