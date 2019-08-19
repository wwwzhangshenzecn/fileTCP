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


int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;

    if(argc != 3){
        printf("Please usage\n %s <IP> <PORT> \n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr* )&serv_addr, sizeof(serv_addr))==-1){
        printf("connect");
        exit(1);
    }
    printf("connection is successful... \n");
    string filename ;
    cout<<"Please input the file's name : ";
    cin>>filename;
    printf("send the file's name: %s .\n", filename.c_str());
    const char * send_message = filename.c_str();
    write(sock, send_message, sizeof(send_message));

    filename = "GET/"+filename;
    printf("Start get the file... \n");
    ofstream foutput;
    foutput.open(filename.c_str());

    int read_len = 0;
    int read_count=0;
    char message[BUFFER_SIZE];
    while((read_len= read(sock, message, BUFFER_SIZE-1))>0 && strlen(message)>0){
        foutput << message;
        read_count+=read_len;
        printf("Has gotten size: %d \n", read_count);
    }

    printf("Has gotten size: %d \n", read_count);
    printf("file is accepted. \n");

    foutput.close();
    close(sock);
    return 0;
}