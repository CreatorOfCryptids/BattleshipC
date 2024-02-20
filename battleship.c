#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include "battleshipio.h"

char ship_symbols[5] = "CBDSP";

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
    
    //printf("DEBUG PLACE_SHIP: X: %d Y: %d\n", coord.x, coord.y);

    if(orientation){// Vertical
        // Check if the ship is on the map.
        if(coord.x >= MAP_SIZE || coord.x < 0 || coord.y + length > MAP_SIZE || coord.y < 0)
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
        if(coord.x + length > MAP_SIZE || coord.x < 0 || coord.y >= MAP_SIZE || coord.y < 0)
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
 * Gets the users target coordinate, sends it to the opponent process, and prints the oppenent's responce.
 * 
 * @param op_map The map of coordinates that the player has already hit.
 * @param input The pipe ID for messages going TO the host.
 * @param output The pipe ID for messages going FROM the host.
 */
void get_user_target_sequence(char ship_map[MAP_SIZE][MAP_SIZE], char op_map[MAP_SIZE][MAP_SIZE], int input, int output, int *op_hits){
    struct coord target_coord;

    while(1){
        print_maps(ship_map, op_map);
        printf("\n");

        target_coord = read_coord();

        if(op_map[target_coord.x][target_coord.y] != ' '){
            printf("\nYou have already hit that coordinate, please select another.\n\n");
        }
        else{
            break;
        }
    }

    // Update op_map
    

    // Send coord
    write(output, &target_coord, sizeof(struct coord));

    // Print results
    char target;
    read(input, &target, sizeof(char));
    print_hit_responce(target);

    op_map[target_coord.x][target_coord.y] = target == 'M' ? 'X' : 'H';

    if(target != 'M')
        op_hits--;
}

/**
 * Determines if a ship was hit. Updates the map to reflect the outcome, and returns the result (H, M, or sink).
 * 
 * @param ship_map A map containing the host's ships.
 * @param target_coord The coord of the incoming shot.
 * @param ship_lengths The remaining lengths of the ships in the host's map.
 * @param hits A pointer to the host's remaining ship health.
 * @return A char representing a Hit, Miss, or Sink. Returns the ship symbol of the sunken ship, when a ship is sunk.
 */
char incoming_shot(char ship_map[MAP_SIZE][MAP_SIZE], struct coord target_coord, int ship_lengths[SHIP_COUNT], int *hits){

    char target = ship_map[target_coord.x][target_coord.y]; // What was stored in the ship_map at the target coord

    if(target == ' ' || target == 'X') {
        ship_map[target_coord.x][target_coord.y] = 'X';
        return 'M';
    }
    else if (target == 'H'){
        return 'M';
    }
    else {

        ship_map[target_coord.x][target_coord.y] = 'H';
        hits++;

        if (target == 'C'){
            ship_lengths[0]--;
        }
        else if (target == 'B'){
            ship_lengths[1]--;
        }
        else if (target == 'D'){
            ship_lengths[2]--;
        }
        else if (target == 'S'){
            ship_lengths[3]--;
        }
        else if (target == 'P'){
            ship_lengths[4]--;
        }

        for(int i = 0; i<SHIP_COUNT; i++){
            if (ship_lengths[i] == 0){
                ship_lengths[i]--;
                return ship_symbols[i];
            }
        }
        return 'H';
    }
}

/**
 * The code for the singleplayer opponent process.
 * 
 * @param input The pipe ID to send info TO the host process FROM the child opponent process.
 * @param output The pipe ID to send info FROM the host process the TO child opponent process.
 */
void singleplayer(int input, int output){
    
    char cpu_ships_map[MAP_SIZE][MAP_SIZE]; // Map containing CPU's ships
    char cpu_hit_map[MAP_SIZE][MAP_SIZE];   // Map containing coordinaties the CPU has attacked.

    init_empty_map(cpu_ships_map);
    init_empty_map(cpu_hit_map);
    
    // Generate CPU's map.

    
    struct coord coord;
    int orientation;

    srand(time(NULL));
    int ship_lengths[SHIP_COUNT] = {CAR_SIZE, BAT_SIZE, DES_SIZE, SUB_SIZE, PAT_SIZE};

    for(int i=0;i<SHIP_COUNT;i++){ // Skip first entry becuase already placed Carrier.
        do{
            if ((orientation = rand() % 2) == 1){  // Vertical
                if((coord.x = rand() % MAP_SIZE) < 0)
                    coord.x = coord.x * -1;
                if((coord.y = rand() % MAP_SIZE - ship_lengths[i]) < 0)
                    coord.y = coord.y * -1;
            }
            else{                                   // Horizontal
                if((coord.x = rand() % MAP_SIZE - ship_lengths[i]) < 0)
                    coord.x = coord.x * - 1;
                if((coord.y = rand() % MAP_SIZE) < 0)
                    coord.y = coord.y * - 1;
            }
        }
        while(place_ship(cpu_ships_map, coord, orientation, ship_lengths[i], ship_symbols[i]) == -1);
    }

    /* Test random map generation.
    print_map(cpu_ships_map);
    exit(0);
    /**/

    int cpu_hits = 0;       // Amount of hits AGAINST the CPU
    int player_hits = 0;    // Amount of hits AGAINST the player

    write(input, "r", 1);   // Send message to parent to let it continue.
    
    struct coord target_coord;
    struct coord last_hit;
    char target;
    read(output, &target, sizeof(char));

    while(cpu_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        // Take in target coord since the user always goes first in single player
        read(output, &target_coord, sizeof(struct coord));
        target = incoming_shot(cpu_ships_map, target_coord, ship_lengths, &cpu_hits);

        write(input, &target, sizeof(char));        
        
        // Pipe random coordinates // TODO: until a hit...
        while(1){
            if((target_coord.x = rand() % MAP_SIZE) < 0)
                target_coord.x = target_coord.x * -1;
            if((target_coord.y = rand() % MAP_SIZE) < 0)
                target_coord.y = target_coord.y * -1;

            if(cpu_hit_map[target_coord.x][target_coord.y] == ' ')
                break;
        }
        
        write(input, &target_coord, sizeof(struct coord));

        read(output, &target, sizeof(char));
        if(target == 'M' || target == 'H')
            cpu_hit_map[target_coord.x][target_coord.y] = target;
        else{
            cpu_hit_map[target_coord.x][target_coord.y] = 'H';
            last_hit = target_coord;
        }
    }

    exit(0);
}

/**NOT YET IMPLEMENTED!
 * The code for the multiplayer opponent process that RECIVES information from a client.
 * 
 * @param input The pipe ID to send info TO the host Process FROM the child opponent process.
 * @param output The pipe ID to send info FROM the host process the TO child opponent process.
 */
void multiplayer_server(int input, int output){

    // Networking bs

    printf("Waiting for connection...\n");

    

    if (0){     // Wait for confirmed connection.
        char confirm_code = 'C';
        write(input, &confirm_code, sizeof(char));   // Allow the main process to continue.
    }
    



    while(1){

    }

    exit(0);
}

/**NOT YET IMPLEMENTED!
 * The code for the multiplayer opponent process that SENDS information to the host.
 * 
 * @param input The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param output The pipe IDs to send info FROM the host process the TO child opponent process.
 */
void multiplayer_client(int input, int output){

    // Networking bs

    char confirm_code = 'C';
    write(input, &confirm_code, sizeof(char));   // Allow the main process to continue.

    while(1){

    }

    exit(0);
}

/**
 * Main function.
 */
int main(){

    int inputs[2];      // Information coming FROM the chosen oponent
    int outputs[2];     // Information going TO the chosen oponent

    // Init pipes and check that they were initiated correctly.
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

    unsigned int choice = 0;
    pid_t oponent_process = 0;

    do{ //Print menu and get selections.
        print_menu();

        if ((choice = get_menu_selection()) == 1){   // If singleplayer, fork a process to play battlship with rand.
            //oponent_process = init_singleplayer(inputs[1], outputs[0]);
            if ((oponent_process = fork()) == 0){
                singleplayer(inputs[1], outputs[0]);
            }
            else if (oponent_process == -1){
                perror("There was an issue forking the singleplayer process");
                return 1;
            }
        }
        else if (choice == 2){    // If multiplayer, ask if the user wants to host or connect.

            print_networking_options();

            if ((choice = get_menu_selection()) == 1){   // User wants to host
                if ((oponent_process = fork()) == 0){
                    multiplayer_server(inputs[1], outputs[0]);
                }
                else if (oponent_process == -1){
                    perror("There was an issue forking the multiplayer process");
                    return 1;
                }
            }
            else if(choice == 2){                // User wants to connect.
                if ((oponent_process = fork()) == 0){
                    multiplayer_client(inputs[1], outputs[0]);
                }
                else if (oponent_process == -1){
                    perror("There was an issue forking the multiplayer process");
                    return 1;
                }
            }
            else{                                    // Other
                choice = 0; // Set to 0, so the while loop keeps working.
                continue;
            }

            char confirmation;

            read(inputs[0], &confirmation, sizeof(char));// Waits until a message is sent by the child process, indicating that the connection has been made.
        }
        else if (choice == 3)
            print_settings_menu();
        else if(choice == 4)
            print_instructions();
        else if(choice == 5)
            return 0;

        else if(choice == 6){
            printf("\t\tTESTING MODE\n");

            /* Test read_coord()
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

    init_empty_map(player_map);
    init_empty_map(op_map);
    
    int op_hits = 0;        // Amount of hits AGAINST the oponent
    int player_hits = 0;    // Amount of hits AGAINST the player
    
    // Generate player's map.
    int ship_lengths[5] = {CAR_SIZE, BAT_SIZE, DES_SIZE, SUB_SIZE, PAT_SIZE};

    int ship_tiles_placed = 0;

    //* Skip inputing ship map to skip to interface
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

            //printf("DEBUG: Placing ship at X: %d Y: %d\n", ship_coord.x, ship_coord.y);

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
            printf("Are you happy with this map?\n\n");
            print_map(player_map);
            printf("Choice (Y/n): ");
            char happy = read_char();
            if (happy == 'Y' || happy == 'y')
                break;
            else 
                continue;
        }
    }/**/

    //Tell other process that host is ready
    if(write(outputs[1], "R", 1) != 1){
        perror("HOST: Issue writng to output.");
        exit(1);
    }

    printf("Waiting for opponent...\n\n");

    char responce;
    read(inputs[0], &responce, 1);

    printf("Your opponent is ready! ");

    struct coord target_coord;
    char target;

    if (responce == 'R'){       // Capital R means that this user goes second.
        printf("You go second.\n\n");
    }
    else if (responce == 'r'){  // This user goes first.

        printf("You go first! Where would you like to aim?\n");
        
        get_user_target_sequence(player_map, op_map, inputs[0], outputs[1], &op_hits);
    }
    else{
        perror("HOST: recived incorrect init responce.");
        exit(1);
    }

    // While both players have an unsunk battleship, let them hit back and forth.
    while(op_hits < TOTAL_HITS-1 && player_hits < TOTAL_HITS-1){
        // Get opponent's target.
        read(inputs[0], &target_coord, sizeof(struct coord));
        target = incoming_shot(player_map, target_coord, ship_lengths, &player_hits);

        // Respond to opponent.
        write(outputs[1], &target, sizeof(char));

        // Inform user.
        printf("Opponent: ");print_coord(target_coord);printf("!!!\n\n");

        //print_map(player_map);

        if(target == 'C'){
            printf("Your oponent sunk your Carrier!!!\n\n");
        }
        else if(target == 'B'){
            printf("Your oponent sunk your Battleship!!!\n\n");
        }
        else if(target == 'D'){
            printf("Your oponent sunk your Destroyer!!!\n\n");
        }
        else if(target == 'S'){
            printf("Your oponent sunk your Submarine!!!\n\n");
        }
        else if(target == 'P'){
            printf("Your oponent sunk your Patrol Boat!!!\n\n");
        }
        
        // Take in user's target coord.
        //printf("Where would you like to aim next?\n\n");
        get_user_target_sequence(player_map, op_map, inputs[0], outputs[1], &op_hits);
    }

    kill(oponent_process, SIGKILL);
    close(inputs[1]);
    close(outputs[1]);

    if(player_hits == TOTAL_HITS){
        printf("Sorry, you lose :(\n Better luck next time!");
    }
    else{
        printf("YOU WIN!!!!\n");
    }

    return 0;
}

// Outdated methods:
/** rand_ship_coord() OUTDATED!!!
 * Generates random coordinates for the start of a ship, and it's orientation. 
 * Will not return a value that puts the ship off the edge of the map.
 * 
 * @param coord A pointer to the coordinate for generation.
 * @param orientation A pointer to the orientation of the ship (with 1 being vertical and 0 being horizontal)
 * @param ship_length The length of the ship.
 *
void rand_ship_coord(struct coord *coord, int *orientation, int ship_length){

    *orientation = rand() % 2; // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)

    if (*orientation){  // Vertical
        if((coord->x = rand() % MAP_SIZE) < 0)
            coord->x = coord->x * -1;
        if((coord->y = rand() % MAP_SIZE - ship_length) < 0)
            coord->y = coord->y * -1;
    }
    else{               // Horizontal
        if((coord->x = rand() % MAP_SIZE - ship_length) < 0)
            coord->x = coord->x * - 1;
        if((coord->y = rand() % MAP_SIZE) < 0)
            coord->y = coord->y * - 1;
    }
}*/

/** init_singleplayer() OUTDATED!!!
 * OUTDATED!!! Initializes the child process that handles the singleplayer process.
 * 
 * @param input The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param output The pipe IDs to send info FROM the host process the TO child opponent process.
*
pid_t init_singleplayer(int input, int output){

    pid_t child;
    if ((child = fork()) == 0){
        singleplayer(input, output);
    }
    else if (child == -1){
        perror("There was an issue forking the singleplayer process");
        exit(1);
    }
    return child;
}*/

/** init_multiplayer() OUTDATED!!!
 * Initializes the child process that handles the multiplayer process.
 * 
 * @param input The pipe IDs to send info TO the host Process FROM the child opponent process.
 * @param output The pipe IDs to send info FROM the host process the TO child opponent process.
 * @param connection_choice The user's decition to host (1) or connect (0);
*
pid_t init_multiplayer(int input, int output, char connection_choice){
    pid_t child;
    if ((child = fork()) == 0){
        if (connection_choice)
            multiplayer_server(input, output);
        else 
            multiplayer_client(input, output);
        exit(1);
    }
    else if (child == -1){
        perror("There was an issue forking the multiplayer process");
        exit(1);
    }
    else{
        char confirmation;

        read(input, &confirmation, sizeof(char));

        return child;
    }
}*/
