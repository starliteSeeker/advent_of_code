#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <utility>
#include <vector>

void part1(const std::vector<std::pair<std::string, std::string>> &vec) {
    std::map<std::string, std::set<std::string>> graph;
    for (auto &[node1, node2] : vec) {
        graph[node1].insert(node2);
        graph[node2].insert(node1);
    }

    int count = 0;
    for (auto i = graph.begin(); i != graph.end(); ++i) {
        for (auto j = std::next(i); j != graph.end(); ++j) {
            // skip if i isn't connected with j
            if (i->second.find(j->first) == i->second.end()) continue;

            // find common third node
            std::vector<std::string> intersection;
            std::set_intersection(i->second.begin(), i->second.end(),
                                  j->second.begin(), j->second.end(),
                                  std::back_inserter(intersection));

            // check for 't'
            for (std::string third : intersection) {
                int ntimes = 0;
                if (i->first[0] == 't') ++ntimes;
                if (j->first[0] == 't') ++ntimes;
                if (third[0] == 't') ++ntimes;

                if (ntimes != 0) count++;
            }
        }
    }
    std::cout << "part1: " << count / 3 << std::endl;
}

// new_node stores nodes that are the neighbors of all nodes in curr
void Travel(std::map<std::string, std::set<std::string>> &graph,
            std::set<std::string> new_node, std::set<std::string> curr,
            std::set<std::string> &max) {
    while (new_node.size() > 0) {
        // consider set `curr + node`
        std::string node = *new_node.begin();
        new_node.erase(new_node.begin());

        std::set<std::string> intersection;
        std::set_intersection(new_node.begin(), new_node.end(),
                              graph[node].begin(), graph[node].end(),
                              std::inserter(intersection, intersection.end()));

        curr.insert(node);
        if (intersection.empty()) {
            // no more node can be add to curr
            if (curr.size() > max.size()) max = curr;
        } else {
            Travel(graph, intersection, curr, max);
        }
        curr.erase(node);
    }
}

void part2(const std::vector<std::pair<std::string, std::string>> &vec) {
    // make graph
    std::map<std::string, std::set<std::string>> graph;
    for (auto &[node1, node2] : vec) {
        graph[node1].insert(node2);
        graph[node2].insert(node1);
    }

    // find group
    std::set<std::string> nodes, max;
    for (auto &[node, _] : graph) nodes.insert(node);
    Travel(graph, nodes, {}, max);

    // print result
    std::cout << "part2: ";
    if (max.size() == 0) return;
    auto i = max.begin();
    std::cout << *i;
    while (++i != max.end()) std::cout << ',' << *i;
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    (void)argc, (void)argv;

    std::ifstream file("input/day23.txt", std::ios_base::in);
    std::vector<std::pair<std::string, std::string>> vec;
    std::string line;
    while (std::getline(file, line)) {
        vec.push_back({line.substr(0, 2), line.substr(3, 2)});
    }

    part1(vec);
    part2(vec);

    return 0;
}
