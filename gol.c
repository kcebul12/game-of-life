#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ROWS 70
#define COLS 55


int main(){
    
    char currentGrid[ROWS][COLS];
    char nextGrid[ROWS][COLS];

    srand(time(NULL));

    //initialize board
    for(int n = 0; n < ROWS; n++){
        for(int m = 0; m < COLS; m++){
            
            if((rand() % 2) == 0){
                nextGrid[n][m] = '#';
            } 
            
            else
                nextGrid[n][m] = ' ';
        }
    } 

    //copy grid into nextGrid for modifying in loop
    for(int n = 0; n < ROWS; n++){
        for(int m = 0; m < COLS; m++){

            currentGrid[n][m] = nextGrid[n][m];
        }
    }


    //start game loop
    while(1){
        //separate generations
        for(int i = 0; i < 5; i++){
            putchar('\n');
        }

        //print board
        for(int n = 0; n < ROWS; n++){
            for(int m = 0; m < COLS; m++){
                putchar(currentGrid[n][m]);
            }

            putchar('\n');
        }


        //calculate new grid
        for(int n = 0; n < ROWS; n++){
            for(int m = 0; m < COLS; m++){

                int numNeighbors = 0;

                int north = (n - 1) % ROWS;
                int south = (n + 1) % ROWS;
                int west = (m - 1) % COLS;
                int east = (m + 1) % COLS;
                
                //check north
                if(currentGrid[north][m] == '#')
                    numNeighbors += 1;

                //check south
                if(currentGrid[south][m] == '#')
                    numNeighbors += 1;

                //check east
                if(currentGrid[n][east] == '#')
                    numNeighbors += 1;
               
                //check west
                if(currentGrid[n][west] == '#')
                    numNeighbors += 1;
                
                //check north-west
                if(currentGrid[north][west] == '#')
                    numNeighbors += 1;

                //check north-east
                if(currentGrid[north][east] == '#')
                    numNeighbors += 1;

                //check south-east
                if(currentGrid[south][east] == '#')
                    numNeighbors += 1;

                //check south-west
                if(currentGrid[south][west] == '#')
                    numNeighbors += 1;

                //does the cell live or die
                if(currentGrid[n][m] == ' ' && numNeighbors == 3)
                    nextGrid[n][m] = '#'; //cell is born

                else if(currentGrid[n][m] == '#' && (numNeighbors == 2 || numNeighbors == 3))
                    nextGrid[n][m] = '#'; //cell survives

                else
                    nextGrid[n][m] = ' '; //cell dies

                
            }
        }
        
        //copy grid into nextGrid for modifying in loop
        for(int n = 0; n < ROWS; n++){
            for(int m = 0; m < COLS; m++){

                currentGrid[n][m] = nextGrid[n][m];
            }
        }
        sleep(1);        
        
    }

    return 0;
}
