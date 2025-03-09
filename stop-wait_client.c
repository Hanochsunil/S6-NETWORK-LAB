#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<time.h>

typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
        int frame_kind;
        int sq;
        int ack;
        Packet packet;
}Frame;

int main(int argc ,char* argv[]){

    if(argc!=2){
        printf("usage of %s <port>",argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char buffer[1024];
    Frame frame_recv,frame_send;
    int rec_ack=1;
    int frame_id=0;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    memset(&serverAddr,'\0',sizeof(serverAddr));

    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    while(1){
        if(rec_ack==1){
            frame_send.sq=frame_id;
            frame_send.frame_kind=1;
            frame_send.ack=0;

            printf("Enter the data\n");
            scanf("%s",frame_send.packet.data);

            sendto(sockfd,&frame_send,sizeof(Frame),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
            printf("[+]Frame send\n");
        }
        addr_size=sizeof(serverAddr);

        int recv_size=recvfrom(sockfd,&frame_recv,sizeof(Frame),0,(struct sockaddr*)&serverAddr,&addr_size);

        if(recv_size>0 && frame_recv.sq==0 && frame_recv.ack==frame_id+1){
            printf("[+]Ack Recieved\n");
            rec_ack=1;
        }else{
            printf("[+]Ack not recieved\n");
            rec_ack=0;
        }

        frame_id++;

    }
    close(sockfd);

    return 0;
}
