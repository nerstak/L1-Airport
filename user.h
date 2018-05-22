#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED
void user_interaction(lists_present_planes * present_planes, Companies_list * all_companies,Companies_list * blacklisted_companies, int time);
void print_companies(Companies_list companies);
void planes_status(Company * companies,int unused,lists_present_planes * present_planes); // if unused==0, it prints all, if 1 it only prints used planes
void print_planes(Planes_list cur,int landing);
#endif // USER_H_INCLUDED
