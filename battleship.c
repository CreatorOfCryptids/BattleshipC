#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 10
#define TOTAL_HITS 17

/**
 * The init_empty_map() method.
 * @return a 2D char array with each value set to ' '.
*/
void init_empty_map(char map[MAP_SIZE][MAP_SIZE]){

    for(int x=0; x<MAP_SIZE; x++) 
        for(int y =0; y<MAP_SIZE; y++)
            map[x][y] = ' ';

}

/**
 * The print_map() method.
 * @param map a 2D array to be printed as a battleship map.
*/
void print_map(char map[MAP_SIZE][MAP_SIZE]){

    char collumns[MAP_SIZE];
    char first_collumn = 'A';
    for(int i =0; i<MAP_SIZE; i++)
        collumns[i] = first_collumn+i;

    for(int i = 0; i<MAP_SIZE+1; i++)
        printf("+———");
    
    printf("+\n");

    // Print collumn lable
    printf("|   ");

    for(int i=0; i<MAP_SIZE; i++)
        printf("| %c ", collumns[i]);
    
    printf("|\n");


    // Print each row with a row number on the leftmost side.
    for(int x = 0; x<MAP_SIZE; x++){

        for(int i = 0; i<MAP_SIZE+1; i++)
            printf("+———");

        printf("+\n");

        printf("|%2d ", x+1);

        for(int y = 0; y<MAP_SIZE; y++)
            printf("| %c ", map[x][y]);

        printf("|\n");
    }
    
    for(int i = 0; i<MAP_SIZE+1; i++)
        printf("+———");
    
    printf("+\n");
}

int main(){
    // Init 2 10x10 2D char arrays all filled with ' '.

    char player_map[MAP_SIZE][MAP_SIZE];
    char op_map[MAP_SIZE][MAP_SIZE];
    int op_hits = 0;        // Amount of hits AGAINST the oponent
    int player_hits = 0;    // Amount of hits AGAINST the player

    init_empty_map(player_map);
    init_empty_map(op_map);

    /*
    printf("Testing printing the player map: \n");
    print_map(player_map);
    printf("\nTesting printing the oponent's map:\n");
    print_map(op_map);
    /**/

    // Ask if singleplayer or multiplayer.
    // If multiplayer, fork a process to establish network connection and handle other player input.

    // Otherwize fork a process to play battlship with rand.

    // While both players have an unsunk battleship, let them hit back and forth.
    while(op_hits < TOTAL_HITS && player_hits < TOTAL_HITS){
        
    }

    return 1;
}