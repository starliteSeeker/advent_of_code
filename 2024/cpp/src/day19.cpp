#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>

enum class Color {
    kW,
    kU,
    kB,
    kR,
    kG,
};

Color FromChar(char c) {
    switch (c) {
        case 'w':
            return Color::kW;
        case 'u':
            return Color::kU;
        case 'b':
            return Color::kB;
        case 'r':
            return Color::kR;
        case 'g':
            return Color::kG;
        default:
            std::cout << "what" << std::endl;
            exit(1);
    }
}

struct TrieNode {
    std::map<Color, TrieNode> next;
    bool valid;

    TrieNode() : valid(false){};
    TrieNode(bool valid) : valid(valid){};
};

class Trie {
   public:
    TrieNode head;

    Trie() : head(TrieNode()){};

    void insert(std::string s) {
        TrieNode* curr = &head;
        for (char c : s) {
            curr = &curr->next[FromChar(c)];
        }
        curr->valid = true;
    }
};

// track all possible path though the trie
void part1(const Trie& towels, const std::vector<std::string>& patterns) {
    int total = 0;
    for (auto& pat : patterns) {
        std::stack<std::pair<size_t, TrieNode const*>> tracks;
        tracks.push({0, &towels.head});

        while (!tracks.empty()) {
            auto [idx, curr] = tracks.top();

            // found match, stop
            if (idx >= pat.size()) break;

            tracks.pop();

            // dead end
            if (curr->next.find(FromChar(pat[idx])) == curr->next.end())
                continue;

            curr = &curr->next.at(FromChar(pat[idx]));
            // restart from top of trie
            if (curr->valid) tracks.push({idx + 1, &towels.head});
            // continue from current position
            tracks.push({idx + 1, curr});
        }

        if (!tracks.empty()) {
            total++;
        }
    }

    std::cout << "part1: " << total << std::endl;
}

long TowelMemo(std::unordered_map<std::string, long>& memo, const Trie& towels,
               std::string pat) {
    if (memo.find(pat) != memo.end()) return memo[pat];

    long total = 0;
    auto curr = &towels.head;

    // find matching towels
    for (size_t i = 0; i < pat.size(); i++) {
        if (curr->next.find(FromChar(pat[i])) == curr->next.end()) break;
        curr = &curr->next.at(FromChar(pat[i]));

        if (curr->valid) total += TowelMemo(memo, towels, pat.substr(i + 1));
    }

    // record result
    memo[pat] = total;
    return total;
}

void part2(const Trie& towels, const std::vector<std::string>& patterns) {
    long total = 0;
    std::unordered_map<std::string, long> memo;
    memo[""] = 1;

    for (auto& pat : patterns) {
        total += TowelMemo(memo, towels, pat);
    }

    std::cout << "part2: " << total << std::endl;
}

int main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day19.txt", std::ios_base::in);

    // get towels
    std::string line;
    getline(file, line);
    Trie towels;
    size_t start = 0, end;
    for (;;) {
        end = line.find(',', start);
        if (end == std::string::npos) {
            towels.insert(line.substr(start));
            break;
        }
        towels.insert(line.substr(start, end - start));
        start = end + 2;
    }

    // blank line
    getline(file, line);

    // towel patterns
    std::vector<std::string> patterns;
    while (getline(file, line)) {
        patterns.push_back(line);
    }

    part1(towels, patterns);
    part2(towels, patterns);

    return 0;
}
