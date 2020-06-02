#include <cstdlib>
#include <iostream> 
#include <unistd.h>
#include <math.h>
using namespace std;
/*  0,0,0,0,1,0,0,0,0,
    0,0,8,6,0,0,1,9,0,
    0,1,0,7,5,0,0,0,8,
    8,7,0,0,0,0,0,2,6,
    0,0,0,0,0,0,0,0,0,
    6,2,0,0,0,0,0,4,1,
    9,0,0,0,4,6,0,8,0,
    0,4,1,0,0,8,5,0,0,
    0,0,0,0,7,0,0,0,0,
    
    2,0,0,0,0,0,0,0,1,
    0,0,0,5,0,9,0,7,2,
    4,9,0,0,3,0,5,0,0,
    5,0,0,0,0,4,0,0,0,
    0,1,0,0,0,0,0,4,0,
    0,8,0,0,5,1,2,6,0,
    0,3,0,7,0,5,6,0,9,
    9,0,0,0,8,0,0,0,0,
    0,4,0,0,9,0,0,2,0,
    
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
*/
int puzzle[9][9] = {
    7,8,0,0,0,0,3,5,1,
    0,5,9,7,0,0,6,0,0,
    0,2,0,0,0,0,4,7,0,
    8,3,0,0,4,0,0,0,7,
    0,0,0,0,0,0,5,0,0,
    0,0,1,8,0,2,0,0,0,
    0,0,0,0,1,4,0,0,0,
    3,0,7,0,0,0,0,0,0,
    0,6,0,2,8,0,0,0,0,
};
int wait = 0;
struct ZERO {
    int guess[9] = {0};  
    int numGuesses;
};
struct GRID {
    int grid[9][9] = {0};
    int arr[81] = {0};
    ZERO fillSquare;
    bool error = false;
};
void displayGrid(int grid[9][9]) 
{
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) 
            cout << " -----------\n";
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) 
                cout << "|";
            cout << grid[i][j];
        }
        cout << "|\n";
    }
    cout << " -----------\n";
}
int countZeroes(int grid[9][9]) {
    int num0s = 0;
    for (int i  = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int x = grid[i][j];
            if (!x) {
                num0s++;
            }
        }
    }
    return num0s;
}        
void copyGrid(int dest[9][9], int src[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            dest[i][j] = src[i][j];
        }
    }
}
void update(int puzzle[9][9],int rst[81][10]) 
{   
	int x;
 	for (int ROW = 0; ROW < 9; ROW++) {
		for (int COL = 0; COL < 9; COL++) {
	        x = puzzle[ROW][COL];
			if (!x) {// if x == 0
		        // horizontal scan
    		    //  find all of the values in row r
				int row = ROW;
				int col = COL;    		    
    		    for (col = 0; col < 9; col++) {
    		        x = puzzle[row][col];
    		        if (x) {
    		            rst[ROW*9 + COL][x] = x;
    		        }
    			}
		        // vertical scan
        		//  find all of the values in col c
        		col = COL;
        		row = ROW;
                for (row = 0; row < 9; row++) {
		            x = puzzle[row][col];
        	    	if (x) {
                		rst[ROW*9 + COL][x] = x;
            		}
        		}
        		// box scan
        		// find all of the values in box
        		int boxR = (ROW/3) * 3;
		        int boxC = (COL/3) * 3;
        		for (int i = 0; i < 3; i++) { 
        		    for (int j = 0; j < 3; j++) { 
        		        x = puzzle[boxR+i][boxC+j];
        		        if (x) {
        		            rst[ROW*9 + COL][x] = x;
}}}}}}}
int findMin(int rst[81][10]) {// returns grid index 0-80;
    int min = 10;
    int minIndex = -1;
    for (int i = 0; i < 81; i++) {
        int validGuesses = 0;
        for (int j = 1; j < 10; j++) {
            if (rst[i][j] == 0)
                validGuesses++;
        }
        if (validGuesses < min && validGuesses > 0) {
            min = validGuesses;
            minIndex = i;
        }
    }
    return minIndex;
}                                  
int main() {
    int arr[2056][82] = {0};
    for (int i = 0; i < 2056; i++) {
        for (int j = 0; j < 82; j++) {
            arr[i][j] = 0;
        }
    }
    int numGrids = 1;
    int num0s = countZeroes(puzzle);
    int cycles = 0;
    while (num0s > 0) {
        for (int nG = 0; nG < numGrids; nG++) {
            cout << "Cycle # " << cycles << "  Grid # " << nG << endl;
            SKIP:
            if (arr[nG][81] != -1) {
                // copy puzzle as given
                int grid[9][9] = {0};
                copyGrid(grid,puzzle);
                // add additions
                for (int i = 0; i < 81; i++) {
                    if (arr[nG][i] > 0) {
                        int r = i / 9;
                        int c = i % 9;
                        grid[r][c] = arr[nG][i];
                    }
                }
                num0s = countZeroes(grid);
                cout << "Zero Count = " << num0s << endl;
                if (num0s == 0) {
                   printf("GRID %d solved!\n",nG);
                   displayGrid(grid);
                   return 0;
                }

                //displayGrid(grid);
                //usleep(500000);
                int rst[81][10] = {0};
                update(grid, rst);
                int gridIndex = findMin(rst);
                if (gridIndex == -1) {
                    arr[nG][81] = -1;
                    goto SKIP;
                    // this branch made a mistake goto SKIP          
                }
                // step 2 b
                int numGuesses = 0;
                int Guesses[9] = {0};
                for (int validGuess = 1; validGuess < 10; validGuess++) {
                    int x = rst[gridIndex][validGuess]; 
                    if (x == 0) {
                        Guesses[numGuesses] = validGuess;
                        numGuesses++;
                    }
                }
                if (numGuesses == 9) {
                    cout << "# of guesses = 9\n"; 
                    arr[nG][81] = -1;
                    goto SKIP;
                }
               
                // step 3
                cout << "Min Num of Guesses = " << numGuesses << endl;
                if (numGuesses > 1) {
                    // plug in the first guess
                    int gs = 0;
                    arr[nG][gridIndex] = Guesses[gs];
                    printf("%d inserted in Grid:%d at grid Index:%d\n", arr[nG][gridIndex],nG,gridIndex);
                    usleep(wait);
                    gs++;
                    int oldNumGrids = numGrids;
                    int newGrids = 0;
                    for (int g = 1; g < numGuesses; g++) {
                        numGrids++;
                        newGrids++;
                    }
                    for (int g = 1; g <= newGrids; g++) {
                        for (int a = 0; a < 81; a++) {
                            arr[numGrids - g][a] = arr[nG][a]; 
                        } 
                        arr[numGrids - g][gridIndex] = Guesses[gs];
                        printf("%d inserted in Grid:%d at grid Index:%d\n", arr[numGrids - g][gridIndex],numGrids - g,gridIndex);
                        usleep(wait);
                        gs++; 
                    }
                }
                else if (numGuesses == 1) {
                    arr[nG][gridIndex] = Guesses[0];
                    printf("%d inserted in Grid:%d at grid Index:%d\n", arr[nG][gridIndex],nG,gridIndex);
                    usleep(wait);
                }
                cout << endl;
            }
        cycles++;
        }
    }
    return 0;
}
/*
    step 1
        start with one grid
        
    step 2
        find minNumGuesses
            a) find zero with minNumGuesses    
            b) find guesses for that zero
    
    step 3
        if greater than 1, create (minNumGuesses - 1) additionals grid
        
    step 4
        plug guess 1 into first grid
        .............................. 
        plug guess n into the nth grid

    step 5 
        loop 2 - 4 until done 

    int i = 1;
    for (int i = 0; i < numGrids; i++) {
        countZeroes        


*/ 
