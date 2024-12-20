#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct board
{
    int board[3][3];
    bool firstToMove;
};

void printBoard(struct board currBoard);
int* getPlayerMove(struct board* currBoard);
bool terminal(struct board* currBoard);
void transform(struct board* currBoard, int* move);

int main()
{
    struct board currBoard = {
        .board = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        .firstToMove = true
    };

    while (!terminal(&currBoard))
    {
        printBoard(currBoard);
        int* move = getPlayerMove(&currBoard);
        transform(&currBoard, move);
        free(move);
    }
}

bool terminal(struct board* currBoard)
{
    return false;
}

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