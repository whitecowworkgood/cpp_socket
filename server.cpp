#include<iostream>
#include<string>
#include <sys/socket.h>  // 소켓 관련 함수와 데이터 타입
#include <netinet/in.h>  // 인터넷 주소 구조체와 상수
#include <arpa/inet.h>   // IP 주소 변환 함수
#include <unistd.h>      // 파일 디스크립터 관련 함수
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]) {
    std::string Port;

    // 커맨드 라인 인수 처리
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
    // 소켓 생성
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Failed Socket Create" << std::endl;
        return 1;
    }
    const char* portStr = Port.c_str();

    int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        // 소켓 생성 실패
        // 오류 처리
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    // INADDR_ANY는 서버 자신의 IP주소
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
    // 서버, 클라이언트 소켓의 연결을 해제

    read(clnt_sock, message, sizeof(message)-1);

    std::cout<<message<<std::endl;

    close(clnt_sock);	
    
    return 0;
}