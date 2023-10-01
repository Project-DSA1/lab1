#include<stdio.h>
#include<stdint.h>
#include<assert.h>

#define board_row (5)
#define board_columns (4)
#define MAX_ORD (3486784400)
#define connect (4)

typedef char player_t;
typedef char board_t[board_row][board_columns];

uint16_t computed_moves[MAX_ORD+1];

void init_board(board_t board){
    for (int row = 0; row < board_row; row++)
    {
        for (int col = 0; col < board_columns; col++)
        {
            board[row][col] = '.';
        }
    }
}

void print_board(board_t board){
    for (int row = 0; row < board_row; row++)
    {
        for (int col = 0; col < board_columns; col++)
        {
            printf("%3c",board[row][col]);
        }
        printf("\n");
    }    
}

typedef struct {
    int row;
    int col;
    int score;
} move_t;

uint16_t encode_move(move_t move){
    uint16_t b = 0;
    assert(0<=move.row && move.row<5);
    b |= move.row;

    assert(0<=move.col && move.col<=5);
    b |= move.col << 3;

    switch (move.score)
    {
    case -1:
        b |= 1 << 6;
        break;
    case 0:
        b |= 1 << 7;
        break;
    case 1:
        b |= 1 << 8;
        break;
    default:
        break;
    }
    return b;
}

uint16_t decode_move(uint16_t b){
    move_t move;
    move.row = b & 0x7;
    move.col = ((b >> 3) & 0x7);
    if ((b >> 6) & 0x1) move.score = -1;
    else if ((b >> 7) & 0x1) move.score = -1;
    else if ((b >> 8) & 0x1) move.score = -1;
}

int has_won(board_t board, player_t player){
    for (int row = 0; row < board_row; row++)
    {   int temp = 0;
        for (int col = 0; col < board_columns; col++)
        {
                if (board[row][col] == player) {temp += 1;}
                else {temp = 0;}
                if (temp == connect) {goto found;}
        }
    }
    for (int col = 0; col < board_columns; col++)
    {   int temp = 0;
        for (int row = 0; row < board_row; row++)
        {
            if (board[row][col] == player) {temp += 1;}
            else {temp = 0;}
            if (temp == connect) {goto found;}
        }   
    }
    
    if (0){
        found:
        return 1;
    }
}