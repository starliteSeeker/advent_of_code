#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

constexpr int kWidth = 101, kHeight = 103;
// constexpr int kWidth = 11, kHeight = 7;

// a might be negative, result always positive, pmod(-7, 5) = 3
int pmod(int a, int b) { return ((a % b) + b) % b; }

class Robot {
   public:
    int px, py, vx, vy;
    Robot(int px, int py, int vx, int vy) : px(px), py(py), vx(vx), vy(vy){};

    void Print() { std::cout << px << py << vx << vy << std::endl; }

    void Step() {
        px = pmod(px + vx, kWidth);
        py = pmod(py + vy, kHeight);
    }

    std::pair<int, int> T100() const {
        return {pmod(px + 100 * vx, kWidth), pmod(py + 100 * vy, kHeight)};
    }
};

void part1(const std::vector<Robot>& robots) {
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (auto robot : robots) {
        const auto& [x, y] = robot.T100();
        if (x == kWidth / 2 || y == kHeight / 2) continue;
        if (x < kWidth / 2) {
            if (y < kHeight / 2)
                q2++;
            else
                q3++;
        } else {
            if (y < kHeight / 2)
                q1++;
            else
                q4++;
        }
    }
    std::cout << "part1: " << long(q1) * q2 * q3 * q4 << std::endl;
}

// write to file, manual inspection
void part2(std::vector<Robot>& robots) {
    std::ofstream file("day14_find_the_tree.txt");

    for (int i = 0; i < 10000; i++) {
        // these number seems special, only print these sumbers
        if ((i + 1 - 30) % 103 != 0 && (i + 1 - 81) % 101 != 0) {
            for (auto& robot : robots) {
                robot.Step();
            }
            continue;
        }

        std::set<std::pair<int, int>> yes;
        for (auto& robot : robots) {
            robot.Step();
            yes.insert({robot.py,
                        robot.px});  // py then px for easier sorting with set
        }

        file << "t = " << i + 1 << std::endl;

        auto it = yes.begin();
        for (int y = 0; y < kHeight; y++) {
            for (int x = 0; x < kWidth; x++) {
                if (it != yes.end() && x == it->second && y == it->first) {
                    file << '@';
                    it++;
                } else
                    file << '.';
            }
            file << std::endl;
        }
        file << std::endl;
    }

    std::cout << "part2: pretty :D" << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day14.txt", std::ios_base::in);

    std::vector<Robot> robots;

    std::string line;
    while (getline(file, line)) {
        int px, py, vx, vy;

        std::istringstream ss(line);
        ss.ignore(2);
        ss >> px;
        ss.ignore(1);
        ss >> py;
        ss.ignore(3);
        ss >> vx;
        ss.ignore(1);
        ss >> vy;
        robots.push_back(Robot(px, py, vx, vy));
    }

    part1(robots);
    part2(robots);

    return 0;
}

/*
 * :D
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @.............................@
 * @.............................@
 * @.............................@
 * @.............................@
 * @..............@..............@
 * @.............@@@.............@
 * @............@@@@@............@
 * @...........@@@@@@@...........@
 * @..........@@@@@@@@@..........@
 * @............@@@@@............@
 * @...........@@@@@@@...........@
 * @..........@@@@@@@@@..........@
 * @.........@@@@@@@@@@@.........@
 * @........@@@@@@@@@@@@@........@
 * @..........@@@@@@@@@..........@
 * @.........@@@@@@@@@@@.........@
 * @........@@@@@@@@@@@@@........@
 * @.......@@@@@@@@@@@@@@@.......@
 * @......@@@@@@@@@@@@@@@@@......@
 * @........@@@@@@@@@@@@@........@
 * @.......@@@@@@@@@@@@@@@.......@
 * @......@@@@@@@@@@@@@@@@@......@
 * @.....@@@@@@@@@@@@@@@@@@@.....@
 * @....@@@@@@@@@@@@@@@@@@@@@....@
 * @.............@@@.............@
 * @.............@@@.............@
 * @.............@@@.............@
 * @.............................@
 * @.............................@
 * @.............................@
 * @.............................@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */
