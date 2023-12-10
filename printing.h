/*
 * This header file contains the methods used to streamline printing.
 */

#include <stdio.h>
#include <string.h>
#include "constants.h"

/**
 * Prints a map.
 * 
 * @param map A square 2D array of size MAP_SIZE to be printed as a battleship map.
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
 * Prints the title
*/
void print_title(){
    printf("\t\tBATTLESHIP\n\n");
}

/**
 * Prints the starter menu.
*/
void print_menu(){
    printf("Choose an option:\n");
    printf("  (1) Single player\n");
    printf("  (2) Multiplayer\n");
    printf("  (3) Settings\n");
    printf("  (4) How to play\n");
    printf("  (5) Quit\n");
}

/**
 * Prints the instructions of the game.
*/
void print_instructions(){
    printf("Havent written them yet, sorry.\n");
}

/**
 * Prints the settings menu.
*/
void print_settings_menu(){
    printf("Not implemented yet, sorry.\n");
}

/**
 * Prints the networking options.
*/
void print_networking_options(){
    printf("Networking options: \n");
    printf("  (1) Host Server\n");
    printf("  (2) Connect to server\n");
    printf("  (3) Return to menu\n");
}

/**
 * Gets an integer user input.
*/
int get_menu_selection(){
    unsigned int choice;
    printf("\nChoice: ");
    scanf("%d", &choice);
    return choice;
}

struct target_coord get_coord(){

    struct target_coord retval;

    while (retval.x_coord != 0){

        printf("Letter: ");
        scanf("%c\n", &retval.x_coord);

        if ((retval.x_coord >'a' && retval.x_coord > 'z'))            // If y is lowercase, change to uppercase
            retval.x_coord = retval.x_coord - 32;

        if (retval.x_coord < 'A' || retval.x_coord > 'A' + MAP_SIZE){ // Make sure the coord is in a valid range
            printf("The coordinate %c is invalid, please choose a character between 'A' and '%c'.\n", retval.x_coord, 'A' + MAP_SIZE);
            retval.x_coord = 0;                          // Continue the loop
        }
    }

    while (retval.y_coord > 0 && retval.y_coord < MAP_SIZE){

        printf("Number: ");
        scanf("%d\n", &retval.y_coord);
        
        if (retval.y_coord < 0 || retval.y_coord > MAP_SIZE)
            prinf("The coordinate %d is invalid, please choose a number between 1 and %d", retval.y_coord, MAP_SIZE);
    }
    
    return retval;
}
