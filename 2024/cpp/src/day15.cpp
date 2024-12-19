#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

void part1(std::vector<std::string>& grid, std::string& dirs, int y, int x) {
    int i;
    std::optional<std::pair<int, int>> pushed;  // {y, x}
    for (auto d : dirs) {
        pushed = std::nullopt;
        switch (d) {
            case '^':
                for (i = y - 1; i >= 0; i--) {
                    if (grid[i][x] == '#')
                        break;
                    else if (grid[i][x] == 'O') {
                        if (!pushed) pushed = {i, x};  // record first box
                        continue;
                    } else {
                        // empty space
                        if (pushed) {
                            // move box
                            grid[pushed->first][pushed->second] = '.';
                            grid[i][x] = 'O';
                        }

                        // move robot
                        y--;
                        break;
                    }
                }
                break;
            case 'v':
                for (i = y + 1; (unsigned int)i < grid.size(); i++) {
                    if (grid[i][x] == '#')
                        break;
                    else if (grid[i][x] == 'O') {
                        if (!pushed) pushed = {i, x};  // record first box
                        continue;
                    } else {
                        // empty space
                        if (pushed) {
                            // move box
                            grid[pushed->first][pushed->second] = '.';
                            grid[i][x] = 'O';
                        }

                        // move robot
                        y++;
                        break;
                    }
                }
                break;
            case '<':
                for (i = x - 1; i >= 0; i--) {
                    if (grid[y][i] == '#')
                        break;
                    else if (grid[y][i] == 'O') {
                        if (!pushed) pushed = {y, i};  // record first box
                        continue;
                    } else {
                        // empty space
                        if (pushed) {
                            // move box
                            grid[pushed->first][pushed->second] = '.';
                            grid[y][i] = 'O';
                        }

                        // move robot
                        x--;
                        break;
                    }
                }
                break;
            case '>':
                for (i = x + 1; (unsigned int)i < grid[0].size(); i++) {
                    if (grid[y][i] == '#')
                        break;
                    else if (grid[y][i] == 'O') {
                        if (!pushed) pushed = {y, i};  // record first box
                        continue;
                    } else {
                        // empty space
                        if (pushed) {
                            // move box
                            grid[pushed->first][pushed->second] = '.';
                            grid[y][i] = 'O';
                        }

                        // move robot
                        x++;
                        break;
                    }
                }
                break;
        }
    }

    int total = 0;
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] != 'O') continue;

            total += 100 * i + j;
        }
    }

    std::cout << "part1: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day15.txt", std::ios_base::in);

    std::string line;
    // read grid
    std::vector<std::string> grid;
    int i = 0, y = 0, x = 0;
    while (getline(file, line)) {
        if (line.empty()) break;

        // find starting position
        auto it = std::find(line.begin(), line.end(), '@');
        if (it != line.end()) {
            *it = '.';
            y = i;
            x = it - line.begin();
        }

        grid.push_back(line);
        i++;
    }
    // read directions
    std::string dirs;
    while (getline(file, line)) {
        dirs += line;
    }

    part1(grid, dirs, y, x);

    return 0;
}
