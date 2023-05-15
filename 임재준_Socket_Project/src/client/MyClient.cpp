#include"header.h"

bool MyClient::Create_Socket(){
    socketfd =socket(AF_INET, SOCK_STREAM, 0);
    return (socketfd!=-1);
}

bool MyClient::connect(const std::string& ServerIp, const std::string& ServerPort){
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
bool MyClient::sendFile(const std::string& ServerPath, const std::string& File){
    std::filesystem::path p(File);


    if(std::filesystem::exists(p)){
        if(File.find("\\") != std::string::npos){
            //std::cout<<"in \\"<<std::endl;
            int find = File.rfind("\\")+1;
            File_name = File.substr(find, File.length()-find);
            //std::cout<<File_name<<std::endl;
        }
        else if(File.find("/") != std::string::npos){
            //std::cout<<"in /"<<std::endl;
            int find = File.rfind("/")+1;
            File_name = File.substr(find, File.length()-find);
            //std::cout<<File_name<<std::endl;
        }
        else{
            File_name = File;
        }
        send(socketfd, (ServerPath).c_str(), (ServerPath).length(), 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        send(socketfd, (File_name).c_str(), (File_name).length(), 0);
        recv(socketfd, buffer, sizeof(buffer),0);

        if(strcmp(buffer, "ready")==0){
            std::ifstream file(File, std::ios::binary);
            //

            std::hash<std::string> hasher;
            std::streamsize bytesRead;
            do{
                buffer[1025] = {'\0',};
                file.read(buffer, 1024);
                bytesRead = file.gcount();
                buffer[bytesRead] ='\0';
                        
                ssize_t byteSent = ::send(socketfd, buffer, bytesRead, 0);
                        
                if(byteSent<=0){
                    std::cout<<"Failed to send data"<<std::endl;
                    file.close();
                    return false;
                }  

                ::recv(socketfd, buffer, sizeof(buffer), 0);

            }while(!file.eof());

            send(socketfd, "send_end", sizeof("send_end"), 0);
            file.close();

            ::recv(socketfd, hash, sizeof(hash), 0);

            std::string fileHash = calculateFileHash(File);
            //std::cout<<"File Hash : "<<fileHash<<std::endl;

            // std::cout<<"recv hash : "<<hash<<std::endl;

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
        else{
            close();
        }
    }
    else{
        //소켓 종료 관련 코드 추가 예정
        std::cout<<"Socket Close"<<std::endl;
        close();
    }
    return true;
}

std::string MyClient::calculateFileHash(const std::string& file_path){
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

void MyClient::close(){
    ::close(socketfd);
    socketfd = -1;
}