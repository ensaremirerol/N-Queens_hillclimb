#include <stdio.h> // std input/output
#include <stdlib.h> // for rand() and srand()
#include <time.h> // for time and clock
#include <stdbool.h> // boolean
#define N  8 // board size and queen count

void printStatistics(double** statistics){
    printf("Move Count\tRandom Restart Count\tTime Spend(Seconds)\n");
    for (int i = 0; i < 25; i++)
    {
        printf("%.0f\t\t%.0f\t\t\t%f\n", statistics[i][0], statistics[i][1], statistics[i][2]);
    }
    
}

double timeDelta(clock_t cl){
    cl = clock() - cl;
    return ((double) cl) / CLOCKS_PER_SEC;
}

void printState(int* state){
    for (int i = 0; i < N; i++)
    {
        printf("%d ", state[i]);
    }
    printf("\n");
    
}

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

bool compareStates(int* state1, int* state2){
    for(int i = 0; i < N; i++){
        if(state1[i] != state2[i])return false;
    } 
    return true;
}

void copyState(int* fromState, int* toState){
    for (int i = 0; i < N; i++)
    {
        toState[i] = fromState[i];
    }
}

void getRandomState(int* state){
    for(int i = 0; i < N; i++){
        state[i] = rand() % N;
    }
}

void getBoardUsingState(int* state, int** board){
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i][j] = 0;
        }
    }
    for (int i = 0; i < N; i++)
    {
        board[state[i]][i] = 1;
    }
}

int evaluteBoard(int** board, int* state){
    int result = 0;
    int row;
    int col;
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
    return (int) result/2; 
}

void getBestNeighbour(int** board ,int* state, int result){
    // Keeps better state
    int* newState;
    newState = (int*) malloc(sizeof(int) * N);
    copyState(state, newState);
    int** newBoard;
    newBoard = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) newBoard[i] = (int*) malloc(sizeof(int) * N);
    getBoardUsingState(newState, newBoard);
    int newResult = result;

    // Keeps temporary state for comparison 
    int* neighbourState;
    neighbourState = (int*) malloc(sizeof(int) * N);
    copyState(state, neighbourState);
    int** neighbourBoard;
    neighbourBoard = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) neighbourBoard[i] = (int*) malloc(sizeof(int) * N);
    getBoardUsingState(neighbourState, neighbourBoard);
    int neighbourResult = result;

    // Checking all states
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
    copyState(newState, state);
    getBoardUsingState(state, board);
}

double* hillClimb(int** board, int* state){
    double randomRestartCount = 0, moveCount = 0;
    double timeSpend = 0.0;
    double* statistic;
    statistic = (double*) malloc(sizeof(double) * 3);
    clock_t cl = clock();
    int result = evaluteBoard(board, state);
    int** neighbourBoard;
    neighbourBoard = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++) neighbourBoard[i] = (int*) malloc(sizeof(int) * N);
    int* neighbourState;
    neighbourState = (int*) malloc(sizeof(int) * N);
    int neighbourResult = result;

    copyState(state, neighbourState);
    getBoardUsingState(state, neighbourBoard);


    while (true)
    {
        copyState(neighbourState, state);
        getBoardUsingState(state, board);
        result = neighbourResult;
        getBestNeighbour(neighbourBoard, neighbourState, neighbourResult);
        moveCount++;
        neighbourResult = evaluteBoard(neighbourBoard, neighbourState);
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
    statistic[0] = moveCount;
    statistic[1] = randomRestartCount;
    statistic[2] = timeDelta(cl);

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
        statistics[i] = hillClimb(board, state);
        printBoard(board); 
    }
    printStatistics(statistics);
    getchar();
    return 0;
}