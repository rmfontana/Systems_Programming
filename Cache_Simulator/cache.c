#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/* Implementing a cache simulator */
/* Direct-mapped cache, Set associative cache, Fully associative cache */
/* As follows:
Multiple sets S > 1, 1 cache line per set E = 1[Direct-mapped cache]
Multiple sets S > 1, Multiple cache lines E > 1 [Set associative cache]
1 cache set S = 1, Multiple cache lines E > 1 [Fully associative cache]
*/

/* Parameter Translation:
S = number of sets
E = number of lines per set
B = block size
m = number of address bits
*/

typedef struct node {
        unsigned long long value;
        int lastUsed;
        int timesUsed;
}node;

typedef struct Cache {
        /* As many columns per sets and rows per lines */
        // Cache parameters
        int S;
        int E;
        int B;
        int m;
        int policy;                                     // 0 is uninitalized, 1 is LFU, 2 is LRU
        int hitTime;
        int missPenalty;

        // Cache itself
        node** cache;
}Cache;

void processInput(unsigned long long input, Cache* myCache);
int numberCycles(int hitTime, int missTime);
unsigned long long uniqueMask(unsigned long long x, unsigned long long y);

int clockTime = 0;
int hits = 0;
int misses = 0;

int main() {
        // Necessary Parameters
        size_t inputSize = 128;

        int S, E, B, m, hitTime, missPenalty;
        int replacement = 0;

        // Get ready for user input
    char* input = (char *)malloc(inputSize * sizeof(char));

        // Fundamental Parameters of the cache
    printf("Time to enter the cache fundamental parameters! Start with S: \n");
    getline(&input, &inputSize, stdin);
    S = atol(input);

        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("Please enter E. \n");
    getline(&input, &inputSize, stdin);
    E = atol(input);

        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("Please enter B. \n");
    getline(&input, &inputSize, stdin);
    B = atol(input);

        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("Please enter m. \n");
    getline(&input, &inputSize, stdin);
    m = atol(input);

        // Replacement policy
        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("Now it's time to enter the replacement policy. \n");
    getline(&input, &inputSize, stdin);

        // Remove carriage return and null terminate for string equality
        input[strlen(input)-1] = '\0';
        if(strcmp(input, "LFU") == 0){
                replacement = 1;
        } else if(strcmp(input, "LRU") == 0){
                replacement = 2;
        }

        // Fundamental Parameters of the hardware
        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("I need the fundamental parameters of the hardware. Please enter hit time. \n");
    getline(&input, &inputSize, stdin);
    hitTime = atol(input);

        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("Finally, I need miss penalty. \n");
    getline(&input, &inputSize, stdin);
    missPenalty = atol(input);

        // Addresses
        input = (char *) realloc(input, inputSize * sizeof(char));
    printf("The rest of the values should be addresses. Scanning will stop when -1 is entered: \n");

        // Allocate the cache
        Cache* myCache = malloc(sizeof(Cache));

        (*myCache).cache = malloc(E*sizeof(node*));

        for(int i=0; i < E; i++){
                (*myCache).cache[i] = malloc(S*sizeof(node));
        }

        // Initalizing data

        for(int i=0; i < E; i++){
                for(int j=0; j < S; j++){
                        (*myCache).cache[i][j].value = ULLONG_MAX;
                        (*myCache).cache[i][j].lastUsed = 0;
                        (*myCache).cache[i][j].timesUsed = 0;
                        // REMOVE ME
                        //printf("Cache Line: %d Set: %d \n", i, j);
                }
        }

        // Instead of passing many parameters put all into cache
        (*myCache).S = S;
        (*myCache).E = E;
        (*myCache).B = B;
        (*myCache).m = m;
        (*myCache).policy = replacement;
        (*myCache).hitTime = hitTime;
        (*myCache).missPenalty = missPenalty;

        // When putting addresses in cache: myCache[E][S], row = E, column = S
        int loopCondition = 1;

        while(loopCondition){
                unsigned long long address;

                scanf("%llx", &address);

                if(address == -1){
                        numberCycles((*myCache).hitTime, (*myCache).missPenalty);

                        float sidebar = ((float) misses / ((float) misses + (float) hits));
                        int missRate = (int) (sidebar * 100);
                        printf("%d %d\n", missRate, clockTime);
                        loopCondition = 0;
                        break;
                } else {
                        processInput(address, myCache);
                }

        }
}

int numberCycles(int hitTime, int missTime){
        clockTime = (hits * hitTime) + (misses * (hitTime + missTime));
        return clockTime;
}

unsigned long long uniqueMask(unsigned long long x, unsigned long long y){
        // couldn't get any masks to work for last problem, try creating them based on parameters on the fly?

   unsigned long long z = 0;
   for (unsigned i=x; i<y; i++)
       z |= 1ULL << i;


   return z;
}



void processInput(unsigned long long input, Cache* myCache){
        /* We need certain items to know where to put the addresses:
        Tag bits: t = m - (s+b) 1st bits
        Set bits: s = log2(S)   2nd bits
        Block bits: b = log2(B) Last bits */

        unsigned long long tag, blockOffset;
        int set;

        int s = (int) (log10((*myCache).S) / log10(2));
        unsigned long long b = (unsigned long long) (log10((*myCache).B) / log10(2));
        unsigned long long m = (*myCache).m;
        unsigned long long t = (unsigned long long) (m - (s + b));
        unsigned long long tagMask = uniqueMask(0, t);
        unsigned long long setMask = uniqueMask(t, s);
        unsigned long long blockMask = uniqueMask(b, m);


        // Get the set index
        // Mask 00011000 to 18
        set = (input & 0x18ULL) >> s;

        // Get the block index
        // Mask 00000111 to 7
        //blockOffset = (input & 0x7ULL) >> b;
        blockOffset = (input >> b) & blockMask;

        // REMOVE ME
        //printf("Block: %llx \n", blockOffset);

        // Get tag index
        // Mask 11100000 to E0
        //tag = (input & 0xE0ULL) >> t;
        tag = (input >> (m - t)) & tagMask;

        int alreadyHit = 0;
        int emptySpaces = 0;
        int emptyBlock = 0;
        for(int block = 0; block < (*myCache).E; block++){
                if((*myCache).cache[block][set].value == tag){
                        // Something is in the spot and it was what we were looking for
                        // Hit

                        numberCycles((*myCache).hitTime, (*myCache).missPenalty);
                        (*myCache).cache[block][set].lastUsed = clockTime;
                        (*myCache).cache[block][set].timesUsed += 1;
                        hits += 1;
                        printf("%llx H \n", input);

                        alreadyHit = 1;
                        break;

                } else if((*myCache).cache[block][set].value == ULLONG_MAX){
                        emptySpaces = 1;
                        emptyBlock = block;
                }
        }

        if(!alreadyHit){
                if(emptySpaces){
                        // Nothing was in the spot
                        // Cold miss

                        numberCycles((*myCache).hitTime, (*myCache).missPenalty);
                        (*myCache).cache[emptyBlock][set].value = tag;
                        (*myCache).cache[emptyBlock][set].lastUsed = clockTime;
                        (*myCache).cache[emptyBlock][set].timesUsed = 1;
                } else {
                        // Something is in the spot and it is not what we were looking for
                        // Must Replace

                        if((*myCache).E == 1){

                                numberCycles((*myCache).hitTime, (*myCache).missPenalty);
                                (*myCache).cache[0][set].value = tag;
                                (*myCache).cache[0][set].lastUsed = clockTime;
                                (*myCache).cache[0][set].timesUsed = 1;

                        } else if((*myCache).policy == 1){
                                // LFU
                                int currentLow = (*myCache).cache[0][set].timesUsed;
                                int newBlock;

                                for(int block = 0; block < (*myCache).E; block++){

                                        if((*myCache).cache[block][set].timesUsed <= currentLow){
                                                currentLow = (*myCache).cache[block][set].timesUsed;
                                                newBlock = block;

                                        }

                                }

                                numberCycles((*myCache).hitTime, (*myCache).missPenalty);
                                (*myCache).cache[newBlock][set].value = tag;
                                (*myCache).cache[newBlock][set].lastUsed = clockTime;
                                (*myCache).cache[newBlock][set].timesUsed = 1;

                        } else if((*myCache).policy == 2){
                                // LRU
                                int currentLow = (*myCache).cache[0][set].lastUsed;
                                int newBlock;

                                for(int block = 0; block < (*myCache).E; block++){

                                        if((*myCache).cache[block][set].lastUsed <= currentLow){
                                                currentLow = (*myCache).cache[block][set].lastUsed;
                                                newBlock = block;

                                        }

                                }

                                numberCycles((*myCache).hitTime, (*myCache).missPenalty);
                                (*myCache).cache[newBlock][set].value = tag;
                                (*myCache).cache[newBlock][set].lastUsed = clockTime;
                                (*myCache).cache[newBlock][set].timesUsed = 1;
                        }

                }

                misses += 1;
                printf("%llx M \n", input);
        }
}
