all: battleship

battleship: battleship.c battleshipio.h constants.h
	gcc battleship.c -o battleship -Wall