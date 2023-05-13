#include<iostream>
#include<string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<cstdio>

class MyClient{
    private:

        int socketfd;
        sockaddr_in serv_addr;

    public:

        MyClient() : socketfd(-1){}

        ~MyClient(){
            std::cout<<"Socket Programe Exit"<<std::endl;
        };

        bool Create_Socket(){
            socketfd =socket(AF_INET, SOCK_STREAM, 0);

            return (socketfd!=-1);

        }
        
        bool connect(const std::string& ServerIp, const std::string& ServerPort){
            sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(atoi(ServerPort.c_str()));
        if (inet_pton(AF_INET, ServerIp.c_str(), &(serverAddress.sin_addr)) <= 0) {
            std::cout << "Invalid server IP address" << std::endl;
            return false;
        }

        if (::connect(socketfd, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
            std::cout << "Failed to connect to server" << std::endl;
            return false;
        }

        return true;
        }

        ssize_t send(const std::string& data) {
            return ::send(socketfd, data.c_str(), data.length(), 0);
        }

        ssize_t receive(std::string& buffer, size_t bufferSize) {
            buffer.resize(bufferSize);
            ssize_t bytesRead = ::recv(socketfd, &buffer[0], bufferSize, 0);
            buffer.resize(bytesRead);
            return bytesRead;
        }
        void close() {
            ::close(socketfd);
            socketfd = -1;
        }
};

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

    MyClient mc;

   
    if(!mc.Create_Socket()){
        std::cout<<"Socket Create Error"<<std::endl;
        return 1;
    }
    if(!mc.connect(ServerIp, ServerPort)){
        std::cout<<"Socket Connect Error"<<std::endl;
        return 1;
    }

    std::string message = "Hello, server";
    mc.send(message);

    mc.close();

    return 0;
}