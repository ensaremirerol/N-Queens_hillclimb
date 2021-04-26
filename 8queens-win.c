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
<<<<<<< HEAD
    double meanTime = 0, meanRandom = 0, meanMove = 0;
=======
    double meanTime = 0;
>>>>>>> evalute_state
    for (int i = 0; i < 25; i++)
    {
        printf("%d\t%.0f\t\t%.0f\t\t\t%f\n",i+1, statistics[i][0], statistics[i][1], statistics[i][2]);
        meanTime = statistics[i][2];
<<<<<<< HEAD
        meanMove = statistics[i][0];
        meanRandom = statistics[i][1];
    }
    meanTime /= 25;
    meanMove /= 25;
    meanRandom /= 25;
    printf("Avarage Move Count: %.2f\nAvarage Random Start: %.2f\nAvarage Time Spend: %f", meanMove, meanRandom, meanTime);
=======
    }
    meanTime /= 25;
    printf("Avarage Time Spend: %f", meanTime);
>>>>>>> evalute_state
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

int evaluteState(int* state){
    int result = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = i-1; j >=0 ; j--){
            if(state[i] == state[j]){
                result++;
                break;
            }
        }
        for (int j = i-1; j >=0 ; j--){
            if(state[i] - (i-j) == state[j]){
                result++;
                break;
            }
        }

        for (int j = i-1; j >=0 ; j--){
            if(state[i] + (i-j) == state[j]){
                result++;
                break;
            }
        }
          
        for (int j = i+1; j < N; j++){
            if(state[i] == state[j]){
                result++;
                break;
            }
        }
        for (int j = i+1; j < N; j++){
            if(state[i] - (i-j) == state[j]){
                result++;
                break;
            }
        }

        for (int j = i+1; j < N; j++){
            if(state[i] + (i-j) == state[j]){
                result++;
                break;
            }
        }

    }
    return (int) result / 2;
}

// Gets best neighbour to current state (Makes only one move)
void getBestNeighbour(int* state, int result){
    // Keeps better state
    int* newState;

    newState = (int*) malloc(sizeof(int) * N);

    // Copies current state to 
    copyState(state, newState);  
    int newResult = result;

    // Keeps temporary state for comparison 
    int* neighbourState;

    neighbourState = (int*) malloc(sizeof(int) * N);

    copyState(state, neighbourState);

    int neighbourResult = result;
    // Checking all neighbour states and keeps best one in newState and newBoard
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            // Skiping current State
            if(j != state[i]){
                neighbourState[i] = j;
                neighbourResult = evaluteState(neighbourState);
                if (neighbourResult <= newResult){
                    copyState(neighbourState, newState);
                    newResult = neighbourResult;
                }
                // Reverting changes for next check
                neighbourState[i] = state[i];
            }
        }
    }
    // Copies newState and board to old ones
    copyState(newState, state);
    // Frees unused memory
    free(newState);
    free(neighbourState);

}

// HillClimb algorithm
void* hillClimb(int* state){
    // Neighbour state
    int* neighbourState;

    neighbourState = (int*) malloc(sizeof(int) * N);

    // Statistics
    double randomRestartCount = 0, moveCount = 0;
    double* statistic;
   
    statistic = (double*) malloc(sizeof(double) * 3);

    // Execution timer from windows.h libary (clock from time.h libary gives round up values in windows)
    LARGE_INTEGER frequency, t1, t2;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);

    int result = evaluteState(state);
   
    int neighbourResult = result;

    copyState(state, neighbourState);

    while (true)
    {
        // Copies previous best state and board to state and board variables
        copyState(neighbourState, state);
        result = neighbourResult;
        // Gets best neighbour to the current state
        getBestNeighbour(neighbourState, neighbourResult);
        // getBestNeighbour only makes one move only
        moveCount++;
        neighbourResult = evaluteState(neighbourState);
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
            neighbourResult = evaluteState(neighbourState);
            randomRestartCount++;
        }
    }
    // Second time point for execution timer
    QueryPerformanceCounter(&t2);
    statistic[0] = moveCount;
    statistic[1] = randomRestartCount;
    // t2 and t1 keeps processor clock times. frequency keeps clock per second
    statistic[2] = (double)(t2.QuadPart - t1.QuadPart) / frequency.QuadPart;

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
        statistics[i] = (double*) hillClimb(state);
        getBoardUsingState(state, board);
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