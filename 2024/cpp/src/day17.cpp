#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

// just following the instructions
std::vector<unsigned int> Run(const std::vector<unsigned int>& prog,
                              unsigned long ra, unsigned long rb,
                              unsigned long rc) {
    std::vector<unsigned int> output;

    unsigned int ip = 0;
    while (ip < prog.size() - 1) {
        unsigned long literal = prog[ip + 1], combo;
        if (prog[ip + 1] == 4)
            combo = ra;
        else if (prog[ip + 1] == 5)
            combo = rb;
        else if (prog[ip + 1] == 6)
            combo = rc;
        else
            combo = prog[ip + 1];

        switch (prog[ip]) {
            case 0:
                ra = ra / (1 << combo);
                break;
            case 1:
                rb = rb ^ literal;
                break;
            case 2:
                rb = combo & 0b111;
                break;
            case 3:
                if (ra != 0)
                    ip = literal - 2;  // minus 2 to be added back later
                break;
            case 4:
                rb = rb ^ rc;
                break;
            case 5:
                output.push_back(combo & 0b111);
                break;
            case 6:
                rb = ra / (1 << combo);
                break;
            case 7:
                rc = ra / (1 << combo);
                break;
            default:
                ip = -3;  // force halt
                break;
        }

        ip += 2;
    }

    return output;
}

void part1(const std::vector<unsigned int>& prog, unsigned int ra,
           unsigned int rb, unsigned int rc) {
    auto res = Run(prog, ra, rb, rc);

    std::cout << "part1: ";
    for (auto i : res) {
        std::cout << i << ',';
    }
    std::cout << std::endl;
}

// ra gets divided by 8 (shifted right by 3) every program loop, rebuild ra in
// 3-bit blocks
std::optional<unsigned long> Try(const std::vector<unsigned int>& prog,
                                 unsigned long ra = 0, int depth = 0) {
    if ((unsigned int)depth == prog.size()) return ra;

    for (int i = 0; i < 8; i++) {
        unsigned long new_ra = (ra << 3) | i;
        auto maybe = Run(prog, new_ra, 0, 0);

        // check if result matches original program
        bool same = true;
        for (int j = 0; j <= depth; j++) {
            if (maybe[j] != prog[prog.size() - depth - 1 + j]) {
                same = false;
                break;
            }
        }
        if (!same) continue;

        auto result = Try(prog, new_ra, depth + 1);
        if (result.has_value()) return result;
    }

    return std::nullopt;
}

// looping through all possible ra failed
// part 2 requires looking at the input and figuring out the general structure
// of the program
void part2(const std::vector<unsigned int>& prog) {
    auto res = Try(prog);
    std::cout << "part2: " << (res.has_value() ? *res : -1) << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day17.txt", std::ios_base::in);
    std::string line;

    // read register values
    unsigned int ra, rb, rc;
    getline(file, line);
    ra = std::stoi(line.substr(12));
    getline(file, line);
    rb = std::stoi(line.substr(12));
    getline(file, line);
    rc = std::stoi(line.substr(12));

    getline(file, line);

    // read program
    getline(file, line);
    std::istringstream ss(line);
    std::vector<unsigned int> prog;
    int c;
    ss.ignore(9);  // ignore "Program: "
    while (ss >> c) {
        prog.push_back(c);
        ss.ignore();  // ignore comma
    }

    part1(prog, ra, rb, rc);
    part2(prog);

    return 0;
}
