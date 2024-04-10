/**
 * Program that manages the time and the cost of parking cars in parks.
 * 
 * @file time.c
 * @author Rodrigo Zink ist1109356
*/

#include "functions.h"

/**
 * @brief Checks if a given date is valid.
 * 
 * @param d The date to be checked for validity.
 * @return Returns 0 if the date is valid, and 1 otherwise.
 */
int invalidDate(date_t d){ 
    int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(d.mon < 1 || d.mon > 12){
        return 1;
    }
    if(d.day < 1 || d.day > days[d.mon -1 ]){
        return 1;
    }
    if(d.hour < 0 || d.hour >= 24 ){
        return 1;
    }
    if(d.min < 0 || d.min >= 60){
        return 1;
    }
    return 0;
}

/**
 * @brief Calculates the number of total minutes since the year 0.
 * 
 * @param d The date to be converted to minutes.
 * @return Returns the number of minutes since the year 0.
*/
long int when(date_t d){
    int min = d.year*MIN_YEAR;
    int day = 0,i;
    int days[]={31,28,31,30,31,30,31,31,30,31,30,31};

    for(i=0;i<(d.mon-1);i++){
        day += days[i]; 
    }

    return min + (day+(d.day))*24*60+d.hour*60+d.min;
}

/**
 * @brief Calculates the difference in minutes between two dates.
 * 
 * @param d1 The first date.
 * @param d2 The second date.
 * @return Returns the difference in minutes between the two dates.
*/
long int diffDate(date_t d1,date_t d2){
    return when(d2)-when(d1);
}


/**
 * @brief Calculates the cost of parking a car in a given park.
 * 
 * @param p The park where the car is parked.
 * @param entry The date and time the car entered the park.
 * @param exit The date and time the car exited the park.
 * @return Returns the cost of parking the car in the park.
*/
double payment(park_t *p,date_t entry,date_t exit){
    /**setting the variables to calculate the cost*/
    long int time = diffDate(entry,exit);
    int rest, interval;
    double cost = 0; 
    /**calculate the cost of the days*/
    double days_cost = (p->z) * (time/MIN_DAY);
    /**rest is the time excluding the days*/
    rest = time % MIN_DAY;

    /**get the total number of intervals of 15 minutes of that rest*/
    interval = rest / INTERVAL;
    /**in case the rest is not a multiple of 15 minutes*/
    if(rest % INTERVAL != 0){
        interval++;
    }
    /**calculate the cost with the x , y , z values of the park*/
    if((interval-QUARTERS_IN_HOUR) > 0){
        cost += (p->x)*QUARTERS_IN_HOUR;
        cost += (interval-QUARTERS_IN_HOUR) * (p->y);
    }else{
        cost += (p->x)*interval;
    }
    if(cost > (p->z)){
        cost = p->z;
    }
    return (cost + days_cost);
}

/**
 * @brief Sets the given date to 0.
 * 
 * @param d The date to be set as 0.
 * @return Returns the date set to 0.
*/
date_t date_null(date_t d){
    d.day = 0; d.mon = 0; d.year = 0;
    return d;
}

/**
 * @brief Sets the hour and minutes of the given date to 0.
 * 
 * @param d The date to be putting as null.
 * @return Returns the date set to 0.
*/
date_t hour_null(date_t d){
    d.hour = 0; d.min = 0;
    return d;
}

/**
 * @brief Reads the command f and returns the date and gets the park name.
 * 
 * @param cmd The command to be read.
 * @param park_name The name of the park.
 * @return Returns the date read from the command.
*/
date_t read_bills(char *cmd,char *park_name){
    /**initialize the record to 0 in case the command is f <park_name>*/
    date_t record = {0};

    if(sscanf(cmd,"f \"%[^\"]\" %d-%d-%d",
    park_name,&record.day,&record.mon,&record.year) == 4){ 
    
    }
    else if(sscanf(cmd,"f \"%[^\"]\" ",park_name) == 1){
    }
    else if(sscanf(cmd,"f %s %d-%d-%d",
    park_name,&record.day,&record.mon,&record.year) == 4){
    }
    else if(sscanf(cmd,"f %s ",park_name)){
    }
    return record;
}

/**
 * @brief Prints all the bills of a given park.
 * 
 * @param head The head of the list of bills.
*/
void print_all_bills(bill_t *head){
    bill_t *curr = head;
    if(head == NULL){
        return;
    }
    while(curr != NULL){
        printf("%2.2d-%2.2d-%2.2d %.2lf\n",
        curr->exit.day,curr->exit.mon,curr->exit.year,
        curr->cost);
        curr = curr->link;
    }
    return;
}

/**
 * @brief Inserts a bill_t in a list that already has already a head.
 * 
 * @param head The head of the list.
 * @param prev The previous node in the list.
 * @param curr The current node in the list.
 * @param cost The cost of parking the car in the park.
 * @param exit The date the car exited the park.
 * @param plate The plate of the car.(only if called by insert_day_bill)
 * @return Returns the head of the list.
*/
bill_t *insert_bill_t(bill_t *head,bill_t *prev,bill_t *curr,double cost,date_t exit,char plate[PLATE_SIZE]){
    bill_t *node = malloc(sizeof(bill_t));
    /**inserts in the begining*/
    if (prev == NULL) {
        node->exit = exit;
        node->cost = cost;
        node->link = head;
        head = node;
    }else{ /**inserts in the middle or end*/
        node->exit = exit;
        node->cost = cost;
        prev->link = node;
        node->link = curr;
    }/**if its called by insert_day_bill exit.year was set to 0*/
    if(exit.year == 0){
        strcpy(node->plate,plate);
    }
    return head;
}


/**
 * @brief Inserts a bill in the list of bills.
 * 
 * @param head The head of the list of bills.
 * @param exit The date the car exited the park.
 * @param cost The cost of parking the car in the park.
 * @return Returns the head of the list of bills.
*/
bill_t *insert_bill(bill_t *head,date_t exit,double cost){
    /**set the hour and minutes to 0*/
    exit = hour_null(exit);  
    char no_plate[PLATE_SIZE] = {0};
    /**setting the variables to go trough the list*/
    bill_t *new = NULL;
    new = malloc(sizeof(bill_t));
    bill_t *curr = head;
    bill_t *prev = NULL;

    /**if the list is empty update the head*/
    if(head == NULL){
        free(new);
        head = malloc(sizeof(bill_t));
        head->exit = exit;
        head->cost = cost;
        head->link = NULL;
        return head;
    }
    while(curr != NULL){
        if((diffDate(curr->exit,exit) < 0)){
            /**node inserted in the middle or begining*/
            head = insert_bill_t(head,prev,curr,cost,exit,no_plate);
            return head;
        }
        if(diffDate(curr->exit,exit) == 0){
            /**no new node inserted*/
            free(new);
            curr->cost += cost;
            return head;
        }
        prev = curr;
        curr = curr->link;
    } 
    /**node inserted at the end*/    
    head = insert_bill_t(head,prev,curr,cost,exit,no_plate);
    return head; 
}


/**
 * @brief Erases all the bills of a given park.
 * 
 * @param head The head of the list of bills.
 * @return Returns the head of the list of bills.
*/
bill_t *erase_all_bills(bill_t *head){
    bill_t *curr = head;
    
    /**goes trough the list freeing the memory of each node*/
    while(curr != NULL){
        curr = curr->link;
        free(head);
        head = curr;
    }
    return head;
}

/**
 * @brief Inserts a bill in the list of bills of a given day.
 * 
 * @param head The head of the list of bills.
 * @param plate The plate of the car.
 * @param exit The date the car exited the park.
 * @param cost The cost of parking the car in the park.
 * @return Returns the head of the list of bills.
*/
bill_t *insert_day_bill(bill_t *head,char plate[PLATE_SIZE],date_t exit,double cost){
    /**set the hour and minutes to 0*/
    exit = date_null(exit);
    /**setting the variables to go trough the list*/
    bill_t *new = NULL;
    new = (bill_t *)malloc(sizeof(bill_t));
    bill_t *curr = head;
    bill_t *prev = NULL;

    /**if the list is empty*/
    if(head == NULL){
        head = (bill_t *)malloc(sizeof(bill_t));
        free(new);  
        head->exit = exit;
        head->cost = cost;
        strcpy(head->plate,plate);
        head->link = NULL;
        return head;
    }
    while(curr != NULL){
        if((diffDate(curr->exit,exit) <= 0)){
            if(prev == NULL){
                /**new node inserted at the beginning*/
                head = insert_bill_t(head,prev,curr,cost,exit,plate);
                return head;
            }
            /**new node inserted in the middle*/
            head = insert_bill_t(head,prev,curr,cost,exit,plate);
            return head;
        }
        prev = curr;
        curr = curr->link;
    }    
    /**new node inserted at the end*/
    head = insert_bill_t(head,prev,curr,cost,exit,plate);
    return head; 
}
    

/**
 * @brief Gets all the bills of a given park.
 * 
 * @param p The park to get the bills from.
 * @param array Pointer to the struct car_node.
 * @return Returns the head of the list of bills.
*/
bill_t *get_bills(park_t *p,array_t *array){
    int i = 0;
    unsigned int hash_value = 0;
    car_node_t *curr = NULL;
    date_t record = {0};
    bill_t *bill_head = NULL;
    double cost = 0;
  
    for(i = 0; i < p->array-1 && p->plates[i] != NULL; i++){
        hash_value = linear_probing(hash(p->plates[i]),array,p->plates[i]);
        curr = (car_node_t*)array[hash_value].head;
        while(curr != NULL){
            if(strcmp(curr->c.park_name,p->name) == 0){
                record = curr->c.exit; 
                
                if(record.year != 0){
                    cost = payment(p,curr->c.entry,record);
                    bill_head = insert_bill(bill_head,record,cost);
                }
            }
            curr = curr->link;
        }
    }
    return bill_head;
}

/**
 * @brief Gets all the bills of a given day of a given park.
 * 
 * @param p The park to get the bills from.
 * @param array Pointer to the struct car_node
 * @param record The date to get the bills from.
 * @return Returns the head of the list of bills.
*/
bill_t *get_day_bills(park_t *p,array_t *array,date_t record){ 
    int i = 0;
    unsigned int hash_value = 0;
    car_node_t *curr = NULL;
    bill_t *bill_h = NULL;
    double cost = 0;

    for(i = 0; i < p->array-1 && p->plates[i] != NULL; i++){
        hash_value = linear_probing(hash(p->plates[i]),array,p->plates[i]);
        curr = (car_node_t*)array[hash_value].head;
        while(curr != NULL){
            if(strcmp(curr->c.park_name,p->name) == 0 && 
            record.year == curr->c.exit.year && 
            record.mon == curr->c.exit.mon && 
            record.day == curr->c.exit.day){
                cost = payment(p,curr->c.entry,curr->c.exit);
                bill_h = insert_day_bill(bill_h,p->plates[i],curr->c.exit,cost);
                
            }
            curr = curr->link;
        }
    }
    return bill_h;
}

/**
 * @brief Prints all the bills of a given day.
 * 
 * @param head The head of the list of bills.
 * @return void
*/
void print_day_bills(bill_t *head){
    bill_t *curr = head;
    if(head == NULL){
        return;
    }
    /**prints all the bills of the day*/
    while(curr != NULL){
        printf("%s %2.2d:%2.2d %.2lf\n",
        curr->plate,
        curr->exit.hour,curr->exit.min,curr->cost);

        curr = curr->link;
    }
    return;
}

/**
 * @brief Reads the command f and validates it prints the bills of a given park.
 * 
 * @param cmd The command to be read.
 * @param head The head of the list of parks.
 * @param curr_time The current time.
 * @param array Pointer to the struct car_node.
 * @return void
*/
void bills(char *cmd,node_t *head,date_t *curr_time,array_t *array){
    /**initialize the variables to check the command*/
    char *park_name = malloc(sizeof(char)*BUFF_SIZE);
    date_t record = read_bills(cmd,park_name);
    park_t *p = get_park(head,park_name);
    bill_t *bill_head = NULL;

    /**if the park does not exist*/
    if(p == NULL){
        printf("%s: no such parking.\n",park_name);
        free(park_name);
        return;
    }
    /**if the year is 0 it must be the f <park_name> command*/
    if(record.year == 0){ 
        bill_head = get_bills(p,array);
        print_all_bills(bill_head);
        bill_head = erase_all_bills(bill_head);
    }else{ /**check if the date is valid*/
        if(invalidDate(record) || (diffDate(*curr_time,record) > 0)){
            printf("invalid date.\n");
            free(park_name);
            return;  
        }
        bill_head = get_day_bills(p,array,record);
        print_day_bills(bill_head);
        bill_head = erase_all_bills(bill_head);
    }
    free(park_name);
}

