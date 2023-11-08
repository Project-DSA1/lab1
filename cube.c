#include<stdio.h>
char C[3][8] = { //coloring
    {'B','F','F','B','F','F','B','B'},
    {'D','L','D','R','U','R','U','L'},
    {'L','D','R','D','L','U','R','U'}
};
int c[8]={0,1,2,3,4,5,6,7}; //permutation - red values in pdf (ith location pe konsa cubie number)
int x[8]={0,0,0,0,0,0,0,0}; //orientation - (follow red cubie initial locations in pdf)
int temp[8];
int Moves[3][8]={ //How permutations change
    {0,4,1,3,5,2,6,7}, //F
    {0,1,5,2,4,6,3,7}, //R
    {0,1,2,3,7,4,5,6}  //U
};
int Offsets[3][8]={ //How orientations change (+ kitna)
    {0,0,0,0,0,0,0,0}, //F
    {0,0,2,1,0,1,2,0}, //R
    {0,0,0,0,1,2,1,2}  //U
};
char MoveNames[]={'F','R','U'};
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
void PrintCode(){
    printf("x :");
    for(int i=0;i<8;i++){
        printf("%d ",x[i]);
    }
    printf("\n c :");
    for(int i=0;i<8;i++){
        printf("%d ",c[i]);
    }
    printf("\n");
}
void applymove(int i){
    int* move = Moves[i];
    int* offset = Offsets[i];
    for(int i=0;i<8;i++){
        temp[i]=c[i];
    }
    for(int i=0;i<8;i++){
        c[i] = temp[move[i]];
    }
    for(int i=0;i<8;i++){
        temp[i]=x[i];
    }
    for(int i=0;i<8;i++){
        x[i] = (temp[move[i]]+offset[i])%3;
    }
};
long int facts[8];
long int toN(){
    int N = 0;
    for(int i=2;i<8;i++){ //indexing orientations
        N = 3*N+x[i];
    }
    N = N*facts[7]; //facts[i] stores i factorial
    for(int i=0;i<8;i++){
        temp[i]=c[i];
    }
    for(int i=7;i>0;i--){
        for(int j=1;j<=i;j++){
            if(temp[j]>=temp[i]){temp[j]=temp[j]-1;}
        }
        N += (temp[i])*(facts[i-1]); //final index
    }
    return N;
}
long int Nx;
void toC(int N){
    Nx = N/facts[7]; //orientation
    N = N%facts[7]; //permutation
    int s = 0;
    for(int i=7;i>1;i--){
        x[i]=Nx%3;
        Nx=Nx/3;
        s=(s+x[i])%3;
    }
    // x[0] is always 0
    x[1]=3-s; //sum = 0
    for(int i=7;i>0;i--){
        c[i]=N/facts[i-1];
        N=N%facts[i-1];
    }
    for(int i=1;i<8;i++){
        for(int j=1;j<i;j++){
            if(c[j]>c[i]){c[j]=c[j]+1;}
        }
        c[i]=c[i]+1;
    }
}
int legal(){ //sum = 0 in orientations or permutations have repeated numbers
    for(int i=0;i<8;i++){
        temp[i]=0;
    }
    for(int i=1;i<8;i++){
        temp[c[i]]=1;
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

int visited[3674160];
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
 int xn[8] = {0,0,0,0,0,0,0,0};
 int cn[8] = {0,1,2,3,4,5,6,7};

// Testcase 2 in main() function

int sol[100];
long int s;
void printSol(int s){
    int i = 0;
    while(s>0){
        sol[i]= -1+s%4;
        i++;
        s=s/4;
    }
    i--; // As we do i++ in the end
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

    long int n0 = toN(); //initial state's index, this is always 0
    printf("%ld",n0);
    long int n;

    printf("%d\n",Q[0]);
    for(int i=0;i<8;i++){
        x[i]=xn[i];
        c[i]=cn[i];
    }
    // Testcase 2: where user does front move after the above moves
    applymove(0);
    applymove(1);
    applymove(2);
    printf("Initial state :\n\n");
    Disp();

    if(!legal()){
        printf("This isn't a legal state. That is to say; your cube cannot be solved.\n");
        return 0;
    }

    for(int i=0;i<N_maxn1;i++){
        visited[i]=0;
    }

    visited[toN()]=1;
    Q[0]=toN(); //This is a queue
    S[0]=0;

    for(int iterat=0;iterat<N_maxn1;iterat++){
        if(done){break;}
        n = Q[cur]; //current index
        s = S[cur]; //current solution (ab tak ka) as a number
        toC(n); //decoding the index, this changes x and c
        if(n==n0){
                printf("found solution : ");
                printSol(s);
                done = 1;
                break;
        }
        int movecount = 0;
        for(int move=0;move<3;move++){
            applymove(move);
            n = toN();
            if(visited[n]==0){
                visited[n]=1;
                last = last+1;
                movecount++;
                Q[last]=n;
                S[last]=4*s+move+1;
            }
            applymove(move);applymove(move);applymove(move);//any basic move 3 times is its inverse.
        }
        //printf("movecount : %d ",movecount);
        //printf("last : %ld ",last);
        //printf("cur : %ld \n",cur);
        n = Q[cur];
        if(visited[n]==0){
            printf("This shouldn't be happening. A node has status 0 and is in list\n");
            break;
        }
        visited[n]=2;
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
