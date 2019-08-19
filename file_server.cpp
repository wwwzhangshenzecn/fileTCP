#include<iostream>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define BUFFER_SIZE 1024

using namespace  std;


int main(int argc, char *argv[]){

    int serv_sock, clnt_sock;

    struct  sockaddr_in serv_addr;
    struct  sockaddr_in clnt_addr;
    

    if(argc != 2){
        printf("Please use :%s <port>", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1){
        printf("socket error");
        exit(1);
    }
    printf("create serv_sock");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr  =htonl(INADDR_ANY);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port =htons(atoi(argv[1]));
    printf("finish memset. \n");
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1)
    {
        printf("bind");
        exit(1);
    }
    printf("bind over...\n");
    if(listen(serv_sock, 5) == -1){
        printf("listen");
    }
    printf("start listening... \n");
    socklen_t serv_addr_size, clnt_addr_size;
    clnt_addr_size=sizeof(clnt_addr);
    for(int i=0;i<5;i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        if(clnt_sock == -1)
        {
            printf("accept.");
        exit(1);
        }
        
        char * clnt_addr_char = inet_ntoa(clnt_addr.sin_addr);
        printf("#### : connect from %s . \n", clnt_addr_char);

        char message[BUFFER_SIZE];
        int str_len;
        string filename="";
        str_len = read(clnt_sock, message, BUFFER_SIZE-1);
        if (str_len <= 0){
            cout<<"get the file's name failed..."<<endl;
            exit(1);
        }
        cout<<message<<endl;
        message[str_len]=0;
        filename = message;

        cout<<"Accept the requestion: send the file "<<filename.c_str()<<endl;
        string content="";
        ifstream finput;
        finput.open(filename.c_str());
        if(!finput.is_open()){
            cout<<"Can't open the file: "<<filename.c_str()<<endl;
        exit(1);
        
        }

        finput.seekg(0, std::ios::end);
        int length = finput.tellg();

        printf("File size: %d \n", length);

        finput.seekg(0, std::ios::beg);
        char send_msg[length];
        finput.read(send_msg, length);
        write(clnt_sock, send_msg, sizeof(send_msg));

        printf("Send over...\n");
        finput.close();
        close(clnt_sock);
    }




}