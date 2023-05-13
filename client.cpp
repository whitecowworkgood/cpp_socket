#include<iostream>
#include<string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<cstdio>

int main(int argc, char* argv[]) {
    std::string ServerIp, ServerPort, File, ServerPath;

    // 커맨드 라인 인수 처리
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-i") {
            if (i + 1 < argc) {
                ServerIp = argv[i + 1];

            } 
        }
        else if(std::string(argv[i]) == "-p"){
            if (i + 1 < argc) {
                ServerPort = argv[i + 1];

            } 
        }
        else if(std::string(argv[i]) == "-f"){
            if (i + 1 < argc) {
                File = argv[i + 1];

            } 
        }
        else if(std::string(argv[i]) ==  "-d"){
            if (i + 1 < argc) {

                ServerPath = argv[i + 1];

            } 
        }
            
    }
    if(ServerIp.empty() || ServerPort.empty() || File.empty()|| ServerPath.empty()) {
            std::cout << "Options Failed." << std::endl;
            return 1;
    }
    const char* ServerIpStr = ServerIp.c_str();
    const char* ServerPortStr = ServerPort.c_str();
    const char* FileStr = File.c_str();
    const char* ServerPathStr = ServerPath.c_str();
    int sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){

    }

    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ServerIpStr);
    serv_addr.sin_port=htons(atoi(ServerPortStr));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) {

    }

    char message[30];
    int str_len;
    str_len=read(sock, message, sizeof(message)-1);
    if(str_len==-1){

    }
    write(sock, ServerPathStr, sizeof(ServerPathStr));
	
    printf("Message from server: %s \n", message);  
    close(sock);

    return 0;
}