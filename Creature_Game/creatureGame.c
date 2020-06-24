#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "myconst.h"

typedef struct Room{
        int name;
        int state;

        struct Creature* creatures[10];

        struct Room* north;
        struct Room* south;
        struct Room* east;
        struct Room* west;
} Room;

typedef struct Creature{
        int name;
        int type;
        Room* location;
} Creature;

void initializeCreatures(Room* room);
int fillRoom(Room* room, Creature* creature);
void look(Room* room);
void clean(Room* room, int cleaner);
void dirty(Room* room, int cleaner);
int move(Room* room, int leaves);
void removePlayer(Room* room);
int findCreature(Room* room, int creatureName);

int respect = 40;


int main() {
    int numRooms, numCreatures;
        size_t bufferSize = 32;
        char *buffer = (char *) malloc(bufferSize * sizeof(char));

        // Set up the Rooms
        printf("Enter the number of rooms. \n");
        getline(&buffer, &bufferSize, stdin);
        numRooms = atoi(buffer);
        Room* allRooms = (Room*) malloc(numRooms * sizeof(Room));

        buffer = (char *) realloc(buffer, bufferSize * sizeof(char));

        char* token;

        // Creating all Rooms
        for(int i=0; i<numRooms; i++){
                allRooms[i].name = i;

                printf("Enter the state of the room %d and its neighbor information. \n", i);

                // Setting Room State
                getline(&buffer, &bufferSize, stdin);
                token = strtok(buffer, " ");
                allRooms[i].state = atoi(token);

                // Setting Room Creatures
                initializeCreatures(&allRooms[i]);

                // Setting Room Neighbors
                token = strtok(NULL, " ");

                if(atoi(token) != -1){
                        allRooms[i].north = &allRooms[atoi(token)];
                } else {
                        allRooms[i].north = NULL;
                }

                token = strtok(NULL, " ");

                if(atoi(token) != -1){
                        allRooms[i].south = &allRooms[atoi(token)];
                } else {
                        allRooms[i].south = NULL;
                }

                token = strtok(NULL, " ");

                if(atoi(token) != -1){
                        allRooms[i].east = &allRooms[atoi(token)];
                } else {
                        allRooms[i].east = NULL;
                }

                token = strtok(NULL, " ");

                if(atoi(token) != -1){
                        allRooms[i].west = &allRooms[atoi(token)];
                } else {
                        allRooms[i].west = NULL;
                }
        }


        // Set up Creatures
        buffer = (char *) realloc(buffer, bufferSize * sizeof(char));

        printf("Enter the number of creatures in game. \n");
        getline(&buffer, &bufferSize, stdin);
        numCreatures = atoi(buffer);

        Creature* allCreatures = (Creature*) malloc(numCreatures*sizeof(Creature));

        buffer = (char *) realloc(buffer, bufferSize * sizeof(char));
        Room* currentRoom = NULL;
        Creature* player = NULL;

        //Creating all Creatures
        for(int i=0; i<numCreatures; i++){
                allCreatures[i].name = i;

                printf("Enter the creature type and location. \n");
                getline(&buffer, &bufferSize, stdin);
                token = strtok(buffer, " ");

                allCreatures[i].type = atoi(token);
                token = strtok(NULL, " ");
                allCreatures[i].location = &allRooms[atoi(token)];

                //fill the room with one creature at a time
                fillRoom(&allRooms[atoi(token)], &allCreatures[i]);

                //remember where the player is
                if(allCreatures[i].type == 0){
                        currentRoom = allCreatures[i].location;
                        player = &allCreatures[i];
                }
        }

        // Main Game Loop
        int loopCondition = 1;
        char* token1;
        int token2;
        int tempNum;
        Creature* tempCreature;
        while(loopCondition){
                buffer = (char *) realloc(buffer, bufferSize * sizeof(char));
                printf("\nEnter a command. \n");
                getline(&buffer, &bufferSize, stdin);

                if(strstr(buffer, colon)){
                        token1 = strtok(buffer, ":");
                        token2 = atoi(token1);
                        token1 = strtok(NULL, ":");

                        if(strcmp("clean\n", token1) == 0){
                                printf("\n");
                                clean(currentRoom, token2);
                        } else if(strcmp("dirty\n", token1) == 0){
                                printf("\n");
                                dirty(currentRoom, token2);
                        } else if(strcmp("north\n", token1) == 0){
                                printf("\n");
                                tempNum = findCreature(currentRoom, token2);
                                tempCreature = currentRoom->creatures[tempNum];
                                currentRoom->creatures[tempNum] = NULL;
                                fillRoom(currentRoom->north, tempCreature);
                                if(tempCreature->type == 1){
                                        printf("You moved %s %d to the north\n", type1, token2);
                                        if(currentRoom->north->state == 2){
                                                printf("The room you move %s %d isn't right! They're cleaning it. \n", type1, token2);
                                                clean(currentRoom->north, token2);
                                        }
                                } else if(tempCreature->type == 2){
                                        printf("You moved %s %d to the north\n", type2, token2);
                                        if(currentRoom->north->state == 0){
                                                printf("The room you move %s %d isn't right! They're making it dirty. \n", type2, token2);
                                                dirty(currentRoom->north, token2);
                                        }
                                }
                        } else if(strcmp("south\n", token1) == 0){
                                printf("\n");
                                tempNum = findCreature(currentRoom, token2);
                                tempCreature = currentRoom->creatures[tempNum];
                                currentRoom->creatures[tempNum] = NULL;
                                fillRoom(currentRoom->south, tempCreature);
                                if(tempCreature->type == 1){
                                        printf("You moved %s %d to the south\n", type1, token2);
                                        if(currentRoom->south->state == 2){
                                                printf("The room you move %s %d isn't right! They're cleaning it. \n", type1, token2);
                                                clean(currentRoom->south, token2);
                                        }
                                } else if(tempCreature->type == 2){
                                        printf("You moved %s %d to the south\n", type2, token2);
                                        if(currentRoom->south->state == 0){
                                                printf("The room you move %s %d isn't right! They're making it dirty. \n", type2, token2);
                                                dirty(currentRoom->south, token2);
                                        }
                                }
                        } else if(strcmp("east\n", token1) == 0){
                                printf("\n");
                                tempNum = findCreature(currentRoom, token2);
                                tempCreature = currentRoom->creatures[tempNum];
                                currentRoom->creatures[tempNum] = NULL;
                                fillRoom(currentRoom->east, tempCreature);
                                if(tempCreature->type == 1){
                                        printf("You moved %s %d to the east\n", type1, token2);
                                        if(currentRoom->east->state == 2){
                                                printf("The room you move %s %d isn't right! They're cleaning it. \n", type1, token2);
                                                clean(currentRoom->east, token2);
                                        }
                                } else if(tempCreature->type == 2){
                                        printf("You moved %s %d to the east\n", type2, token2);
                                        if(currentRoom->east->state == 0){
                                                printf("The room you move %s %d isn't right! They're making it dirty. \n", type2, token2);
                                                dirty(currentRoom->east, token2);
                                        }
                                }
                        } else if(strcmp("west\n", token1) == 0){
                                printf("\n");
                                tempNum = findCreature(currentRoom, token2);
                                tempCreature = currentRoom->creatures[tempNum];
                                currentRoom->creatures[tempNum] = NULL;
                                fillRoom(currentRoom->west, tempCreature);
                                if(tempCreature->type == 1){
                                        printf("You moved %s %d to the west\n", type1, token2);
                                        if(currentRoom->west->state == 2){
                                                printf("The room you move %s %d isn't right! They're cleaning it. \n", type1, token2);
                                                clean(currentRoom->west, token2);
                                        }
                                } else if(tempCreature->type == 2){
                                        printf("You moved %s %d to the west\n", type2, token2);
                                        if(currentRoom->west->state == 0){
                                                printf("The room you move %s %d isn't right! They're making it dirty. \n", type2, token2);
                                                dirty(currentRoom->west, token2);
                                        }
                                }
                        }


                } else if(strcmp("look\n", buffer) == 0){
                        printf("\n");
                        look(currentRoom);
                } else if(strcmp("exit\n", buffer) == 0){
                        printf("Goodbye \n");
                        loopCondition = 0;
                } else if(strcmp("help\n", buffer) == 0){
                        printf("Hello and welcome to the room game! \n");
                        printf("There are many options you can enter: \n");
                        printf("\n\'look\' to see what is in your currentRoom \n");
                        printf("\n\'clean\' to change a room from dirty to half-dirty or from half-dirty to clean (humans hate this!) \n");
                        printf("\n\'dirty\' to change a room from clean to half-dirty or from half-dirty to dirty (animals hate this!) \n");
                        printf("\n\'direction\' where the direction is north south east or west to move in that direction (only works if there is a neighbor in that direction) \n");
                        printf("\n\'[creatureName]:clean\' to force a creature to clean the current room. This will make them extra happy or angry! \n");
                        printf("\n\'[creatureName]:dirty\' to force a creature to dirty the current room. This will make them extra happy or angry! \n");
                        printf("\n\'[creatureNum]:direction\' where the direction is north south east or west to force a creature to leave the current room and move into a neighbor in that direction. \n");
                        printf("\n\'exit\' if you are done playing the game. \n");
                } else if(strcmp("clean\n", buffer) == 0){
                        printf("\n");
                        clean(currentRoom, -1);
                } else if(strcmp("dirty\n", buffer) == 0){
                        printf("\n");
                        dirty(currentRoom, -1);
                } else if(strcmp("north\n", buffer) == 0){
                        printf("\n");
                        fillRoom(currentRoom->north, player);
                        removePlayer(currentRoom);
                        currentRoom = currentRoom->north;
                        printf("You leave towards the north \n");
                } else if(strcmp("south\n", buffer) == 0){
                        printf("\n");
                        fillRoom(currentRoom->south, player);
                        removePlayer(currentRoom);
                        currentRoom = currentRoom->south;
                        printf("You leave towards the south \n");
                } else if(strcmp("east\n", buffer) == 0){
                        printf("\n");
                        fillRoom(currentRoom->east, player);
                        removePlayer(currentRoom);
                        currentRoom = currentRoom->east;
                        printf("You leave towards the east \n");
                } else if(strcmp("west\n", buffer) == 0){
                        printf("\n");
                        fillRoom(currentRoom->west, player);
                        removePlayer(currentRoom);
                        currentRoom = currentRoom->west;
                        printf("You leave towards the west \n");
                }


                if(respect > 80){
                        printf("All of the creatures respect you! You win!\n");
                        loopCondition = 0;
                } else if(respect < 0){
                        printf("All of the creatures hate you! Where did you go wrong?\n");
                        loopCondition = 0;
                }

        }

        free(buffer);
        return 0;
}

int findCreature(Room* room, int creatureNum){
        // you know the name of the creature, not what spot it is in
        for(int i=0; i<10; i++){
                if(room->creatures[i]){
                        if(room->creatures[i]->name == creatureNum){
                                return i;
                        }
                }
        }

        printf("That creature is not in this room. \n");
        return -1;
}


void removePlayer(Room* room){
        // player wont necessarily be in any certain spot if been moved already
        for(int i=0; i<10; i++){
                if(room->creatures[i]){
                        if(room->creatures[i]->type == 0){
                                // found the player in the room
                                room->creatures[i] = NULL;
                                return;
                        }
                }
        }
}

void initializeCreatures(Room* room) {
        for(int i=0; i<10; i++){
                room->creatures[i] = NULL;
        }
}

int fillRoom(Room* room, Creature* creature){
        int foundSpot = 0;

        if(!room){
                return 0;
        }

        for(int i=0; i<10;i++){
                if(!room->creatures[i]){
                        room->creatures[i] = creature;
                        foundSpot = 1;

                        break;
                }
        }

        if(!foundSpot){
                printf("Room filled.");
                return 0;
        }

        return 1;
}

void look(Room* room){
        printf("Room %d", room->name);

        if(room->state == 0){
                printf(",%s,", state0);
        } else if(room->state == 1){
                printf(",%s,", state1);
        } else if(room->state == 2){
                printf(",%s,", state2);
        }

        if(room->north){
                printf(" neighbor %d to the %s, ", room->north->name, room0);
        }

        if(room->south){
                printf("neighbor %d to the %s, ", room->south->name, room1);
        }

        if(room->east){
                printf("neighbor %d to the %s, ", room->east->name, room2);
        }

        if(room->west){
                printf("neighbor %d to the %s, ", room->west->name, room3);
        }

        printf("contains: \n");

        for(int i=0;i<10;i++){
                if(room->creatures[i]){
                        if(room->creatures[i]->type == 0){
                                printf("%s \n", type0);
                        } else if(room->creatures[i]->type == 1){
                                printf("%s %d \n", type1, room->creatures[i]->name);
                        }else if(room->creatures[i]->type == 2){
                                printf("%s %d \n", type2, room->creatures[i]->name);
                        }
                }
        }
}

int move(Room* room, int leaves){
        int ceilingCount = 0;
        int itWorked;
        int confirmedDirection;
        for(int i=0; i<10; i++){
                if(room->creatures[i]){
                        if(room->creatures[i]->type == 2 && leaves == 2){
                                printf("%s %d leaves towards the ", type2, room->creatures[i]->name);

                                // get random int
                                // CANT GET RANDOMNESS TO WORK???
                                /*time_t t;
                                int randomDirection;
                                int confirmedDirection;
                                int itWorked;
                                srand((unsigned) time(&t));
                                randomDirection = rand() % 5;


                                if(roomNorth == randomDirection){
                                        itWorked = fillRoom(room->north, room->creatures[i]);
                                        if(itWorked){
                                                confirmedDirection = 0;
                                        }
                                } else if(roomSouth == randomDirection){
                                        itWorked = fillRoom(room->south, room->creatures[i]);
                                        if(itWorked){
                                                confirmedDirection = 1;
                                        }
                                } else if(roomEast == randomDirection){
                                        itWorked = fillRoom(room->east, room->creatures[i]);
                                        if(itWorked){
                                                confirmedDirection = 2;
                                        }
                                } else if(roomWest == randomDirection){
                                        itWorked = fillRoom(room->west, room->creatures[i]);
                                        if(itWorked){
                                                confirmedDirection = 3;
                                        }
                                }

                                */


                                itWorked = fillRoom(room->north, room->creatures[i]);

                                if(itWorked){
                                        confirmedDirection = 0;
                                } else {
                                        itWorked = fillRoom(room->south, room->creatures[i]);
                                        if(itWorked){
                                                confirmedDirection = 1;
                                        } else {
                                                itWorked = fillRoom(room->east, room->creatures[i]);
                                                if(itWorked){
                                                        confirmedDirection = 2;
                                                } else {
                                                        itWorked = fillRoom(room->west, room->creatures[i]);
                                                        if(itWorked){
                                                                confirmedDirection = 3;
                                                        } else {
                                                                confirmedDirection = 4;
                                                        }
                                                }
                                        }

                                }


                                if(confirmedDirection == 0){
                                        printf("%s \n", room0);
                                        if(room->north->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type2, room->creatures[i]->name, room->north->name);
                                                dirty(room->north, -1);
                                        }
                                } else if(confirmedDirection == 1){
                                        printf("%s \n", room1);
                                        if(room->south->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type2, room->creatures[i]->name, room->south->name);
                                                dirty(room->south, -1);
                                        }
                                } else if(confirmedDirection == 2){
                                        printf("%s \n", room2);
                                        if(room->east->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type2, room->creatures[i]->name, room->east->name);
                                                dirty(room->east, -1);
                                        }
                                } else if(confirmedDirection == 3){
                                        printf("%s \n", room3);
                                        if(room->west->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type2, room->creatures[i]->name, room->west->name);
                                                dirty(room->west, -1);
                                        }
                                } else if(confirmedDirection == 4){
                                        printf("The ceiling! \n");
                                        ceilingCount ++;
                                }

                                room->creatures[i] = NULL;
                        } else if(room->creatures[i]->type == 1 && leaves == 1){
                                printf("%s %d leaves towards the", type1, room->creatures[i]->name);

                                itWorked = fillRoom(room->north, room->creatures[i]);

                                if(itWorked){
                                        confirmedDirection = 0;
                                } else {
                                        itWorked = fillRoom(room->south, room->creatures[i]);
                                        if(itWorked){
                                                confirmedDirection = 1;
                                        } else {
                                                itWorked = fillRoom(room->east, room->creatures[i]);
                                                if(itWorked){
                                                        confirmedDirection = 2;
                                                } else {
                                                        itWorked = fillRoom(room->west, room->creatures[i]);
                                                        if(itWorked){
                                                                confirmedDirection = 3;
                                                        } else {
                                                                confirmedDirection = 4;
                                                        }
                                                }
                                        }

                                }


                                if(confirmedDirection == 0){
                                        printf("%s \n", room0);
                                        if(room->north->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type1, room->creatures[i]->name, room->north->name);
                                                dirty(room->north, -1);
                                        }
                                } else if(confirmedDirection == 1){
                                        printf("%s \n", room1);
                                        if(room->south->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type1, room->creatures[i]->name, room->south->name);
                                                dirty(room->south, -1);
                                        }
                                } else if(confirmedDirection == 2){
                                        printf("%s \n", room2);
                                        if(room->east->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type1, room->creatures[i]->name, room->east->name);
                                                dirty(room->east, -1);
                                        }
                                } else if(confirmedDirection == 3){
                                        printf("%s \n", room3);
                                        if(room->west->state == 0){
                                                printf("%s %d is not satisfied! They are making Room %d dirty! \n", type1, room->creatures[i]->name, room->west->name);
                                                dirty(room->west, -1);
                                        }
                                } else if(confirmedDirection == 4){
                                        printf("The ceiling! \n");
                                        ceilingCount ++;
                                }

                                room->creatures[i] = NULL;
                                }
                        }
                }


        return ceilingCount;
}

void dirty(Room* room, int cleaner){
        int animalsLeave = 0;
        int ceilingCount = 0; // how many left through the ceiling?

        if(room->state == 0){

                room->state = 1;

        } else if(room->state == 1){

                room->state = 2;

                // animals are mad at dirty rooms
                animalsLeave = 1;

        } else if(room->state == 2){
                printf("Room cannot get any dirtier! \n");
        }

        for(int i=0; i<10; i++){
                if(room->creatures[i]){
                        if(room->creatures[i]->type == 2){

                                if(cleaner == -1){
                                        respect = respect + 1;
                                        printf("%s %d %s \n", type2, room->creatures[i]->name, humanHappy);
                                        printf("Respect is now at %d \n", respect);
                                } else if(room->creatures[i]->name == cleaner){
                                        respect = respect + 3;
                                        printf("%s %d %s a lot\n", type2, room->creatures[i]->name, humanHappy);
                                        printf("Respect is now at %d \n", respect);
                                } else {
                                        respect = respect + 1;
                                        printf("%s %d %s \n", type2, room->creatures[i]->name, humanHappy);
                                        printf("Respect is now at %d \n", respect);
                                }

                        } else if(room->creatures[i]->type == 1){

                                if(cleaner == -1){
                                        respect = respect - 1;
                                        printf("%s %d %s \n", type1, room->creatures[i]->name, animalMad);
                                        printf("Respect is now at %d \n", respect);
                                } else if(room->creatures[i]->name == cleaner){
                                        respect = respect - 3;
                                        printf("%s %d %s a lot\n", type1, room->creatures[i]->name, animalMad);
                                        printf("Respect is now at %d \n", respect);
                                } else {
                                        respect = respect - 1;
                                        printf("%s %d %s \n", type1, room->creatures[i]->name, animalMad);
                                        printf("Respect is now at %d \n", respect);
                                }
                        }
                }
        }

        if(animalsLeave){
                ceilingCount = move(room, 1);

                for(int i=0; i<10; i++){
                        if(room->creatures[i]){
                                if(room->creatures[i]->type == 2){
                                printf("%s %d %s \n", type2, room->creatures[i]->name, humanMad);
                                respect = respect - 1;
                                printf("Respect is now at %d \n", respect);
                        } else if(room->creatures[i]->type == 1){
                                printf("%s %d %s \n", type1, room->creatures[i]->name, animalMad);
                                respect = respect - 1;
                                printf("Respect is now at %d \n", respect);
                        }
                        }
                }
        }
}

void clean(Room* room, int cleaner){
        int humansLeave = 0; // are the humans mad enough to walk out?
        int ceilingCount = 0; // how many left through the ceiling?

        if(room->state == 2){
                // if room was dirty, it is now half-dirty
                // nobody is mad about this

                room->state = 1;

        } else if(room->state == 1){
                // if room was half-dirty it is now clean
                // humans are so mad about this

                room->state = 0;
                humansLeave = 1; // yes
        } else if(room->state == 0){
                printf("Room cannot get any cleaner! \n");
        }

        for(int i=0; i<10; i++){
                if(room->creatures[i]){
                        if(room->creatures[i]->type == 2){

                                if(cleaner == -1){
                                        respect = respect - 1;
                                        printf("%s %d %s \n", type2, room->creatures[i]->name, humanMad);
                                        printf("Respect is now at %d \n", respect);
                                } else if(room->creatures[i]->name == cleaner){
                                        respect = respect - 3;
                                        printf("%s %d %s a lot\n", type2, room->creatures[i]->name, humanMad);
                                        printf("Respect is now at %d \n", respect);
                                } else {
                                        respect = respect - 1;
                                        printf("%s %d %s \n", type2, room->creatures[i]->name, humanMad);
                                        printf("Respect is now at %d \n", respect);
                                }

                        } else if(room->creatures[i]->type == 1){

                                if(cleaner == -1){
                                        respect = respect + 1;
                                        printf("%s %d %s \n", type1, room->creatures[i]->name, animalHappy);
                                        printf("Respect is now at %d \n", respect);
                                } else if(room->creatures[i]->name == cleaner){
                                        respect = respect + 3;
                                        printf("%s %d %s a lot\n", type1, room->creatures[i]->name, animalHappy);
                                        printf("Respect is now at %d \n", respect);
                                } else {
                                        respect = respect + 1;
                                        printf("%s %d %s \n", type1, room->creatures[i]->name, animalHappy);
                                        printf("Respect is now at %d \n", respect);
                                }
                        }
                }
        }

        if(humansLeave){
                ceilingCount = move(room, 2);

                for(int i=0; i<10; i++){
                        if(room->creatures[i]){
                                if(room->creatures[i]->type == 2){
                                printf("%s %d %s \n", type2, room->creatures[i]->name, humanMad);
                                respect = respect - 1;
                                printf("Respect is now at %d \n", respect);
                        } else if(room->creatures[i]->type == 1){
                                printf("%s %d %s \n", type1, room->creatures[i]->name, animalMad);
                                respect = respect - 1;
                                printf("Respect is now at %d \n", respect);
                        }
                        }
                }
        }
}
