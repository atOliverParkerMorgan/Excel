#ifndef __PROGTEST__

#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class CPos {
public:
    CPos(int y, int x, int hour)
            : m_hour(hour), m_posY(y), m_posX(x), m_id(to_string(m_posX) + "|" + to_string(m_posY)) {
    }

    int m_hour;
    int m_posY;
    int m_posX;
    string m_id;

};

#endif /* __PROGTEST__ */

constexpr int MOVES[][2] = {{0,  1},
                            {0,  -1},
                            {1,  0},
                            {-1, 0}};


class CBattleRoyale {
public:
    CBattleRoyale(unsigned height, unsigned width, const std::list<CPos> &zones) : m_Height(height), m_Width(width) {
        for (const CPos &pos: zones) {
            m_Zones.insert({pos.m_id, pos.m_hour});
        }
    };


    ~CBattleRoyale() = default;

    using EPos = shared_ptr<CPos>;

    unsigned bfs(int ySt, int xSt, int yEn, int xEn) const {
        queue<EPos> queue;
        bool visited[m_Height][m_Width];
        for (unsigned y = 0; y < m_Height; ++y) {
            for (unsigned x = 0; x < m_Width; ++x) {
                visited[y][x] = false;
            }
        }

        visited[ySt][xSt] = true;
        queue.push(make_shared<CPos>(ySt, xSt, 0));


        while (!queue.empty()) {
            EPos currentPos = queue.front();

            auto it = m_Zones.find(currentPos->m_id);
            if (it != m_Zones.end()) {
                if (currentPos->m_hour >= it->second) {
                    queue.pop();
                    continue;
                }
            }

            visited[currentPos->m_posY][currentPos->m_posX] = true;

            if (currentPos->m_posX == xEn && currentPos->m_posY == yEn) {
                return currentPos->m_hour;
            }

            queue.pop();

            for (const int *move: MOVES) {
                int x = move[0] + currentPos->m_posX;
                int y = move[1] + currentPos->m_posY;

                if (x < 0 || x >= m_Width || y < 0 || y >= m_Height || visited[y][x]) continue;

                visited[y][x] = true;
                queue.push(make_shared<CPos>(y, x, (currentPos->m_hour + 1)));
            }

        }
        throw logic_error("Path doesn't exist");

    }

    unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const {
        if (ySt < 0 || ySt >= m_Height || xSt < 0 || xSt >= m_Width || yEn < 0 || yEn >= m_Height || xEn < 0 ||
            xEn >= m_Width)
            throw logic_error("Invalid input");

        return bfs(ySt, xSt, yEn, xEn);
    }

private:
    unsigned m_Height;
    unsigned m_Width;
    std::unordered_map<string, int> m_Zones;
};

#ifndef __PROGTEST__

int main() {
    CBattleRoyale r1(5, 5, {});
    assert(r1.findRoute(0, 0, 4, 0) == 4);
    assert(r1.findRoute(4, 4, 4, 4) == 0);

    CBattleRoyale r2(6, 7, {CPos(1, 0, 1), CPos(2, 1, 2), CPos(3, 2, 5)});
    cout << r2.findRoute(0, 0, 4, 0) << endl;
    assert(r2.findRoute(0, 0, 4, 0) == 10);


    CBattleRoyale r3(8, 8, {CPos(0, 2, 1), CPos(3, 1, 2), CPos(2, 1, 0)});
    try {
        r3.findRoute(2, 1, 4, 7);
        assert("Exception missing!" == nullptr);
    } catch (const logic_error &e) {
    } catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    assert(r3.findRoute(0, 2, 3, 0) == 5);

    CBattleRoyale b(5, 5, {CPos(0, 1, 1), CPos(1, 1, 0)});
    assert(b.findRoute(0, 0, 2, 2) == 4);
    assert(b.findRoute(0, 0, 0, 2) == 6);
    assert(b.findRoute(3, 3, 3, 3) == 0);
    try {
        assert(b.findRoute(1, 1, 2, 1) == 1);
        assert("Kde vyjimka?" == nullptr);
    } catch (logic_error &x) {}
    try {
        assert(b.findRoute(1, 1, 1, 1) == 0);
        assert("Kde vyjimka? xd" == nullptr);
    } catch (logic_error &x) {}

    CBattleRoyale b1(5, 5, {CPos(2, 0, 2), CPos(2, 1, 1), CPos(2, 2, 1), CPos(2, 3, 3), CPos(2, 4, 4)});
    try {
        b1.findRoute(1, 1, 3, 1);
        assert("Kde vyjimka?" == nullptr);
    } catch (logic_error &x) {}

}

#endif /* __PROGTEST__ */