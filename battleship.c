#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "printing.h"
#include "constants.h"



/**
 * The init_empty_map() method.
 * @return a 2D char array with each value set to ' '.
*/
void init_empty_map(char map[MAP_SIZE][MAP_SIZE]){

    for(int x=0; x<MAP_SIZE; x++) 
        for(int y =0; y<MAP_SIZE; y++)
            map[x][y] = ' ';
}

int place_ship(char *map[MAP_SIZE][MAP_SIZE], int x, int y, int orientation, int length, char value){
    
    if(orientation){
        for(int i =0; i<length; i++)
            if (*map[x][y-i] != ' ')
                return -1;
        
        for(int i = 0; i<length; i++)
            *map[x][y-i] = 'C';
    }
    else{
        for(int i =0; i<length; i++)
            if (*map[x-i][y] != ' ')
                return -1;
        
        for(int i = 0; i<length; i++)
            *map[x-i][y] = 'C';
    }
    
    return 0;
}

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
    int x,y,orientation;
    if ((orientation = rand() % 2)){    // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)
        // Vertical
        x = rand() % MAP_SIZE;
        y = rand() % MAP_SIZE - CAR_SIZE;
    }
    else{   // Horizontal
        x = rand() % MAP_SIZE - CAR_SIZE;
        y = rand() % MAP_SIZE;
    }

    place_ship(&cpu_ships_map, x, y, orientation, CAR_SIZE, 'C'); 

    // place battleship
    do{
        if ((orientation = rand() % 2)){    // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)
            // Vertical
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE - BAT_SIZE;
        }
        else{   // Horizontal
            x = rand() % MAP_SIZE - BAT_SIZE;
            y = rand() % MAP_SIZE;
        }
    }
    while(place_ship(&cpu_ships_map, x, y, orientation, BAT_SIZE, 'B'));

    // place destroyer
    do{
        if ((orientation = rand() % 2)){    // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)
            // Vertical
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE - DES_SIZE;
        }
        else{   // Horizontal
            x = rand() % MAP_SIZE - DES_SIZE;
            y = rand() % MAP_SIZE;
        }
    }
    while(place_ship(&cpu_ships_map, x, y, orientation, DES_SIZE, 'D'));

    // place sub
    do{
        if ((orientation = rand() % 2)){    // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)
            // Vertical
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE - SUB_SIZE;
        }
        else{   // Horizontal
            x = rand() % MAP_SIZE - SUB_SIZE;
            y = rand() % MAP_SIZE;
        }
    }
    while(place_ship(&cpu_ships_map, x, y, orientation, SUB_SIZE, 'S'));

    // place patrol boat
    do{
        if ((orientation = rand() % 2)){    // Get a random 1 or 0 to see if the ship is vertical (1) or horizontal (0)
            // Vertical
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE - PAT_SIZE;
        }
        else{   // Horizontal
            x = rand() % MAP_SIZE - PAT_SIZE;
            y = rand() % MAP_SIZE;
        }
    }
    while(place_ship(&cpu_ships_map, x, y, orientation, PAT_SIZE, 'P'));

    print_map(cpu_ships_map);
    exit(0);

    int carrier = CAR_SIZE;
    int battleship = BAT_SIZE;
    int destroyer = DES_SIZE;
    int submarine = SUB_SIZE;
    int patrol_boat = PAT_SIZE;

    int cpu_hits = 0;        // Amount of hits AGAINST the oponent
    int player_hits = 0;    // Amount of hits AGAINST the player

    write(inputs[1], "R", 1);   // Send message to parent to let it continue.

    while(cpu_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        // Pipe random coordinates until a hit.

    }
    

    exit(0);
}

void multiplayer_server(int inputs[2], int outputs[2]){

    // Networking bs

    write(inputs[1], "1", 1);   // Allow the main process to continue.



    while(1){

    }

    exit(0);
}

void multiplayer_client(int inputs[2], int outputs[2]){

    // Networking bs

    write(inputs[1], "1", 1);   // Allow the main process to continue.



    while(1){

    }

    exit(0);
}

void init_singleplayer(pid_t *pid, int inputs[2], int outputs[2]){
    pid_t child;
    if ((child = fork()) == 0){
        singleplayer(inputs, outputs);
    }
    else if (child == -1){
        perror("There was an issue forking the singleplayer process");
        exit(1);
    }
    else{
        *pid = child;
    }
}

void init_multiplayer(pid_t *pid, int inputs[2], int outputs[2], char connection_choice){
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
    else{
        *pid = child;
    }
}

int main(){

    unsigned char choice = 0;
    pid_t oponent_process = 0;
    int inputs[2];      // Information coming from the chosen oponent
    int outputs[2];     // Information going to the chosen oponent

    if (pipe(inputs) == -1){
        perror("Input piping error");
        exit(1);
    }

    if (pipe(outputs) == -1){
        perror("Output piping error");
        exit(1);
    }

    while(choice <= 3 && choice !=0){
        //Print title and menu
        print_menu();

        if ((choice = get_selection()) == '1')   // If singleplayer, fork a process to play battlship with rand.
            init_singleplayer(&oponent_process, inputs, outputs);
        else if(choice == '2'){   // If multiplayer, ask if user wants to host or connect.

            print_networking_options();

            if ((choice = get_selection()) == '1')   // User wants to host
                init_multiplayer(&oponent_process, inputs, outputs, 0);
            else if(choice == '2')                    // User wants to connect.
                init_multiplayer(&oponent_process, inputs, outputs, 1);
            else{                                    // Other
                choice = 0; // Set to 0, so the while loop keeps working.
                break;
            }

            char* output = 0;

            read(inputs[0], output, 1); // Waits until a message is sent by the child process, indicating that 
            if (*output == -1)  // Quit if we recive an error message.
                exit(1);
        }
        else if (choice == '3')
            print_settings_menu();
        else if(choice == '4')
            print_instructions();
        else if(choice == '5')
            return 0;
        else
            printf("The symbol '%c' is not a valid choice, please choose another selection", choice);
    }

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



    // While both players have an unsunk battleship, let them hit back and forth.
    while(op_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        
    }

    kill(oponent_process);

    return 1;
}