#include<stdio.h>
#include<stdint.h>
#include<assert.h>

#define board_row (5)
#define board_columns (4)
#define MAX_ORD (3486784400)

typedef char player_t;
typedef char board_t[board_row][board_columns];

uint8_t computed_moves[MAX_ORD+1];

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