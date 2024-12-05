#include <fstream>
#include <iostream>
#include <vector>

// find 'X', then search in eight directions
void part1(std::vector<std::string>& vec) {
    int width = vec[0].size(), height = vec.size();
    int total = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (vec[i][j] != 'X') continue;

            // left
            if (j - 3 >= 0 && vec[i][j - 1] == 'M' && vec[i][j - 2] == 'A' &&
                vec[i][j - 3] == 'S')
                total++;

            // right
            if (j + 3 < width && vec[i][j + 1] == 'M' && vec[i][j + 2] == 'A' &&
                vec[i][j + 3] == 'S')
                total++;

            // up
            if (i - 3 >= 0 && vec[i - 1][j] == 'M' && vec[i - 2][j] == 'A' &&
                vec[i - 3][j] == 'S')
                total++;

            // down
            if (i + 3 < height && vec[i + 1][j] == 'M' &&
                vec[i + 2][j] == 'A' && vec[i + 3][j] == 'S')
                total++;

            // up left
            if (i - 3 >= 0 && j - 3 >= 0 && vec[i - 1][j - 1] == 'M' &&
                vec[i - 2][j - 2] == 'A' && vec[i - 3][j - 3] == 'S')
                total++;

            // down right
            if (i + 3 < height && j + 3 < width && vec[i + 1][j + 1] == 'M' &&
                vec[i + 2][j + 2] == 'A' && vec[i + 3][j + 3] == 'S')
                total++;

            // up right
            if (i - 3 >= 0 && j + 3 < width && vec[i - 1][j + 1] == 'M' &&
                vec[i - 2][j + 2] == 'A' && vec[i - 3][j + 3] == 'S')
                total++;

            // down left
            if (i + 3 < height && j - 3 >= 0 && vec[i + 1][j - 1] == 'M' &&
                vec[i + 2][j - 2] == 'A' && vec[i + 3][j - 3] == 'S')
                total++;
        }
    }

    std::cout << total << std::endl;
}

// find 'A', then search four diagnal
void part2(std::vector<std::string>& vec) {
    int width = vec[0].size(), height = vec.size();
    int total = 0;

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (vec[i][j] != 'A') continue;

            // adding is okay because only 'X', 'M', 'A', 'S' are in the input
            if (vec[i - 1][j - 1] + vec[i + 1][j + 1] == 'M' + 'S' &&
                vec[i - 1][j + 1] + vec[i + 1][j - 1] == 'M' + 'S')
                total++;
        }
    }

    std::cout << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day4.txt", std::ios_base::in);

    std::vector<std::string> vec;
    std::string line;
    while (getline(file, line)) {
        vec.push_back(line);
    }

    part2(vec);

    return 0;
}
