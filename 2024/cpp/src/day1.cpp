#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// sort, then compare
void part1(std::vector<int>& list1, std::vector<int>& list2) {
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    long total = 0;
    for (std::vector<int>::size_type i = 0; i < list1.size(); i++) {
        total += abs(list1[i] - list2[i]);
    }

    std::cout << total << std::endl;
}

// sort, then search in list2 for every value in list1
void part2(std::vector<int>& list1, std::vector<int>& list2) {
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    long total = 0;
    for (int i : list1) {
        // binary search for start and end of consecutive i's
        int lo =
            std::lower_bound(list2.begin(), list2.end(), i) - list2.begin();
        int hi =
            std::upper_bound(list2.begin(), list2.end(), i) - list2.begin();
        total += i * (hi - lo);
    }

    std::cout << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day1.txt", std::ios_base::in);

    std::vector<int> list1, list2;
    std::string line;
    while (getline(file, line)) {
        size_t after;
        list1.push_back(std::stoi(line, &after));
        list2.push_back(std::stoi(line.substr(after)));
    }

    part2(list1, list2);

    return 0;
}