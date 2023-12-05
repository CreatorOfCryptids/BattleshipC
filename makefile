all: battleship

battleship: battleship.c printing.h constants.h
	gcc battleship.c -o battleship -Wall