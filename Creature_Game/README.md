# Creature_Game

Simple game (Which I did not invent, just programed) where the player has to clean or dirty rooms depending on the creatures that are in each of the rooms. If player respect gets too high, they win the game. If player respect gets too low, they lose the game. 

Expects the following user input:
1. Number of rooms between 1 and 100. 
2. Five integers, first being room cleanliness and all of the neighbors. Options: 0 clean, 1 half dirty, 2 dirty, -1 no neighbor
3. Number of creatures between 1 and 100.
4. Two integers, first being creaturetype. Options: 0 for player, 1 for animal, 2 for NPC. Second being location of room 

To run this project:

        make creaturegame.c
        ./a.out
        follow on screen instructions
