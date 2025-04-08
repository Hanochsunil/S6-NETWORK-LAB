#include<stdio.h>

void main(){
    int n,in,bsize,leak,strg;
    printf("no.of inputs : ");
    scanf("%d",&n);
    printf("bucket size : ");
    scanf("%d",&bsize);
    printf("outgoing rate : ");
    scanf("%d",&leak);
    strg = 0;
    printf("Initial state : contains %d out of %d\n",strg,bsize);
    while(n != 0){
        printf("incoming packet size : ");
        scanf("%d",&in);
        if(in <= (bsize-strg)){
            strg = strg + in;
            printf("bucket status : contains %d out of %d\n",strg,bsize);
        }
        else{
            printf("dropped packets : %d\n",in-(bsize-strg));
            strg = bsize;
            printf("bucket status : contains %d out of %d\n",strg,bsize);
        }
        if(strg >= leak){
            strg = strg - leak;
            printf("packets send : %d\nbucket status : contains %d out of %d\n",leak,strg,bsize);
        }
        else{
            printf("packets send : %d\nbucket status : contains 0 out of %d\n",strg,bsize);
            strg = 0;
        }
        n--;
    }
    if(n == 0 && strg != 0){
        printf("remaining : %d\n",strg);
        strg = 0;
        printf("After sending,bucket status : contains %d out of %d\n",strg,bsize);
    }
}
