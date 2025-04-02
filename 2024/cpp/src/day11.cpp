#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>

// boring labor, list for faster inserting in the middle of the sequence (order
// doesn't matter though)
void part1(const std::list<long>& ls_copy) {
    std::list<long> ls(ls_copy);

    for (int i = 0; i < 25; i++) {
        for (auto it = ls.begin(); it != ls.end(); it++) {
            if (*it == 0)
                *it = 1;
            else if (int(log10(*it)) % 2 == 1) {
                long left = *it / (long)pow(10, (int)(log10(*it) + 1) / 2);
                long right = *it % (long)pow(10, (int)(log10(*it) + 1) / 2);
                ls.insert(it, left);
                *it = right;
            } else
                *it *= 2024;
        }
    }

    std::cout << "part1: " << ls.size() << std::endl;
}

// recursive and memoing
long Count(std::unordered_map<long, long[75]>& track, long n, int count) {
    if (count == 0) return 1;

    if (track[n][count - 1] != 0) return track[n][count - 1];

    long val;
    if (n == 0)
        val = Count(track, 1, count - 1);
    else if (int(log10(n)) % 2 == 1) {
        long left = n / (long)pow(10, (int)(log10(n) + 1) / 2);
        long right = n % (long)pow(10, (int)(log10(n) + 1) / 2);
        val = Count(track, left, count - 1) + Count(track, right, count - 1);
    } else
        val = Count(track, 2024 * n, count - 1);

    track[n][count - 1] = val;
    return val;
}

void part2(const std::list<long>& ls) {
    std::unordered_map<long, long[75]> track;

    long total = 0;
    for (long n : ls) total += Count(track, n, 75);

    std::cout << "part2: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day11.txt", std::ios_base::in);
    std::list<long> ls;
    std::copy(std::istream_iterator<long>(file), std::istream_iterator<long>(),
              std::back_inserter(ls));

    part1(ls);
    part2(ls);

    return 0;
}
