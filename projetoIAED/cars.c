/**
 * Program that manages the parking of cars in a park.
 * 
 * @file cars.c
 * @author Rodrigo Zink ist1109356
*/

#include "functions.h"


/**
 * @brief Reads the command and processes it.
 * 
 * @param head Pointer to the head of the list of parks.
 * @param cmd Command to be read.
 * @param curr_time Current time.
 * @param array Pointer to the struct car_node.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @return void
 */
void read_car_cmd(node_t *head,char *cmd,date_t *curr_time,array_t *array,int mode){
    /**variables to receive the input*/
    char* name = malloc(sizeof(char)*BUFF_SIZE);
    park_t ipark;
    car_t icar = {0};
    car_t car = {0};
    double cost = 0;

    /**in case its a car ENTRY*/
    if(mode == ENTRY && 
    (name = read_car(head,&icar,cmd,curr_time,array))!=NULL){
        regist_car(head,&icar,array,mode);   
        ipark = *get_park(head,name);   
        printf("%s %d\n",ipark.name,(ipark.cap - ipark.ncars));     
    }/**in case its a car EXIT*/
    else if(mode == EXIT && 
    (name = read_car(head,&icar,cmd,curr_time,array))!=NULL){
        /**need to get the entry time*/
        car = *get_car_inside(array,icar.plate); 
        regist_car(head,&icar,array,mode);   
        ipark = *get_park(head,name);   

        /**needs to be the car.entry and the icar.exit*/
        cost = payment(&ipark,car.entry,icar.exit);
        print_exit(car.plate,car.entry,icar.exit,cost);   
    }
    free(name);
    return;
}

/**
 * @brief Initializes an array of heads that will point to struct car.
 *
 * @param array Head of the struct car_node, initially set to NULL.
 */
void init_array(array_t *array){
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        array[i].head = NULL;
    }
}

/**
 * @brief Hash function that returns the hash value of a plate.
 * 
 * @param plate plate of a car.
 * @return hash_value hash value of the plate.
*/
unsigned int hash(char *plate){ 
    unsigned int hash_value = 0;
    int a = 31415, b = 27183;

    for (hash_value = 0; *plate != '\0'; plate++, a = a*b % (TABLE_SIZE-1)){
        hash_value = (a*hash_value + *plate) % TABLE_SIZE;
    }   
    return hash_value;
}

/**
 * @brief Linear probing function that returns the hash value of a plate.
 * 
 * @param hash_value hash value of the plate.
 * @param array Pointer to the struct car_node.
 * @param plate plate of a car.
 * @return hash_value hash value of the plate.
*/
int linear_probing(int hash_value,array_t *array,char *plate){
    while (array[hash_value].head != NULL) {

        if(strcmp(array[hash_value].head->c.plate,plate) == 0){
            return hash_value;
        }
        hash_value= (hash_value + 1) % TABLE_SIZE;
    }
    return hash_value;
}



/**
 * @brief Gets the car info and inserts it in the hash table of cars.
 * 
 * @param array Pointer to the struct car_node.
 * @param icar Car received as input.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @return void
*/
void insert_car(array_t *array,car_t *icar,char mode){
    /**setting a null date in case of exit*/
    date_t null_date = {0};
    /**getting the hash value of the plate*/
    unsigned int hash_value = hash(icar->plate); 
    hash_value = linear_probing(hash_value,array,icar->plate);

    /**creating the car nodes for the insertion*/
    car_node_t *car = (car_node_t*) malloc(sizeof(car_node_t)); 

    /**allocating memory for the park name*/
    car->c.park_name = malloc(sizeof(char)*BUFF_SIZE);

    /**passing the values and key to the car thats going to be inserted*/
    if(mode == ENTRY){
        strcpy(car->c.plate,icar->plate);
        strcpy(car->c.park_name,icar->park_name);
        car->c.park_name = realloc(car->c.park_name,strlen(car->c.park_name)+1);
        car->c.entry = icar->entry;
        car->c.exit = null_date;
    }
    else{
        array[hash_value].head->c.inside = 0;
        car->c.exit = icar->exit;
    }
    insert_car_in_hash(array,icar->park_name, mode, hash_value, car);
}


/**
 * @brief Inserts a car in the hash table of cars in order by park name.
 * 
 * @param array Pointer to the struct car_node.
 * @param icar Car received as input.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @param hash_value Hash value of the plate.
 * @param car Car node to be inserted.
 * @return void
*/
void insert_car_in_hash(array_t *array,char *park_name,char mode,int hash_value,car_node_t *car){
    car_node_t *curr = (car_node_t*)array[hash_value].head;
    car_node_t *prev = NULL;

    /**insertion at the head*/
    if (array[hash_value].head == NULL) {
        array[hash_value].head = car;
        array[hash_value].head->c.inside = 1;
        car->link = NULL;
    } else {
        /**goes through list until it finds a park name greater 
         * than the park name of the current car. Or it reaches the end.*/
        while (curr != NULL && strcmp(curr->c.park_name, park_name) <= 0) {
            prev = curr;
            curr = curr->link;
        }
        /**inserts car in the selected node and updates the inside value*/
        if (mode == ENTRY) {
            car->link = curr;
            if (prev == NULL) {
                array[hash_value].head = car;
            } else {
                prev->link = car;
            }
            array[hash_value].head->c.inside = 1;
        } else {
            prev->c.exit = car->c.exit;
            array[hash_value].head->c.inside = 0;
        }
    }
}


/**
 * @brief Get the car info if he is inside a park.
 * 
 * @param array Pointer to the struct car_node.
 * @param plate plate of a car.
 * @return car_t* Pointer to the car that is inside the park.
*/
car_t *get_car_inside(array_t *array,char *plate){
    /**getting the hash value of the plate*/
    unsigned int hash_value = hash(plate);
    hash_value = linear_probing(hash_value,array,plate);

    /**in case the car doesnt exist or is outside any park*/
    car_t *no_car = NULL;
    car_t *no_car_inside = CAR_OUTSIDE; 

    /**getting the head of the list of cars*/
    car_node_t *curr = (car_node_t*)array[hash_value].head;

    if(array[hash_value].head == NULL){
        return no_car; 
    }
    
    /**going through the list of cars*/
    while(curr != NULL && curr->c.exit.year != 0){
            curr = curr->link;
        }
    /**if the car is not in any park*/
    if(curr == NULL){
        return no_car_inside;
    }
    return &curr->c;
}

/**
 * @brief Checks if a car is inside/outside a park.
 * 
 * @param array Pointer to the struct car_node.
 * @param plate plate of a car.
 * @return 0 if the car is outside, 1 if the car is inside.
*/
int check_in_out(array_t *array,char *plate){
    unsigned int hash_value = hash(plate);
    hash_value = linear_probing(hash_value,array,plate); 
    /**if there is no information about the car then it is outside*/
    if(array[hash_value].head == NULL){
        return 0;
    }
    return array[hash_value].head->c.inside;
}

/**
 * @brief Checks if the plate of a car is valid.
 * 
 * @param plate plate of a car.
 * @return 0 if the plate is valid, 1 if the plate is invalid.
*/
int check_plate(char *plate){
    int letters = 0, numbers = 0,i = 0;
    if(strlen(plate) == (PLATE_SIZE-1)){
        while(i < PLATE_SIZE - 2){
            /**checks if the plate has 2 letters together*/   
            if((('A'<= plate[i]) && (plate[i] <= 'Z')) &&
            (('A'<= plate[i+1]) && (plate[i+1] <= 'Z'))){ 
            letters++;
            i+=2;
            }
            /**checks if the plate has 2 numbers together*/
            else if((('0'<= plate[i]) && (plate[i] <= '9')) &&
            (('0'<= plate[i+1]) && (plate[i+1] <= '9'))){ 
            numbers++;
            i+=2;
            }
            /**ignores '-'*/
            else if(plate[i] == '-'){
                i++;
            }
            /**if the plate has any other character*/
            else{
                return 1;
            }
        }
    }
    return ((numbers == 0) || (letters == 0));
}

/**
 * @brief Checks if the date is valid.
 * 
 * @param itime date of the car entry/exit.
 * @return 0 if the date is valid, 1 if the date is invalid.
*/
int check_time(date_t itime,date_t *curr_time){
    if(invalidDate(itime)){
        return 1;
    }
    if(diffDate(*curr_time,itime) < 0){ 
        return 1;
    }
    /**the time is valid and needs to be updated*/
    *curr_time = itime; 
    return 0; 
}

/**
 * @brief Performs the routine to check if a car is valid and if the park 
 *        is valid and can receive the car.
 *     
 * @param park Pointer to the struct park.
 * @param icar Car received as input.
 * @param curr_time Current time.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @return 0 if the car is valid, 1 if the car is invalid.
*/
int check_car_routine(park_t *park,car_t *icar,char mode){
    if (park == NULL) {
        printf("%s: no such parking.\n", icar->park_name);
        return 1;
    }
    if ((park->cap - park->ncars) <= 0 && mode == ENTRY) {
        printf("%s: parking is full.\n", icar->park_name);
        return 1;
    }
    if (check_plate(icar->plate)) {
        printf("%s: invalid licence plate.\n", icar->plate);
        return 1;
    }
    return 0;
}
/**
 * @brief Checks if the car is valid to enter/exit a park.
 * 
 * @param head_park Pointer to the struct node of parks.
 * @param icar Car received as input.
 * @param curr_time Current time.
 * @param array Pointer to the struct car_node.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @return 0 if the car is valid, 1 if the car is invalid.
*/
int check_car(node_t *head_park,car_t *icar,date_t *curr_time,array_t *array,char mode){ 
    /**getting the info about the park selected and car*/
    park_t *park = get_park(head_park,icar->park_name);
    int in_out = check_in_out(array,icar->plate);
    car_t *car = get_car_inside(array,icar->plate);

    /**check if the car and park themselves are valid*/
    if(check_car_routine(park,icar,mode)){
        return 1;
    }
    if(mode == ENTRY){ /**the car only needs to be outside to enter a park*/
        if(in_out){
        printf("%s: invalid vehicle entry.\n",icar->plate);
        return 1;
        }/**check if the time is valid*/
        if(check_time(icar->entry,curr_time)){ 
        printf("invalid date.\n");
        return 1;
        }
    }
    else{/**the car needs to exit the correct park to be a valid exit*/
        if(car == NULL||car == CAR_OUTSIDE||strcmp(park->name,car->park_name)){
        printf("%s: invalid vehicle exit.\n",icar->plate);
        return 1;
        }/**check if the time is valid*/
        if(check_time(icar->exit,curr_time)){ 
        printf("invalid date.\n");
        return 1;
        }
    }
    return 0;
}

/**
 * @brief Records the entry/exit of a car.
 * 
 * @param icar Car received as input.
 * @param record Date of the car entry/exit.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @return void
*/
void car_record(car_t *icar,date_t *record,char mode){
    if(mode == ENTRY){
        icar->entry = *record;
    }
    else{
        icar->exit = *record;
    }
}

/**
 * @brief Reads the car info from the input.
 * 
 * @param head_park Pointer to the struct node of parks.
 * @param icar Car received as input.
 * @param cmd Command received as input.
 * @param curr_time Current time.
 * @param array Pointer to the struct car_node.
 * @return icar->park_name Name of the park where the car is parked.
*/                                              
char *read_car(node_t *head_park,car_t *icar,char *cmd,date_t *curr_time,
array_t *array){
    /**setting variables to receive the input*/
    char mode;
    date_t record = {0};
    icar->park_name = malloc(sizeof(char)*BUFF_SIZE);

    /**reading the input in case of park name with spaces and without*/
    if(sscanf(cmd,"%c \"%[^\"]\"  %9s %d-%d-%d %d:%d",
    &mode,icar->park_name,icar->plate,&record.day,&record.mon,&record.year,
    &record.hour,&record.min) == 8){ 
    
    }
    else if(sscanf(cmd,"%c %s %9s %d-%d-%d %d:%d",
    &mode,icar->park_name,icar->plate,&record.day,&record.mon,&record.year,
    &record.hour,&record.min)){
    }

    /**setting the car info*/
    car_record(icar,&record,mode);

    /**checking if the car is valid */
    if(check_car(head_park,icar,curr_time,array,mode)){
        free(icar->park_name);
        return NULL;
    }
    return icar->park_name;
}

/**
 * @brief checks if plate is valid and if the car has any entries/exits.
 * then prints the info of the car with print_registry(array,plate).
 * 
 * @param array Pointer to the struct car_node.
 * @param cmd Command received as input.
 * @return void
*/
void registry(array_t *array,char *cmd){
    char plate[PLATE_SIZE];
    sscanf(cmd,"v %9s",plate);

    if(check_plate(plate)){
        printf("%s: invalid licence plate.\n",plate);
        return;
    }
    if(get_car_inside(array,plate) == NULL){
        printf("%s: no entries found in any parking.\n",plate);
        return;
    }
    print_registry(array,plate);
    return;
}

/**
 * @brief Registe a car info for the insertion of the park.
 * 
 * @param head Pointer to the struct node of parks.
 * @param icar Car received as input.
 * @param array Pointer to the struct car_node.
 * @param mode Mode of the insertion of the car, either ENTRY or EXIT.
 * @return void
*/
void regist_car(node_t * head,car_t *icar,array_t *array,char mode){ 
    /**getting the head of the list of parks*/
    node_t * curr = head;          
    int i = 0;
    /**variable to realloc more memory*/
    int more_mem = 0;
    /**result of the strcmp so it doesnt repeat in the if & else if*/
    int cmp;

    /**regists the plate of the car in the park and updates the n of cars*/
    while (curr != NULL) { 
        cmp = strcmp(icar->park_name,curr->p.name);
        if((!cmp) && (mode == ENTRY)){ 
            while(i < curr->p.array-1 && curr->p.plates[i] != NULL 
            && strcmp(curr->p.plates[i],icar->plate) != 0){
                i++;
            }
            /**adding the plate of the car to the park*/
            curr->p.plates[i] = malloc(sizeof(char)*PLATE_SIZE);
            strcpy(curr->p.plates[i],icar->plate);
            /**if it starts to get full realloc more memory*/
            if(i >= (curr->p.array/TWO)){
                more_mem = curr->p.array * TWO;
                curr->p.plates = realloc(curr->p.plates,sizeof(char*)*more_mem);
                curr->p.array *= TWO;
            }
            /**updates the number of cars in the park*/
            curr->p.ncars++;
            break;
            }
        else if((!cmp) && (mode == EXIT)){
            /**updates the number of cars in the park*/
            curr->p.ncars--;
            break;
        }
        curr = curr->link;
        }
    insert_car(array,icar,mode);
}

/**
 * @brief Prints the registry of a car.
 * 
 * @param array Pointer to the struct car_node.
 * @param plate plate of a car.
 * @return void
*/
void print_registry(array_t *array,char *plate){
    
    unsigned int hash_value = hash(plate);
    hash_value = linear_probing(hash_value,array,plate);
    car_node_t *curr = (car_node_t*)array[hash_value].head;

    while (curr != NULL) {
        /**if the exit is still at zeros the car is still in that park*/
        if(curr->c.exit.year == 0){
            printf("%s %2.2d-%2.2d-%2.2d %2.2d:%2.2d\n",
            curr->c.park_name,curr->c.entry.day,
            curr->c.entry.mon,curr->c.entry.year,curr->c.entry.hour,
            curr->c.entry.min);
        }
        else{
            printf("%s %2.2d-%2.2d-%2.2d %2.2d:%2.2d "
            "%2.2d-%2.2d-%2.2d %2.2d:%2.2d\n",
            curr->c.park_name,curr->c.entry.day,
            curr->c.entry.mon,curr->c.entry.year,curr->c.entry.hour,
            curr->c.entry.min,curr->c.exit.day,curr->c.exit.mon,
            curr->c.exit.year,curr->c.exit.hour,curr->c.exit.min);
        }
        curr = curr->link;
    }        
      
}

/**
 * @brief Reinserts the cars in the hash table of cars after a car is removed.
 * 
 * @param array Pointer to the struct car_node.
 * @return void
*/
void re_insert(array_t *array,int hash_value){
    unsigned int j;
    unsigned int i = hash_value;
    /**reinserts the next cars until it finds a NULL*/
    for(j = (i+1) % TABLE_SIZE; array[j].head != NULL;j = (j+1) % TABLE_SIZE){
        hash_value = hash(array[j].head->c.plate);

        /**if the hash value has free space, put the car there*/
        if(array[hash_value].head == NULL){
            array[hash_value].head = array[j].head;
            array[j].head = NULL;
            continue;
        }else{
        /**if the hash value is already occupied, find the next free space*/
        hash_value = linear_probing(hash_value,array,array[j].head->c.plate);
        array[hash_value].head = array[j].head;
        array[j].head = NULL;
        }
    }
}

/**
 * @brief Removes a car from the hash table of cars.
 * 
 * @param park_name Name of the park where the car is parked.
 * @param plate plate of a car.
 * @param array Pointer to the struct car_node.
 * @return void
*/
void remove_cars(char *park_name,char *plate,array_t *array){
    /**setting the variables to go through the list of cars*/
    unsigned int hash_value = linear_probing(hash(plate),array,plate);
    car_node_t *curr = (car_node_t*)array[hash_value].head;
    car_node_t *prev = NULL;
    while(curr != NULL){
        /**removes the registration of the car in the selected park*/
        if(strcmp(curr->c.park_name,park_name) == 0){
            if(prev == NULL){
                if(curr->link == NULL){
                    free_car_node(curr);
                    array[hash_value].head = NULL;
                    re_insert(array,hash_value);
                    return;
                }else{
                    array[hash_value].head = curr->link;
                    free_car_node(curr);
                    curr = array[hash_value].head;
                }
            }else{
                prev->link = curr->link;
                free_car_node(curr);
                curr = prev->link;
            }
        }else{
            prev = curr;
            curr = curr->link;
        }
    }
}

/**
 * @brief Frees the memory occupied by a car node.
 * 
 * @param node Pointer to the car node to be freed.
 * @return void
*/
void free_car_node(car_node_t *node){
    free(node->c.park_name);
    free(node);
}


/**
 * @brief Prints the exit of a car and the respective cost.
 * 
 * @param plate plate of a car.
 * @param entry Date of the car entry.
 * @param exit Date of the car exit.
 * @param cost Cost of the car parking.
 * @return void
*/
void print_exit(char *plate,date_t entry,date_t exit,double cost){
    printf("%s %2.2d-%2.2d-%2.2d %2.2d:%2.2d " 
    "%2.2d-%2.2d-%2.2d %2.2d:%2.2d %.2lf\n",
    plate,
    entry.day,entry.mon,entry.year,entry.hour,entry.min,
    exit.day,exit.mon,exit.year,exit.hour,exit.min,
    cost);   
    return;
}

car_node_t *shutdown_cars(car_node_t *head){
    car_node_t *curr = head;
    car_node_t *next;
    while(curr != NULL){
        next = curr->link;
        free(curr->c.park_name);
        free(curr);
        curr = next;
    }
    return head;
}