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
    std::vector<unsigned int> neighbors;
    Node *parent = nullptr;
    bool visited = false;
};

void loadData(std::vector<Node *> &allNodes, unsigned int &numberOfNodes) {
    std::cin >> numberOfNodes;
    allNodes.reserve(numberOfNodes);
    for (unsigned int id = 0; id < numberOfNodes; ++id) {
        allNodes.push_back(new Node(id));
    }

    int numberOfEdges;
    std::cin >> numberOfEdges;

    for (int i = 0; i < numberOfEdges; ++i) {
        unsigned int nodeId;
        unsigned int neighbourId;
        std::cin >> nodeId;
        std::cin >> neighbourId;
        allNodes[nodeId]->neighbors.push_back(neighbourId);

    }
}

int bfs(Node *from, Node *to, std::vector<Node *> &allNodes) {
    for (Node *node: allNodes) {
        node->visited = false;
        node->parent = nullptr;
    }
    std::queue<Node *> queue;
    queue.push(from);
    from->visited = true;
    while (!queue.empty()) {
        Node *currentNode = queue.front();
        queue.pop();
        currentNode->visited = true;

        if (currentNode == to) {
            int cnt = 0;
            Node *tmp = to;
            while (tmp->parent != nullptr) {
                cnt++;
                tmp = tmp->parent;
            }

            return cnt;
        }

        for (unsigned int neighbourId: currentNode->neighbors) {
            Node *neighbour = allNodes[neighbourId];
            if (!neighbour->visited) {
                queue.push(neighbour);
                neighbour->parent = currentNode;
                neighbour->visited = true;
            }
        }
    }

    return NOT_FOUND;
}

int main() {
    std::vector<Node *> allNodes;
    unsigned int numberOfNodes;

    loadData(allNodes, numberOfNodes);

    unsigned int from, to;
    while (std::cin >> from >> to) {
        int output = bfs(allNodes[from], allNodes[to], allNodes);
        if (output != NOT_FOUND) {
            std::cout << output << std::endl;
        } else {
            std::cout << 'X'<< std::endl;
        }
    }

    for (unsigned int i = 0; i < numberOfNodes; ++i) {
        delete allNodes[i];
    }

    return 0;
};
