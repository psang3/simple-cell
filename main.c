/*-------------------------------------------
Program 1: Elementary Cellular Automaton
Course: CS 211, Spring 2025, UIC
Author: Pathik Sangani
-------------------------------------------*/


//Elementary Cellular Automaton

#include <stdio.h>
#include <stdbool.h> 

const int WORLD_SIZE = 65;

typedef struct cell_struct{
    bool state[3]; //active status for [left, me, right] cells 
    bool active; //current status for me
    int total; //running accumulated count of this cell's active status for all generations
} cell;

//convert an 8-bit integer rule (0-255) to array of bits 
//(stored in reverse order)
//   ex: rule = 6  = 00000110 -> [01100000] 
//   ex: rule = 30 = 00011110 -> [01111000] 
//   ex: rule = 65 = 01000001 -> [10000010]
//return true if input rule is valid (0-255)
//return false if input rule is invalid
bool setBitArray(bool bitArray[8], int rule) {

        //TODO: Task 1 - write the setBitArray() function
        if (rule < 0 || rule > 255) {
            return false; // Validate rule
        }
        for (int i = 0; i < 8; i++){
            // Extract bits from MSB to LSB and store in reverse order
            bitArray[i] = (rule >> (7 - i)) & 1;
        }
        return true;
}

//convert a 3-bit state array to its 
//associated index of the rule# bit array
//   ex: {0 0 0} -> 0
//   ex: {0 0 1} -> 1
//   ex: {1 0 1} -> 5
int stateToIndex(bool state[3]) {

    //TODO: Task 4 - write the stateToIndex() function
    // Convert 3-bit state to a decimal index (0-7)
    return state[0] * 4 + state[1] * 2 + state[2];
}

//update the state array for each cell in the world array based on the
//current status of active for the nearby [left,me,right] cells
//note: world is assumed periodic/cyclic,
//with front-to-back and back-to-front wrapping 
void setStates(cell world[WORLD_SIZE]) {

    //TODO: Task 5 - write the setStates() function
    for (int i = 0; i < WORLD_SIZE; i++){
        // Handle periodic boundaries (wrapping)
        int left = (i - 1 + WORLD_SIZE) % WORLD_SIZE;
        int right = (i + 1) % WORLD_SIZE;
        // Update state using left, current, and right cell statuses
        world[i].state[0] = world[left].active;
        world[i].state[1] = world[i].active;
        world[i].state[2] = world[right].active;
    }

}

//evolve each cell's active status to the next generation 
//  using its state array
//ruleBitArray contains the 8-bits for the rule#, 
//  stored in reverse order
//make sure to also update the total subitem for each cell
//return the number of active cells in the new generation
int evolveWorld(cell world[WORLD_SIZE], bool ruleBitArray[8]) {

    //TODO: Task 7 - write the evolveWorld() function
    bool newActive[WORLD_SIZE];
    for (int i = 0; i < WORLD_SIZE; i++){\
        // Get the next state using the rule bit array
        int index = stateToIndex(world[i].state);
        newActive[i] = ruleBitArray[7 - index]; // Access rule in reverse order
    }
    int sum = 0;
    for(int i = 0; i < WORLD_SIZE; i++){
        // Update cell activity and total
        world[i].active = newActive[i];
        world[i].total += newActive[i];
        sum += newActive[i]; // Track total active cells
    }
    setStates(world); // Update states for the next generation
    return sum;
}

void printWorld(cell world[WORLD_SIZE], int activeCount){
    for(int i = 0; i < WORLD_SIZE; i++){
        printf("%c", world[i].active ? 'o' : '\'');
    }
    printf(" %2d\n", activeCount); // Print active count
}


int main() {
    cell world[WORLD_SIZE];
    bool bitArray[8];
    int rule, generations;

    printf("Welcome to the Elementary Cellular Automaton!\n");
    
    // Read and validate rule number
    do {
        printf("Enter the rule # (0-255): ");
        scanf("%d", &rule);
    } while (!setBitArray(bitArray, rule));

    // Print the rule's bit array
    printf("\nThe bit array for rule #%d is ", rule);
    for (int i = 0; i < 8; i++){
        printf("%d", bitArray[i]);
    }
    printf("\n\n");
    
    // Display evolution rules for all possible states
    printf("The evolution of all possible states are as follows:\n");
    for (int i = 7; i >= 0; i--) {
        bool state[3] = {(i & 4) >> 2, (i & 2) >> 1, i & 1}; // Generate state
        printf("|%c%c%c|", state[0] ? 'o' : '\'', state[1] ? 'o' : '\'', state[2] ? 'o' : '\'');
        if (i != 0) printf("   ");
    }
   
    printf("\n");
    for(int i = 7; i >= 0; i--){
        bool state[3] = {(i & 4) >> 2, (i & 2) >> 1, i & 1};
        int index = stateToIndex(state);
        bool nextActive = bitArray[7 - index]; // Get next state from rule
        printf("|%c|", nextActive ? 'o' : '\'');
        if (i != 0) printf("     ");
    }
    printf("    \n\n");
 
    // Read and validate number of generations
    do {
        printf("Enter the number of generations (1-99): ");
        scanf("%d", &generations);
    } while (generations < 1 || generations > 99);
  
    // Initialize world with middle cell active
    printf("Initializing world & evolving...\n");
    int midpoint = (WORLD_SIZE - 1) / 2;
    for (int i = 0; i < WORLD_SIZE; i++){
        world[i].active = (i == midpoint);
        world[i].total = (i == midpoint) ? 1 : 0;
    }
    setStates(world);
    printWorld(world, 1);

    // Evolve and print each generation
    for (int g = 1; g < generations; g++) {
        int activeCount = evolveWorld(world, bitArray);
        printWorld(world, activeCount);
    }

    // Print separator line
    for (int i = 0; i < WORLD_SIZE; i++) {
        printf("_");
    }
    printf("\n");

    // Print total activity vertically
    char tensLine[WORLD_SIZE + 1];
    char unitsLine[WORLD_SIZE + 1];
    for (int i = 0; i < WORLD_SIZE; i++) {
        int total = world[i].total;
        if (total == 0) {
            tensLine[i] = ' ';
            unitsLine[i] = ' ';
        } else {
            int tens = total / 10;
            int units = total % 10;
            tensLine[i] = tens > 0 ? tens + '0' : ' ';
            unitsLine[i] = units + '0';
        }
    }
    tensLine[WORLD_SIZE] = '\0';
    unitsLine[WORLD_SIZE] = '\0';
    printf("%s\n", tensLine);
    printf("%s\n", unitsLine);

    return 0;
}[ ]