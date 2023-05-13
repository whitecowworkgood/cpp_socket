#include<iostream>
#include<string>
#include <sys/socket.h>  // ���� ���� �Լ��� ������ Ÿ��
#include <netinet/in.h>  // ���ͳ� �ּ� ����ü�� ���
#include <arpa/inet.h>   // IP �ּ� ��ȯ �Լ�
#include <unistd.h>      // ���� ��ũ���� ���� �Լ�
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]) {
    std::string Port;

    // Ŀ�ǵ� ���� �μ� ó��
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-p") {
            if (i + 1 < argc) {
                Port = argv[i + 1];

            } 
        }
    }

    if(Port.empty()) {
        std::cout << "Options Failed" << std::endl;
        return 1;
    }
    // ���� ����
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Failed Socket Create" << std::endl;
        return 1;
    }
    const char* portStr = Port.c_str();

    int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        // ���� ���� ����
        // ���� ó��
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    // INADDR_ANY�� ���� �ڽ��� IP�ּ�
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    const char* PortStr = Port.c_str();
    serv_addr.sin_port=htons(atoi(PortStr));

     if(bind(socketfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 ){

     }
        //error_handling("bind() error"); 
    if(listen(socketfd, 5)==-1){

    }
        //error_handling("listen() error");


    struct sockaddr_in clientAddress;

    socklen_t clnt_addr_size=sizeof(clientAddress);
    int clnt_sock=accept(socketfd, (struct sockaddr*)&socketfd,&clnt_addr_size);
    if(clnt_sock==-1){

    }
        //error_handling("accept() error"); 

    char message[]="Hello World!";
    write(clnt_sock, message, sizeof(message));
    // ����, Ŭ���̾�Ʈ ������ ������ ����

    read(clnt_sock, message, sizeof(message)-1);

    std::cout<<message<<std::endl;

    close(clnt_sock);	
    
    return 0;
}