#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void part1(std::vector<std::vector<int>>& vec) {
    int total = 0;
    for (auto v : vec) {
        if (vec.size() <= 1) {
            // safeguard, shouldn't happen with given input
            std::cout << "invalid" << std::endl;
            continue;
        }

        bool valid = true;
        // track increasing or decreasing
        bool is_neg = v[1] - v[0] > 0 ? false : true;
        for (unsigned long i = 0; i < v.size() - 1; i++) {
            if (is_neg) {
                if (v[i + 1] - v[i] < -3 || v[i + 1] - v[i] > -1) {
                    valid = false;
                    break;
                }
            } else {
                if (v[i + 1] - v[i] > 3 || v[i + 1] - v[i] < 1) {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {
            total++;
        }
    }
    std::cout << total << std::endl;
}

unsigned long check(std::vector<int> vec) {
    // track increasing or decreasing
    bool is_neg = vec[1] - vec[0] > 0 ? false : true;
    unsigned long i;
    for (i = 0; i < vec.size() - 1; i++) {
        if (is_neg) {
            if (vec[i + 1] - vec[i] < -3 || vec[i + 1] - vec[i] > -1) {
                break;
            }
        } else {
            if (vec[i + 1] - vec[i] > 3 || vec[i + 1] - vec[i] < 1) {
                break;
            }
        }
    }

    return i;
}

void part2(std::vector<std::vector<int>>& vec) {
    int total = 0;
    for (auto v : vec) {
        unsigned long i = check(v), j;
        // success without removing anything
        if (i == v.size() - 1) {
            total++;
            continue;
        }

        // try removing the level with error, ex. 0 5 6 7 8
        auto v2 = v;
        v2.erase(v2.begin() + i);
        j = check(v2);
        if (j == v2.size() - 1) {
            total++;
            continue;
        }

        // and the next level, ex. 2 3 2 4 5
        v2 = v;
        v2.erase(v2.begin() + i + 1);
        j = check(v2);
        if (j == v2.size() - 1) {
            total++;
            continue;
        }

        // and maybe the first, ex. 3 2 3 4 5
        if (i == 1) {
            v2 = v;
            v2.erase(v2.begin());
            j = check(v2);
            if (j == v2.size() - 1) {
                total++;
                continue;
            }
        }
    }
    std::cout << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day2.txt", std::ios_base::in);

    std::string line;
    std::vector<std::vector<int>> vec;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> inner;
        int a;
        while (ss >> a) {
            inner.push_back(a);
        }
        if (inner.size() > 0) vec.push_back(inner);
    }

    part2(vec);

    return 0;
}
