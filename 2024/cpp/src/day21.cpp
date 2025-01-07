#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

// slight cheat by looking up what the answer for example input should be
// 82050061710, 72242026390, 81251039228, 80786362258, 77985628636

using Coord = std::pair<int, int>;  // {x, y}, bottom left 0, 0

Coord operator-(const Coord& a, const Coord& b) {
    return {a.first - b.first, a.second - b.second};
}

std::ostream& operator<<(std::ostream& os, const Coord& c) {
    os << '(' << c.first << ',' << c.second << ')';
    return os;
}

// key position lookup and reverse lookup for easier readability when printing
const std::unordered_map<char, Coord> numpad = {
    {'0', {1, 0}}, {'A', {2, 0}}, {'1', {0, 1}}, {'2', {1, 1}},
    {'3', {2, 1}}, {'4', {0, 2}}, {'5', {1, 2}}, {'6', {2, 2}},
    {'7', {0, 3}}, {'8', {1, 3}}, {'9', {2, 3}}};

const std::map<Coord, char> numpad_r = {
    {{1, 0}, '0'}, {{2, 0}, 'A'}, {{0, 1}, '1'}, {{1, 1}, '2'},
    {{2, 1}, '3'}, {{0, 2}, '4'}, {{1, 2}, '5'}, {{2, 2}, '6'},
    {{0, 3}, '7'}, {{1, 3}, '8'}, {{2, 3}, '9'}};

const std::unordered_map<char, Coord> dpad = {
    {'l', {0, 0}}, {'d', {1, 0}}, {'r', {2, 0}}, {'u', {1, 1}}, {'A', {2, 1}}};

// align the hole to {0, 0}
const std::unordered_map<char, Coord> dpad_e = {{'l', {0, -1}},
                                                {'d', {1, -1}},
                                                {'r', {2, -1}},
                                                {'u', {1, 0}},
                                                {'A', {2, 0}}};

const std::map<Coord, char> dpad_r = {
    {{0, 0}, 'l'}, {{1, 0}, 'd'}, {{2, 0}, 'r'}, {{1, 1}, 'u'}, {{2, 1}, 'A'}};

std::vector<Coord> OneStep(std::vector<Coord> moves, bool is_numpad) {
    std::vector<Coord> new_moves = {dpad.at('A')};
    for (size_t i = 1; i < moves.size(); i++) {
        // move from a to b
        const auto& [ax, ay] = moves[i - 1];
        const auto& [bx, by] = moves[i];

        int dx = bx - ax, dy = by - ay;
        if (is_numpad) {
            // bottom left is empty
            if (ay == 0 && bx == 0) {
                // cross over bottom left, go up then left instead
                while (dy > 0) {
                    new_moves.push_back(dpad.at('u'));
                    dy--;
                }
            }
            if (ax == 0 && by == 0) {
                // cross over bottom left, go right then down instead
                while (dx > 0) {
                    new_moves.push_back(dpad.at('r'));
                    dx--;
                }
            }
        } else {
            // top left is empty
            if (ay == 1 && bx == 0) {
                // cross over top left, go down then left instead
                while (dy < 0) {
                    new_moves.push_back(dpad.at('d'));
                    dy++;
                }
            }
            if (ax == 0 && by == 1) {
                // cross over top left, go right then up instead
                while (dx > 0) {
                    new_moves.push_back(dpad.at('r'));
                    dx--;
                }
            }
        }
        while (dx < 0) {
            new_moves.push_back(dpad.at('l'));
            dx++;
        }
        while (dy < 0) {
            new_moves.push_back(dpad.at('d'));
            dy++;
        }
        while (dx > 0) {
            new_moves.push_back(dpad.at('r'));
            dx--;
        }
        while (dy > 0) {
            new_moves.push_back(dpad.at('u'));
            dy--;
        }
        new_moves.push_back(dpad.at('A'));
    }

    return new_moves;
}

void part1(const std::vector<std::string>& input) {
    int total = 0;
    for (auto& str : input) {
        // records all positions for robot to visit
        std::vector<Coord> moves = {numpad.at('A')};
        for (size_t i = 0; i < str.size(); i++) {
            moves.push_back(numpad.at(str[i]));
        }

        // numpad to dpad
        moves = OneStep(moves, true);

        // dpad to dpad
        moves = OneStep(moves, false);

        int tot = 0;
        for (size_t i = 1; i < moves.size(); i++) {
            const auto& [ax, ay] = moves[i - 1];
            const auto& [bx, by] = moves[i];
            tot += abs(bx - ax) + abs(by - ay) + 1;
        }
        total += tot * stoi(str);

        std::cout << tot << std::endl;
    }

    std::cout << "part1: " << total << std::endl;
}

// cost to move from start to end, then press end button
// apparently the part 1 algorithm is wrong and I have to consider 2 possibility
// for each step
// and i didn't properly consider the cases that goes through the empty space,
// which cost me hours :)
long OneStepMemo(std::map<std::pair<Coord, Coord>, long[30]>& memo, Coord start,
                 Coord end, int layer) {
    if (long res = memo[{start, end}][layer]; res != 0) return res;

    // move from a to b
    std::vector<Coord> moves = {dpad_e.at('A')};
    const auto& [ax, ay] = start;
    const auto& [bx, by] = end;
    int dx = bx - ax, dy = by - ay;

    if (layer == 0) {
        long res = abs(dx) + abs(dy) + 1;
        memo[{start, end}][0] = res;
        return res;
    }

    // try going horizontal then vertical
    while (dx < 0) {
        moves.push_back(dpad_e.at('l'));
        dx++;
    }
    while (dx > 0) {
        moves.push_back(dpad_e.at('r'));
        dx--;
    }
    while (dy < 0) {
        moves.push_back(dpad_e.at('d'));
        dy++;
    }
    while (dy > 0) {
        moves.push_back(dpad_e.at('u'));
        dy--;
    }
    moves.push_back(dpad_e.at('A'));

    // tally results
    long total1 = 0;
    for (size_t i = 1; i < moves.size(); i++) {
        total1 += OneStepMemo(memo, moves[i - 1], moves[i], layer - 1);
    }

    // try goind vertical then horizontal
    moves = {dpad_e.at('A')};
    dx = bx - ax, dy = by - ay;
    while (dy < 0) {
        moves.push_back(dpad_e.at('d'));
        dy++;
    }
    while (dy > 0) {
        moves.push_back(dpad_e.at('u'));
        dy--;
    }
    while (dx < 0) {
        moves.push_back(dpad_e.at('l'));
        dx++;
    }
    while (dx > 0) {
        moves.push_back(dpad_e.at('r'));
        dx--;
    }
    moves.push_back(dpad_e.at('A'));

    // tally results
    long total2 = 0;
    for (size_t i = 1; i < moves.size(); i++) {
        total2 += OneStepMemo(memo, moves[i - 1], moves[i], layer - 1);
    }

    // compare results
    long total;
    // don't go over empty space
    if (ay == 0 && bx == 0)
        total = total2;
    else if (ax == 0 && by == 0)
        total = total1;
    else
        total = std::min(total1, total2);

    memo[{start, end}][layer] = total;

    return total;
}

void part2(const std::vector<std::string>& input) {
    long total = 0;
    for (auto& str : input) {
        // records all positions for robot to visit
        std::vector<Coord> moves = {numpad.at('A')};
        for (size_t i = 0; i < str.size(); i++) {
            moves.push_back(numpad.at(str[i]));
        }

        long tot = 0;
        std::map<std::pair<Coord, Coord>, long[30]> memo;
        for (size_t i = 1; i < moves.size(); i++) {
            tot += OneStepMemo(memo, moves[i - 1], moves[i], 25);
        }
        total += tot * stoi(str);

        std::cout << tot << std::endl;
    }

    std::cout << "part2: " << total << std::endl;
}

// unused in solution
void Test(const std::vector<std::string>& input) {
    const std::string& str = input[0];

    for (int layer = 2; layer <= 27; layer++) {
        // records all positions for robot to visit
        std::vector<Coord> moves = {numpad.at('A')};
        for (size_t i = 0; i < str.size(); i++) {
            moves.push_back(numpad.at(str[i]));
        }

        // part 1
        // numpad to dpad
        /*
        std::vector<Coord> new_moves = OneStep(moves, true);

        for (int i = layer - 1; i > 0; i--) {
            new_moves = OneStep(new_moves, false);
        }
        */

        long tot1 = 0;
        /*
        for (size_t i = 1; i < new_moves.size(); i++) {
            const auto& [ax, ay] = new_moves[i - 1];
            const auto& [bx, by] = new_moves[i];
            tot1 += abs(bx - ax) + abs(by - ay) + 1;
        }
        */

        // part 2
        long tot2 = 0;
        std::map<std::pair<Coord, Coord>, long[30]> memo;
        for (size_t i = 1; i < moves.size(); i++) {
            tot2 += OneStepMemo(memo, moves[i - 1], moves[i], layer);
        }

        std::cout << str << " layer" << layer << ' ' << tot1 << ' ' << tot2
                  << std::endl;
    }
}

// didn't consider the expty space of the numpad and dpad, so wrong :)
std::vector<Coord> Robot_useless(const std::vector<Coord>& moves,
                                 bool first = false) {
    std::vector<Coord> new_moves;
    Coord curr;
    for (auto [x, y] : moves) {
        curr = dpad.at('A');
        if (!first && x == -2 && y == -1) {
            new_moves.push_back(dpad.at('d') - curr);
            curr = dpad.at('d');
            y++;
        }
        while (x < 0) {
            new_moves.push_back(dpad.at('l') - curr);
            curr = dpad.at('l');
            x++;
        }
        while (y < 0) {
            new_moves.push_back(dpad.at('d') - curr);
            curr = dpad.at('d');
            y++;
        }
        while (y > 0) {
            new_moves.push_back(dpad.at('u') - curr);
            curr = dpad.at('u');
            y--;
        }
        while (x > 0) {
            new_moves.push_back(dpad.at('r') - curr);
            curr = dpad.at('r');
            x--;
        }
        new_moves.push_back(dpad.at('A') - curr);
    }
    return new_moves;
}

// just wrong :)
void part1_wrong(const std::vector<std::string>& input) {
    int total = 0;
    for (auto& str : input) {
        std::vector<Coord> moves = {numpad.at(str[0]) - numpad.at('A')};
        for (size_t i = 1; i < str.size(); i++) {
            moves.push_back(numpad.at(str[i]) - numpad.at(str[i - 1]));
        }

        /*
        for (auto& m : moves) {
            std::cout << m << std::endl;
        }
        std::cout << std::endl;
        for (auto& m : Robot(moves)) {
            std::cout << m << std::endl;
        }
        */

        int presses = 0;
        for (auto& [x, y] : Robot_useless(Robot_useless(moves, true))) {
            presses += abs(x) + abs(y) + 1;
        }

        std::cout << "press: " << presses << std::endl;

        total += stoi(str) * presses;
    }

    std::cout << "part1: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day21.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> input;
    while (getline(file, line)) input.push_back(line);

    part1(input);
    part2(input);

    return 0;
}
