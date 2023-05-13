#include<iostream>
#include<string>



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
        std::cout << "올바른 옵션이 제공되지 않았습니다." << std::endl;
        return 1;
    }

    
    
    return 0;
}