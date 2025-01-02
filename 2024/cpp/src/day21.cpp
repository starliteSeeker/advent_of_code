#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

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

const std::map<Coord, char> dpad_r = {
    {{0, 0}, 'l'}, {{1, 0}, 'd'}, {{2, 0}, 'r'}, {{1, 1}, 'u'}, {{2, 1}, 'A'}};

void part1(const std::vector<std::string>& input) {
    int total = 0;
    for (auto& str : input) {
        std::vector<Coord> moves = {numpad.at('A')}, new_moves;
        for (size_t i = 0; i < str.size(); i++) {
            moves.push_back(numpad.at(str[i]));
        }

        // numpad to dpad
        new_moves = {dpad.at('A')};
        for (size_t i = 1; i < moves.size(); i++) {
            // move from a to b
            const auto& [ax, ay] = moves[i - 1];
            const auto& [bx, by] = moves[i];

            int dx = bx - ax, dy = by - ay;
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
        moves = new_moves;
        new_moves = {dpad.at('A')};

        // dpad to dpad
        for (size_t i = 1; i < moves.size(); i++) {
            // move from a to b
            const auto& [ax, ay] = moves[i - 1];
            const auto& [bx, by] = moves[i];

            int dx = bx - ax, dy = by - ay;
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
        moves = new_moves;

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

    std::ifstream file("input/day21_0.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> input;
    while (getline(file, line)) input.push_back(line);

    part1(input);

    return 0;
}
