#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include "printing.h"
#include "constants.h"

/**
 * Initializes a square 2D array of size MAP_SIZE.
 * 
 * @param map The map to be initilized as a 2D char array with each value set to ' '.
 */
void init_empty_map(char map[MAP_SIZE][MAP_SIZE]){
    for(int x=0; x<MAP_SIZE; x++) 
        for(int y =0; y<MAP_SIZE; y++)
            map[x][y] = ' ';
}

/**
 * Places a ship into a map with the desiered coordinates and orientation.
 * 
 * @param map A 2D square array of size MAP_SIZE
 * @param x The starting x coordinate of the ship to be added to the map
 * @param y The starting y coordinate of the ship to be added to the map
 * @param orientation The orientation of the ship, with 1 being vertical and 2 being horizontal
 * @param ship_type The type of ship being added
 * 
*/
int place_ship(char map[MAP_SIZE][MAP_SIZE], int x, int y, int orientation, int length, char ship_type){
    
    if(orientation){// Vertical
        for(int i = 0; i<length; i++)
            if (map[x][y+i] != ' ')
                return -1;
        
        for(int i = 0; i<length; i++)
            map[x][y+i] = ship_type;
    }
    else{           // Horizontal
        for(int i = 0; i<length; i++)
            if (map[x+i][y] != ' ')
                return -1;
        
        for(int i = 0; i<length; i++)
            map[x+i][y] = ship_type;
    }
    
    return 0;
}

/**
 * Generates random coordinates for the start of a ship, and it's orientation. 
 * Will not return a value that puts the ship off the edge of the map.
 * 
 * @param x A pointer to the x coordinate
 * @param y A pointer to the y coordinate
 * @param orientation A pointer to the orientation of the ship (with 1 being vertical and 0 being horizontal)
 * @param ship_length The length of the ship.
*/
void rand_ship_coord(int *x, int *y, int *orientation, int ship_length){

    *orientation = rand() % 2; // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)

    if (*orientation){  // Vertical
        if((*x = rand() % MAP_SIZE) < 0)
            *x = *x * -1;
        if((*y = rand() % MAP_SIZE - CAR_SIZE) < 0)
            *y = *y * -1;
    }
    else{               // Horizontal
        if((*x = rand() % MAP_SIZE - CAR_SIZE) < 0)
            *x = *x * -1;
        if((*y = rand() % MAP_SIZE) < 0)
            *y = *y * -1;
    }
}

/**
 * The code for the singleplayer opponent process.
 * 
 * @param inputs The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param outpus The pipe IDs to send info FROM the host process the TO child opponent process.
*/
void singleplayer(int inputs[2], int outputs[2]){
    
    srand(time(NULL));
    
    // Generate random map.
    // Init 2 10x10 2D char arrays all filled with ' '.
    char cpu_ships_map[MAP_SIZE][MAP_SIZE]; // Map containing CPU's ships
    char cpu_hit_map[MAP_SIZE][MAP_SIZE];   // Map containing coordinaties the CPU has attacked.

    init_empty_map(cpu_ships_map);
    init_empty_map(cpu_hit_map);
    
    // Generate CPU's map.

    // place carrier
    int x, y, orientation;

    rand_ship_coord(&x, &y, &orientation, CAR_SIZE);
    //printf("CAR: x: %d, y: %d, Orientation: %d\n", x, y, orientation);
    place_ship(cpu_ships_map, x, y, orientation, CAR_SIZE, 'C'); 


    // place battleship
    do{
        rand_ship_coord(&x, &y, &orientation, BAT_SIZE);
        //printf("BAT: x: %d, y: %d, Orientation: %d\n", x, y, orientation);
    }
    while(place_ship(cpu_ships_map, x, y, orientation, BAT_SIZE, 'B') == -1);

    // place destroyer
    do{
        rand_ship_coord(&x, &y, &orientation, DES_SIZE);
        //printf("DES: x: %d, y: %d, Orientation: %d\n", x, y, orientation);
    }
    while(place_ship(cpu_ships_map, x, y, orientation, DES_SIZE, 'D') == -1);

    // place sub
    do{
        rand_ship_coord(&x, &y, &orientation, SUB_SIZE);
        //printf("SUB: x: %d, y: %d, Orientation: %d\n", x, y, orientation);
    }
    while(place_ship(cpu_ships_map, x, y, orientation, SUB_SIZE, 'S') == -1);

    // place patrol boat
    do{
        rand_ship_coord(&x, &y, &orientation, PAT_SIZE);
        //printf("PAT: x: %d, y: %d, Orientation: %d\n", x, y, orientation);
    }
    while(place_ship(cpu_ships_map, x, y, orientation, PAT_SIZE, 'P') == -1);

    //print_map(cpu_ships_map);
    exit(0);

    int carrier = CAR_SIZE;
    int battleship = BAT_SIZE;
    int destroyer = DES_SIZE;
    int submarine = SUB_SIZE;
    int patrol_boat = PAT_SIZE;

    int cpu_hits = 0;        // Amount of hits AGAINST the oponent
    int player_hits = 0;    // Amount of hits AGAINST the player

    struct target_coord target;

    write(inputs[1], "R", 1);   // Send message to parent to let it continue.
    read(outputs[0], &target, sizeof(struct target_coord));

    while(cpu_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        // Pipe random coordinates until a hit.


    }
    

    exit(0);
}

/**
 * The code for the multiplayer opponent process that RECIVES information from a client.
 * 
 * @param inputs The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param outpus The pipe IDs to send info FROM the host process the TO child opponent process.
*/
void multiplayer_server(int inputs[2], int outputs[2]){

    // Networking bs

    write(inputs[1], "1", 1);   // Allow the main process to continue.



    while(1){

    }

    exit(0);
}

/**
 * The code for the multiplayer opponent process that SENDS information to the host.
 * 
 * @param inputs The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param outpus The pipe IDs to send info FROM the host process the TO child opponent process.
*/
void multiplayer_client(int inputs[2], int outputs[2]){

    // Networking bs

    write(inputs[1], "1", 1);   // Allow the main process to continue.



    while(1){

    }

    exit(0);
}

/**
 * Initializes the child process that handles the singleplayer process.
 * 
 * @param inputs The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param outpus The pipe IDs to send info FROM the host process the TO child opponent process.
*/
pid_t init_singleplayer(int inputs[2], int outputs[2]){
    pid_t child;
    if ((child = fork()) == 0){
        singleplayer(inputs, outputs);
    }
    else if (child == -1){
        perror("There was an issue forking the singleplayer process");
        exit(1);
    }
    return child;
}

/**
 * Initializes the child process that handles the multiplayer process.
 * 
 * @param inputs The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param outpus The pipe IDs to send info FROM the host process the TO child opponent process.
 * @param connection_choice The user's decition to host (1) or connect (0);
*/
pid_t init_multiplayer(int inputs[2], int outputs[2], char connection_choice){
    pid_t child;
    if ((child = fork()) == 0){
        if (connection_choice)
            multiplayer_server(inputs, outputs);
        else 
            multiplayer_client(inputs, outputs);

    }
    else if (child == -1){
        perror("There was an issue forking the multiplayer process");
        exit(1);
    }
    return child;
}

int main(){

    unsigned int choice = 0;
    pid_t oponent_process = 0;
    int inputs[2];      // Information coming FROM the chosen oponent
    int outputs[2];     // Information going TO the chosen oponent

    // Init pipes and check that they work
    if (pipe(inputs) == -1){
        perror("Input piping error");
        exit(1);
    }

    if (pipe(outputs) == -1){
        perror("Output piping error");
        exit(1);
    }

    // Print title
    print_title();

    do{
        //Print menu and get selections.
        print_menu();

        if ((choice = get_menu_selection()) == 1)    // If singleplayer, fork a process to play battlship with rand.
            oponent_process = init_singleplayer(inputs, outputs);
        else if (choice == 2){    // If multiplayer, ask if the user wants to host or connect.

            print_networking_options();

            if ((choice = get_menu_selection()) == 1)   // User wants to host
                oponent_process = init_multiplayer(inputs, outputs, 0);
            else if(choice == 2)                    // User wants to connect.
                oponent_process = init_multiplayer( inputs, outputs, 1);
            else{                                    // Other
                choice = 0; // Set to 0, so the while loop keeps working.
                continue;
            }

            char* output = 0;

            read(inputs[0], output, 1); // Waits until a message is sent by the child process, indicating that 
            if (*output == -1)  // Quit if we recive an error message.
                exit(1);
        }
        else if (choice == 3)
            print_settings_menu();
        else if(choice == 4)
            print_instructions();
        else if(choice == 5)
            return 0;
        else
            printf("The symbol '%d' is not a valid choice, please choose another selection", choice);
    }while(choice >= 2 || choice == 0);

    // Init 2 10x10 2D char arrays all filled with ' '.
    char player_map[MAP_SIZE][MAP_SIZE];
    char op_map[MAP_SIZE][MAP_SIZE];

    int op_hits = 0;        // Amount of hits AGAINST the oponent
    int player_hits = 0;    // Amount of hits AGAINST the player

    init_empty_map(player_map);
    init_empty_map(op_map);
    
    // Generate player's map.
    int carrier = CAR_SIZE;
    int battleship = BAT_SIZE;
    int destroyer = DES_SIZE;
    int submarine = SUB_SIZE;
    int patrol_boat = PAT_SIZE;

    // Make player's map.
    


    // While both players have an unsunk battleship, let them hit back and forth.
    struct target_coord target;

    while(op_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        
    }

    kill(oponent_process, SIGKILL);

    return 1;
}