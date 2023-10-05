#include<stdio.h>
#include<stdint.h>
#include<assert.h>

#define board_rows (4)
#define board_columns (5)
#define MAX_ORD (1)
#define connect (4)

typedef char player_t;
typedef char board_t[board_rows][board_columns];

uint8_t computed_moves[MAX_ORD+1];

int min_personal(int a,int b){
    if (a>b){return b;}
    else{return a;}
}

void init_board(board_t board){
    for (int row = 0; row < board_rows; row++)
    {
        for (int col = 0; col < board_columns; col++)
        {
            board[row][col] = '.';
        }
    }
}

void print_board(board_t board){
    for (int row = 0; row < board_rows; row++)
    {
        for (int col = 0; col < board_columns; col++)
        {
            printf("%3c",board[row][col]);
        }
        printf("\n");
    }    
}

typedef struct {
    int col;
    int score;
} move_t;

uint8_t encode_move(move_t move){
    uint8_t b = 0;

    assert(0<=move.col && move.col<=5);
    b |= move.col;

    switch (move.score)
    {
    case -1:
        b |= 1 << 3;
        break;
    case 0:
        b |= 1 << 4;
        break;
    case 1:
        b |= 1 << 5;
        break;
    default:
        break;
    }
    return b;
}

uint8_t decode_move(uint8_t b){
    move_t move;
    move.col = ((b) & 0x7);
    if ((b >> 3) & 0x1) move.score = -1;
    else if ((b >> 4) & 0x1) move.score = -1;
    else if ((b >> 5) & 0x1) move.score = -1;
}

int has_won(board_t board, player_t player){
    for (int row = 0; row < board_rows; row++)
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
        for (int row = 0; row < board_rows; row++)
        {
            if (board[row][col] == player) {temp += 1;}
            else {temp = 0;}
            if (temp == connect) {goto found;}
        }   
    }
    // Cheking for the normal diagonals 
    for (int row = 0; row < board_rows - connect + 1; row++)
    {
        int ref = 0;
        for (int iter = 0; iter < min_personal(board_rows - row,board_columns); iter++)
        {
                if (board[row+iter][iter] == player) {ref += 1;}
                else {ref = 0;}
                if (ref == connect) {goto found;}
        }
    }
    for (int col = 1; col < board_columns - connect + 1; col++)
    {
        int ref = 0;
        for (int iter = 0; iter < min_personal(board_columns - col,board_rows); iter++)
        {
                if (board[iter][col+iter] == player) {ref += 1;}
                else {ref = 0;}
                if (ref == connect) {goto found;}
        }
    }
    for (int row = connect - 1; row < board_rows; row++)
    {   int ref = 0;
        for (int iter = 0; iter < min_personal(board_columns,board_rows - row); iter++)
        {
                if (board[row - iter][iter] == player) {ref += 1;}
                else {ref = 0;}
                if (ref == connect) {goto found;}
        }
        
    }
    for (int col = 0; col < board_columns - connect + 1; col++)
    {
        int ref = 0;
        for (int iter = 0; iter < min_personal(board_rows,board_columns - col); iter++)
        {
                if (board[board_rows - 1 - iter][col + iter] == player) {ref += 1;}
                else {ref = 0;}
                if (ref == connect) {goto found;}
        }
        
    }

    if (0){
        found:
        return 1;
    }
    return 0;
}

int is_full(board_t board){
    for (int row = 0; row < board_rows; row++)
    {
            for (int col = 0; col < board_columns; col++)
            {
                if(board[row][col]=='.') return 0;
            }
            
    }
    return 1;
}

player_t other_player(player_t player){
    switch (player)
    {
    case 'x':
        return 'o';
        break;
    case 'o':
        return 'x';
        break;
    default:
        assert(0);
    }
}



int main(){
    board_t board;
    init_board(board);
    print_board(board);
    for (int i = 0; i < board_rows; i++)
    {
        for (int j = 0; j < board_columns; j++)
        {
            scanf(" %c",&board[i][j]);
            print_board(board);
        } 
    }
    
    player_t player = 'x';

    printf("%d\n",has_won(board,player));
    return 0;
}