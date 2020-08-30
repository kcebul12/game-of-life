#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>


int main(){
    
    // basic curses setup
    initscr();
    noecho();
    keypad(stdscr,1);
    nodelay(stdscr,1);

    int rows,cols;
    getmaxyx(stdscr,rows,cols);
    int gRows = (rows/3) * 2; // create a 2/3 screen size grid
    int gCols = (cols/3) * 2;
    char currentGrid[gRows][gCols];
    char nextGrid[gRows][gCols];

    srand(time(NULL)); // set seed

    //initialize board
    for(int n = 0; n < gRows; n++){
        for(int m = 0; m < gCols; m++){
            
            if((rand() % 2) == 0){
                nextGrid[n][m] = '#';
            } 
            
            else
                nextGrid[n][m] = ' ';
        }
    } 

    //copy grid into nextGrid for modifying in loop
    for(int n = 0; n < gRows; n++){
        for(int m = 0; m < gCols; m++){

            currentGrid[n][m] = nextGrid[n][m];
        }
    }


    char quitMessage[] = "Press F1 to Quit";
    int ch;
    //start game loop
    while((ch = getch()) != KEY_F(1)){
        // print quit message at bottom center
        mvprintw(rows,(cols - strlen(quitMessage)) / 2,"%s",quitMessage);

        //clear current screen
        clear();
        //print board
        int topLeft_y = (rows - gRows) / 2;
        int topLeft_x = (cols - gCols) / 2;

        for(int n = 0; n < gRows; n++){

            for(int m = 0; m < gCols; m++){
                putchar(currentGrid[n][m]);
            }

            putchar('\n');
        }


        //calculate new grid
        for(int n = 0; n < gRows; n++){
            for(int m = 0; m < gCols; m++){

                int numNeighbors = 0;

                int north = (n - 1) % gRows;
                int south = (n + 1) % gRows;
                int west = (m - 1) % gCols;
                int east = (m + 1) % gCols;
                
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
        for(int n = 0; n < gRows; n++){
            for(int m = 0; m < gCols; m++){

                currentGrid[n][m] = nextGrid[n][m];
            }
        }
        sleep(1);        
        
    }

    endwin();

    return 0;
}
