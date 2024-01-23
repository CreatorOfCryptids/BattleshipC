# BattleshipC
This is a Battleship game that lets the user play a game of battleship either alone or with a friend.

## Interprocess communiction:
### Multiplayer Connection init:
Neither player should be able to start playing without an established connection, so the main process will be blocked until the connection is made. This is communiated by sending the char 'C' thru the pipe.

### Pre-Game Communication:
Neither player should be able to start playing before the other has finalized their board. To prevent this, main cannot continue untill it receives either a 'r' or 'R'. To ensure that the host starts first, the multiplayer_client will replace the first recived char from upper case to lower case.

### In-Game Communication: 
The players will take turns entering coordinates and will instantly get a responce of ('H') "HIT", ('M')"MISS", or "SINK!!!". A sink will return the letter of the ship that was sunk ('C', 'B', 'D', 'S', or 'P').
