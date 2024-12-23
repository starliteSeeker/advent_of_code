#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

enum class Dir {
    kUp,
    kRight,
    kDown,
    kLeft,
};

class Reindeer {
   public:
    int row, col;
    Dir dir;

    Reindeer(int row, int col, Dir dir = Dir::kRight)
        : row(row), col(col), dir(dir) {}
};

bool operator==(const Reindeer& lhs, const Reindeer& rhs) {
    return lhs.row == rhs.row && lhs.col == rhs.col && lhs.dir == rhs.dir;
}
bool operator<(const Reindeer& lhs, const Reindeer& rhs) {
    return std::tie(lhs.row, lhs.col, lhs.dir) <
           std::tie(rhs.row, rhs.col, rhs.dir);
}
bool operator>(const Reindeer& lhs, const Reindeer& rhs) { return rhs < lhs; }

// BFS
// track all four directions
std::vector<std::vector<std::array<int, 4>>> Run(
    const std::vector<std::string>& grid) {
    std::vector<std::vector<std::array<int, 4>>> visited(
        grid.size(), std::vector<std::array<int, 4>>(
                         grid[0].size(), {INT_MAX, INT_MAX, INT_MAX, INT_MAX}));

    using data = std::pair<int, Reindeer>;
    std::priority_queue<data, std::vector<data>, std::greater<data>> checks;
    checks.push({0, Reindeer(grid.size() - 2, 1)});

    while (!checks.empty()) {
        // input is surround by walls, no need for boundary check

        const auto check = checks.top();
        checks.pop();
        auto& pos = check.second;

        // visited check
        if (visited[pos.row][pos.col][int(pos.dir)] < check.first) {
            continue;
        }
        visited[pos.row][pos.col][int(pos.dir)] = check.first;

        // maybe turn, and take one step
        if (grid[pos.row - 1][pos.col] != '#') {
            Reindeer new_check(pos.row - 1, pos.col, Dir::kUp);
            int add_score = pos.dir == Dir::kUp ? 1 : 1001;
            checks.push({check.first + add_score, new_check});
        }
        if (grid[pos.row + 1][pos.col] != '#') {
            Reindeer new_check(pos.row + 1, pos.col, Dir::kDown);
            int add_score = pos.dir == Dir::kDown ? 1 : 1001;
            checks.push({check.first + add_score, new_check});
        }
        if (grid[pos.row][pos.col - 1] != '#') {
            Reindeer new_check(pos.row, pos.col - 1, Dir::kLeft);
            int add_score = pos.dir == Dir::kLeft ? 1 : 1001;
            checks.push({check.first + add_score, new_check});
        }
        if (grid[pos.row][pos.col + 1] != '#') {
            Reindeer new_check(pos.row, pos.col + 1, Dir::kRight);
            int add_score = pos.dir == Dir::kRight ? 1 : 1001;
            checks.push({check.first + add_score, new_check});
        }
    }

    return visited;
}

void part1(const std::vector<std::string>& grid) {
    auto result = Run(grid);

    // top right corner
    auto end = result[1][grid[0].size() - 2];
    std::cout << "part1: " << *std::min_element(end.begin(), end.end())
              << std::endl;
}

void part2(const std::vector<std::string>& grid) {
    auto result = Run(grid);

    // walk back from end to start
    std::set<std::pair<int, int>> visited;

    // std::cout << "part2: " << result[1][grid[0].size() - 2] << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day16.txt", std::ios_base::in);

    std::vector<std::string> grid;
    std::string line;
    while (getline(file, line)) {
        grid.push_back(line);
    }

    part1(grid);
    part2(grid);

    return 0;
}
