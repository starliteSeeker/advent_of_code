#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// check every pair in book for violation
bool check(const std::unordered_map<int, std::unordered_set<int>>& rules,
           const std::vector<int>& book) {
    for (std::size_t j = 1; j < book.size(); j++) {
        std::unordered_set<int> rule;
        try {
            rule = rules.at(book[j]);
        } catch (std::out_of_range) {
            // no rule about book[j], can't fail here
            continue;
        };

        for (std::size_t i = 0; i < j; i++) {
            // make sure no rule says j comes before i
            if (rule.find(book[i]) != rule.end()) return false;
        }
    }
    return true;
}

void part1(const std::unordered_map<int, std::unordered_set<int>>& rules,
           const std::vector<std::vector<int>>& books) {
    int total = 0;
    for (auto book : books) {
        if (check(rules, book)) total += book[book.size() / 2];
    }

    std::cout << "part1: " << total << std::endl;
}

void part2(std::unordered_map<int, std::unordered_set<int>>& rules,
           std::vector<std::vector<int>>& books) {
    int total = 0;
    for (auto book : books) {
        if (!check(rules, book)) {
            std::sort(book.begin(), book.end(), [&rules](int a, int b) {
                // has the rule a|b, or doesn't have to rule b|a
                return rules[a].find(b) != rules[a].end() ||
                       rules[b].find(a) == rules[b].end();
            });
            total += book[book.size() / 2];
        }
    }

    std::cout << "part2: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day5.txt", std::ios_base::in);

    std::unordered_map<int, std::unordered_set<int>> rules;
    std::string line;
    // ordering rules
    while (getline(file, line)) {
        if (line.size() == 0) break;
        int idx = line.find('|');
        int key = stoi(line.substr(0, idx));
        int val = stoi(line.substr(idx + 1));
        rules[key].insert(val);
    }

    // pages
    std::vector<std::vector<int>> books;
    std::string num;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> book;
        while (getline(ss, num, ',')) {
            book.push_back(stoi(num));
        }
        books.push_back(book);
    }

    part1(rules, books);
    part2(rules, books);

    return 0;
}
