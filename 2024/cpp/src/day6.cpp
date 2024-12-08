#include <fstream>
#include <iostream>
#include <map>
#include <set>  // unordered_set can't hash int pair

typedef std::pair<int, int> coord;

class Walker {
   public:
    coord curr;
    int direction;

    Walker(coord curr, int direction)
        : curr(curr), direction(direction & 0b11) {}
    Walker(coord curr) : curr(curr), direction(0) {}

    void TurnRight() { direction = (direction + 1) & 0b11; };

    void StepFront() {
        switch (direction) {
            case 0:
                curr.second--;
                break;
            case 1:
                curr.first++;
                break;
            case 2:
                curr.second++;
                break;
            case 3:
                curr.first--;
                break;
        }
    };

    coord front(int dir_offset = 0) {
        switch ((direction + dir_offset) & 0b11) {
            case 0:
                return {curr.first, curr.second - 1};
                break;
            case 1:
                return {curr.first + 1, curr.second};
                break;
            case 2:
                return {curr.first, curr.second + 1};
                break;
            case 3:
                return {curr.first - 1, curr.second};
                break;
        }
        return {-1, -1};
    }

    coord right() { return front(1); }
};

// walk until out of bounds or loop
int walk(const std::set<coord>& obstacles, Walker walker, int width,
         int height) {
    std::map<coord, std::bitset<4>> visited;

    while (walker.curr.first >= 0 && walker.curr.first < width &&
           walker.curr.second >= 0 && walker.curr.second < height) {
        if (visited[walker.curr][walker.direction])
            return -1;  // stuck in loop
        else
            visited[walker.curr].set(walker.direction, true);

        if (obstacles.find(walker.front()) != obstacles.end())
            walker.TurnRight();
        else
            walker.StepFront();
    }

    return visited.size();
}

// cleaner version using class
void part1(const std::set<coord>& obstacles, coord start, int width,
           int height) {
    std::cout << "part1 " << walk(obstacles, start, width, height) << std::endl;
}

// try turning right at every step
void part2(const std::set<coord>& obstacles, coord start, int width,
           int height) {
    std::set<coord> visited;
    std::set<coord> make_a_loop;
    Walker walker(start);

    while (walker.curr.first >= 0 && walker.curr.first < width &&
           walker.curr.second >= 0 && walker.curr.second < height) {
        visited.insert(walker.curr);

        bool turned = false;
        if (obstacles.find(walker.front()) != obstacles.end()) {
            turned = true;
            walker.TurnRight();
        }

        // try turning right
        // first check for no preexisting obstacle and uncrossed path
        if (obstacles.find(walker.front()) == obstacles.end() &&
            visited.find(walker.front()) == visited.end()) {
            std::set<coord> new_obstacles = obstacles;
            new_obstacles.insert(walker.front());
            Walker new_walker = walker;

            if (walk(new_obstacles, new_walker, width, height) == -1) {
                make_a_loop.insert(walker.front());
            }
        }

        if (!turned) walker.StepFront();
    }

    std::cout << "part2 " << make_a_loop.size() << std::endl;
}

// simple tedious walking while checking forward
void part1_bad(const std::set<coord>& obstacles, coord start, int width,
               int height) {
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

// FAILED attempt at being smart
// walking while constantly looking to the right, searching for spaces that
// are visited
void part2_wrong(std::set<coord>& obstacles, coord start, int width,
                 int height) {
    std::map<coord, std::bitset<4>> visited;
    std::set<coord> make_a_loop;  // obstacle positions that would make a loop
    coord curr = start;
    int direction = 0;  // 0 - up, 1 - right, 2 - down, 3 - left

    // while curr is still in range
    while (curr.first >= 0 && curr.first < width && curr.second >= 0 &&
           curr.second < height) {
        // add to visited, and track direction as well
        visited[curr].set(direction, true);

        bool turned = false;
        // turn
        switch (direction) {
            case 0:
                if (obstacles.find({curr.first, curr.second - 1}) !=
                    obstacles.end()) {
                    direction = 1;
                    turned = true;
                }
                break;
            case 1:
                if (obstacles.find({curr.first + 1, curr.second}) !=
                    obstacles.end()) {
                    direction = 2;
                    turned = true;
                }
                break;
            case 2:
                if (obstacles.find({curr.first, curr.second + 1}) !=
                    obstacles.end()) {
                    direction = 3;
                    turned = true;
                }
                break;
            case 3:
                if (obstacles.find({curr.first - 1, curr.second}) !=
                    obstacles.end()) {
                    direction = 0;
                    turned = true;
                }
                break;
        }

        // check what would happen if an obstable is placed in front
        switch (direction) {
            case 0: {
                // obstacle can't be placed somewhere visited
                coord front = {curr.first, curr.second - 1};
                if (visited.find(front) != visited.end()) break;

                for (int i = curr.first + 1; i < width; i++) {
                    coord cell = {i, curr.second};

                    // stop looking if a obstacle is found
                    if (obstacles.find(cell) != obstacles.end()) break;

                    // new obstacle will make a loop if a visited cell with
                    // the correct direction is found
                    if (auto c = visited.find(cell);
                        c != visited.end() && (*c).second[1]) {
                        make_a_loop.insert(front);
                        break;
                    }
                }
                break;
            }
            case 1: {
                // obstacle can't be placed somewhere visited
                coord front = {curr.first + 1, curr.second};
                if (visited.find(front) != visited.end()) break;

                for (int i = curr.second + 1; i < width; i++) {
                    coord cell = {curr.first, i};

                    // stop looking if a obstacle is found
                    if (obstacles.find(cell) != obstacles.end()) break;

                    // new obstacle will make a loop if a visited cell with
                    // the correct direction is found
                    if (auto c = visited.find(cell);
                        c != visited.end() && (*c).second[2]) {
                        make_a_loop.insert(front);
                        break;
                    }
                }
                break;
            }
            case 2: {
                // obstacle can't be placed somewhere visited
                coord front = {curr.first, curr.second + 1};
                if (visited.find(front) != visited.end()) break;

                for (int i = curr.first - 1; i >= 0; i--) {
                    coord cell = {i, curr.second};

                    // stop looking if a obstacle is found
                    if (obstacles.find(cell) != obstacles.end()) break;

                    // new obstacle will make a loop if a visited cell with
                    // the correct direction is found
                    if (auto c = visited.find(cell);
                        c != visited.end() && (*c).second[3]) {
                        make_a_loop.insert(front);
                        break;
                    }
                }
                break;
            }
            case 3: {
                // obstacle can't be placed somewhere visited
                coord front = {curr.first - 1, curr.second};
                if (visited.find(front) != visited.end()) break;

                for (int i = curr.second - 1; i > 0; i--) {
                    coord cell = {curr.first, i};

                    // stop looking if a obstacle is found
                    if (obstacles.find(cell) != obstacles.end()) break;

                    // new obstacle will make a loop if a visited cell with
                    // the correct direction is found
                    if (auto c = visited.find(cell);
                        c != visited.end() && (*c).second[0]) {
                        make_a_loop.insert(front);
                        break;
                    }
                }
                break;
            }
        }

        if (turned) continue;

        // go forward
        switch (direction) {
            case 0:
                curr.second--;
                break;
            case 1:
                curr.first++;
                break;
            case 2:
                curr.second++;
                break;
            case 3:
                curr.first--;
                break;
        }
    }

    std::cout << "part2 " << make_a_loop.size() << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    // day6_1.txt checks for passing through walls (stepping after turning w/o
    // checking for wall again)
    // day6_2.txt checks for better search algorithm (why part2_wrong failed)
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
    part2(obstacles, start, width, height);
}
