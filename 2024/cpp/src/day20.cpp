#include <fstream>
#include <iostream>
#include <vector>

constexpr int kSave = 100;

void part1(std::vector<std::string> vec, int sy, int sx, int ey, int ex) {
    // record distant for normal path
    int width = vec[0].size(), height = vec.size();
    std::vector<std::vector<int>> dists(height,
                                        std::vector<int>(width, INT_MAX));
    dists[sy][sx] = 0;
    int x = sx, y = sy;
    int d = 0;
    while (x != ex || y != ey) {
        if (x > 0 && vec[y][x - 1] != '#' && dists[y][x - 1] == INT_MAX)
            x--;
        else if (x < width - 1 && vec[y][x + 1] != '#' &&
                 dists[y][x + 1] == INT_MAX)
            x++;
        else if (y > 0 && vec[y - 1][x] != '#' && dists[y - 1][x] == INT_MAX)
            y--;
        else if (y < height - 1 && vec[y + 1][x] != '#' &&
                 dists[y + 1][x] == INT_MAX)
            y++;
        dists[y][x] = ++d;
    }

    // walk again, and look two steps apart, check for shortcut that saves at
    // least kSave units
    x = sx, y = sy;
    int shortcuts = 0;
    while (x != ex || y != ey) {
        // take one step
        if (x > 0 && dists[y][x - 1] - dists[y][x] == 1)
            x--;
        else if (x < width - 1 && dists[y][x + 1] - dists[y][x] == 1)
            x++;
        else if (y > 0 && dists[y - 1][x] - dists[y][x] == 1)
            y--;
        else if (y < height - 1 && dists[y + 1][x] - dists[y][x] == 1)
            y++;

        // check for shortcut
        // left
        if (x > 1 && dists[y][x - 2] != INT_MAX &&
            dists[y][x - 2] - (dists[y][x] + 2) >= kSave) {
            shortcuts++;
        }
        // right
        if (x < width - 2 && dists[y][x + 2] != INT_MAX &&
            dists[y][x + 2] - (dists[y][x] + 2) >= kSave) {
            shortcuts++;
        }
        // up
        if (y > 1 && dists[y - 2][x] != INT_MAX &&
            dists[y - 2][x] - (dists[y][x] + 2) >= kSave) {
            shortcuts++;
        }
        // down
        if (y < height - 2 && dists[y + 2][x] != INT_MAX &&
            dists[y + 2][x] - (dists[y][x] + 2) >= kSave) {
            shortcuts++;
        }
        // doesn't seem to have diagnals in the input
    }

    std::cout << "part1: " << shortcuts << std::endl;
}

void part2(std::vector<std::string> vec, int sy, int sx, int ey, int ex) {
    std::cout << "part2: " << 1232123 << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day20.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> vec;

    int sy = -1, sx = -1, ey = -1, ex = -1;
    int i = 0;
    while (getline(file, line)) {
        vec.push_back(line);
        if (size_t j = line.find('S'); j != std::string::npos) {
            sy = i;
            sx = j;
        }
        if (size_t j = line.find('E'); j != std::string::npos) {
            ey = i;
            ex = j;
        }

        i++;
    }

    part1(vec, sy, sx, ey, ex);
    part2(vec, sy, sx, ey, ex);

    return 0;
}
