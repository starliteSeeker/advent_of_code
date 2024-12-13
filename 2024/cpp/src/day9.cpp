#include <fstream>
#include <iostream>
#include <list>
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
    // almost fell into to trap of not properly converting to long
    return file_id * ((long)file_pos + file_pos + length - 1) * length / 2;
}

void part2(const std::vector<int> disk) {
    // also record which file (file_id) is stored in which chunk
    std::list<std::pair<int, int>> chunks;  // [{spaces, file_id}, ...]
    bool is_free = false;
    int file_id = 0;
    for (int i : disk) {
        chunks.push_back({i, is_free ? -1 : file_id++});
        is_free = !is_free;
    }

    // try moving files leftwards if possible, starting from the end
    // iterate from end to begin without reverse iterator (cursed?)
    for (auto back = chunks.end(); (back--) != chunks.begin();) {
        if (back->second == -1) continue;

        // search for empty space, from begin to back
        for (auto front = chunks.begin(); front != back; front++) {
            if (front->second != -1) continue;
            if (front->first < back->first) continue;

            // move file, repartition front block
            chunks.insert(front, *back);
            front->first -= back->first;
            back->second = -1;
            break;
        }
    }

    // calculate checksum
    long total = 0;
    int curr_pos = 0;
    for (auto& [a, b] : chunks) {
        if (b != -1) {
            total += ChunkChecksum(curr_pos, b, a);
        }
        curr_pos += a;
    }
    std::cout << "part2: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    // day9_1.txt moves 2 file chunks into one empty space
    std::ifstream file("input/day9.txt", std::ios_base::in);

    std::vector<int> disk;

    char c;
    while (file >> c) {
        disk.push_back(c - '0');
    }

    part1(disk);
    part2(disk);

    return 0;
}
