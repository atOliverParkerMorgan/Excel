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

    size_t id;
    std::vector<std::pair<size_t, Node *>> neighbors;
    std::pair<size_t, Node *> parent;
    bool visited = false;
};

using EQueue = std::pair<size_t, Node *>;


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
        size_t distance;
        std::cin >> nodeId;
        std::cin >> neighbourId;
        std::cin >> distance;
        allNodes[nodeId]->neighbors.emplace_back(distance, allNodes[neighbourId]);
    }
}

int bfs(Node *from, Node *to, std::vector<Node *> &allNodes) {
    for (Node *node: allNodes) {
        node->visited = false;
        node->parent = std::make_pair(0, nullptr);
    }

    std::vector<size_t> distanceVector;
    distanceVector.reserve(allNodes.size());
    for (int i = 0; i < allNodes.size(); ++i) {
        distanceVector.push_back(INTMAX_MAX);
    }
    distanceVector[from->id] = 0;

    std::priority_queue<EQueue, std::vector<EQueue>, std::greater<>> queue;
    queue.emplace(0, from);
    from->visited = true;

    while (!queue.empty()) {
        Node *currentNode = queue.top().second;

        queue.pop();
        currentNode->visited = true;

        if (currentNode == to) {
            return (int) distanceVector[currentNode->id];
        }

        for (std::pair<size_t, Node *> neighbourNode: currentNode->neighbors) {
            const size_t distance = neighbourNode.first;
            Node *neighbour = neighbourNode.second;
            size_t newDistance = distance + distanceVector[currentNode->id];
            if (!neighbour->visited && newDistance < distanceVector[neighbour->id]) {
                distanceVector[neighbour->id] = newDistance;

                queue.emplace(newDistance, neighbour);
            }
        }
    }

    return NOT_FOUND;
}

int main() {
    std::vector<Node *> allNodes;
    size_t numberOfNodes;

    loadData(allNodes, numberOfNodes);

    unsigned int from, to;
    while (std::cin >> from >> to) {
        int output = bfs(allNodes[from], allNodes[to], allNodes);
        if (output != NOT_FOUND) {
            std::cout << output << std::endl;
        } else {
            std::cout << 'X' << std::endl;
        }
    }

    for (size_t i = 0; i < numberOfNodes; ++i) {
        delete allNodes[i];
    }

    return 0;
}
