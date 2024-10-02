#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <limits>

#endif /* __PROGTEST__ */
using namespace std;

class CMap {
public:

    CMap(const vector<vector<int>> &map) : m_Map(map), m_Height(map.size()) {
        if (m_Height == 0) {
            throw invalid_argument("Size of map is not valid");
        }
        m_Width = map[0].size();
    };

    int findDistance(size_t x1, size_t y1, size_t x2, size_t y2) {
        queue<tuple<size_t, size_t, int>> q;
        vector<vector<bool>> visited(m_Height, vector<bool>(m_Width, false));


        q.emplace(x1, y1, 0);

        while (!q.empty()) {
            auto &[currentX, currentY, currentDistance] = q.front();
            q.pop();

            if (currentX == x2 && currentY == y2) {
                return currentDistance;
            }

            for (auto &neighbour: getValidNeighbours(currentX, currentY)) {
                auto &[neighbourX, neighbourY] = neighbour;
                if (!visited[neighbourY][neighbourX]) {
                    visited[neighbourY][neighbourX] = true;
                    q.emplace(neighbourX, neighbourY, currentDistance + 1);
                }
            }
        }

        return NOT_FOUND;
    }

private:


    bool isValid(size_t x, size_t y, int currentElevation) const{
        // check size_t underflow!
        return MAX_SIZE_T_VALUE != x && MAX_SIZE_T_VALUE != y && x < m_Width && y < m_Height &&
               m_Map[y][x] - currentElevation <= MAX_ELEVATION_DIFFERENCE &&
               m_Map[y][x] - currentElevation >= MAX_DESCEND_DIFFERENCE;
    }

    vector<pair<size_t, size_t>> getValidNeighbours(size_t x, size_t y) const {
        vector<pair<size_t, size_t>> neighbours;
        for (const auto &[dx, dy]: DIRECTIONS) {
            size_t newX = x + dx;
            size_t newY = y + dy;
            if (isValid(newX, newY, m_Map[y][x])) {
                neighbours.emplace_back(newX, newY);
            }
        }
        return neighbours;
    }

    vector<vector<int>> m_Map;
    size_t m_Height = 0;
    size_t m_Width = 0;
    static const int MAX_ELEVATION_DIFFERENCE = 6;
    static const int MAX_DESCEND_DIFFERENCE = -3;
    static const int NOT_FOUND = -1;
    static const size_t MAX_SIZE_T_VALUE = std::numeric_limits<size_t>::max();
    static const vector<pair<size_t, size_t>> DIRECTIONS;


};

const vector<pair<size_t, size_t>> CMap::DIRECTIONS = {
        {-1, 0},
        {1,  0},
        {0,  1},
        {0,  -1}
};
#ifndef __PROGTEST__

int main() {
    CMap m0({
                    {5,  24, 21},
                    {11, 27, 18},
                    {17, 30, 22},
                    {21, 27, 19}});

    assert(m0.findDistance(0, 0, 2, 3) == 11);
    assert(m0.findDistance(0, 1, 0, 0) == -1);
    assert(m0.findDistance(2, 0, 1, 3) == 4);


    CMap m1({
                    {1,  2,  3,  4,  5},
                    {6,  7,  8,  9,  10},
                    {11, 12, 13, 14, 15},
                    {16, 17, 18, 19, 20},
                    {21, 22, 23, 24, 25}
            });

    assert(m1.findDistance(0, 0, 4, 4) == 8);
    assert(m1.findDistance(4, 4, 0, 0) == -1);
    assert(m1.findDistance(0, 0, 0, 0) == 0);
    assert(m1.findDistance(4, 4, 4, 4) == 0);

    CMap m2({
                    {14, 7,  1,   1, 5},
                    {17, 13, 100, 1, 5},
                    {14, 1,  4,   1, 4},
                    {11, 2,  100, 1, 4},
                    {8,  5,  100, 1, 4}
            });

    assert(m2.findDistance(2, 0, 4, 4) == 6);
    assert(m2.findDistance(3, 1, 1, 4) == 5);
    assert(m2.findDistance(2, 1, 4, 0) == -1);
    assert(m2.findDistance(2, 0, 1, 4) == 7);

    CMap m3({
                    {1, 2, 3, 4, 5},
                    {1, 1, 1, 1, 1},
                    {1, 2, 3, 4, 5}
            });

    assert(m3.findDistance(0, 0, 4, 2) == 6);
    assert(m3.findDistance(4, 2, 0, 0) == 6);
    assert(m3.findDistance(0, 1, 4, 1) == 4);
    assert(m3.findDistance(4, 1, 0, 1) == 4);

    CMap m4({
                    {12, 15, 18},
                    {15, 12, 15},
                    {18, 15, 12}
            });

    assert(m4.findDistance(0, 0, 2, 2) == 4);
    assert(m4.findDistance(2, 2, 0, 0) == 4);
    assert(m4.findDistance(1, 1, 0, 0) == 2);
    assert(m4.findDistance(0, 0, 1, 1) == 2);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */