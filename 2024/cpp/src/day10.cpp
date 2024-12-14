#include <fstream>
#include <iostream>
#include <vector>

// classic recursion
int ClimbUp(std::vector<std::string>& vec, size_t row, size_t col) {
    if (vec[row][col] == '9') {
        vec[row][col] = 'x';  // destroy mountain top to avoid repeated counting
        return 1;
    }

    int total = 0;
    if (col > 0 && vec[row][col - 1] - vec[row][col] == 1)
        total += ClimbUp(vec, row, col - 1);
    if (col < vec[0].size() - 1 && vec[row][col + 1] - vec[row][col] == 1)
        total += ClimbUp(vec, row, col + 1);
    if (row > 0 && vec[row - 1][col] - vec[row][col] == 1)
        total += ClimbUp(vec, row - 1, col);
    if (row < vec.size() - 1 && vec[row + 1][col] - vec[row][col] == 1)
        total += ClimbUp(vec, row + 1, col);

    return total;
}

void part1(const std::vector<std::string>& vec) {
    int total = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec[0].size(); j++) {
            if (vec[i][j] != '0') continue;

            // climb the mountain
            auto vec_copy = vec;
            total += ClimbUp(vec_copy, i, j);
        }
    }
    std::cout << "part1: " << total << std::endl;
}

// my dumb ass forgot size_t is unsigned
long ClimbUp2(const std::vector<std::string>& vec, size_t row, size_t col) {
    long rates[19][19] = {{0}};
    for (size_t i = 0; i < 19; i++) {
        for (size_t j = 0; j < 19; j++) {
            rates[i][j] = 0;
        }
    }
    rates[9][9] = 1;  // 1 at the trailhead

    // scan the 19*19 area for 1's, then 2's ...
    for (int r = '1'; r <= '9'; r++) {
        for (size_t i = 0; i < 19; i++) {
            for (size_t j = 0; j < 19; j++) {
                size_t ii = row + i - 9, jj = col + j - 9;
                if (ii >= vec.size() || jj >= vec[0].size()) continue;
                if (vec[ii][jj] != r) continue;

                // sum up surrounding tiles
                if (j - 1 < 19 && jj - 1 < vec[0].size() &&
                    vec[ii][jj - 1] == r - 1) {
                    rates[i][j] += rates[i][j - 1];
                }
                if (j + 1 < 19 && jj + 1 < vec[0].size() &&
                    vec[ii][jj + 1] == r - 1) {
                    rates[i][j] += rates[i][j + 1];
                }
                if (i - 1 < 19 && ii - 1 < vec.size() &&
                    vec[ii - 1][jj] == r - 1) {
                    rates[i][j] += rates[i - 1][j];
                }
                if (i + 1 < 19 && ii + 1 < vec.size() &&
                    vec[ii + 1][jj] == r - 1) {
                    rates[i][j] += rates[i + 1][j];
                }
            }
        }
    }

    // collect the 9's
    long total = 0;
    for (size_t i = 0; i < 19; i++) {
        for (size_t j = 0; j < 19; j++) {
            size_t ii = row + i - 9, jj = col + j - 9;
            if (ii >= vec.size() || jj >= vec[0].size()) continue;

            if (vec[ii][jj] == '9') total += rates[i][j];
        }
    }

    return total;
}

// same as part 1 but a differnt (non-recursive) function
void part2(const std::vector<std::string>& vec) {
    long total = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec[0].size(); j++) {
            if (vec[i][j] != '0') continue;

            // climb the mountain
            total += ClimbUp2(vec, i, j);
        }
    }
    std::cout << "part2: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day10.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> vec;

    while (getline(file, line)) {
        vec.push_back(line);
    }

    part1(vec);
    part2(vec);

    return 0;
}
