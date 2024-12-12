#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

typedef std::pair<int, int> coord;

bool InRange(coord& c, int width, int height) {
    return c.first >= 0 && c.second >= 0 && c.first < width &&
           c.second < height;
}

// for each type of antennas, loop through pairs and check
void part1(const std::map<char, std::set<coord>>& antennas, int width,
           int height) {
    std::set<coord> yes;

    for (auto towers : antennas) {
        if (towers.second.size() <= 1) continue;

        // loop through pairs
        auto one = towers.second.begin();
        while (one != towers.second.end()) {
            auto two = std::next(one);
            while (two != towers.second.end()) {
                coord c = {one->first * 2 - two->first,
                           one->second * 2 - two->second};
                if (InRange(c, width, height)) yes.insert(c);

                c = {two->first * 2 - one->first,
                     two->second * 2 - one->second};
                if (InRange(c, width, height)) yes.insert(c);

                two++;
            }
            one++;
        }
    }
    std::cout << "part1: " << yes.size() << std::endl;
}

// same but more checks
void part2(const std::map<char, std::set<coord>>& antennas, int width,
           int height) {
    std::set<coord> yes;

    for (auto towers : antennas) {
        if (towers.second.size() <= 1) continue;

        // loop through pairs
        auto one = towers.second.begin();
        while (one != towers.second.end()) {
            auto two = std::next(one);
            while (two != towers.second.end()) {
                yes.insert(*one);
                yes.insert(*two);

                // check past tower one
                coord c = {one->first * 2 - two->first,
                           one->second * 2 - two->second};
                while (InRange(c, width, height)) {
                    yes.insert(c);
                    c = {c.first + one->first - two->first,
                         c.second + one->second - two->second};
                }

                // check past tower two
                c = {two->first * 2 - one->first,
                     two->second * 2 - one->second};
                while (InRange(c, width, height)) {
                    yes.insert(c);
                    c = {c.first + two->first - one->first,
                         c.second + two->second - one->second};
                }

                // no need to check between? whatever

                two++;
            }
            one++;
        }
    }
    std::cout << "part2: " << yes.size() << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day8.txt", std::ios_base::in);
    std::string line;

    // collect antennas
    std::map<char, std::set<coord>> antennas;
    int width, height = 0;
    while (getline(file, line)) {
        width = line.size();  // only need to run once, but whatever
        int i = 0;
        for (char& c : line) {
            if (std::isalnum(c)) antennas[c].insert({i, height});

            i++;
        }
        height++;
    }

    part1(antennas, width, height);
    part2(antennas, width, height);
}
