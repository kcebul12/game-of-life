#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

void copy_grid(int gRows,int gCols,int nextGrid[][gCols],int currentGrid[][gCols]);
void print(int *rows,int *cols,int gRows,int gCols,int currentGrid[][gCols]);
void new_grid(int gRows,int gCols,int nextGrid[][gCols]);

int main(){
    
    // basic curses setup
    initscr();
    noecho();
    keypad(stdscr,1);
    nodelay(stdscr,1); // allow for loop interruption
    curs_set(0); // hide cursor

    //start color
    if(has_colors()){start_color();}

    //initialize color pairs
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(3,COLOR_CYAN,COLOR_BLACK);
    init_pair(4,COLOR_WHITE,COLOR_CYAN);

    int rows,cols;
    

    getmaxyx(stdscr,rows,cols);
    const int gRows = (rows/4) * 3; // create a 3/4 screen size grid
    const int gCols = (cols/4) * 3;

    int currentGrid[gRows][gCols];
    int nextGrid[gRows][gCols];

    srand(time(NULL)); // set seed

    new_grid(gRows,gCols,nextGrid);
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

            new_grid(gRows,gCols,nextGrid);
            copy_grid(gRows,gCols,nextGrid,currentGrid);

        }

        getmaxyx(stdscr,rows,cols); // keep game centered if screen is resized
        // print quit message at bottom center
        attrset(A_BOLD | COLOR_PAIR(1));
        mvaddstr(rows - 1,(cols - strlen(quitMessage)) / 2,quitMessage);
        mvaddstr(0,(cols - strlen(resetMessage)) / 2,resetMessage);
        attrset(A_NORMAL);

        //print board
        print(&rows,&cols,gRows,gCols,currentGrid);

        //calculate new grid
        for(int n = 0; n < gRows; n++){
            for(int m = 0; m < gCols; m++){

                int numNeighbors = 0;

                int north = (n - 1) % gRows;
                int south = (n + 1) % gRows;
                int west = (m - 1) % gCols;
                int east = (m + 1) % gCols;
                
                //check north
                if(currentGrid[north][m])
                    numNeighbors += 1;

                //check south
                if(currentGrid[south][m])
                    numNeighbors += 1;

                //check east
                if(currentGrid[n][east])
                    numNeighbors += 1;
               
                //check west
                if(currentGrid[n][west])
                    numNeighbors += 1;
                
                //check north-west
                if(currentGrid[north][west])
                    numNeighbors += 1;

                //check north-east
                if(currentGrid[north][east])
                    numNeighbors += 1;

                //check south-east
                if(currentGrid[south][east])
                    numNeighbors += 1;

                //check south-west
                if(currentGrid[south][west])
                    numNeighbors += 1;

                //does the cell live or die
                if(currentGrid[n][m] == 0 && numNeighbors == 3)
                    nextGrid[n][m]++; //cell is born

                else if(currentGrid[n][m] >= 1 && (numNeighbors == 2 || numNeighbors == 3))
                    nextGrid[n][m]++; //cell survives

                else
                    nextGrid[n][m] = 0; //cell dies

                
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

void new_grid(int gRows,int gCols,int nextGrid[][gCols]){
    for(int n = 0; n < gRows; n++){
        for(int m = 0; m < gCols; m++){
            
            nextGrid[n][m] = rand() % 2; // fill grid with 0's and 1's.  O is dead; 1 is alive
        }
    } 
}

void copy_grid(int gRows,int gCols,int nextGrid[][gCols],int currentGrid[][gCols]){
    for(int n = 0; n < gRows; n++){
        for(int m = 0; m < gCols; m++){

            currentGrid[n][m] = nextGrid[n][m];
        }
    }
}

void print(int *rows,int *cols,int gRows,int gCols,int currentGrid[][gCols]){
        int topLeft_y = (*rows - gRows) / 2;
        int topLeft_x;

        for(int n = 0; n < gRows; n++){

            topLeft_y++; // go down a row for printing
            topLeft_x = (*cols - gCols) / 2; // column position to start printing

            for(int m = 0; m < gCols; m++){

                if(currentGrid[n][m] == 1){

                    mvaddch(topLeft_y,topLeft_x,ACS_BULLET);

                }

                else if(currentGrid[n][m] == 2){

                    mvaddch(topLeft_y,topLeft_x,A_BOLD | ACS_DIAMOND | COLOR_PAIR(2));
                }

                else if(currentGrid[n][m] >= 3 && currentGrid[n][m] <= 10){

                    mvaddch(topLeft_y,topLeft_x,A_BOLD | ACS_PLUS | COLOR_PAIR(3));
                }

                else if(currentGrid[n][m] > 10){

                    mvaddch(topLeft_y,topLeft_x,A_BOLD | ACS_PLUS | COLOR_PAIR(4));
                }

                else{

                    mvaddch(topLeft_y,topLeft_x,' ');

                }

                topLeft_x++; //move column over by one
            }

        }
}

