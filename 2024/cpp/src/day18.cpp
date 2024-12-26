#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

// constexpr int kSide = 7;
// constexpr int kFirst = 12;
constexpr int kSide = 71;
constexpr int kFirst = 1024;

using Coord = std::pair<int, int>;  // row, col

struct Hash {
    size_t operator()(const Coord& p) const {
        return (p.first << 8) | p.second;
    }
};

// A* algorithm
// https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
int h_score(int row, int col) { return kSide * 2 - 2 - row - col; }

int AStar(const std::vector<Coord>::const_iterator first,
          const std::vector<Coord>::const_iterator last) {
    // obstacles
    std::unordered_set<Coord, Hash> bytes(first, last);
    auto blocked = [&bytes](int row, int col) {
        return bytes.find({row, col}) != bytes.end();
    };

    std::vector<std::vector<int>> f_score(kSide,
                                          std::vector<int>(kSide, INT_MAX));
    std::vector<std::vector<int>> g_score(kSide,
                                          std::vector<int>(kSide, INT_MAX));
    f_score[0][0] = h_score(0, 0);
    g_score[0][0] = 0;

    // priority queue sorted by f_score
    auto comp = [&f_score](const Coord& c1, const Coord& c2) {
        return f_score[c1.first][c1.second] > f_score[c2.first][c2.second];
    };
    std::priority_queue<Coord, std::vector<Coord>, decltype(comp)> queue(comp);
    queue.push({0, 0});

    while (!queue.empty()) {
        auto [row, col] = queue.top();

        // found path
        if (row == kSide - 1 && col == kSide - 1) break;

        queue.pop();

        // go up
        int new_g = g_score[row][col] + 1;
        if (row > 0 && !blocked(row - 1, col) &&
            new_g < g_score[row - 1][col]) {
            g_score[row - 1][col] = new_g;
            f_score[row - 1][col] = new_g + h_score(row - 1, col);
            queue.push({row - 1, col});
        }
        // go down
        new_g = g_score[row][col] + 1;
        if (row < kSide - 1 && !blocked(row + 1, col) &&
            new_g < g_score[row + 1][col]) {
            g_score[row + 1][col] = new_g;
            f_score[row + 1][col] = new_g + h_score(row + 1, col);
            queue.push({row + 1, col});
        }
        // go left
        new_g = g_score[row][col] + 1;
        if (col > 0 && !blocked(row, col - 1) &&
            new_g < g_score[row][col - 1]) {
            g_score[row][col - 1] = new_g;
            f_score[row][col - 1] = new_g + h_score(row, col - 1);
            queue.push({row, col - 1});
        }
        // go right
        new_g = g_score[row][col] + 1;
        if (col < kSide - 1 && !blocked(row, col + 1) &&
            new_g < g_score[row][col + 1]) {
            g_score[row][col + 1] = new_g;
            f_score[row][col + 1] = new_g + h_score(row, col + 1);
            queue.push({row, col + 1});
        }
    }

    return g_score[kSide - 1][kSide - 1];
}

void part1(const std::vector<Coord>& bytes) {
    int result = AStar(bytes.begin(), bytes.begin() + kFirst);
    std::cout << "part1: " << result << std::endl;
}

// dumb way
void part2(const std::vector<Coord>& bytes) {
    for (auto it = bytes.begin(); it != bytes.end(); it++) {
        int res = AStar(bytes.begin(), it);
        if (res == INT_MAX) {
            std::cout << "part2: " << (it - 1)->second << ',' << (it - 1)->first
                      << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day18.txt", std::ios_base::in);

    std::vector<std::pair<int, int>> bytes;

    std::string line;
    int a, b;
    char c;  // discard comma
    while (file >> a) {
        file >> c;
        file >> b;
        bytes.push_back({b, a});
    }

    part1(bytes);
    part2(bytes);

    return 0;
}
