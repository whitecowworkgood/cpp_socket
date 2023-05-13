#include<iostream>
#include<string>
#include <sys/socket.h>  // 소켓 관련 함수와 데이터 타입
#include <netinet/in.h>  // 인터넷 주소 구조체와 상수
#include <arpa/inet.h>   // IP 주소 변환 함수
#include <unistd.h>      // 파일 디스크립터 관련 함수
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<functional>

class MyServer{

    private:
        int socketfd;
        int clientSocketfd;

        std::string ReadyFlag = "ready";
        std::string StopFlag = "stop";

        std::string File_Path;

        char buffer[1025] = {'\0',};
        
    public:
        MyServer() : socketfd(-1), clientSocketfd(-1){}
        ~MyServer(){
            std::cout<<"Socket Programe Exit"<<std::endl;
        }

        bool create(const std::string& Port){
            socketfd = socket(AF_INET, SOCK_STREAM, 0);
            if(socketfd==-1){
                return false;
            }

            sockaddr_in serv_addr{};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port=htons(atoi(Port.c_str()));


            if (bind(socketfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
                std::cout << "Failed to bind server socket" << std::endl;
                return false;
            }

            if (listen(socketfd, 5) == -1) {
                std::cout << "Failed to listen on server socket" << std::endl;
                return false;
            }

            return true;
        }

        bool accept(){
            sockaddr_in client_addr{};
            socklen_t clientAddressLength = sizeof(client_addr);
            clientSocketfd = ::accept(socketfd, reinterpret_cast<sockaddr*>(&client_addr), &clientAddressLength);

            if(clientSocketfd == -1){
                std::cout<<"Client Socket Connect Failed"<<std::endl;
                return false;
            }
            return true;
        }

        //나중에 데이터 통신시, 수신 확인 및 다음 패킷 전송 요청용을 만들 예정
        bool receiveFile() {
            

            ssize_t bytesRead = ::recv(clientSocketfd, buffer, sizeof(buffer),0);
            if(bytesRead >= 0){
                buffer[bytesRead] = '\0';
                File_Path = buffer;
            }
            if(std::ifstream(buffer)){
                if(std::remove(buffer) != 0){
                    std::cout<<"Remove File Error"<<std::endl;
                }
                std::cout<<"Remove File"<<std::endl;
            }
            std::ofstream file(buffer, std::ios::binary|std::ios::app);
            if(file.is_open()){
                std::cout<<"file create success"<<std::endl;
                ::send(clientSocketfd, ReadyFlag.c_str(), ReadyFlag.length(), 0);
               
                while(true){

                    bytesRead = ::recv(clientSocketfd, buffer, sizeof(buffer), 0);
                    if(strcmp(buffer, "eof") == 0){
                        break;
                    }
                    file<<buffer;

                    ::send(clientSocketfd, "received", sizeof("received"), 0);

                }
                file.close();
                std::cout<<"File Received Successed! : "<<File_Path<<std::endl;
            }
            
            return true;
        }


    void close() {
        ::close(clientSocketfd);
        ::close(socketfd);
        clientSocketfd = -1;
        socketfd = -1;
    }

};

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


    MyServer ms;

    if(!ms.create(Port)){
        std::cout<<"Socket Create Error"<<std::endl;
        return -1;
    }

    if(!ms.accept()){
        std::cout<<"Socket Accept Error"<<std::endl;
        return -1;
    }

    ms.receiveFile();
    
    ms.close();
    
    return 0;
}