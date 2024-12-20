#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct board
{
    int board[3][3];
    bool firstToMove;
};

void printBoard(struct board currBoard);
int* getPlayerMove(struct board* currBoard);
int* getBestMove(struct board currBoard);
int terminal(struct board* currBoard);
void transform(struct board* currBoard, int* move);

int main()
{
    struct board currBoard = {
        .board = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        .firstToMove = true
    };

    int state = 0;
    while (state == 0)
    {
        printBoard(currBoard);
        int* move = getPlayerMove(&currBoard);
        transform(&currBoard, move);

        state = terminal(&currBoard);
        if (state == 0)
        {
            move = getBestMove(currBoard);
            transform(&currBoard, move);
        }
        free(move);
        state = terminal(&currBoard);
    }

    printBoard(currBoard);
    if (state == 1)
    {
        printf("X has won.");
    }
    else if (state == -1)
    {
        printf("O has won.");
    }
    else
    {
        printf("Draw.");
    }
}

int terminal(struct board* currBoard)
{
    int over = 0;
    // horizontal
    for (int i = 0; (i < 3 && over == 0); i++)
    {
        if ((currBoard->board[i][0] == currBoard->board[i][1]) && (currBoard->board[i][1] == currBoard->board[i][2]) && currBoard->board[i][0] != 0)
        {
            over = currBoard->board[i][0];
        } 
    }
    // vertical
    for (int i = 0; (i < 3 && over == 0); i++)
    {
        if((currBoard->board[0][i] == currBoard->board[1][i]) && (currBoard->board[1][i] == currBoard->board[2][i]) && currBoard->board[i][0] != 0)
        {
            over = currBoard->board[0][i];
        } 
    }
    // across
    if (over == 0)
    {
        if((currBoard->board[0][0] == currBoard->board[1][1]) && (currBoard->board[1][1] == currBoard->board[2][2]) && currBoard->board[0][0] != 0)
        {
            over = currBoard->board[0][0];
        } 
    }
    if (over == 0)
    {
        if((currBoard->board[0][2] == currBoard->board[1][1]) && (currBoard->board[1][1] == currBoard->board[2][0]) && currBoard->board[0][2] != 0){
            over = currBoard->board[0][2];
        }
    }

    // board full
    bool full = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (currBoard->board[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (!full) break;
    }

    if (full) {
        over = 3;
    }

    return over;
}

/*
    Make changes to the board;
*/
int getCharToPlace(bool firstToMove);
void transform(struct board* currBoard, int* move)
{
    currBoard->board[move[0]][move[1]] = getCharToPlace(currBoard->firstToMove);
    currBoard->firstToMove = !currBoard->firstToMove;
}
int getCharToPlace(bool firstToMove)
{
    char toMove = 1;
    if (!firstToMove)
    {
        toMove = -1;
    }
    return toMove;
}

/*
    Engine making move
*/
struct possibleMovesStruct 
{
    int moveCount;
    int moves[9][2];
};
struct possibleMovesStruct getPossibleMoves(struct board* currBoard);
int getValue(struct board currBoard);
int* getBestMove(struct board currBoard){
    struct possibleMovesStruct possibleMoves = getPossibleMoves(&currBoard);
    bool maximizing = currBoard.firstToMove;
    int bestValue = maximizing ? -1000 : 1000;
    int* bestMove = malloc(2 * sizeof(int));
    for (int i = 0; i < possibleMoves.moveCount; i++) {
        struct board new_pos;
        memcpy(&new_pos, &currBoard, sizeof(struct board));
        transform(&new_pos, possibleMoves.moves[i]);
        int val = getValue(new_pos);

        if ((maximizing && val > bestValue) || (!maximizing && val < bestValue)){
            bestValue = val;
            bestMove[0] = possibleMoves.moves[i][0];
            bestMove[1] = possibleMoves.moves[i][1];
        }
    }
    return bestMove;
}
int getValue(struct board currBoard)
{
    int termVal = terminal(&currBoard);
    if (termVal != 0) {
        return (termVal == 3) ? 0 : termVal;
    }

    struct possibleMovesStruct possibleMoves = getPossibleMoves(&currBoard);
    bool maximizing = currBoard.firstToMove; 
    int best_value = maximizing ? -10000 : 10000;

    for (int i = 0; i < possibleMoves.moveCount; i++) {
        struct board new_pos;
        memcpy(&new_pos, &currBoard, sizeof(struct board));
        transform(&new_pos, possibleMoves.moves[i]);
        int val = getValue(new_pos);
        
        if (maximizing) {
            best_value = (val > best_value) ? val : best_value;
        } else {
            best_value = (val < best_value) ? val : best_value;
        }
    }
    
    return best_value;
}

struct possibleMovesStruct getPossibleMoves(struct board* currBoard)
{
    struct possibleMovesStruct possibleMoves = {
        .moveCount = 0,
    };


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if ((currBoard->board[i][j]) == 0){
                possibleMoves.moves[possibleMoves.moveCount][0] = i;
                possibleMoves.moves[possibleMoves.moveCount][1] = j;
                possibleMoves.moveCount++;
            }

            if (possibleMoves.moveCount >= 9) {
                break;
            }
        }
    }

    return possibleMoves;
}


/*
    User making the move
*/
bool validRow(char in);
bool validCol(char in);
int transformRow(char in);
int transformCol(char in);
bool filled(char row, char col, struct board* currBoard);
int* getPlayerMove(struct board* currBoard)
{
    char inp[3];
    printf("Input move: ");
    scanf("%2s", &inp);
    while (!validRow(inp[0]) || !validCol(inp[1]) || !filled(inp[0], inp[1], currBoard)){
        printf("Invalid Move! Move format should be [a-c][1-3] and should be empty\n");
        printf("Input move: ");
        scanf("%2s", &inp);
    }
    int* outp = (int[]){transformRow(inp[0]), transformCol(inp[1])};
    return outp;
}

bool validRow(char in)
{
    return (in >= 'a' && in <= 'c');
}
int transformRow(char in)
{
    return ((int)in)-97;
}
bool validCol(char in)
{
    return (in >= '1' && in <= '3');
}
int transformCol(char in)
{
    return ((int)in)-49;
}
bool filled(char row, char col, struct board* currBoard)
{
    bool fill = true;
    if (validRow(row) || validCol(col)){
        fill = currBoard->board[transformRow(row)][transformCol(col)] == 0;
    }
    return fill;
}

/*
    Printing the board
*/
char mapBoardSquare(int square);
char mapBoardChars(int h);
void printBoard(struct board currBoard)
{
    // headers 
    printf("  ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", i+1);
    }
    printf("\n");

    // board
    for (int i = 0; i < 3; i++)
    {
        printf(" %c", mapBoardChars(i));
        for (int j = 0; j < 3; j++)
        {
            printf(" %c", mapBoardSquare(currBoard.board[i][j]));
        }
        printf("\n");
    }
}
char mapBoardChars(int h)
{
    char mapped;
    switch (h)
    {
    case 0:
        mapped = 'a';
        break;
    case 1:
        mapped = 'b';
        break;
    case 2:
        mapped = 'c';
        break;
    default:
        mapped = 'd';
        break;
    }
    return mapped;
}

char mapBoardSquare(int square)
{
    char mapped;
    switch (square)
    {
    case -1:
        mapped = 'O';
        break;
    case 1:
        mapped = 'X';
        break;
    default:
        mapped = ' ';
        break;
    }
    return mapped;
}