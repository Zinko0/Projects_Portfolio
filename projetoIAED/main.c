/**
 * Program that recieves the commands and processes them.
 * 
 * @file main.c
 * @author Rodrigo Zink ist1109356
*/

#include "functions.h"

/**
 * @brief Reads the command and processes it.
 * 
 * @return 0 if the program runs successfully.
 */
int main(void){
    /**variables and possible inputs*/
    int mode;
    int len = 0;
    char cmd[BUFF_SIZE]; 
    char parks_sorted[MAX_PARK][BUFF_SIZE] = {0}; 
    node_t *head = NULL;
    date_t curr_time = {0}; 

    /**initialize the hash table*/
    array_t *array = malloc(sizeof(array_t)*TABLE_SIZE); 
    init_array(array); 
    
    do{ 
        /**reads the command*/
        fgets(cmd,BUFF_SIZE,stdin);
        switch(cmd[0]) {
            case 'p':
            head = read_park_cmd(head,cmd,parks_sorted,&len);
            break;
            case 'e':
            mode = ENTRY;
            read_car_cmd(head,cmd,&curr_time,array,mode);
            break;
            case 's':
            mode = EXIT;
            read_car_cmd(head,cmd,&curr_time,array,mode);
            break;
            case 'v': 
            registry(array,cmd);
            break;
            case 'f':
            bills(cmd,head,&curr_time,array);
            break;
            case 'r':
            head = remove_park(head,cmd,array,parks_sorted,&len);
        } 
        /**if the command is 'q' end the cicle*/
    }while(cmd[0] != 'q');

    /**free the memory*/
    head = erase_all_parks(head,array);
    free(array);
    return 0;
}


