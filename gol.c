#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

void copy_grid(int gRows,int gCols,char nextGrid[][gCols],char currentGrid[][gCols]);
void new_grid(int gRows,int gCols,char nextGrid[][gCols],char currentGrid[][gCols]);

int main(){
    
    // basic curses setup
    initscr();
    noecho();
    keypad(stdscr,1);
    nodelay(stdscr,1); // allow for loop interruption
    curs_set(0); // hide cursor

    int rows,cols;

    getmaxyx(stdscr,rows,cols);
    const int gRows = (rows/4) * 3; // create a 2/3 screen size grid
    const int gCols = (cols/4) * 3;

    char currentGrid[gRows][gCols];
    char nextGrid[gRows][gCols];

    srand(time(NULL)); // set seed

    new_grid(gRows,gCols,nextGrid,currentGrid);
    copy_grid(gRows,gCols,nextGrid,currentGrid);

    char quitMessage[] = "Press F1 to Quit";
    char resetMessage[] = "Press Enter to reset game";
    int ch;
    //start game loop
    while((ch = getch()) != KEY_F(1)){

        //clear current screen
        clear();

        // if Enter is hit, reset board
        if(ch == '\n'){

            new_grid(gRows,gCols,nextGrid,currentGrid);
            copy_grid(gRows,gCols,nextGrid,currentGrid);

        }

        getmaxyx(stdscr,rows,cols); // keep game centered if screen is resized
        // print quit message at bottom center
        mvprintw(rows - 1,(cols - strlen(quitMessage)) / 2,"%s",quitMessage);
        mvprintw(0,(cols - strlen(resetMessage)) / 2,"%s",resetMessage);

        //print board
        int topLeft_y = (rows - gRows) / 2;
        int topLeft_x;

        for(int n = 0; n < gRows; n++){

            topLeft_y++; // go down a row for printing
            topLeft_x = (cols - gCols) / 2; // column position to start printing

            for(int m = 0; m < gCols; m++){
                mvprintw(topLeft_y,topLeft_x,"%c",currentGrid[n][m]);
                topLeft_x++; //move column over by one
            }

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
        copy_grid(gRows,gCols,nextGrid,currentGrid);
        refresh();
        napms(100);
    }

    endwin();

    return 0;
}

void new_grid(int gRows,int gCols,char nextGrid[][gCols],char currentGrid[][gCols]){
    for(int n = 0; n < gRows; n++){
        for(int m = 0; m < gCols; m++){
            
            if((rand() % 2) == 0){
                nextGrid[n][m] = '#';
            } 
            
            else
                nextGrid[n][m] = ' ';
        }
    } 
}

void copy_grid(int gRows,int gCols,char nextGrid[][gCols],char currentGrid[][gCols]){
    for(int n = 0; n < gRows; n++){
        for(int m = 0; m < gCols; m++){

            currentGrid[n][m] = nextGrid[n][m];
        }
    }
}
