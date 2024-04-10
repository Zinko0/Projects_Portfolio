/**
 * Header file with all the constants and functions structures.
 * 
 * @file const_struct.h
 * @author Rodrigo Zink ist1109356 
*/

#ifndef const_struct_h
#define const_struct_h

/**Max size of the buffer*/
#define BUFF_SIZE 8192 
/**Max size of the plate*/
#define PLATE_SIZE 9
/**Max size of the hash table*/
#define TABLE_SIZE 10159
/**Pointer that represents a car that is outside*/
#define CAR_OUTSIDE (car_t*)(0xFFFFFFFFFFFFFFFUL) 
/**Command for entry*/
#define ENTRY 'e'
/**Command for exit*/
#define EXIT 's'
/**Max number of parks*/
#define MAX_PARK 20
/**Minutes in a year*/
#define MIN_YEAR (60*24*365)
/**Minutes in a day*/
#define MIN_DAY (60*24)
/**Interval where the park charges*/
#define INTERVAL 15
/**Quarters in an hour*/
#define QUARTERS_IN_HOUR 4
/**Array size for the plates in each park*/
#define ARRAY_SIZE 1000
/**TWO*/
#define TWO 2

/**
 * @brief Structure representing a date.
 */
typedef struct date{
    int day, mon, year;
    int hour, min;
}date_t;

/**
 * @brief Structure representing a park bill.
 */ 
typedef struct bill{
    char plate[PLATE_SIZE];
    struct date exit;
    double cost;
    /**Linked list of bills*/
    struct bill *link;
}bill_t;


/**
 * @brief Structure representing a park.
*/
typedef struct park{
    /**Name of the park*/
    char *name; 
    /**Capacity of the park and number of cars*/
    int  cap, ncars; 
    /**Prices of the park*/
    float x, y, z;
    /**Array of plates*/
    char **plates; 
    /**Size of the array*/
    int array;
}park_t;

/**
 * @brief Structure representing a node that has a park.
 */
typedef struct node{ 
    struct park p; 
    struct node *link;
}node_t;

/**
 * @brief Structure representing a car.
 */
typedef struct car{
    char plate[PLATE_SIZE]; 
    /**Name of the park*/
    char *park_name;
    struct date entry;
    struct date exit;
    /**0 if its outside 1 if its inside*/
    int inside;
}car_t;

/**
 * @brief Structure representing a car node.
 */
typedef struct car_node{
    struct car c;
    struct car_node *link;
}car_node_t; 

/**
 * @brief Structure representing an array of car nodes.
 */
typedef struct array{
    struct car_node *head;
}array_t;

#endif