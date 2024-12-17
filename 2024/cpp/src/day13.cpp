#include <fstream>
#include <iostream>
#include <vector>

class ClawMachine {
   public:
    long ax, ay, bx, by;
    long tx, ty;

    ClawMachine(long ax, long ay, long bx, long by, long tx, long ty)
        : ax(ax), ay(ay), bx(bx), by(by), tx(tx), ty(ty){};

    std::pair<long, long> Solve() const {
        long det = ax * by - ay * bx;
        if (det == 0) {
            // linear
            // WARNING: will not work if a, b, and t are on the same line
            std::cout << "!" << std::endl;
            return {0, 0};
        } else {
            // non-linear, cramer's rule
            long deta = tx * by - ty * bx;
            deta /= det;
            long detb = ax * ty - ay * tx;
            detb /= det;
            if (deta < 0 || detb < 0 || deta * ax + detb * bx != tx ||
                deta * ay + detb * by != ty) {
                // negative or fractional button presses
                return {0, 0};
            }

            return {deta, detb};
        }
    }

    void Part2Mod() {
        tx += 10000000000000;
        ty += 10000000000000;
    }
};

// no linear case (accident?) in part 1 :)
void part1(const std::vector<ClawMachine>& claws) {
    long total = 0;
    for (auto claw : claws) {
        const auto& [a_press, b_press] = claw.Solve();
        total += a_press * 3 + b_press * 1;
    }
    std::cout << "part1: " << total << std::endl;
}

// wow no linear cases here either, i'll take it
void part2(std::vector<ClawMachine>& claws) {
    long total = 0;
    for (auto claw : claws) {
        claw.Part2Mod();
        const auto& [a_press, b_press] = claw.Solve();
        total += a_press * 3 + b_press * 1;
    }
    std::cout << "part2: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day13.txt", std::ios_base::in);
    std::vector<long> temp;

    // read only the numbers
    // https://en.cppreference.com/w/cpp/io/basic_istream/ignore
    while (1) {
        int n;
        file >> n;

        if (file.eof() || file.bad())
            break;
        else if (file.fail()) {
            file.clear();    // unset failbit
            file.ignore(1);  // skip bad input
        } else
            temp.push_back(n);
    }
    if (temp.size() % 6 != 0) return 1;

    // group up 6 numbers
    std::vector<ClawMachine> claws;
    for (unsigned int i = 0; i < temp.size(); i += 6) {
        claws.push_back(ClawMachine(temp[i], temp[i + 1], temp[i + 2],
                                    temp[i + 3], temp[i + 4], temp[i + 5]));
    }

    part1(claws);
    part2(claws);

    return 0;
}
