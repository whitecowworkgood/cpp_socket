#include<iostream>
#include<string>



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
        std::cout << "�ùٸ� �ɼ��� �������� �ʾҽ��ϴ�." << std::endl;
        return 1;
    }

    
    
    return 0;
}