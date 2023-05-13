#include<iostream>
#include<string>
#include <sys/socket.h>  // ���� ���� �Լ��� ������ Ÿ��
#include <netinet/in.h>  // ���ͳ� �ּ� ����ü�� ���
#include <arpa/inet.h>   // IP �ּ� ��ȯ �Լ�
#include <unistd.h>      // ���� ��ũ���� ���� �Լ�
#include<stdlib.h>
#include<string.h>

class MyServer{

    private:
        int socketfd;
        int clientSocketfd;
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

        ssize_t send(const std::string& data) {
        return ::send(clientSocketfd, data.c_str(), data.length(), 0);
    }

    ssize_t receive(std::string& buffer, size_t bufferSize) {
        buffer.resize(bufferSize);
        ssize_t bytesRead = ::recv(clientSocketfd, &buffer[0], bufferSize, 0);
        buffer.resize(bytesRead);
        return bytesRead;
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


    MyServer ms;

    if(!ms.create(Port)){
        std::cout<<"Socket Create Error"<<std::endl;
        return -1;
    }

    if(!ms.accept()){
        std::cout<<"Socket Accept Error"<<std::endl;
        return -1;
    }

    std::string recv_data;
    ms.receive(recv_data, 1024);
    std::cout<<recv_data<<std::endl;

    ms.close();
    
    return 0;
}