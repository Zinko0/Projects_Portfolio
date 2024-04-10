/**
 * Program that manages the parking lots.
 * 
 * @file parking.c
 * @author Rodrigo Zink ist1109356
*/
#include "functions.h"

/**
 * @brief Reads the command and processes it.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param cmd Command to be read.
 * @param p_sorted Array of strings with the sorted names of the parks.
 * @param len The current number of parks in the list.
 * @return head of the park linked list.
 */
node_t *read_park_cmd(node_t *head,char *cmd,char p_sorted[MAX_PARK][BUFF_SIZE],int *len){
    park_t ipark = {0};
    /**if the command is just 'p\n' print the parks*/
    if(cmd[1] == '\n'){
        print_park(head);
    }
    else{ /**else checks the command and if valid insert the park*/
        if(!read_park(head,&ipark,*len,cmd)){
            head = insert_node(head,ipark);
            sort_parks(ipark.name,p_sorted,*len);
            *len += 1;
        }
    }
    return head;
}

/**
 * @brief Gets a park from the list of parks.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param str Name of the park to be found.
 * @return Pointer to the park if it exists, NULL otherwise.
*/
park_t *get_park(node_t *head,char *str){
    node_t * curr = head;
    /**searches for the park*/
    while (curr != NULL) { 
        if(!(strcmp(str,curr->p.name))){ 
            return &curr->p;
        }
        curr = curr->link;
    }
    /**if the park is not found returns NULL*/
    park_t *empty = NULL;
    return empty;
}

/**
 * @brief Prints the list of parks with its capacity and free spaces.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param len The current number of parks in the list.
*/
void print_park(node_t *head){
    if(head != NULL){
        node_t *ptr = NULL;
        ptr = head;
        while(ptr != NULL){
            printf("%s %d %d\n", ptr->p.name,ptr->p.cap,
            (ptr->p.cap - ptr->p.ncars));
            ptr = ptr->link;
        }
    }
}
/**
 * @brief Checks if a park is valid and can be added to the list.

 * @param head Pointer to the head of the list of parks.
 * @param p Pointer to the park to be checked.
 * @param len The current number of parks in the list.
 * @return 1 if the park is invalid, 0 otherwise.
 */
int check_park(node_t *head, park_t *p, int len) {
    /**checks if the park already exists*/
    if (get_park(head, p->name)) { 
        printf("%s: parking already exists.\n", p->name);
        return 1;
    }
    /**checks if the capacity is valid*/
    if (p->cap <= 0) {
        printf("%d: invalid capacity.\n", p->cap);
        return 1;
    }
    /**checks if the prices are valid*/
    if (!((p->x > 0) && (p->x < p->y) && (p->y < p->z))) {
        printf("invalid cost.\n");
        return 1;
    }
    /**park size limit*/
    if (len >= MAX_PARK) { 
        printf("too many parks.\n");
        return 1;
    }
    
    return 0;
}

/**
 * @brief Reads the park from the command and checks if it is valid.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param p Pointer to the park to be read.
 * @param len The current number of parks in the list.
 * @param cmd Command to be read.
 * 
 * @return 0 if the park is valid, 1 otherwise.
*/
int read_park(node_t *head,park_t *p,int len,char *cmd){ 
    /**setting variables to receive the input*/
    p->name = malloc(sizeof(char)*BUFF_SIZE);

    /**reading the input in case of park name with spaces and without*/
    if(sscanf(cmd,"p \"%[^\"]\" %d %f %f %f",p->name,&p->cap,&p->x,&p->y,&p->z)==5){
    }
    else if(sscanf(cmd,"p %s %d %f %f %f",p->name,&p->cap,&p->x,&p->y,&p->z)){
    }
    
    p->name = realloc(p->name,strlen(p->name)+1);

    /**checks if the park is valid*/
    if(!check_park(head,p,len)){
        /**initializes the park ncars and array of plates*/
        p->ncars = 0;
        p->plates = malloc(sizeof(char*)*ARRAY_SIZE); 
        memset(p->plates, 0, ARRAY_SIZE * sizeof(char *));
        p->array = ARRAY_SIZE;
        return 0;
    }
    free(p->name);
    return 1;
}

/**
 * @brief Inserts a park at the end of the list of parks.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param ipark Park to be inserted.
 * @return void
*/
node_t *insert_node(node_t *head,park_t ipark){
    node_t* temp;
    node_t* ptr;
    /**if the list is empty inserts the park as the head*/
    if(head == NULL){
        head = (node_t *)malloc(sizeof(node_t));
        head->p = ipark;
        head->link = NULL;
        return head;
    }else{
        /**inserts the park at the end of the list*/
        ptr = head;
        temp = (node_t *)malloc(sizeof(node_t));
        temp->p = ipark;
        temp->link= NULL;
        while(ptr->link != NULL){
            ptr = ptr->link;
        }
        ptr->link = temp;
    }
    return head;
}



/**
 * @brief Sorts the list of parks alphabetically.
 * 
 * @param park_name Name of the park to be sorted.
 * @param parks_sorted Array of strings with the sorted names of the parks.
 * @param len The current number of parks in the list.
 * @return void
*/
void sort_parks(char *park_name,char parks_sorted[MAX_PARK][BUFF_SIZE],int len){
    int i = 0;
    if(len == 0){
        strcpy(parks_sorted[0], park_name); 
    }
    else{
        for(i = 0; i < len; i++){
            /**insert park in alphabetical order*/ 
            if(strcmp(park_name,parks_sorted[i]) < 0){
                for(int j = len; j > i; j--){
                    /**shifts the array to the right*/
                    strcpy(parks_sorted[j], parks_sorted[j-1]);
                }
                /**inserts the park_name*/
                strcpy(parks_sorted[i], park_name);
                break;
            }
        }
    }
    /**If we reach here, park_name is greater than all existing names*/
    strcpy(parks_sorted[i],park_name);
}


/**
 * @brief Erases a park from the list of parks.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param park_name Name of the park to be erased.
 * @return Pointer to the head of the list of parks.
*/
node_t* erase_park(node_t *head,char *park_name){
    /**setting the variables to go through the list*/
    node_t *curr = head;
    node_t *previous = NULL;

    /**searches for the park and erases it*/
    while (curr != NULL) {
        if(strcmp(park_name,curr->p.name) == 0){
            if(previous == NULL){
                head = head->link;
            }else{
                previous->link = curr->link;
            }
            free(curr->p.plates);
            free(curr->p.name);
            free(curr);
            return head;
        }
        previous = curr;
        curr = curr->link;
    }
    return head;
}
/**
 * @brief Erases all the parks from the list of parks.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param array Pointer to the struct car_node.
 * @return NULL
*/
node_t* erase_all_parks(node_t *head,array_t *array) {
    /**setting the variables to go through the list*/
    node_t *curr = head;
    node_t *next;
    int i = 0;
    while (curr != NULL) {
        next = curr->link;
        /**removes all the plates from the park*/
        while(curr->p.plates[i] != NULL){ 
            remove_cars(curr->p.name,curr->p.plates[i],array);
            free(curr->p.plates[i]);
            i++;
        }
        free(curr->p.plates);
        free(curr->p.name);
        free(curr);
        curr = next;
    }
    /**head should be NULL*/
    return head;
}
/**
 * @brief Erases a park from the array of sorted parks.
 * 
 * @param park_name Name of the park to be erased.
 * @param p_sorted Array of strings with the sorted names of the parks.
 * @param len The current number of parks in the list.
 * @return void
*/
void erase_p_sorted(char *park_name,char p_sorted[MAX_PARK][BUFF_SIZE],int len){
    int i = 0;
    for(i = 0; i < len; i++){
        /**searches for the park and erases it*/
        if(strcmp(park_name,p_sorted[i]) == 0){
            /**shifts the array to the left*/
            for(int j = i; j < len-1; j++){
                strcpy(p_sorted[j], p_sorted[j+1]);
            }
            break;
        }
    }
    len--;
}

/**
 * @brief Prints the array of sorted parks.
 * 
 * @param p_sorted Array of strings with the sorted names of the parks.
 * @param len The current number of parks in the list.
 * @return void
*/
void print_park_sorted(char parks_sorted[MAX_PARK][BUFF_SIZE],int len){
    if(len == 0){
        return;
    }
    for(int i = 0; i < len; i++){
        printf("%s\n",parks_sorted[i]);
    }
}

/**
 * @brief Removes a park from the list of parks.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param cmd Command to be read.
 * @param array Pointer to the struct car_node.
 * @param parks_sorted Array of strings with the sorted names of the parks.
 * @param len The current number of parks in the list.
 * @return Pointer to the head of the list of parks.
*/
node_t *remove_park(node_t *head,char *cmd, array_t *array,char p_sorted[MAX_PARK][BUFF_SIZE],int *len){
    int i = 0;
    char command;
    char park_name[BUFF_SIZE];
    park_t *park;
    /**reads the command*/
    if(sscanf(cmd,"%c \"%[^\"]\" ",&command,park_name) == 2){ 
    }
    else if(sscanf(cmd,"%c %s ",&command,park_name)){
    }
    if( (park = get_park(head,park_name)) == NULL){
        printf("%s: no such parking.\n",park_name);
        return head;
    }
    
    /**removes all the cars from the park*/
    while(park->plates[i] != NULL){ 
        remove_cars(park->name,park->plates[i],array);
        free(park->plates[i]);
        i++;
    }
    /**erases the park from the list*/
    head = erase_park(head,park_name);
    /**erases the park from the array of sorted parks*/
    erase_p_sorted(park_name,p_sorted,*len);
    *len -= 1;
    /**prints the array of sorted parks*/
    print_park_sorted(p_sorted,*len);
    return head;
}