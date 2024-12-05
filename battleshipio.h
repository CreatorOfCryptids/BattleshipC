/*
 * This header file contains the methods used to streamline printing.
 */

#include <stdio.h>
#include <string.h>
#include "constants.h"

#define HIT "\x1b[31m"
#define SHIP "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

char buff[256];

/**
 * Gets a character input from the user.
 *
 * @return The selected character.
 */
char read_char()
{
    scanf(" %[^\n]%*c", buff);
    return buff[0];
}

/**
 * Gets an integer from the user. If the inputted item is not a number, returns 0.
 *
 * @return The selected number.
 * @
 */
int read_int()
{
    scanf(" %[^\n]%*c", buff);
    return atoi(buff);
}

/**
 * Prints a map.
 *
 * @param map A square 2D array of size MAP_SIZE to be printed as a battleship map.
 */
void print_map(char map[MAP_SIZE][MAP_SIZE])
{

    char columns[MAP_SIZE];
    char first_column = 'A';
    for (int i = 0; i < MAP_SIZE; i++)
        columns[i] = first_column + i;

    for (int i = 0; i < MAP_SIZE + 1; i++)
        printf("+———");

    printf("+\n");

    // Print column label
    printf("|   ");

    for (int i = 0; i < MAP_SIZE; i++)
        printf("| %c ", columns[i]);

    printf("|\n");

    // Print each row with a row number on the leftmost side.
    for (int y = 0; y < MAP_SIZE; y++)
    {

        for (int i = 0; i < MAP_SIZE + 1; i++)
            printf("+———");

        printf("+\n");

        printf("|%2d ", y + 1);

        for (int x = 0; x < MAP_SIZE; x++)
            if (map[x][y] != 'X')
            {
                printf("|" SHIP " %c " COLOR_RESET, map[x][y]);
            }

        printf("|\n");
    }

    for (int i = 0; i < MAP_SIZE + 1; i++)
        printf("+———");

    printf("+\n");
}

/**
 * Prints the two maps side by side.
 *
 * @param ship_map The map containing the user's ships.
 * @param hit_map The map containing the user's hits/misses.
 */
void print_maps(char ship_map[MAP_SIZE][MAP_SIZE], char hit_map[MAP_SIZE][MAP_SIZE])
{
    // Print map labels
    printf("\t\tOPPONENT MAP\t\t\t\t\t   YOUR MAP\n");

    // Print map top borders
    for (int i = 0; i < MAP_SIZE + 1; i++)
        printf("+———");

    printf("+\t");

    for (int i = 0; i < MAP_SIZE + 1; i++)
        printf("+———");

    printf("+\n");

    // Print map column headers.
    printf("|   ");

    for (int i = 0; i < MAP_SIZE; i++)
        printf("| %c ", 'A' + i);

    printf("|\t|   ");

    for (int i = 0; i < MAP_SIZE; i++)
        printf("| %c ", 'A' + i);

    printf("|\n");

    // Print each row.
    for (int y = 0; y < MAP_SIZE; y++)
    {

        // Boarders:
        for (int i = 0; i < MAP_SIZE + 1; i++)
            printf("+———");

        printf("+\t");

        for (int i = 0; i < MAP_SIZE + 1; i++)
            printf("+———");

        printf("+\n");

        // First map
        printf("|%2d ", y + 1);

        for (int x = 0; x < MAP_SIZE; x++)
            if (hit_map[x][y] == 'H')
            {
                printf("|" HIT " %c " COLOR_RESET, hit_map[x][y]);
            }
            else
            {
                printf("| %c ", hit_map[x][y]);
            }

        printf("|\t");

        // Second map
        printf("|%2d ", y + 1);

        for (int x = 0; x < MAP_SIZE; x++)
            if (ship_map[x][y] == 'H')
            {
                printf("|" HIT " %c " COLOR_RESET, ship_map[x][y]);
            }
            else if (ship_map[x][y] != 'X')
            {
                printf("|" SHIP " %c " COLOR_RESET, ship_map[x][y]);
            }
            else
            {
                printf("| %c ", ship_map[x][y]);
            }

        printf("|\n");
    }

    // Bottom boarders
    for (int i = 0; i < MAP_SIZE + 1; i++)
        printf("+———");

    printf("+\t");

    for (int i = 0; i < MAP_SIZE + 1; i++)
        printf("+———");

    printf("+\n");
}

/**
 * Prints the title
 */
void print_title()
{
    printf("\t\tBATTLESHIP\n\n");
}

/**
 * Prints the starter menu.
 */
void print_menu()
{
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
void print_instructions()
{
    printf("Haven't written them yet, sorry.\n");
}

/**
 * Prints the settings menu.
 */
void print_settings_menu()
{
    printf("Not implemented yet, sorry.\n");
}

/**
 * Prints the networking options.
 */
void print_networking_options()
{
    printf("Networking options: \n");
    printf("  (1) Host Server\n");
    printf("  (2) Connect to server\n");
    printf("  (3) Return to menu\n");
}

/**
 * Gets an integer user input.
 *
 * @return The inputted integer, or 0 if input is unreadable.
 */
int get_menu_selection()
{
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
struct coord read_coord()
{

    struct coord retval;
    retval.x = 0;
    retval.y = -1;

    char x_letter = 0;

    do
    {
        printf("Letter: ");
        x_letter = read_char();

        if ((x_letter >= 'a' && x_letter <= 'z')) // If y is lowercase, change to uppercase
            x_letter = x_letter - 32;

        if (x_letter < 'A' || x_letter > 'A' + MAP_SIZE - 1)
        { // Make sure the coord is in a valid range
            printf("The coordinate %c is invalid, please choose a character between 'A' and '%c'.\n", x_letter, ('A' + MAP_SIZE - 1));
            x_letter = 0; // Continue the loop
        }

        retval.x = (int)(x_letter - 'A');

    } while (x_letter == 0);

    // printf("Escape???");

    while (1)
    {

        printf("Number: ");
        retval.y = read_int() - 1; // Start at zero.

        if (retval.y < 0 || retval.y > MAP_SIZE)
        { // If the input is invalid, loop again
            printf("The coordinate %d is invalid, please choose a number between 1 and %d\n", retval.y + 1, MAP_SIZE);
            continue;
        }
        else // Otherwise break;
            break;
    }

    return retval;
}

/**
 * Prints a coordinate.
 *
 * @param coord The coordinate to be printed.
 */
void print_coord(struct coord coord)
{
    printf("%c%d", coord.x + 'A', coord.y + 1);
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
int get_ship_selection()
{
    int choice = -1;

    printf("Which ship would you like to move/place?\n");
    printf("  (C)arrier\t%d tiles long.\n", CAR_SIZE);
    printf("  (B)attleship\t%d tiles long.\n", BAT_SIZE);
    printf("  (D)estroyer\t%d tiles long.\n", DES_SIZE);
    printf("  (S)ubmarine\t%d tiles long.\n", SUB_SIZE);
    printf("  (P)atrol Boat\t%d tiles long.\n", PAT_SIZE);
    printf("Choice: ");

    do
    {
        char input = read_char();

        if ((input >= 'a' && input <= 'z')) // If input is lowercase, change to uppercase
            input = input - 32;

        if (input == 'C')
            choice = 0;
        else if (input == 'B')
            choice = 1;
        else if (input == 'D')
            choice = 2;
        else if (input == 'S')
            choice = 3;
        else if (input == 'P')
            choice = 4;
        else
            printf("The selection '%c' is not on the list, please choose a selection from the list.", input);

    } while (choice == -1);

    return choice;
}

/**
 * Prints the appropriate message, for each hit response.
 */
void print_hit_response(char response)
{

    printf("\nOpponent: ");

    if (response == 'M')
    {
        printf("MISS!!!\n");
    }
    else if (response == 'H')
    {
        printf("HIT!!!\n");
    }
    else if (response == 'C')
    {
        printf("SINK!!! You sunk their Carrier!\n");
    }
    else if (response == 'B')
    {
        printf("SINK!!! You sunk their Battleship!\n");
    }
    else if (response == 'D')
    {
        printf("SINK!!! You sunk their Destroyer!\n");
    }
    else if (response == 'S')
    {
        printf("SINK!!! You sunk their Submarine!\n");
    }
    else if (response == 'P')
    {
        printf("SINK!!! You sunk their Patrol Boat!\n");
    }
    else
    {
        perror("Host: Incorrect response");
        exit(1);
    }
}