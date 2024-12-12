#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Equation {
   public:
    long lhs;
    std::vector<long> rhs;

    Equation(long lhs, std::vector<long> rhs) : lhs(lhs), rhs(rhs){};

    bool Check1() {
        // iterate through all possibility, represented as binary numbers
        for (int i = 0; i < 1 << (rhs.size() - 1); i++) {
            long curr = rhs[0];
            int ops = i;
            for (size_t j = 1; j < rhs.size(); j++) {
                if (ops & 0b1) {
                    // multiply
                    curr *= rhs[j];
                } else {
                    // add
                    curr += rhs[j];
                }
                ops >>= 1;
            }

            if (curr == lhs) return true;
        }
        return false;
    }

    bool Check2() {
        // iterate through all possibility, represented as binary numbers
        for (int i = 0; i < pow(3, (rhs.size() - 1)); i++) {
            long curr = rhs[0];
            int ops = i;
            for (size_t j = 1; j < rhs.size(); j++) {
                switch (ops % 3) {
                    case 2:
                        // concat
                        curr =
                            stol(std::to_string(curr) + std::to_string(rhs[j]));
                        break;
                    case 1:
                        // multiply
                        curr *= rhs[j];
                        break;
                    case 0:
                        // add
                        curr += rhs[j];
                        break;
                }
                ops /= 3;
            }

            if (curr == lhs) return true;
        }
        return false;
    }
};

void part1(const std::vector<Equation>& input) {
    long total = 0;

    for (auto e : input) {
        if (e.Check1()) total += e.lhs;
    }

    std::cout << "part1 " << total << std::endl;
}

void part2(const std::vector<Equation>& input) {
    long total = 0;

    for (auto e : input) {
        if (e.Check2()) total += e.lhs;
    }

    std::cout << "part2 " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day7.txt", std::ios_base::in);
    std::vector<Equation> input;

    std::string line;
    while (getline(file, line)) {
        std::stringstream contents(line);

        // get target
        long lhs;
        contents >> lhs;

        // skip colon
        char c;
        contents >> c;

        // get list of numbers
        std::vector<long> vec;
        long rhs;
        while (contents >> rhs) {
            vec.push_back(rhs);
        }
        input.push_back(Equation(lhs, vec));
    }

    part1(input);
    part2(input);
}
