#ifndef MYCLASS_H
#define MYCLASS_H

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
#include<vector>
#include<thread>
#include<openssl/md5.h>
#include <iomanip>
#include<sstream>
#include<cstdio>

class MyClient{
    private:
        int socketfd;
        sockaddr_in serv_addr;
        char buffer[1025]={'\0',};
        char hash[128] = {'\0'};

    public:

        MyClient() : socketfd(-1){}
        ~MyClient(){std::cout<<"Socket Programe Exit"<<std::endl;}

        bool Create_Socket();
        bool connect(const std::string& ServerIp, const std::string& ServerPort);
        bool sendFile(const std::string& ServerPath, const std::string& File);
        std::string calculateFileHash(const std::string& file_path);
        void close();
};

class MyServer{

    private:
        int socketfd;
        std::vector<int> clientSocketfds;
        std::string ReadyFlag = "ready";
        char buffer[1025] = {'\0',};
        
    public:
        MyServer() : socketfd(-1){}
        ~MyServer(){std::cout<<"Socket Programe Exit"<<std::endl;}

        bool create(const std::string& Port);
        int accept();
        void threadService();
        void receiveFile(int clientSocketfd);
        std::string calculateFileHash(const std::string& file_path);
        void close();
};

#endif