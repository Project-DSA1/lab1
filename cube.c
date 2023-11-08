#include<stdio.h>
char C[3][8] = { //coloring
    {'B','F','F','B','F','F','B','B'},
    {'D','L','D','R','U','R','U','L'},
    {'L','D','R','D','L','U','R','U'}
};
int c[8]={0,1,2,3,4,5,6,7}; //permutation - red values in pdf (ith location pe konsa cubie number)
int x[8]={0,0,0,0,0,0,0,0}; //orientation - (follow red cubie initial locations in pdf)
int temp[8];
const int m_cnt=6;
int Moves[6][8]={ //How permutations change
    {0,4,1,3,5,2,6,7}, //F' = f
    {0,1,5,2,4,6,3,7}, //R' = r
    {0,1,2,3,7,4,5,6},  //U' = u
    {0,2,5,3,1,4,6,7}, //F
    {0,1,3,6,4,2,5,7}, //R
    {0,1,2,3,5,6,7,4}  //U
};
int Offsets[6][8]={ //How orientations change (+ kitna)
    {0,0,0,0,0,0,0,0}, //F' = f
    {0,0,2,1,0,1,2,0}, //R' = r
    {0,0,0,0,1,2,1,2}, //U' = u
    {0,0,0,0,0,0,0,0}, //F
    {0,0,2,1,0,1,2,0}, //R
    {0,0,0,0,1,2,1,2}  //U
};
char MoveNames[]={'f','r','u','F','R','U'}; // small letters are anti-clockwise, capital are clockwise moves
int Faces[3][2][2]={ //These are locations (initial red cubie locations in pdf)
{ //Front
    {4,5},
    {1,2}
},
{ //Right
    {5,6},
    {2,3}
},
{ //Top
    {7,6},
    {4,5}
}
};
char FacesNames[][6]={"Front","Right","Top  "};
void Disp(){
    int offset = 0;
    for(int k=0;k<3;k++){
        printf("%s : \n",FacesNames[k]);
        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++){
                int loc = Faces[k][i][j]; //Ye location hai
                //printf("loc : %d\n",loc);
                //printf("offset : %d\n",offset);
                //printf("x[loc] :%d\n",x[loc]);
                //printf("c[loc] : %d\n",c[loc]);
                printf("%c ",C[(x[loc]+offset)%3][c[loc]]);
                offset = 3-offset;
            }
            printf("\n");
            offset = 3-offset;
        }
        offset++;
        printf("\n");
    }
}
// void PrintCode(){
//     printf("x :");
//     for(int i=0;i<8;i++){
//         printf("%d ",x[i]);
//     }
//     printf("\n c :");
//     for(int i=0;i<8;i++){
//         printf("%d ",c[i]);
//     }
//     printf("\n");
// }
void applymove(int i){
    int* move = Moves[i];
    int* offset = Offsets[i];
    for(int i=0;i<8;i++){
        temp[i]=c[i]; // stores permutations (c)
    }
    for(int i=0;i<8;i++){
        c[i] = temp[move[i]]; // c now stores the new permutation after the move
    }
    for(int i=0;i<8;i++){
        temp[i]=x[i]; // stores orientations (x)
    }
    for(int i=0;i<8;i++){
        x[i] = (temp[move[i]]+offset[i])%3; // x now stores the new orientations after the move
    }
};
long int facts[8];
long int encode(){ // 7! * (orientation index) + (permutation index) = index of cube state
    // 
    int N = 0;
    for(int i=2;i<8;i++){ // indexing orientations
        N = 3*N+x[i];
    }
    N = N*facts[7]; // facts[i] stores i factorial, so fact[7] = 7!
    for(int i=0;i<8;i++){
        temp[i]=c[i]; // stores permutations
    }

    // Now find the index of the permutation in all 7! permutations
    for(int i=1;i<8;i++){
        N += (temp[i]-1)*(facts[7-i]); //final index
        for(int j=i+1;j<8;j++){
            if(temp[j]>temp[i]){temp[j]=temp[j]-1;}
        }
    }
    // printf("encoding done\n");
    return N; // index of cube state = 7! * (orientation index) + (permutation index)
}
long int Nx;
void decode(int N){ // decodes the index of the cube state
    Nx = N/facts[7]; // orientation index
    N = N%facts[7]; // permutation index
    
    int s = 0; // sum used to find x[1] as x[0]=0 always and sum of x is a multiple of 3
    for(int i=7;i>1;i--){
        x[i]=Nx%3;
        Nx=Nx/3;
        s=(s+x[i])%3;
    }
    // x[0] is always 0
    x[1]=3-s; //as the sum = 0
    // x decoded completely till here

    for(int i=1;i<8;i++){
        c[i]=N/facts[7-i]+1;
        N=N%facts[7-i];
    } // till here, we found c, but we need to incorporate the decrements done while encoding
    for(int i=7;i>0;i--){
        for(int j=7;j>i;j--){
            if(c[j]>=c[i]){c[j]=c[j]+1;}
        }
        // c[i]=c[i]+1;
    }
    // printf("decoding done\n");
}
int legal(){ //sum = 0 in orientations or permutations have repeated numbers
    for(int i=0;i<8;i++){
        temp[i]=0;
    }
    for(int i=1;i<8;i++){
        temp[c[i]]=1; // stores permutations (c)
    }
    for(int i=1;i<8;i++){
        if(temp[i]==0){
            return 0;
        }
    }
    if(temp[0]!=0){return 0;}
    int s = 0;
        for(int i=0;i<8;i++){
            s = (s+x[i])%3;
        }
    return (s==0);
}

int visited[3674160]; // 3^6 * 7!
int Q[3674160]; //This is a queue
long int S[3674160];
long int cur = 0;
long int last = 0;
long int N_maxn1=3674160;

//initial state
// Testcase: User moves R'
//int xn[8]={0,0,2,1,0,1,2,0}; //user input of orientations (of front and back face, order is red numbers in pdf denoting initial state)
//int cn[8]={0,1,3,6,4,2,5,7}; //user input of permutations

// Test case : Do nothing
//  int xn[8] = {0,0,0,0,0,0,0,0};
//  int cn[8] = {0,1,2,3,4,5,6,7};

// Testcase 2 in main() function

int sol[100];
long int s;
void printSol(int s){ // s is the encoded move sequence (NOT the index of the cube state)
    int i = 0; // calculates the number of moves in the sequence
    while(s>0){
        sol[i]= -1+s%(m_cnt+1); // -1 as we considered f to be 1 not 0 else FFR and FR would be the same number
        i++;
        s=s/(m_cnt+1);
    }
    i--; // indexing starts from 0, we convert i from length to index so -1
    for(int j=i;j>=0;j--){
        printf("%c",MoveNames[sol[j]]);
    }
    printf("\n");
}
int done = 0;
int main(){
    facts[0]=1;
    for(int i=1;i<8;i++){
        facts[i]=facts[i-1]*i;
    }
    /*
    for(int i=0;i<N_maxn1;i++){
        decode(i);
        if(encode()!=i) printf("%ld\n", i-encode());
    }
    */    

    long int n0 = encode(); //initial state's index, this is always 0, we can remove this too 
    // printf("n0 is %ld\n",n0); // this is true
    long int n;

    // printf("%d\n",Q[0]);
    // for(int i=0;i<8;i++){ 
    //     x[i]=xn[i]; 
    //     c[i]=cn[i];
    // }
    // Testcase 2: where user does front move after the above moves
    applymove(3);
    applymove(4);
    applymove(5);
    // applymove(0);
    // applymove(1);
    // applymove(2);
    // applymove(0);
    // applymove(2);
    // applymove(1);
    printf("Initial state :\n\n");
    Disp();

    if(!legal()){
        printf("This isn't a legal state. That is to say; your cube cannot be solved.\n");
        return 0;
    }

    for(int i=0;i<N_maxn1;i++){
        visited[i]=0;
    }

    visited[encode()]=1;
    Q[0]=encode(); //This is a queue
    S[0]=0; // this is no moves as f corresponds to 1

    // BFS
    for(int iterat=0;iterat<N_maxn1;iterat++){
        if(done){break;}
        n = Q[cur]; // current index
        s = S[cur]; // current solution (ab tak ka) as a number (ie; index of the solution TILL NOW)
        decode(n); // decoding the index, this changes x and c
        if(n==n0){
                printf("found solution : ");
                printSol(s);
                done = 1;
                break;
        }
        // int movecount = 0;
        for(int move=0;move<m_cnt;move++){
            applymove(move);
            n = encode();
            if(visited[n]==0){
                visited[n]=1;
                last = last+1;
                // movecount++;
                Q[last]=n;
                S[last]=(m_cnt+1)*s+move+1; // Encoding move sequence uptill now
            }
            applymove((move+3)%6); // inverse move
        }
        //printf("movecount : %d ",movecount);
        //printf("last : %ld ",last);
        //printf("cur : %ld \n",cur);
        // n = Q[cur];
        // if(visited[n]==0){
        //     printf("This shouldn't be happening. A node has status 0 and is in list\n");
        //     break;
        // }
        // visited[n]=2;
        if(cur>=last){
            printf("searched all possiblities. No solution found. This shouldn't be happening. \n");
            break;
            }
        cur++;
        //ones=0;
        //twos=0;
        //for(int i=0;i<N_maxn1;i++){
        //    if(visited[i]==2){twos++;}
        //    else if(visited[i]==1){ones++;}
        //}
        //printf("ones : %ld ",ones);
        //printf("twos : %ld ",twos);
        //printf("last : %ld\n",last);
    }
    //long int ones = 0;
    //long int twos = 0;
    //for(int i=0;i<N_maxn1;i++){
    //    if(visited[i]==1){ones++;}
    //    if(visited[i]==2){twos++;}
    //}
    //printf("ones : %ld\n",ones);
    //printf("twos : %ld\n",twos);
    //printf("last : %ld\n",last);
    
    return 0;
}
