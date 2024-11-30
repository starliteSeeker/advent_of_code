#include <cstdio>
#include <fstream>
#include <iostream>

int useless() { return 0; }

int main(int argc, char *argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day0.txt", std::ios_base::in);

    std::string line;
    while (getline(file, line)) {
        std::cout << line << std::endl;
    }

    std::cout << "123" << std::endl;
    return 0;
}
