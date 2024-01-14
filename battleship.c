#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include "battleshipio.h"

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
 * @param coord The starting coordinate of the ship to be added to the map
 * @param orientation The orientation of the ship, with 1 being vertical and 0 being horizontal
 * @param ship_symbol The type of ship being added
 * @return -1 for a collision, 0 if the ship can be added, and 1 if the ship goes off the edge of the map.
*/
int place_ship(char map[MAP_SIZE][MAP_SIZE], struct coord coord, int orientation, int length, char ship_symbol){
    
    printf("DEBUG PLACE_SHIP: X: %d Y: %d\n", coord.x, coord.y);

    if(orientation){// Vertical
        // Check if the ship is on the map.
        if(coord.x >= MAP_SIZE || coord.x < 0 || coord.y + length >= MAP_SIZE || coord.y < 0)
            return 1;

        // Check for collisions
        for(int i = 0; i<length; i++)
            if (map[coord.x][coord.y+i] != ' ')
                return -1;
        
        // Place in map
        for(int i = 0; i<length; i++)
            map[coord.x][coord.y+i] = ship_symbol;
    }
    else{           // Horizontal
        // Check if its on the map
        if(coord.x + length >= MAP_SIZE || coord.x < 0 || coord.y >= MAP_SIZE || coord.y < 0)
            return 1;

        // Check for collisions
        for(int i = 0; i<length; i++)
            if (map[coord.x+i][coord.y] != ' ')
                return -1;
        
        // Place in map
        for(int i = 0; i<length; i++)
            map[coord.x+i][coord.y] = ship_symbol;
    }
    
    return 0;
}

/**
 * Generates random coordinates for the start of a ship, and it's orientation. 
 * Will not return a value that puts the ship off the edge of the map.
 * 
 * @param coord A pointer to the coordinate for generation.
 * @param orientation A pointer to the orientation of the ship (with 1 being vertical and 0 being horizontal)
 * @param ship_length The length of the ship.
*/
void rand_ship_coord(struct coord *coord, int *orientation, int ship_length){

    *orientation = rand() % 2; // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)

    if (*orientation){  // Vertical
        if((coord->x = rand() % MAP_SIZE) < 0)
            coord->x = coord->x * -1;
        if((coord->y = rand() % MAP_SIZE - CAR_SIZE) < 0)
            coord->y = coord->y * -1;
    }
    else{               // Horizontal
        if((coord->x = rand() % MAP_SIZE - CAR_SIZE) < 0)
            coord->x = coord->x * - 1;
        if((coord->y = rand() % MAP_SIZE) < 0)
            coord->y = coord->y * - 1;
    }
}

/**
 * The code for the singleplayer opponent process.
 * 
 * @param inputs The pipe IDs to send info TO the host process FROM the child opponent process.
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
    struct coord coord;
    int orientation;

    rand_ship_coord(&coord, &orientation, CAR_SIZE);
    //printf("CAR: x: %d, y: %d, Orientation: %d\n", x, y, orientation);
    place_ship(cpu_ships_map, coord, orientation, CAR_SIZE, 'C'); 

    int ship_lengths[5] = {CAR_SIZE, BAT_SIZE, DES_SIZE, SUB_SIZE, PAT_SIZE};
    char ship_symbols[5] = "CBDSP";

    for(int i=1;i<5;i++){ // Skip first entry becuase already placed Carrier.
        do{
            rand_ship_coord(&coord, &orientation, ship_lengths[i]);
        }
        while(place_ship(cpu_ships_map, coord, orientation, ship_lengths[i], ship_symbols[i]) == -1);
    }

    /* Test random map generation.
    print_map(cpu_ships_map);
    exit(0);
    /**/

    int cpu_hits = 0;       // Amount of hits AGAINST the CPU
    int player_hits = 0;    // Amount of hits AGAINST the player

    struct coord target;

    write(inputs[1], "C", 1);   // Send message to parent to let it continue.
    read(outputs[0], &target, sizeof(struct coord));

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

    printf("Waiting for connection...\n");

    if (0){     // Wait for confirmed connection.
        write(inputs[1], "1", sizeof(char));   // Allow the main process to continue.
    }
    



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

/**
 * Main function.
*/
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

    do{ //Print menu and get selections.
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

            read(inputs[0], output, 1); // Waits until a message is sent by the child process, indicating that the connection has been made.

            if (*output == -1)          // Quit if we recive an error message.
                exit(1);
        }
        else if (choice == 3)
            print_settings_menu();
        else if(choice == 4)
            print_instructions();
        else if(choice == 5)
            return 0;

        else if(choice == 6){
            printf("\t\tTESTING MODE\n");

            //* Test read_coord()
            struct coord test;
            while(1){
                test = read_coord();
                print_coord(test);
                printf("\n");
            }
            /**/
        }
        else
            printf("The symbol '%d' is not a valid choice, please choose another selection", choice);
    }while(choice >= 2 || choice == 0);

    // Init 2 10x10 2D char arrays all filled with ' '.
    char player_map[MAP_SIZE][MAP_SIZE];    // Map containing the player's ships
    char op_map[MAP_SIZE][MAP_SIZE];        // Map containing the player's attacks

    int op_hits = 0;        // Amount of hits AGAINST the oponent
    int player_hits = 0;    // Amount of hits AGAINST the player

    init_empty_map(player_map);
    init_empty_map(op_map);
    
    // Generate player's map.
    int ship_lengths[5] = {CAR_SIZE, BAT_SIZE, DES_SIZE, SUB_SIZE, PAT_SIZE};
    char ship_symbols[5] = "CBDSP";

    int ship_tiles_placed = 0;

    while(1){
        // Display the current map
        printf("\n");   // Formatting.
        print_map(player_map);
        printf("\n");   // Formatting.

        // Ask the user which ship they'd like to select.
        int ship_choice = get_ship_selection();

        // Remove ship if already placed.
        for(int x=0; x<MAP_SIZE; x++) 
            for(int y =0; y<MAP_SIZE; y++)
                if(player_map[x][y] == ship_symbols[ship_choice])
                    player_map[x][y] = ' ';

        // Ask where they want to put the ship.
        struct coord ship_coord;
        int orientation = 1;

        while (1){
            // Vertical or Horizontal?
            printf("\nDo you want the ship to be placed (V)erticaly or (H)orizontally?\n");
            char choice = 0;
            while(choice == 0){
                printf("Selection: ");
                choice = read_char();

                if (choice == 'V' || choice == 'v')
                    orientation = 1;
                else if (choice == 'H' || choice == 'h')
                    orientation = 0;
                else{
                    printf("The selection %c is invalid, please enter either (V)ertical or (H)orizontal.\n", choice);
                    choice = 0;
                }
            }

            printf("\nWhere do you want to place the ship?\n(Note that the entered coordinate is the leftmost/highest point of the ship)\n");
            ship_coord = read_coord();

            printf("DEBUG: Placing ship at X: %d Y: %d\n", ship_coord.x, ship_coord.y);

            int retval = place_ship(player_map, ship_coord, orientation, ship_lengths[ship_choice], ship_symbols[ship_choice]);

            if(retval == 0){
                ship_tiles_placed += ship_lengths[ship_choice];
                break;
            }
            else if (retval == 1)
                printf("The ship goes off the edge of the map, please choose a valid coordinate.\n");
            else if (retval == -1)
                printf("The ship overlaps with another ship, please choose a different location.\n");
        }

        // If all the ships have been placed, ask if they are ready. If so, break.
        if(ship_tiles_placed == TOTAL_HITS){
            printf("Are you happy with this map (Y/N)?\n\n");
            print_map(player_map);
            char happy = read_char();
            if (happy == 'Y' || happy == 'y')
                break;
            else 
                continue;
        }
    }

    //Tell other process that hoast is ready
    if(write(outputs[1], "R", 1) != 1){
        perror("HOST: Issue writng to output.");
        exit(1);
    }

    printf("Waiting for opponent...\n");

    char responce;
    read(inputs[0], responce, 1);

    if (responce == 'R'){   // Capital R means that this user goes second.
    
    }

    // While both players have an unsunk battleship, let them hit back and forth.
    struct coord target;

    while(op_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        
    }

    kill(oponent_process, SIGKILL);

    return 0;
}