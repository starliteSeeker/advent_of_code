#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

// return {area, perimeter}
std::pair<int, int> CollectRegion(std::vector<std::string>& vec, size_t row,
                                  size_t col) {
    assert(vec[row][col] >= 'A' && vec[row][col] <= 'Z');
    // mark visited spaces as lowercase
    vec[row][col] -= 'A' - 'a';

    int area = 1, perimeter = 0;

    // up
    if (row > 0) {
        if (vec[row - 1][col] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion(vec, row - 1, col);
            area += a;
            perimeter += p;
        } else if (vec[row - 1][col] != vec[row][col]) {
            perimeter++;
        }
    } else
        perimeter++;
    // down
    if (row < vec.size() - 1) {
        if (vec[row + 1][col] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion(vec, row + 1, col);
            area += a;
            perimeter += p;
        } else if (vec[row + 1][col] != vec[row][col]) {
            perimeter++;
        }
    } else
        perimeter++;
    // left
    if (col > 0) {
        if (vec[row][col - 1] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion(vec, row, col - 1);
            area += a;
            perimeter += p;
        } else if (vec[row][col - 1] != vec[row][col]) {
            perimeter++;
        }
    } else
        perimeter++;
    // right
    if (col < vec[0].size() - 1) {
        if (vec[row][col + 1] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion(vec, row, col + 1);
            area += a;
            perimeter += p;
        } else if (vec[row][col + 1] != vec[row][col]) {
            perimeter++;
        }
    } else
        perimeter++;

    return {area, perimeter};
}

void part1(const std::vector<std::string>& vec_copy) {
    std::vector<std::string> vec(vec_copy);
    long total = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec[0].size(); j++) {
            if (vec[i][j] < 'A' || vec[i][j] > 'Z') continue;

            const auto& [a, p] = CollectRegion(vec, i, j);
            total += long(a) * p;
        }
    }
    std::cout << "part1: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day12.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> vec;

    while (getline(file, line)) {
        vec.push_back(line);
    }

    part1(vec);

    return 0;
}
