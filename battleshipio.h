/*
 * This header file contains the methods used to streamline printing.
 */

#include <stdio.h>
#include <string.h>
#include "constants.h"

char buff[256];

/**
 * Gets a character input from the user.
 * 
 * @return The selected character.
*/
char read_char(){
    scanf(" %[^\n]%*c", buff);
    return buff[0];
}

/**
 * Gets an integer from the user. If the inputed item is not a number, returns 0.
 * 
 * @return The selected number.
 * @
*/
int read_int(){
    scanf(" %[^\n]%*c", buff);
    return atoi(buff);
}

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
 * 
 * @return The inputed integer, or 0 if input is unreadable.
*/
int get_menu_selection(){
    unsigned int choice;

    printf("\nChoice: ");
    choice = read_int();

    return choice;
}

/**
 * Gets a coordinate from user input.
 * 
 * @return The selected coordinate.
*/
struct coord read_coord(){

    struct coord retval;
    retval.x = 0;
    retval.y = -1;

    char x_letter = 0;

    do {
        printf("Letter: ");
        x_letter = read_char();

        if ((x_letter >='a' && x_letter <= 'z'))            // If y is lowercase, change to uppercase
            x_letter = x_letter - 32;

        if (x_letter < 'A' || x_letter > 'A' + MAP_SIZE -1){ // Make sure the coord is in a valid range
            printf("The coordinate %c is invalid, please choose a character between 'A' and '%c'.\n", retval.x, ('A' + MAP_SIZE - 1));
            x_letter = 0;                          // Continue the loop
        }

        retval.x = (int) (x_letter - 'A' - 1);

    }while (x_letter == 0);

    // printf("Escape???");

    do {

        printf("Number: ");
        retval.y = read_int();
        
        if (retval.y < 1 || retval.y > MAP_SIZE){   // If the input is invalid, loop again
            printf("The coordinate %d is invalid, please choose a number between 1 and %d\n", retval.y, MAP_SIZE);
            continue;
        }
        else    // Otherwize break;
            break;  

    }while(1);
    
    return retval;
}

/**
 * Prints a coordinate.
 * 
 * @param coord The coordinate to be printed.
*/
void print_coord(struct coord coord){
    printf("%c%d", coord.x + 'A' +1, coord.y);
}

/**
 * Gets the user's selection of which ship they'd like to place.
 * 
 * @return A number 0-4 That corresponds to the user's choice. 
 * 0 - Carrier
 * 1 - Battleship
 * 2 - Destroyer
 * 3 - Submarine
 * 4 - Patrol Boat
*/
int get_ship_selection(){
    int choice = -1;

    printf("Which ship would you like to move/place?\n");
    printf("  (C)arrier\t%d tiles long.\n", CAR_SIZE);
    printf("  (B)attleship\t%d tiles long.\n", BAT_SIZE);
    printf("  (D)estroyer\t%d tiles long.\n", DES_SIZE);
    printf("  (S)ubmarine\t%d tiles long.\n", SUB_SIZE);
    printf("  (P)atrol Boat\t%d tiles long.\n", PAT_SIZE);
    printf("Choice: ");

    do{
        char input = read_char();

        if ((input >='a' && input <= 'z'))            // If input is lowercase, change to uppercase
            input = input - 32;

        if(input == 'C')
            choice = 0;
        else if(input == 'B')
            choice = 1;
        else if(input == 'D')
            choice = 2;
        else if(input == 'S')
            choice = 3;
        else if(input == 'P')
            choice = 4;
        else
            printf("The selection '%c' is not on the list, please choose a selection from the list.", input);

    }while(choice == -1);

    return choice;
}
