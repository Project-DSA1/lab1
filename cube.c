#include<stdio.h>
char C[3][8] = {
    {'B','F','F','B','F','F','B','B'},
    {'D','L','D','R','U','R','U','L'},
    {'L','D','R','D','L','U','R','U'}
};
int c[8]={0,1,2,3,4,5,6,7};
int x[8]={0,0,0,0,0,0,0,0};
int temp[8];
int Moves[3][8]={
    {0,4,1,3,5,2,6,7}, //F
    {0,1,5,2,4,6,3,7}, //R
    {0,1,2,3,7,4,5,6}  //U
};
int Offsets[3][8]={
    {0,0,0,0,0,0,0,0}, //F
    {0,0,2,1,0,1,2,0}, //R
    {0,0,0,0,1,2,1,2}  //U
};
char MoveNames[]={'F','R','U'};
int Faces[3][2][2]={
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
                int loc = Faces[k][i][j];
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
    for(int i=2;i<8;i++){
        N = 3*N+x[i];
    }
    N = N*facts[7];
    for(int i=0;i<8;i++){
        temp[i]=c[i];
    }
    for(int i=7;i>0;i--){
        for(int j=1;j<=i;j++){
            if(temp[j]>=temp[i]){temp[j]=temp[j]-1;}
        }
        N += (temp[i])*(facts[i-1]);
    }
    return N;
}
long int Nx;
void toC(int N){
    Nx = N/facts[7];
    N = N%facts[7];
    int s = 0;
    for(int i=7;i>1;i--){
        x[i]=Nx%3;
        Nx=Nx/3;
        s=(s+x[i])%3;
    }
    x[1]=3-s;
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
int legal(){
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
int n0 = toN();
int visited[3674160];
int Q[3674160];
long int S[3674160];
long int cur = 0;
long int last = 0;
long int N_maxn1=3674160;
long int n=toN();
//initial state
int xn[8]={0,1,2,1,2,1,2,1};
int cn[8]={0,1,3,4,2,6,7,5};
int sol[100];
long int s;
void printSol(int s){
    int i = 0;
    while(s>0){
        sol[i]= -1+s%4;
        i++;
        s=s/4;
    }
    for(int j=i;j>=0;j--){
        printf("%c",MoveNames[sol[j]]);
    }
    printf("\n");
}
int done = 0;
int main(){
    for(int i=0;i<8;i++){
        x[i]=xn[i];
        c[i]=cn[i];
    }
    printf("Initial state :\n\n");
    Disp();
    if(!legal()){
        printf("This isn't a legal state. That is to say; your cube cannot be solved.\n");
        return 0;
    }
    facts[0]=1;
    for(int i=1;i<8;i++){
        facts[i]=facts[i-1]*i;
    }
    for(int i=0;i<N_maxn1;i++){
        visited[i]=0;
    }
    visited[toN()]=1;
    Q[0]=toN();
    S[0]=0;
    for(int iterat=0;iterat<N_maxn1;iterat++){
        if(done){break;}
        n = Q[cur];
        s = S[cur];
        toC(n);
        for(int move=0;move<3;move++){
            applymove(move);
            n = toN();
            if(n==n0){
                printf("found solution : ");
                printSol(s);
                done = 1;
                break;
            }
            if(visited[n]==0){
                last++;
                Q[last]=n;
                S[last]=4*s+move+1;
                visited[n]=1;
            }
            applymove(move);applymove(move);applymove(move);//any basic move 3 times is its inverse.

        }
        visited[cur]=2;
        if(cur==last){
            printf("searched all possiblities. No solution found. This shouldn't be happening. \n");
            break;
            }
        cur++;
    }
    
    return 0;
}