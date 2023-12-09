/*
 * This header file contains the methods used to streamline printing.
 */

#include <stdio.h>
#include <string.h>
#include "constants.h"

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

/**
 * Prints the starter menu.
*/
void print_menu(){
    printf("\t\tBATTLESHIP\n\n");
    printf("Choose an option:\n");
    printf("  (1) Single player\n");
    printf("  (2) Multiplayer\n");
    printf("  (3) Settings\n");
    printf("  (3) How to play\n");
    printf("  (4) Quit\n");
}

/**
 * Prints the instructions of the game.
*/
void print_instructions(){
    printf("Havent written them yet, sorry.\n");
}

void print_settings_menu(){
    printf("Not implemented yet, sorry.\n");
}

void print_networking_options(){
    printf("Networking options: \n");
    printf("  (1) Host Server\n");
    printf("  (2) Connect to server\n");
    printf("  (3) Return to menu\n");
}

int get_selection(){
    unsigned int choice;
    printf("\nChoice: ");
    scanf("%d", &choice);
    return choice;
}
