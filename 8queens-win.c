#include <stdio.h> // std input/output
#include <stdlib.h> // for rand() and srand()
#include <time.h> // for time
#include <stdbool.h> // boolean
#include <windows.h> // for time
#define N  8 // board size and queen count

/*
    8-QUEEN Problem
    Chess board is 8 by 8
    State indexes represents columns. It contents represents rows
    Chess boards are evaluated by possible attack counts
    If GetBestNeighbour function returns same state hillClimb functions stops and prints the result
*/

// Frees allocated board memory
void freeBoard(int** board){
    for (int i = 0; i < N; i++) free(board[i]);
    free(board);
}

// Prints given 2d array as table
void printStatistics(double** statistics){
    printf("No.\tMove Count\tRandom Restart Count\tTime Spend(Seconds)\n");
    for (int i = 0; i < 25; i++)
    {
        printf("%d\t%.0f\t\t%.0f\t\t\t%f\n",i+1, statistics[i][0], statistics[i][1], statistics[i][2]);
    }    
}

// Prints given chess board: 1 represents queens
void printBoard(int** board){
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks equality of two states
bool compareStates(int* state1, int* state2){
    for(int i = 0; i < N; i++){
        if(state1[i] != state2[i])return false;
    } 
    return true;
}

// Copies a state to another state (Not allocating new memory)
void copyState(int* fromState, int* toState){
    for (int i = 0; i < N; i++)
    {
        toState[i] = fromState[i];
    }
}

// Writes a random state to given state
void getRandomState(int* state){
    for(int i = 0; i < N; i++){
        state[i] = rand() % N;
    }
}

// Creates a chess board using state
void getBoardUsingState(int* state, int** board){
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i][j] = 0;
        }
        
    }

    for (int i = 0; i < N; i++) board[state[i]][i] = 1;
    
}

// Evalutes board by attack counts
int evaluteBoard(int** board, int* state){
    // Attack count
    int result = 0;
    int row;
    int col;
    // Checks every queen
    for (int i = 0; i < N; i++)
    {
        row = state[i];
        col = i;
        // Right scan
        while(++col < N){
            if(board[row][col] == 1){
                result++;
                break;
            }
        }

        col = i;
        // Left scan
        while(--col >= 0){
            if(board[row][col] == 1){
                result++;
                break;
            }
        }
        // Diagonal scans

        col = i;
        // Right Down scan
        while(++col < N && ++row < N){
            if(board[row][col] == 1){
                result++;
                break;
            }
        }

        row = state[i];
        col = i;
        // Right Up scan
        while(++col < N && --row >= 0){
            if(board[row][col] == 1){
                result++;
                break;
            }
        }
        
        row = state[i];
        col = i;
        //Left Up scan
        while(--col >= 0 && --row >= 0){
            if(board[row][col] == 1){
                result++;
                break;
            }
        }
        
        row = state[i];
        col = i;
        // Left Down scan
        while(--col >= 0 && ++row < N){
            if(board[row][col] == 1){
                result++;
                break;
            }
        }
    }
    // Retruns only pairs
    return (int) result/2; 
}

// Gets best neighbour to current state (Makes only one move)
void getBestNeighbour(int** board ,int* state, int result){
    // Keeps better state
    int* newState;

    newState = (int*) malloc(sizeof(int) * N);

    int** newBoard;

    newBoard = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) newBoard[i] = (int*) malloc(sizeof(int) * N);

    // Copies current state to 
    copyState(state, newState);  
    getBoardUsingState(newState, newBoard);
    int newResult = result;

    // Keeps temporary state for comparison 
    int* neighbourState;

    neighbourState = (int*) malloc(sizeof(int) * N);

    int** neighbourBoard;

    neighbourBoard = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) neighbourBoard[i] = (int*) malloc(sizeof(int) * N);

    copyState(state, neighbourState);
    getBoardUsingState(neighbourState, neighbourBoard);

    int neighbourResult = result;
    // Checking all neighbour states and keeps best one in newState and newBoard
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            // Skiping current State
            if(j != state[i]){
                neighbourState[i] = j;
                neighbourBoard[neighbourState[i]][i] = 1;
                neighbourBoard[state[i]][i] = 0;
                neighbourResult = evaluteBoard(neighbourBoard, neighbourState);
                if (neighbourResult <= newResult){
                    copyState(neighbourState, newState);
                    getBoardUsingState(newState, newBoard);
                    newResult = neighbourResult;
                }
                // Reverting changes for next check
                neighbourBoard[neighbourState[i]][i] = 0;
                neighbourBoard[state[i]][i] = 1;
                neighbourState[i] = state[i];
            }
        }
    }
    // Copies newState and board to old ones
    copyState(newState, state);
    getBoardUsingState(state, board);

    // Frees unused memory
    freeBoard(newBoard);
    freeBoard(neighbourBoard);
    free(newState);
    free(neighbourState);

}

// HillClimb algorithm
void* hillClimb(int** board, int* state){
    // Neighbour state and board
    int* neighbourState;
    int** neighbourBoard;

    neighbourState = (int*) malloc(sizeof(int) * N);

    neighbourBoard = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) neighbourBoard[i] = (int*) malloc(sizeof(int) * N);

    // Statistics
    double randomRestartCount = 0, moveCount = 0;
    double* statistic;
   
    statistic = (double*) malloc(sizeof(double) * 3);

    // Execution timer from windows.h libary (clock from time.h libary gives round up values in windows)
    LARGE_INTEGER frequency, t1, t2;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);

    int result = evaluteBoard(board, state);
    
    int neighbourResult = result;

    copyState(state, neighbourState);
    getBoardUsingState(state, neighbourBoard);


    while (true)
    {
        // Copies previous best state and board to state and board variables
        copyState(neighbourState, state);
        getBoardUsingState(state, board);
        result = neighbourResult;
        // Gets best neighbour to the current state
        getBestNeighbour(neighbourBoard, neighbourState, neighbourResult);
        // getBestNeighbour only makes one move only
        moveCount++;
        neighbourResult = evaluteBoard(neighbourBoard, neighbourState);
        // If getBestNeighbour returns the same state it means we achieved our goal
        // Checks result
        if(compareStates(state, neighbourState)){
            break;
        }
        else if(result == neighbourResult){
            // Probably we are approaching a shoulder or a local max
            // either way we need random restart
            // printf("Random Restart\n");
            getRandomState(neighbourState);
            getBoardUsingState(neighbourState, neighbourBoard);
            neighbourResult = evaluteBoard(neighbourBoard, neighbourState);
            randomRestartCount++;
        }
    }
    // Second time point for execution timer
    QueryPerformanceCounter(&t2);
    statistic[0] = moveCount;
    statistic[1] = randomRestartCount;
    // t2 and t1 keeps processor clock times. frequency keeps clock per second
    statistic[2] = (double)(t2.QuadPart - t1.QuadPart) / frequency.QuadPart;

    freeBoard(neighbourBoard);
    free(neighbourState);

    return statistic;
}

int main(){
    clock_t cl;
    srand(time(NULL));
    int* state;
    state = (int*) malloc(sizeof(int) * N);
    int** board;
    board = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) board[i] = (int*) malloc(sizeof(int) * N);

    double** statistics;

    statistics = (double**) malloc(sizeof(double*) * 25);
    
    for (int i = 0; i < 25; i++)
    {
        getRandomState(state);
        getBoardUsingState(state, board);
        statistics[i] = (double*) hillClimb(board, state);
        printBoard(board); 
    }
    printStatistics(statistics);


    freeBoard(board);
    free(state);

    for (int i = 0; i < 25; i++)free(statistics[i]);
    free(statistics);

    getchar();
    return 0;
}