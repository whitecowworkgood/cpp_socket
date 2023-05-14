#include"header.h"
#include<iostream>

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