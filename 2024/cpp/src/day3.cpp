#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

// simple regex
void part1(std::string str) {
    long total = 0;

    std::regex r(R"(mul\(([0-9]{1,3}),([0-9]{1,3})\))");
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), r);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        total += stoi(match[1]) * stoi(match[2]);
    }

    std::cout << total << std::endl;
}

// regex also catches `do()`s and `don't()`s
void part2(std::string str) {
    long total = 0;
    bool enabled = true;

    std::regex r(R"(do\(\)|don't\(\)|mul\(([0-9]{1,3}),([0-9]{1,3})\))");
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), r);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str == "do()") {
            enabled = true;
        } else if (match_str == "don't()") {
            enabled = false;
        } else if (enabled) {
            total += stoi(match[1]) * stoi(match[2]);
        }
    }

    std::cout << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day3.txt", std::ios_base::in);
    std::stringstream contents;
    contents << file.rdbuf();

    part2(contents.str());

    return 0;
}
