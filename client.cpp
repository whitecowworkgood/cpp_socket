#include<iostream>
#include<string>

int main(int argc, char* argv[]) {
    std::string ServerIp, ServerPort, File, ServerPath;

    // Ŀ�ǵ� ���� �μ� ó��
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
            std::cout << "�ùٸ� �ɼ��� �������� �ʾҽ��ϴ�." << std::endl;
            return 1;
    }



    return 0;
}