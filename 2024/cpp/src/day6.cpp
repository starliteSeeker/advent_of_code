#include <fstream>
#include <iostream>
#include <set>  // unordered_set can't hash int pair

typedef std::pair<int, int> coord;

// simple tedious walking while checking forward
void part1(std::set<coord> obstacles, coord start, int width, int height) {
    std::set<coord> visited;
    coord curr = start;
    int direction = 0;  // 0 - up, 1 - right, 2 - down, 3 - left

    // while curr is still in range
    while (curr.first >= 0 && curr.first < width && curr.second >= 0 &&
           curr.second < height) {
        visited.insert(curr);

        switch (direction) {
            case 0:
                if (obstacles.find({curr.first, curr.second - 1}) !=
                    obstacles.end()) {
                    // turn
                    direction = 1;
                    break;
                }
                // go forward
                curr.second--;
                break;
            case 1:
                if (obstacles.find({curr.first + 1, curr.second}) !=
                    obstacles.end()) {
                    // turn
                    direction = 2;
                    break;
                }
                // go forward
                curr.first++;
                break;
            case 2:
                if (obstacles.find({curr.first, curr.second + 1}) !=
                    obstacles.end()) {
                    // turn
                    direction = 3;
                    break;
                }
                // go forward
                curr.second++;
                break;
            case 3:
                if (obstacles.find({curr.first - 1, curr.second}) !=
                    obstacles.end()) {
                    // turn
                    direction = 0;
                    break;
                }
                // go forward
                curr.first--;
                break;
        }
    }

    std::cout << "part1 " << visited.size() << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day6.txt", std::ios_base::in);
    std::string line;

    // collect obstacles, starting position, and grid size
    std::set<coord> obstacles;
    int width, height = 0;
    coord start;
    while (getline(file, line)) {
        width = line.size();  // only need to run once, but whatever
        int i = 0;
        for (char& c : line) {
            switch (c) {
                case '#':
                    obstacles.insert({i, height});
                    break;
                case '^':
                    start = {i, height};
                    break;
            }
            i++;
        }
        height++;
    }

    part1(obstacles, start, width, height);
}
