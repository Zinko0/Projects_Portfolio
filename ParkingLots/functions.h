/**
 * Header file with all the functions declarations.
 * 
 * @file functions.h
 * @author Rodrigo Zink ist1109356 
*/

#ifndef functions_h
#define functions_h

/**CONSTANTS AND STRUCTS*/
#include "const_struct.h"

/**C STANDARD LIBRARY INCLUDE*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
   
/**PARK FUNCTIONS DECLARATIONS*/

node_t *read_park_cmd(node_t *head,char *cmd,char p_sorted[MAX_PARK][BUFF_SIZE],int *len);

park_t *get_park(node_t *head,char *str);

void print_park(node_t *head);

int read_park(node_t *head,park_t *p,int len,char *cmd);

int check_park(node_t *head, park_t *p, int len);

int read_park(node_t *head,park_t *p,int len,char *cmd);

node_t *insert_node(node_t *head,park_t ipark);

void sort_parks(char *park_name,char parks_sorted[MAX_PARK][BUFF_SIZE],int len);

node_t* erase_park(node_t *head,char *park_name);

node_t* erase_all_parks(node_t *head,array_t *array);

void erase_p_sorted(char *park_name,char parks_sorted[MAX_PARK][BUFF_SIZE],int len);

void print_park_sorted(char parks_sorted[MAX_PARK][BUFF_SIZE],int len);

node_t *remove_park(node_t *head,char *cmd, array_t *array,char parks_sorted[MAX_PARK][BUFF_SIZE],int *len);

/*CARS FUNCTIONS DECLARATIONS */

void read_car_cmd(node_t *head,char *cmd,date_t *curr_time,array_t *array,int mode);

void init_array(array_t *array);

unsigned int hash(char *plate);

int linear_probing(int hash_value,array_t *array,char *plate);

void insert_car(array_t *array,car_t *icar,char mode);

void insert_car_in_hash(array_t *array,char *park_name,char mode,int hash_value,car_node_t *car);

car_t *get_car_inside(array_t *array,char *plate);

int check_in_out(array_t *array,char *plate);

int check_plate(char *plate);

int check_time(date_t itime,date_t *curr_time);

int check_car_routine(park_t *park,car_t *icar,char mode);

int check_car(node_t *head_park,car_t *icar,date_t *curr_time,array_t *array,char mode);

void car_record(car_t *icar,date_t *record,char mode);

char *read_car(node_t *head_park,car_t *icar,char *cmd,date_t *curr_time,array_t *array);

void registry(array_t *array,char *cmd);

void regist_car(node_t *head,car_t *icar,array_t *array,char mode);

void print_registry(array_t *array,char *plate);

void re_insert(array_t *array,int hash_value);

void remove_cars(char *park_name,char *plate,array_t *array);

void free_car_node(car_node_t *node);

void print_exit(char *plate,date_t entry,date_t exit,double cost);

/**TIME FUNCTION DECLARATIONS*/

int invalidDate(date_t d);

long int when(date_t d1);

long int diffDate(date_t d1,date_t d2);

double payment(park_t *p,date_t entry,date_t exit);

date_t date_null(date_t d);

date_t hour_null(date_t d);

date_t read_bills(char *cmd,char *park_name);

void print_all_bills(bill_t *head);

bill_t *insert_bill_t(bill_t *head,bill_t *prev,bill_t *curr,double cost,date_t exit,char plate[PLATE_SIZE]);

bill_t *insert_bill(bill_t *head,date_t exit,double cost);

bill_t *erase_all_bills(bill_t *head);

bill_t *insert_day_bill(bill_t *head,char plate[PLATE_SIZE],date_t exit,double cost);

bill_t *get_bills(park_t *p,array_t *array);

bill_t *get_day_bills(park_t *p,array_t *array,date_t record);

void print_day_bills(bill_t *head);

void bills(char *cmd,node_t *head,date_t *curr_time,array_t *array);

#endif