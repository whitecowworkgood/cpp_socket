#include"header.h"

bool MyServer::create(const std::string& Port){
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
int MyServer::accept(){
    sockaddr_in client_addr{};
    socklen_t clientAddressLength = sizeof(client_addr);
    int clientSocketfd = ::accept(socketfd, reinterpret_cast<sockaddr*>(&client_addr), &clientAddressLength);

    if(clientSocketfd == -1){
        std::cout<<"Client Socket Connect Failed"<<std::endl;
        return -1;
    }
    return clientSocketfd;
}
void MyServer::threadService(){
    while(true){
        int clientSocketfd = accept();
        if(clientSocketfd != -1){
            clientSocketfds.push_back(clientSocketfd);
            std::thread clientThread(&MyServer::receiveFile, this, clientSocketfd);
            clientThread.detach();
        } 
    }
}
void MyServer::receiveFile(int clientSocketfd) {
    std::string file_path;
    ssize_t bytesRead = ::recv(clientSocketfd, buffer, sizeof(buffer),0);

    if(bytesRead >= 0){
        buffer[bytesRead] = '\0';
        file_path = buffer;
    }
    if(std::ifstream(file_path)){
        if(std::remove(file_path.c_str()) != 0){
            std::cout<<"Remove File Error"<<std::endl;
        }
    }
    std::ofstream file(buffer, std::ios::binary|std::ios::app);
    if(file.is_open()){
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

        std::string fileHash = calculateFileHash(file_path);
        //std::cout<<"filehash : "<<fileHash<<std::endl;

        ::send(clientSocketfd, fileHash.c_str(), fileHash.length(), 0);

        ::recv(clientSocketfd, buffer, sizeof(buffer), 0);
        //std::cout<<buffer<<std::endl;
        if(strcmp(buffer, "compare") == 0){
            std::cout<<"File Received Successed! : "<<file_path<<std::endl;
        }
        else if(strcmp(buffer, "diff") == 0){
            std::cout<<"diffrent file will remove file"<<std::endl;
            if(std::remove(file_path.c_str()) != 0){
                std::cout<<"Remove File Error"<<std::endl;
            }
        }        
    }     
    ::close(clientSocketfd);
}
std::string MyServer::calculateFileHash(const std::string& file_path){
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
void MyServer::close() {
    for(int clientSocketfd:clientSocketfds){
        ::close(clientSocketfd);
    }
    ::close(socketfd);
    socketfd = -1;
}