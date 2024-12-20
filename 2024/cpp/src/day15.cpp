#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <vector>

void part1(const std::vector<std::string>& grid_copy, std::string& dirs, int y,
           int x) {
    std::vector<std::string> grid(grid_copy);

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

// overall bad logic on part 2 :)
// strong reliance on having walls surrounding the input

// move up and pushing boxes in the way
// only use this function if it's possible to push up
void PushUp(std::vector<std::string>& grid, int y, int x) {
    if (y <= 0 || x < 0 || (unsigned int)x > grid[0].size())
        return;  // out of range
    switch (grid[y - 1][x]) {
        case '.':
            // nothing to push
            break;
        case '[':
            PushUp(grid, y - 1, x);
            PushUp(grid, y - 1, x + 1);
            break;
        case ']':
            PushUp(grid, y - 1, x);
            PushUp(grid, y - 1, x - 1);
            break;
    }
    grid[y - 1][x] = grid[y][x];
    grid[y][x] = '.';
}

// move down and pushing boxes in the way
// only use this function if it's possible to push down
void PushDown(std::vector<std::string>& grid, int y, int x) {
    if ((unsigned int)y >= grid.size() || x < 0 ||
        (unsigned int)x >= grid[0].size())
        return;  // out of range

    switch (grid[y + 1][x]) {
        case '.':
            // nothing to push
            break;
        case '[':
            PushDown(grid, y + 1, x);
            PushDown(grid, y + 1, x + 1);
            break;
        case ']':
            PushDown(grid, y + 1, x);
            PushDown(grid, y + 1, x - 1);
            break;
    }
    grid[y + 1][x] = grid[y][x];
    grid[y][x] = '.';
}

void part2(std::vector<std::string>& grid, std::string& dirs, int y, int x) {
    // convert to new grid
    std::vector<std::string> big_grid;
    for (auto& g : grid) {
        std::string s;
        for (char c : g) {
            switch (c) {
                case '#':
                    s += "##";
                    break;
                case 'O':
                    s += "[]";
                    break;
                case '.':
                    s += "..";
                    break;
            }
        }
        big_grid.push_back(s);
    }
    x *= 2;

    // run simulation
    int i;
    std::queue<std::pair<int, int>> checks;
    for (auto d : dirs) {
        checks = {};

        switch (d) {
            case '<':
                // find empty space
                for (i = x - 1; i >= 0; i--) {
                    if (big_grid[y][i] == '#') break;

                    if (big_grid[y][i] == '.') {
                        // wind back, moving boxes in the process
                        while (i != x) {
                            big_grid[y][i] = big_grid[y][i + 1];
                            i++;
                        }
                        // move robot
                        x--;
                        break;
                    }
                }
                break;
            case '>':
                // find empty space
                for (i = x + 1; (unsigned int)i < big_grid[0].size(); i++) {
                    if (big_grid[y][i] == '#') break;

                    if (big_grid[y][i] == '.') {
                        // wind back, moving boxes in the process
                        while (i != x) {
                            big_grid[y][i] = big_grid[y][i - 1];
                            i--;
                        }
                        // move robot
                        x++;
                        break;
                    }
                }
                break;
            case '^':
                // BFS check for pushable
                checks.push({y - 1, x});

                while (!checks.empty()) {
                    auto& [curr_y, curr_x] = checks.front();
                    if (big_grid[curr_y][curr_x] == '#') break;
                    if (big_grid[curr_y][curr_x] == '[') {
                        checks.push({curr_y - 1, curr_x});
                        checks.push({curr_y - 1, curr_x + 1});
                    } else if (big_grid[curr_y][curr_x] == ']') {
                        checks.push({curr_y - 1, curr_x});
                        checks.push({curr_y - 1, curr_x - 1});
                    }
                    checks.pop();
                }
                if (!checks.empty()) break;

                // push boxes
                PushUp(big_grid, y, x);
                y--;
                break;
            case 'v':
                // BFS check for pushable
                checks.push({y + 1, x});

                while (!checks.empty()) {
                    auto& [curr_y, curr_x] = checks.front();
                    if (big_grid[curr_y][curr_x] == '#') break;
                    if (big_grid[curr_y][curr_x] == '[') {
                        checks.push({curr_y + 1, curr_x});
                        checks.push({curr_y + 1, curr_x + 1});
                    } else if (big_grid[curr_y][curr_x] == ']') {
                        checks.push({curr_y + 1, curr_x});
                        checks.push({curr_y + 1, curr_x - 1});
                    }
                    checks.pop();
                }
                if (!checks.empty()) break;

                // push boxes
                PushDown(big_grid, y, x);
                y++;
                break;
        }
    }

    long total = 0;
    for (size_t i = 0; i < big_grid.size(); i++) {
        for (size_t j = 0; j < big_grid[0].size(); j++) {
            if (big_grid[i][j] == '[') total += 100 * i + j;
        }
    }

    std::cout << "part2: " << total << std::endl;
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
    part2(grid, dirs, y, x);

    return 0;
}
