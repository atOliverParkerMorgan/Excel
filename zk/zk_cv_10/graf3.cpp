#ifndef __TRAINER__

#include <iostream>
#include <cassert>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <stack>

#endif

constexpr int NOT_FOUND = -1;


struct Node {
    ~Node() = default;

    explicit Node(unsigned int id) : id(id) {};

    unsigned int id;
    std::vector<Node *> neighbors;
    Node *parent = nullptr;
    bool visited = false;
};

void loadData(std::vector<Node *> &allNodes, size_t &numberOfNodes) {
    std::cin >> numberOfNodes;
    allNodes.reserve(numberOfNodes);
    for (size_t id = 0; id < numberOfNodes; ++id) {
        allNodes.push_back(new Node(id));
    }

    int numberOfEdges;
    std::cin >> numberOfEdges;

    for (int i = 0; i < numberOfEdges; ++i) {
        size_t nodeId;
        size_t neighbourId;
        std::cin >> nodeId;
        std::cin >> neighbourId;
        allNodes[nodeId]->neighbors.push_back(allNodes[neighbourId]);

    }
}

std::stack<Node *> dfs(std::vector<Node *> &allNodes) {
    std::stack<Node *> stackAfterDfs;
    for (Node *node: allNodes) {
        if (!node->visited) {
            std::stack<Node *> s;
            s.push(node);
            std::vector<Node *> orderOfSearch;
            while (!s.empty()) {
                Node *current = s.top();
                orderOfSearch.insert(orderOfSearch.begin(), s.top());
                s.pop();
                current->visited = true;


                for (Node *neighbour: current->neighbors) {
                    if (!neighbour->visited) {
                        s.push(neighbour);
                    }
                }
            }
            for (Node *nodeInOrder: orderOfSearch) {
                stackAfterDfs.push(nodeInOrder);
            }
        }
    }
    return stackAfterDfs;
}

bool detectedCycles(std::vector<Node *> &allNodes, std::stack<Node *> stackAfterDfs) {
    std::map<size_t, int> pos;
    int index = 0;

    while (!stackAfterDfs.empty()) {
        pos[stackAfterDfs.top()->id] = index++;
        stackAfterDfs.pop();
    }

    for (auto &node: allNodes) {
        for (Node *neighbor: node->neighbors) {
            if (pos[node->id] > pos[neighbor->id]) {
                return true;
            }
        }
    }
    return false;
}


int main() {
    std::vector<Node *> allNodes;
    size_t numberOfNodes;

    for (Node *node: allNodes) {
        node->visited = false;
    }

    loadData(allNodes, numberOfNodes);
    // check if contains self cycle
    for (Node *node: allNodes) {
        if (std::find(node->neighbors.begin(), node->neighbors.end(), node) != node->neighbors.end()) {
            std::cout << true << std::endl;
            for (size_t i = 0; i < numberOfNodes; ++i) {
                delete allNodes[i];
            }
            return 0;
        }
    }

    std::stack<Node *> stackAfterDfs = dfs(allNodes);
    std::cout << detectedCycles(allNodes, stackAfterDfs) << std::endl;

    for (size_t i = 0; i < numberOfNodes; ++i) {
        delete allNodes[i];
    }

    return 0;
};

