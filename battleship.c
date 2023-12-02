#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

void singleplayer(int *inputs_[2], int *outputs_[2]){
    int inputs[2] = *inputs_;
    int outputs[2] = *outputs_;
    
    srand(time(NULL));
    
    // Generate random map.

    while(){
        // Pipe random coordinates until a hit.

    }
    

    exit(0);
}

void multiplayer(int *inputs_[2], int *outputs_[2]){
    int inputs[2] = *inputs_;
    int outputs[2] = *outputs_;

    // Networking bs

    

    exit(0);
}

void init_singleplayer(pid_t *pid, int *inputs[2], int *outputs[2]){
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

void init_multiplayer(pid_t *pid, int *inputs[2], int *outputs[2]){
    pid_t child;
    if ((child = fork()) == 0){
        multiplayer(inputs, outputs);
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

    //Print title and menu
    print_menu();
    unsigned char choice = 0;
    pid_t oponent_process = 0;
    int inputs[2];      // Information coming from the chosen oponent
    int outputs[2];     // Information going to the chosen oponent

    while(choice <= 3 && choice !=0){
        scanf("%c\n", choice);

        if (choice == 1)        // If singleplayer, fork a process to play battlship with rand.
            init_singleplayer(&oponent_process, &inputs, &outputs);
        else if(choice == 2)    // If multiplayer, fork a process to establish network connection and handle other player input.
            init_multiplayer(&oponent_process, &inputs, &outputs);
        else if (choice == 3)
            settings_menu();
        else if(choice == 4)
            print_instructions();
        else if(choice == 5)
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
    
    // Init game
    int carrier = CAR_SIZE;
    int battleship = BAT_SIZE;
    int destroyer = DES_SIZE;
    int submarine = SUB_SIZE;
    int patrol_boat = PAT_SIZE;

    // While both players have an unsunk battleship, let them hit back and forth.
    while(op_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        
    }

    wait(oponent_process);

    return 1;
}