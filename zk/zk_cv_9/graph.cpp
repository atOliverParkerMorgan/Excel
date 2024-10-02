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
using namespace std;
using EQueue = std::pair<size_t, size_t>;

struct Graph {
    map<size_t, vector<pair < size_t, size_t>>>
    neighbors;
    map<size_t, size_t> parents;


    void loadData(std::vector<size_t> &allNodes, size_t &numberOfNodes) {
        std::cin >> numberOfNodes;
        allNodes.reserve(numberOfNodes);
        for (size_t id = 0; id < numberOfNodes; ++id) {
            allNodes.push_back(id);
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
            neighbors[nodeId].emplace_back(distance, neighbourId);
        }
    }

    size_t bfs(size_t from, size_t to, std::vector<size_t> &allNodes) {
        map<size_t, bool> visited;
        queue<size_t> q;

        for (size_t id: allNodes) {
            visited[id] = false;
        }

        q.push(from);
        visited[from] = true;

        while (!q.empty()) {
            size_t currentId = q.front();
            q.pop();

            if (currentId == to) {
                size_t tmp = currentId;
                size_t cnt = 0;
                while (tmp != from) {
                    ++cnt;
                    tmp = parents[tmp];
                }

                return cnt;
            }

            if (!neighbors.contains(currentId)) continue;

            vector<pair < size_t, size_t>>
            currentNeighbours = neighbors[currentId];
            for (auto neighbour: currentNeighbours) {
                auto [neighbourDistance, neighbourId] = neighbour;
                if (!visited[neighbourId]) {
                    visited[neighbourId] = true;
                    parents[neighbourId] = currentId;
                    q.push(neighbourId);
                }
            }

        }
        return NOT_FOUND;

    };

    size_t dijkstra(size_t from, size_t to, std::vector<size_t> &allNodes) {
        map<size_t, bool> visited;
        map<size_t, size_t> distance;

        for (const size_t nodeId: allNodes) {
            visited[nodeId] = false;
            distance[nodeId] = INTMAX_MAX;
        }

        priority_queue<EQueue, vector<EQueue>, greater<>> pq;
        pq.emplace(0, from);
        distance[from] = 0;

        while (!pq.empty()) {
            auto [currentDist, currentId] = pq.top();
            pq.pop();
            visited[currentId] = true;

            if (currentId == to) {
                return distance[to];
            }

            if (!neighbors.contains(currentId)) continue;

            vector<pair < size_t, size_t>>
            currentNeighbours = neighbors[currentId];
            for (auto &neighbour: currentNeighbours) {
                auto &[neighbourDistance, neighbourId] = neighbour;

                size_t newDist = neighbourDistance + currentDist;
                if (!visited[neighbourId] && newDist < distance[neighbourId]) {
                    pq.emplace(newDist, neighbourId);
                    distance[neighbourId] = newDist;
                }
            }
        }

        return NOT_FOUND;
    }

};

int main() {
    std::vector<size_t> allNodes;
    size_t numberOfNodes;
    Graph g;
    g.loadData(allNodes, numberOfNodes);

    unsigned int from, to;
    while (std::cin >> from >> to) {
        size_t output = g.dijkstra(from, to, allNodes);
        if (output != NOT_FOUND) {
            std::cout << output << std::endl;
        } else {
            std::cout << 'X' << std::endl;
        }
    }

    return 0;
}

