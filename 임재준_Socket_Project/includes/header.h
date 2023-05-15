#ifndef MYCLASS_H
#define MYCLASS_H

#include<iostream>
#include<string>
#include <sys/socket.h>  // ���� ���� �Լ��� ������ Ÿ��
#include <netinet/in.h>  // ���ͳ� �ּ� ����ü�� ���
#include <arpa/inet.h>   // IP �ּ� ��ȯ �Լ�
#include <unistd.h>      // ���� ��ũ���� ���� �Լ�
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
#include <sys/stat.h>
#include <filesystem>



class MyClient{
    private:
        int socketfd;
        std::string File_name;
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
        std::string file_path;
        std::string file_name;
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