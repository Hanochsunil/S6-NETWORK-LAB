#include<stdio.h>

int main(){
    int leakrate,bsize,n,bucket_storage=0,in;
    printf("Enter the bucket size");
    scanf("%d",&bsize);
    printf("Enter the no of packets");
    scanf("%d",&n);
    printf("Enter the leakrate");
    scanf("%d",&leakrate);
    print("Intial bucket status contains %d out of %d",bucket_storage,bsize);

    while(n!=0){
        printf("Incoming packet size");
        scanf("%d",&in);

        if(in<=bsize-bucket_storage){
            bucket_storage=in;
            print("Intial bucket status contains %d out of %d",bucket_storage,bsize);

        }else{
            print("Dropped packet %d",in-(bsize-bucket_storage));
            bucket_storage=bsize;
        }
        bucket_storage-=leakrate;

        if(bucket_storage<0){
            print("%d transmitted ,bucket status contains %d out of %d",leakrate,bucket_storage,bsize);
            bucket_storage=0;
        }
        n--;
    }

    if(bucket_storage!=0){
        bucket_storage=0;
        print("After sending bucket status contains %d out of %d",bucket_storage,bsize);
    }

}