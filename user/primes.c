#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

int read_left_first(int lpipe[2], int *dst){
    if(read(lpipe[RD], &dst, INT_LEN)==INT_LEN){
        printf("prime %d\n",dst);
        return 1;
    }
    return 0;
}

void transmit(int lpipe[2], int rpipe[2], int first){
    int tmp;
    while(read(lpipe[RD],&tmp,INT_LEN)==INT_LEN){
        if(tmp%first!=0){
          write(rpipe[WR],&tmp,INT_LEN);
        }
    }
    close(lpipe[RD]);
    close(rpipe[WR]);
}

void prime(int lpipe[2]){
    close(lpipe[WR]);
    int first;
    
    if(read_left_first(lpipe, &first)==1){
        int rpipe[2];
        pipe(rpipe);
        transmit(lpipe,rpipe, first);

        if(fork()==0){
            prime(rpipe);
        }else{
            close(rpipe[RD]);
            wait(0);
        }
    }
    exit(0);
    
}

int main(){
    int p[2];
    pipe(p);
    for(int i=2;i<=35;i++){
        write(p[WR],&i,INT_LEN);    
    }
    if(fork()==0){
        prime(p);
    }else{
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);

}