#include <iostream>
#include <cstdlib>
#include <fstream>
#include<windows.h>

int main(int argc, char **argv)
{
    std::string instruction;
    std::string tmp;
    int i = 1;
    while(i < argc)
    {
        instruction = argv[i];
        std::cout << instruction << std::endl;
        if(instruction ==  "supprimer")
        {
            i++;
            remove(argv[i]);
            std::cout << argv[i] << std::endl;
        }
        else if(instruction == "supprimer_dossier")
        {
            i++;
            tmp = argv[i];
            tmp = "rmdir " + tmp;
            system(tmp.c_str());
            std::cout << argv[i] << std::endl;
        }
        else if(instruction == "deplacer")
        {
            i++;
            MoveFile(argv[i], argv[i+1]);
            i++;
            std::cout << argv[i-1] << std::endl;
            std::cout << argv[i] << std::endl;
        }
        else if(instruction == "Patcher")
        {
            i++;
            tmp = argv[i];
            instruction = tmp;
            instruction.erase(instruction.end()-4, instruction.end());
            MoveFile(tmp.c_str(), instruction.c_str());
            std::cout << argv[i] << std::endl;
        }
        i++;
    }
    if(i != argc) return 1;
    //system("pause");
    return 0;
}
