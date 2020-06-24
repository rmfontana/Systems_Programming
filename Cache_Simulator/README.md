# Cache_Simulator

Cache simulator which indicating how different parameters of cache would perform under a variety of instances. It allows for a direct-mapped cache, set associative cache, and fully associative cache depending on parameters entered. 

Input (all as ints except replacement policy 3 char):
1. Number of sets
2. Number of lines per set
3. Block Size
4. Number of address bits
5. Replacement policy (Options: LFU and LRU) least frequently used and least recently used
6. Hit time
7. Miss penalty
8. n number of addresses followed by -1

To run this project:

        make creaturegame.c
        ./a.out
        follow on screen instructions
