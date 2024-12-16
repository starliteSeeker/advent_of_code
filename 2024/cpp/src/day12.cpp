#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

enum class Dir {
    kUp,
    kRight,
    kDown,
    kLeft,
};

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

// collects data for the edges and not just the total
// return {area, [side, ...]}
std::pair<int, std::set<std::tuple<Dir, size_t, size_t>>> CollectRegion2(
    std::vector<std::string>& vec, size_t row, size_t col) {
    assert(vec[row][col] >= 'A' && vec[row][col] <= 'Z');
    // mark visited spaces as lowercase
    vec[row][col] -= 'A' - 'a';

    int area = 1;
    std::set<std::tuple<Dir, size_t, size_t>> sides;

    // up
    if (row > 0) {
        if (vec[row - 1][col] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion2(vec, row - 1, col);
            area += a;
            sides.insert(p.begin(), p.end());
        } else if (vec[row - 1][col] != vec[row][col]) {
            sides.insert({Dir::kUp, row, col});
        }
    } else
        sides.insert({Dir::kUp, row, col});
    // down
    if (row < vec.size() - 1) {
        if (vec[row + 1][col] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion2(vec, row + 1, col);
            area += a;
            sides.insert(p.begin(), p.end());
        } else if (vec[row + 1][col] != vec[row][col]) {
            sides.insert({Dir::kDown, row, col});
        }
    } else
        sides.insert({Dir::kDown, row, col});
    // left
    if (col > 0) {
        if (vec[row][col - 1] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion2(vec, row, col - 1);
            area += a;
            sides.insert(p.begin(), p.end());
        } else if (vec[row][col - 1] != vec[row][col]) {
            sides.insert({Dir::kLeft, row, col});
        }
    } else
        sides.insert({Dir::kLeft, row, col});
    // right
    if (col < vec[0].size() - 1) {
        if (vec[row][col + 1] - vec[row][col] == 'A' - 'a') {
            const auto& [a, p] = CollectRegion2(vec, row, col + 1);
            area += a;
            sides.insert(p.begin(), p.end());
        } else if (vec[row][col + 1] != vec[row][col]) {
            sides.insert({Dir::kRight, row, col});
        }
    } else
        sides.insert({Dir::kRight, row, col});

    return {area, sides};
}

int CountSides(std::set<std::tuple<Dir, size_t, size_t>>& sides) {
    int side_count = 0;

    while (!sides.empty()) {
        side_count++;

        // retrieve an element
        auto head = sides.begin();
        Dir dir = std::get<0>(*head);
        size_t row = std::get<1>(*head);
        size_t col = std::get<2>(*head);
        sides.erase(head);
        std::set<std::tuple<Dir, size_t, size_t>>::iterator itt;

        // try removing all edges related to head
        switch (dir) {
            case Dir::kUp:
            case Dir::kDown:
                // wall goes right
                col++;
                while ((itt = sides.find({dir, row, col})) != sides.end()) {
                    sides.erase(itt);
                    col++;
                }
                break;
            case Dir::kLeft:
            case Dir::kRight:
                // wall goes down
                row++;
                while ((itt = sides.find({dir, row, col})) != sides.end()) {
                    sides.erase(itt);
                    row++;
                }
                break;
        }
    }

    return side_count;
}

void part2(const std::vector<std::string>& vec_copy) {
    std::vector<std::string> vec(vec_copy);
    long total = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec[0].size(); j++) {
            if (vec[i][j] < 'A' || vec[i][j] > 'Z') continue;

            auto res = CollectRegion2(vec, i, j);
            int side_count = CountSides(res.second);
            total += long(res.first) * side_count;
        }
    }
    std::cout << "part2: " << total << std::endl;
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
    part2(vec);

    return 0;
}
