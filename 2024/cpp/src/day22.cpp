#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

auto GenerateNext = [](long curr) {
    long res = (curr ^ (curr << 6)) & 0x00ffffff;
    res = (res ^ (res >> 5)) & 0x00ffffff;
    res = (res ^ (res << 11)) & 0x00ffffff;
    return res;
};

void part1(const std::vector<long>& vec) {
    long total = 0;

    for (long v : vec) {
        for (int i = 0; i < 2000; ++i) {
            v = GenerateNext(v);
        }
        total += v;
    }

    std::cout << "part1: " << total << std::endl;
}

void part2(const std::vector<long>& vec) {
    std::map<std::array<int, 4>, int> tally;

    for (long v : vec) {
        // generate list of prices
        std::vector<long> randoms = {v};
        for (int i = 0; i < 2000; ++i) {
            v = GenerateNext(v);
            randoms.push_back(v);
        }
        for (long& r : randoms) {
            r %= 10;
        }

        // tally
        std::map<std::array<int, 4>, int> curr_tally;
        std::array<int, 4> diff;
        diff[1] = randoms[1] - randoms[0];
        diff[2] = randoms[2] - randoms[1];
        diff[3] = randoms[3] - randoms[2];
        for (unsigned int i = 4; i < randoms.size(); i++) {
            // update diff array
            diff[0] = diff[1];
            diff[1] = diff[2];
            diff[2] = diff[3];
            diff[3] = randoms[i] - randoms[i - 1];

            // add to current tally
            if (curr_tally.find(diff) == curr_tally.end())
                curr_tally[diff] = randoms[i];
        }

        // add to main tally;
        for (auto& [key, value] : curr_tally) {
            tally[key] += value;
        }
    }

    // find max;
    int max_val = -1;
    for (auto& [_, value] : tally) {
        if (value > max_val) max_val = value;
    }

    std::cout << "part2: " << max_val << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day22_0.txt", std::ios_base::in);
    std::vector<long> vec;
    long a;
    while (file >> a) vec.push_back(a);

    part1(vec);
    part2(vec);

    return 0;
}
