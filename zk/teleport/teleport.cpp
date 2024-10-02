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

using namespace std;
#endif /* __PROGTEST__ */

struct CCity {
    explicit CCity(string name) : name(std::move(name)) {};
    string name;

    // duration start_time destination node
    vector<tuple<int, unsigned, string>> neighbour;

};

using ECity = shared_ptr<CCity>;

class CTeleport {

public:

    CTeleport &Add(const string &from,
                   const string &to,
                   unsigned fromTime,
                   unsigned toTime) {
        if (!allCities.contains(from)) {
            allCities.insert({from, make_shared<CCity>(from)});
        }
        if (!allCities.contains(to)) {
            allCities.insert({to, make_shared<CCity>(to)});
        }

        allCities[from]->neighbour.emplace_back(toTime - fromTime, fromTime, to);

        return *this;
    }

    CTeleport &Optimize() {
        return *this;
    }

    using ECityData = pair<long, ECity>;

    unsigned FindWay(const string &from,
                     const string &to,
                     unsigned time) {
        unordered_map<string, bool> visited;
        unordered_map<string, long> timeDistance;
        priority_queue<ECityData, vector<ECityData>, greater<>> queue;

        for (auto &element: allCities) {
            visited[element.first] = false;
            timeDistance[element.first] = INT16_MAX;
        }

        queue.emplace(time, allCities[from]);
        timeDistance[from] = time;

        while (!queue.empty()) {
            long currentTime = queue.top().first;
            ECity currentNode = queue.top().second;
            visited[currentNode->name] = true;

            queue.pop();

            if (currentNode->name == to) {
                return timeDistance[to];
            }

            for (auto neighbour: currentNode->neighbour) {
                const unsigned departure = get<1>(neighbour);
                if (departure < currentTime) continue;

                const unsigned arrival = get<0>(neighbour) + departure;
                const string neighbourName = get<2>(neighbour);
                if (!visited[neighbourName] && arrival < timeDistance[neighbourName]) {
                    timeDistance[neighbourName] = arrival;
                    queue.emplace(arrival, allCities[neighbourName]);
                }
            }

        }


        throw invalid_argument("No valid path found");
    }

private:
    unordered_map<string, ECity> allCities;
};

#ifndef __PROGTEST__

int main() {
    CTeleport t;
    t.Add("Prague", "Vienna", 0, 7)
            .Add("Vienna", "Berlin", 9, 260)
            .Add("Vienna", "London", 8, 120)
            .Add("Vienna", "Chicago", 4, 3)
            .Add("Prague", "Vienna", 10, 10).Optimize();

    assert (t.FindWay("Prague", "Vienna", 0) == 7);
    assert (t.FindWay("Prague", "Vienna", 1) == 10);
    assert (t.FindWay("Prague", "London", 0) == 120);
    assert (t.FindWay("Vienna", "Chicago", 4) == 3);

    try {
        t.FindWay("Prague", "London", 2);
        assert ("Missing exception" == nullptr);
    }
    catch (const invalid_argument &e) {}
    catch (...) { assert ("Invalid exception" == nullptr); }
    try {
        t.FindWay("Prague", "Chicago", 0);
        assert ("Missing exception" == nullptr);
    }
    catch (const invalid_argument &e) {}
    catch (...) { assert ("Invalid exception" == nullptr); }

    t.Add("Dallas", "Atlanta", 150, 30)
            .Add("Berlin", "Helsinki", 1080, 2560)
            .Add("Chicago", "Frankfurt", 50, 0)
            .Add("Helsinki", "Vienna", 3200, 3)
            .Add("Chicago", "London", 10, 12)
            .Add("London", "Atlanta", 20, 40)
            .Add("Vienna", "Atlanta", 10, 50)
            .Add("Prague", "Vienna", 1, 6)
            .Add("Berlin", "Helsinki", 265, 265)
            .Add("Berlin", "London", 259, 0).Optimize();

    assert (t.FindWay("Prague", "Frankfurt", 0) == 0);
    cout << t.FindWay("Prague", "Atlanta", 0) << endl;

    assert (t.FindWay("Prague", "Atlanta", 0) == 40);

    assert (t.FindWay("Prague", "Atlanta", 10) == 50);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */