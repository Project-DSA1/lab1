#include <stdio.h>
#include <assert.h>

#include "test.h"

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * 12 13 14 15 16 23 24 25 26 34 35 36 45 46 56
 * ...
 * board[14] = color of 56
 */
typedef int board_t[15];
typedef int player_t; /* A player should be RED or BLUE. */

player_t other_player(player_t player){
    switch(player){
        case RED: return BLUE;
        case BLUE: return RED;
        // default : assert(0);
    }
}

void init_board(board_t board){
    for(int i=0;i<15;i++){
        board[i] = NO;
    }
}

int has_lost(board_t board, player_t player)
{
    if(board[0]==board[1] && board[0]==board[5] && board[0]==player){
        return 1;
    }
    if(board[0]==board[2] && board[0]==board[6] && board[0]==player){
        return 1;
    }
    if(board[0]==board[3] && board[0]==board[7] && board[0]==player){
        return 1;
    }
    if(board[0]==board[4] && board[0]==board[8] && board[0]==player){
        return 1;
    }
    if(board[1]==board[2] && board[1]==board[9] && board[1]==player){
        return 1;
    }
    if(board[1]==board[3] && board[1]==board[10] && board[1]==player){
        return 1;
    }
    if(board[1]==board[4] && board[1]==board[11] && board[1]==player){
        return 1;
    }
    if(board[2]==board[3] && board[2]==board[12] && board[2]==player){
        return 1;
    }
    if(board[2]==board[4] && board[2]==board[13] && board[2]==player){
        return 1;
    }
    if(board[3]==board[4] && board[3]==board[14] && board[3]==player){
        return 1;
    }
    if(board[5]==board[6] && board[5]==board[9] && board[5]==player){
        return 1;
    }
    if(board[5]==board[7] && board[5]==board[10] && board[5]==player){
        return 1;
    }
    if(board[5]==board[8] && board[5]==board[11] && board[5]==player){
        return 1;
    }
    if(board[6]==board[7] && board[6]==board[12] && board[6]==player){
        return 1;
    }
    if(board[6]==board[8] && board[6]==board[13] && board[6]==player){
        return 1;
    }
    if(board[7]==board[8] && board[7]==board[14] && board[7]==player){
        return 1;
    }
    if(board[9]==board[10] && board[9]==board[12] && board[9]==player){
        return 1;
    }
    if(board[9]==board[11] && board[9]==board[14] && board[9]==player){
        return 1;
    }
    if(board[10]==board[11] && board[10]==board[14] && board[10]==player){
        return 1;
    }
    if(board[12]==board[13] && board[12]==board[14] && board[12]==player){
        return 1;
    }
    return 0;
}

int is_full(board_t board)
{
    for(int i=0;i<15;i++){
        if(board[i]==NO) return 0;
    }
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

move_t best_move(board_t board, player_t player)
{
    // assert(!is_full(board));
    move_t candidate;
    move_t response;
    int no_candidate = 1;

    /* No tictactoe like block here
    for(int i=0;i<15;i++){
        if(board[i]==NO){
            board[i] = player;
            if(has_lost(board,player)){
                board[i] = NO;
                return (move_t){
                    .line = i,
                    .score = 1
                };
            }
            board[i] = NO;
        }
    }*/
    
    for(int i=0;i<15;i++){
        if(board[i]==NO){
            board[i] = player;
            if(is_full(board)){
                // printf("Board is full\n");
                board[i] = NO;
                return (move_t){ // The board is full and no one lost before this move, so this is the only possible move and hence has to be returned even if it leads to a loss
                    .line = i,
                    .score = -1 // The game always ends, so the last move leads to a loss for the player 
                };
            }
            if(has_lost(board,player)){
                board[i] = NO;
                if(no_candidate){ 
                    candidate = (move_t){
                        .line = i,
                        .score = -1,
                    };
                    no_candidate = 0;
                }
                continue;
            }
            response = best_move(board,other_player(player));
            board[i] = NO;
            if(response.score==-1){
                return (move_t){
                    .line = i,
                    .score = 1
                };
            }
            // else if(response.score==0){ // Game of sim always ends, this must not arise
            //     candidate = (move_t){ // We are not checking for no_candidate here, so even if there is a candidate (which may have score -1), the score is set to 0
            //         .line = i,
            //         .score = 0,
            //     };
            //     no_candidate = 0;
            // }
            else{
                if(no_candidate){
                    candidate = (move_t){
                        .line = i,
                        .score = -1,
                    };
                    no_candidate = 0;
                }
            }
        }
    }
    return candidate;
}

void print_board(board_t board)
{
    for(int i=0;i<=9;i++){
        printf("%d  ",i);
    }
    for(int i=10;i<15;i++){
        printf("%d ",i);
    }
    printf("\n");
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case RED: printf("R  "); break;
        case BLUE: printf("B  "); break;
        case NO: printf("N  ");break;
        }
    }
    printf("\n");
}

int main()
{
    player_t player;
    board_t board;
    printf("Welcome to the Game of Sim\n");
    printf("Choose your color;\nR for Red and B for Blue\n");
    char playerchar;
    scanf("%c",&playerchar);
    if(playerchar=='R'){
        player = RED;
    }
    else{
        player = BLUE;
    }
    init_board(board);
    player_t current = player;
    move_t response;
    int move;
    /* Bug test - No bug detected. Output: Perfect (Expected output)
    board[0] = RED;
    board[1] = BLUE;
    if(board[0]==board[1]){
        printf("bug");
    }
    else{
        printf("Perfect");
    }*/
    while(1){
        print_board(board);
        if(current == player){
            printf("Enter your move: ");
            scanf("%d",&move);
            assert(board[move]==NO);
            board[move] = current;
        }
        else{
            response = best_move(board, current);
            board[response.line] = current;
        }
        if(has_lost(board, current)){
            print_board(board);
            if(current == RED) printf("BLUE won.\n");
            else printf("RED won.\n");
            break;
        }
        if(is_full(board)){ // Not needed, just for testing
            print_board(board);
            printf("DRAW.\nBug exists, this should not happen.");
            break;
        }
        current = other_player(current);
    }
    // print_board(board);
    // printf("%d",player);
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    return 0;
}