#include<stdio.h>
#include<stdint.h>
#include<assert.h>
#include<math.h>
#include<stdlib.h>
#define board_rows (4)
#define board_columns (5)
#define MAX_ORD (28629151)
#define connect (4)
#define col_combinations (31)

typedef char player_t;
typedef char board_t[board_rows][board_columns];

int computed_moves[col_combinations][col_combinations][col_combinations][col_combinations][col_combinations];

enum {
    EMPTY = 0,
    RED,
    BLUE,
};

int Pow(int x,int n){
    int to_ret = 1;
    for(int i=0;i<n;i++){ to_ret *= x;}
    return to_ret;
}


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
            printf("%3c ",board[row][col]);
        }
        printf("\n");
    }    
}

typedef struct {
    int col;
    int score;
    int time;
} move_t;

uint16_t encode_move(move_t move){
    uint16_t b = 0;

    assert(0<=move.col && move.col<board_columns);
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
    // assert(0<=move.time && move.time <= (board_columns)*(board_rows));
    b |= (move.time << 6);
    return b;
}

move_t decode_move(uint16_t b){
    move_t move;
    move.col = ((b) & 0x7);
    if ((b >> 3) & 0x1) move.score = -1;
    else if ((b >> 4) & 0x1) move.score = 0;
    else if ((b >> 5) & 0x1) move.score = 1;
    move.time = (b >> 6);
    return move;
}

// This is your has_won function:
// int has_won(board_t board, player_t player){
//     for (int row = 0; row < board_rows; row++)
//     {   int temp = 0;
//         for (int col = 0; col < board_columns; col++)
//         {
//                 if (board[row][col] == player) {temp += 1;}
//                 else {temp = 0;}
//                 if (temp == connect) {goto found;}
//         }
//     }
//     for (int col = 0; col < board_columns; col++)
//     {   int temp = 0;
//         for (int row = 0; row < board_rows; row++)
//         {
//             if (board[row][col] == player) {temp += 1;}
//             else {temp = 0;}
//             if (temp == connect) {goto found;}
//         }   
//     }
//     // Cheking for the normal diagonals 
//     for (int row = 0; row < board_rows - connect + 1; row++)
//     {
//         int ref = 0;
//         for (int iter = 0; iter < min_personal(board_rows - row,board_columns); iter++)
//         {
//                 if (board[row+iter][iter] == player) {ref += 1;}
//                 else {ref = 0;}
//                 if (ref == connect) {goto found;}
//         }
//     }
//     for (int col = 1; col < board_columns - connect + 1; col++)
//     {
//         int ref = 0;
//         for (int iter = 0; iter < min_personal(board_columns - col,board_rows); iter++)
//         {
//                 if (board[iter][col+iter] == player) {ref += 1;}
//                 else {ref = 0;}
//                 if (ref == connect) {goto found;}
//         }
//     }
//     for (int row = connect - 1; row < board_rows; row++)
//     {   int ref = 0;
//         for (int iter = 0; iter < min_personal(board_columns,board_rows - row); iter++)
//         {
//                 if (board[row - iter][iter] == player) {ref += 1;}
//                 else {ref = 0;}
//                 if (ref == connect) {goto found;}
//         }    
//     }
//     for (int col = 0; col < board_columns - connect + 1; col++)
//     {
//         int ref = 0;
//         for (int iter = 0; iter < min_personal(board_rows,board_columns - col); iter++)
//         {
//                 if (board[board_rows - 1 - iter][col + iter] == player) {ref += 1;}
//                 else {ref = 0;}
//                 if (ref == connect) {goto found;}
//         }       
//     }
//     if (0){
//         found:
//         return 1;
//     }
//     return 0;
// }

int has_won(board_t board, player_t player){
    for (int row = 0; row < board_rows; row++)
    {   int temp = 0;
        for (int col = 0; col < board_columns; col++)
        {
                if (board[row][col] == player) {temp += 1;}
                else {temp = 0;}
                if (temp == connect) {return 1;}
        }
    }
    for (int col = 0; col < board_columns; col++)
    {   int temp = 0;
        for (int row = 0; row < board_rows; row++)
        {
            if (board[row][col] == player) {temp += 1;}
            else {temp = 0;}
            if (temp == connect) {return 1;}
        }   
    }
    if(board[0][0]==board[1][1] && board[0][0]==board[2][2] && board[0][0]==board[3][3] && board[0][0]!='.'){
        return 1;
    }
    if(board[0][1]==board[1][2] && board[0][1]==board[2][3] && board[0][1]==board[3][4] && board[0][1]!='.'){
        return 1;
    }
    if(board[0][3]==board[1][2] && board[0][3]==board[2][1] && board[0][3]==board[3][0] && board[0][3]!='.'){
        return 1;
    }
    if(board[0][4]==board[1][3] && board[0][4]==board[2][2] && board[0][4]==board[3][1] && board[0][4]!='.'){
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

int* my_ord(board_t board,int* track){
    int* indexing = malloc(board_columns * sizeof(int));
    for (int col = 0; col < board_columns; col++)
    {
        int temp = 0;

        for (int i = 1; i < track[col]; i++)
        {
            temp += Pow(2,i);
        }
        int temp2 = 0;
        for (int j = 0; j < track[col]; j++)
        {
            if (board[board_rows-j-1][col] == 'x'){
                temp2 += Pow(2,j);
            }
        }
        indexing[col] = temp + temp2;
        
    }
    return indexing;
}

move_t best_move(board_t board, player_t player, int move_ctr){

    move_t response;
    move_t candidate;
    int no_candidate = 1;
    assert(!is_full(board));
    assert(!has_won(board, player));
    assert(!has_won(board, other_player(player)));
    int track[5] = {0,0,0,0,0};

    for (int col = 0; col < board_columns; col++)
    {   
        for (int row = board_rows-1; row >=0; row--)
        {   
            if (board[row][col] == '.'){break;}
            else{track[col]++;}
        }
    }   

    int* ptr = my_ord(board, track);
    int ord[5];
    for (int i = 0; i < 5; i++) {
        ord[i] = *(ptr + i);
    }


    if (computed_moves[ord[0]][ord[1]][ord[2]][ord[3]][ord[4]]){
        return decode_move(computed_moves[ord[0]][ord[1]][ord[2]][ord[3]][ord[4]]);
    }

    for (int col = 0; col < board_columns; col++)
    {   
        if (track[col] != board_rows){
            board[board_rows-track[col]-1][col] = player;
            if (has_won(board,player)){

                board[board_rows-track[col]-1][col] = '.';
                candidate = (move_t) {
                        .col = col,
                        .score = 1,
                        .time = 1
                        };
                computed_moves[ord[0]][ord[1]][ord[2]][ord[3]][ord[4]] = encode_move(candidate);
                return candidate;
            }
            board[board_rows-track[col]-1][col] = '.' ;
        }
    }
    for (int col = 0; col < board_columns; col++)
    {
        if (track[col] != board_rows){
            board[board_rows-track[col]-1][col] = player;
            if (is_full(board)){
                board[board_rows-track[col]-1][col] = '.';
                candidate = (move_t) {
                        .col = col,
                        .score = 0,
                        .time=1
                        };
                computed_moves[ord[0]][ord[1]][ord[2]][ord[3]][ord[4]] = encode_move(candidate);
                return candidate;
            }

            response = best_move(board, other_player(player),0);

            board[board_rows-track[col]-1][col] = '.';
            if (response.score == -1) {
                    computed_moves[ord[0]][ord[1]][ord[2]][ord[3]][ord[4]] = encode_move(candidate = (move_t) {
                        .col = col,
                        .score = 1,
                        .time = response.time + 1
                        });
                    return candidate;
                } else if (response.score == 0) {
                    if(no_candidate || (candidate.time < response.time + 1))
                    candidate = (move_t) {
                        .col = col,
                        .score = 0,
                        .time = response.time + 1
                    };
                    no_candidate = 0;
                } else { /* response.score == +1 */
                    if (no_candidate || (candidate.time < response.time + 1)) {
                        candidate = (move_t) {
                            .col = col,
                            .score = -1,
                            .time = response.time + 1
                        };
                        no_candidate = 0;
                    }
                }
        }

    }
    computed_moves[ord[0]][ord[1]][ord[2]][ord[3]][ord[4]] = encode_move(candidate);
    return candidate;
}

void print_key()
{
    int ii = 0;
    for (int col = 0; col < board_columns; ++col) {
        
            printf("%3d ", ii++);
        
    }
    printf("\n");
}

int main(){
    int move, col;
    board_t board;
    move_t response;
    player_t current = 'x';
    init_board(board);
 
    while (1) {
        int track[5] = {0,0,0,0,0};

        for (int col = 0; col < board_columns; col++)
        {
            for (int row = board_rows-1; row >= 0 ; row--)
            {
                if (board[row][col] == '.'){break;}
                else{track[col]++;}
            }
        }

        print_board(board);
        if (current == 'x') {
            print_key();
            printf("Enter your move: ");
            scanf("%d", &move);
            assert(0<=move & move<board_columns);
            col = move;

            assert(board[board_rows-track[col]-1][col] == '.');
            board[board_rows-track[col]-1][col] = current;
        } else {
            printf("Computer's turn :\n");
            response = best_move(board, current,0);
            board[board_rows-track[response.col]-1][response.col] = current;
        }
        if (has_won(board, current)) {
            print_board(board);
            printf("Player %c has won!\n", current);
            break;
        } else if (is_full(board)) {
            print_board(board);
            printf("Draw.\n");
            break;
        }
        current = other_player(current);
    }

    return 0;

}
