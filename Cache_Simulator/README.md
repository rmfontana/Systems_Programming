# Cache_Simulator

Cache simulator which indicating how different parameters of cache would perform under a variety of instances. It allows for a direct-mapped cache, set associative cache, and fully associative cache depending on parameters entered. 

Input (all as ints except replacement policy 3 char):
Number of sets
Number of lines per set
Block Size
Number of address bits
Replacement policy (Options: LFU and LRU) least frequently used and least recently used
Hit time
Miss penalty
n number of addresses followed by -1

To run this project:

        make creaturegame.c
        ./a.out
        follow on screen instructions
