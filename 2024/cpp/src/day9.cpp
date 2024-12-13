#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

// move one block at a time, considering what file would go there
void part1(const std::vector<int> disk) {
    if (disk.size() < 2) return;

    long total = 0;

    // front points to the chunk idx points to,
    // back points to the data blocks to move
    auto front = disk.begin(), back = disk.end() - 1;
    bool is_free = false;
    if (disk.size() % 2 == 0) {
        // last entry is not data (could this even happen)
        std::cout << '!';
        back -= 1;
    }

    // not checking for dereferencing front and back because it'll be fine for
    // the inputs :)
    int idx = 0, cap = *front, blocks_left = *back;
    int front_id = 0, back_id = (disk.size() - 1) / 2;
    while (front < back) {
        if (is_free) {
            // fill in data from back
            total += idx * back_id;

            // update back position
            blocks_left--;
            if (blocks_left <= 0) {
                back -= 2;
                blocks_left = *back;
                back_id--;
            }
        } else {
            // count data that's already there
            total += idx * front_id;
        }

        // update front position
        idx++;
        while (cap <= idx) {
            front++;
            cap += *front;
            is_free = !is_free;
            if (!is_free) front_id++;
        }
    }

    // handle end
    if (front == back) {
        while (blocks_left--) {
            total += idx * front_id;
            idx++;
        }
    }

    std::cout << "part1: " << total << std::endl;
}

long ChunkChecksum(int file_pos, int file_id, int length) {
    return file_id * (file_pos + file_pos + length - 1) * length / 2;
}

void part2(const std::vector<int> disk) {
    std::cout << "part2: " << 1000 << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day9_1.txt", std::ios_base::in);

    std::vector<int> disk;

    char c;
    while (file >> c) {
        disk.push_back(c - '0');
    }

    part1(disk);
    part2(disk);

    return 0;
}
