#ifndef __TRAINER__

#include <cassert>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <utility>
#include <map>
#include <set>
#include <optional>

#endif

using Grid = std::vector<std::string>;
using NodeData = std::optional<unsigned>;
constexpr int MOVES[][2] = {{0,  1},
                            {0,  -1},
                            {1,  0},
                            {-1, 0}};

NodeData bfs(const Grid &g, const std::pair<int, int> from, const std::pair<int, int> to, bool hasKey) {
    std::map<std::pair<int, int>, bool> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parents;

    const int height = (int) g.size();
    const int width = (int) g[0].size();

    size_t pathLengthCnt = 0;

    // find start

    std::queue<std::pair<int, int>> queue;
    queue.push(from);
    visited[from] = true;

    while (!queue.empty()) {
        std::pair<int, int> current = queue.front();
        queue.pop();

        if (current == to) {
            std::pair<int, int> tmp = current;
            while (parents.contains(tmp)) {
                ++pathLengthCnt;
                tmp = parents[tmp];
            }

            return pathLengthCnt;
        }

        for (auto move: MOVES) {
            std::pair<int, int> neighbour;
            const int x = (current.first + move[0]);
            const int y = (current.second + move[1]);
            if (x >= 0 && x < width && y >= 0 && y < height) {
                neighbour = std::make_pair(x, y);
                if (!visited[neighbour] && g[y][x] != '#' && (g[y][x] != 'D' || hasKey)) {
                    queue.push(neighbour);
                    visited[neighbour] = true;
                    parents[neighbour] = current;
                }
            }
        }
    }

    return std::nullopt;
}

std::pair<int, int> findCords(const Grid &g, char c) {
    std::pair<int, int> startPos = {-1, 0};
    for (const std::string &s: g) {
        size_t pos = s.find(c);
        if (pos != std::string::npos) {
            startPos.first = (int) pos;
            return startPos;
        }
        ++startPos.second;
    }
    return {-1, 0};
}


std::optional<unsigned> shortestPath(const Grid &g) {
    std::pair<int, int> start = findCords(g, 's');
    std::pair<int, int> end = findCords(g, 'e');


    NodeData pathFromKey, pathToKey, path = bfs(g, start, end, false);

    // find all keys
    std::vector<std::pair<int, int>> keys;
    std::pair<int, int> key;
    for (const std::string &s: g) {
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == 'k') {
                key.first = i;
                keys.push_back(key);
            }
        }
        ++key.second;
    }

    std::vector<unsigned> allPathLengthsWithKey;
    for (std::pair<int, int> element: keys) {
        pathToKey = bfs(g, start, element, false);
        pathFromKey = bfs(g, element, end, true);


        unsigned pathLengthWithKey;
        if (pathToKey != std::nullopt && pathFromKey != std::nullopt) {
            pathLengthWithKey = pathToKey.value() + pathFromKey.value();
            allPathLengthsWithKey.push_back(pathLengthWithKey);
        }
    }
    unsigned pathLengthWithKey;
    if (!allPathLengthsWithKey.empty()) {
        unsigned min = allPathLengthsWithKey[0];
        for (unsigned el: allPathLengthsWithKey) {
            if (el < min) {
                min = el;
            }
        }

        pathLengthWithKey = min;

    } else {
        return path;
    }

    if (path == std::nullopt || pathLengthWithKey < path) {
        return std::make_optional<unsigned>(pathLengthWithKey);
    }

    return path;
}

#ifndef __TRAINER__

int main() {
    assert(shortestPath({
                                "######",
                                "#skDek#",
                                "######",
                        }) == 3);

    assert(shortestPath({
                                "#####",
                                "#s e#",
                                "#####",
                        }) == 2);
    assert(shortestPath({
                                " ",
                                "s",
                                " ",
                                " ",
                                " ",
                                " ",
                                "e",
                        }) == 5);
    assert(!shortestPath({
                                 "############################",
                                 "#s                   #    e#",
                                 "############################",
                         }));
    assert(!shortestPath({
                                 "s #  ",
                                 "### e",
                         }));
    assert(shortestPath({
                                "     ",
                                " s#e ",
                                " ##  ",
                        }) == 4);
    assert(!shortestPath({
                                 "s#e",
                         }));
    assert(shortestPath({
                                "#########",
                                "#   #   #",
                                "# # # # #",
                                "#s#   #e#",
                                "#########",
                        }) == 14);
    assert(shortestPath({
                                "       ",
                                " ####  ",
                                "   s## ",
                                "###  #e",
                        }) == 14);
    assert(!shortestPath({
                                 "s    #     e",
                         }));
    assert(!shortestPath({
                                 "e#",
                                 "#s",
                         }));
    assert(shortestPath({
                                "s #    #  ",
                                " #   ## # ",
                                " #   #    ",
                                "          ",
                                "#### # #  ",
                                "       #  ",
                                "  ##### e ",
                                "          ",
                        }) == 14);


    assert(shortestPath({
                                "########",
                                "#    ###",
                                "# k sDe#",
                                "#    ###",
                                "########",
                        }) == 6);
    assert(shortestPath({
                                "########",
                                "#     ##",
                                "# k sDe#",
                                "#     ##",
                                "########",
                        }) == 6);
    assert(shortestPath({
                                "      k        ",
                                "s############# ",
                                "D#             ",
                                "e  ############",
                        }) == 16);
    assert(shortestPath({
                                "s             ",
                                "D####### #####",
                                "      e# #    ",
                                "       #k#    ",
                                "       ###    ",
                        }) == 30);
    assert(!shortestPath({
                                 "s             ",
                                 "D####### #####",
                                 "       # #    ",
                                 "       #k# e  ",
                                 "       ###    ",
                         }));
    assert(shortestPath({
                                "skDe",
                        }) == 3);
    assert(!shortestPath({
                                 "sDke",
                         }));
    assert(shortestPath({
                                "kseD",
                        }) == 1);
    assert(!shortestPath({
                                 "ks#De",
                         }));
    assert(!shortestPath({
                                 "k###",
                                 "#sDe",
                         }));
    assert(shortestPath({
                                "               ",
                                " ############# ",
                                "    s        # ",
                                "############ # ",
                                "             # ",
                                " #############D",
                                "             # ",
                                "############ # ",
                                "             # ",
                                " ############# ",
                                "             # ",
                                "       ###k  #e",
                        }) == 157);

    return EXIT_SUCCESS;
}

#endif

