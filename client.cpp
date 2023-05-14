#include<iostream>
#include<string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<cstdio>
#include<fstream>
#include<functional>
#include<openssl/md5.h>
#include <iomanip>
#include<sstream>

class MyClient{
    private:

        int socketfd;
        sockaddr_in serv_addr;
        char buffer[1025]={'\0',};
        char hash[128] = {'\0'};

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

        bool sendFile(const std::string& ServerPath, const std::string& File){
            send(socketfd, (ServerPath+File).c_str(), (ServerPath+File).length(), 0);
            recv(socketfd, buffer, sizeof(buffer),0);

            if(strcmp(buffer, "ready")==0){
                //std::cout<<"recv data : "<<buffer<<std::endl;
                buffer[1025] = {'\0',};
                std::ifstream file(File, std::ios::binary);

                if(file.is_open()){
                    std::hash<std::string> hasher;
                    std::streamsize bytesRead;
                    do{
                        file.read(buffer, 1024);
                        bytesRead = file.gcount();
                        buffer[bytesRead] ='\0';
                        
                        ssize_t byteSent = ::send(socketfd, buffer, sizeof(buffer), 0);
                        
                        if(byteSent<=0){
                            std::cout<<"Failed to send data"<<std::endl;
                            file.close();
                            return false;
                        }

                        ::recv(socketfd, buffer, sizeof(buffer), 0);

                    }while(!file.eof());

                    send(socketfd, "eof", sizeof("eof"), 0);
                    file.close();

                    ::recv(socketfd, hash, sizeof(hash), 0);

                    std::string fileHash = calculateFileHash(File);
                    std::cout<<"File Hash : "<<fileHash<<std::endl;

                    std::cout<<"recv hash : "<<hash<<std::endl;


                    if(std::string(hash) == fileHash){
                        ::send(socketfd, "compare", sizeof("compare"), 0);
                        std::cout<<"File Upload Successed"<<std::endl;
                        
                    }
                    else{
                        ::send(socketfd, "diff", sizeof("diff"), 0);
                        std::cout<<"File Diffrent and will remove file at server"<<std::endl;
                        return false;
                    }
                   
                }
            }
            return true;
        }
        /*
        ssize_t receive(std::string& buffer, size_t bufferSize) {
            buffer.resize(bufferSize);
            ssize_t bytesRead = ::recv(socketfd, &buffer[0], bufferSize, 0);
            buffer.resize(bytesRead);
            return bytesRead;
        }
        */

        std::string calculateFileHash(const std::string& file_path){
        std::ifstream file(file_path, std::ios::binary);
        if(!file){
            std::cout<<"Failed open file"<<std::endl;
            return "";
        }
        MD5_CTX md5Context;
        MD5_Init(&md5Context);

        char buffer[1024];
        while(file.read(buffer, sizeof(buffer)).gcount()>0){
            MD5_Update(&md5Context, buffer, file.gcount());
        }
        unsigned char hash[16];
        MD5_Final(hash, &md5Context);

        std::stringstream hashValue;
        for(int i=0; i<16; i++){
            hashValue<<std::hex<<std::setw(2)<<std::setfill('0')<<static_cast<int>(hash[i]);
        }

        return hashValue.str();

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
                std::ifstream in(File, std::ios::binary);

                if(!in.is_open()){
                    std::cout<<"Don't have a File"<<std::endl;
                    return false;
                }
                in.close();
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

    mc.sendFile(ServerPath, File);

    mc.close();

    
    return 0;

}