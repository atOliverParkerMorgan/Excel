<hidden řešení (nebylo testovnáno na Progtestu)>
<code cpp>
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

#endif /* __PROGTEST__ */

using namespace std;



class CSpread {
public:

    static constexpr char BELIEVER = 'B';
    static constexpr char IMMUNE = 'I';
    static constexpr char UNTOUCHED = 'U';

    CSpread &addFriend(const std::string &p1, const std::string &p2) {
        // Kamarádství by mělo být oboustranné :)
        if (friendships.contains(p1)) {
            friendships.at(p1).push_back(p2);
        } else {
            friendships.insert({p1, {p2}});
        }

        if (friendships.contains(p2)) {
            friendships.at(p2).push_back(p1);
        } else {
            friendships.insert({p2, {p1}});
        }

        return *this;
    }

    CSpread &optimize() {
        return *this;
    }

    std::map<std::string, int> findCoverage(const std::map<std::string, std::string> &spreaders) {
        queue<pair<string, string>> todayQueue;
        queue<pair<string, string>> nextDayQueue;

        map<string, int> result;
        unordered_map<string, char> believeState;
        unordered_map<string, string> believeIn;

        vector<string> converted;

        for (auto &friendship: friendships) {
            believeState[friendship.first] = UNTOUCHED;
        }

        // ti co šíří víru jsou nastaveny na BELIEVER a rovnou přičteny do result
        for (auto &spreader: spreaders) {
            believeState[spreader.first] = BELIEVER;
            result[spreader.second]++;
            todayQueue.emplace(spreader);
        }

        while (!todayQueue.empty() || !nextDayQueue.empty()) {

            if (todayQueue.empty()) {
                for (const string &c: converted) {
                    if (believeState[c] == UNTOUCHED) {
                        believeState[c] = BELIEVER;
                        result[believeIn[c]]++;
                    }
                }
                converted.clear();
                while (!nextDayQueue.empty()) {
                    todayQueue.push(nextDayQueue.front());
                    nextDayQueue.pop();
                }
            }

            auto &[currentName, currentConspiracy] = todayQueue.front();
            todayQueue.pop();

            // 3) Pokud se člověk setká s nějakou konspirační teorií, ale už v nějakou konspirační teorii věří (z předchozích dnů), pak si ponechá víru v původní teorii a novou teorii nepřijme (může dále šířit původní teorii)
            // nebo nemá kamarády
            if (!friendships.contains(currentName) || believeState[currentName] != BELIEVER)
                continue;

            const vector<string> &friends = friendships[currentName];
            for (auto &f: friends) {
                const char state = believeState[f];

                if (state == IMMUNE || state == BELIEVER) continue;

                // 1) Pokud se člověk setká v nějaký den se svou první konspirační... (ne celá logika je tady)
                if (!believeIn.contains(f)) {
                    converted.push_back(f);
                    believeIn[f] = currentConspiracy;
                    nextDayQueue.emplace(f, currentConspiracy);
                    continue;
                }

                // 2) Pokud se člověk setká v nějaký den se svou první konspirační teorií a v ten samý den ho někdo seznámí s jinou konspirační teorii... (ne celá logika je tady)
                const string friendConspiracy = believeIn[f];
                if (friendConspiracy != currentConspiracy) {
                    believeState[f] = IMMUNE;
                }

                // 1) a v ten samý den se nepotká již s žádnou jinou nebo !!!pouze s tou samou!!!
                else {
                    converted.push_back(f);
                    believeIn[f] = {currentConspiracy};
                    nextDayQueue.emplace(f, currentConspiracy);
                }
            }
        }
        return result;
    }

private:
    unordered_map<string, vector<string>> friendships;
};



#ifndef __PROGTEST__

int main() {
    CSpread x0;
    x0.addFriend("Alice", "Bob")
            .addFriend("Alice", "Dave")
            .addFriend("Bob", "Charlie")
            .addFriend("Charlie", "Dave")
            .addFriend("Charlie", "Eve")
            .addFriend("Dave", "Grace")
            .addFriend("Eve", "Frank")
            .addFriend("Frank", "Grace")
            .optimize();

    assert(x0.findCoverage({
                                   {"Alice", "Chemtrails"} //             <- Spreaders
                           }) == (std::map<std::string, int>{
            {"Chemtrails", 7} //                   <- Result
    }));

    assert(x0.findCoverage({
                                   {"Alice",   "Chemtrails"},
                                   {"Charlie", "Chemtrails"}
                           }) == (std::map<std::string, int>{
            {"Chemtrails", 7}
    }));

    assert(x0.findCoverage({
                                   {"Alice", "Chemtrails"},
                                   {"Bob",   "5G"}
                           }) == (std::map<std::string, int>{
            {"5G",         3},
            {"Chemtrails", 3}
    }));

    assert(x0.findCoverage({
                                   {"Alice", "5G"},
                                   {"Eve",   "Chemtrails"}
                           }) == (std::map<std::string, int>{
            {"5G",         3},
            {"Chemtrails", 3}
    }));

    assert(x0.findCoverage({
                                   {"Alice",   "5G"},
                                   {"Charlie", "Chemtrails"}
                           }) == (std::map<std::string, int>{
            {"5G",         1},
            {"Chemtrails", 4}
    }));

    assert(x0.findCoverage({
                                   {"Grace", "Chemtrails"},
                                   {"Frank", "5G"}
                           }) == (std::map<std::string, int>{
            {"5G",         2},
            {"Chemtrails", 4}
    }));

    assert(x0.findCoverage({
                                   {"Bob",     "Chemtrails"},
                                   {"Charlie", "5G"},
                                   {"Eve",     "Moon landing"}
                           }) == (std::map<std::string, int>{
            {"5G",           2},
            {"Chemtrails",   2},
            {"Moon landing", 2}
    }));

    assert(x0.findCoverage({
                                   {"Alice",   "Chemtrails"},
                                   {"Bob",     "5G"},
                                   {"Charlie", "Moon landing"}
                           }) == (std::map<std::string, int>{
            {"5G",           1},
            {"Chemtrails",   1},
            {"Moon landing", 4}
    }));

    assert(x0.findCoverage({
                                   {"Bob",   "Chemtrails"},
                                   {"Dave",  "5G"},
                                   {"Eve",   "Chemtrails"},
                                   {"Frank", "5G"}
                           }) == (std::map<std::string, int>{
            {"5G",         3},
            {"Chemtrails", 2}
    }));

    assert(x0.findCoverage({
                                   // empty
                           }) == (std::map<std::string, int>{
            // empty result
    }));

    CSpread x1;
    x1.addFriend("Alice", "Bob")
            .addFriend("Alice", "Dave")
            .addFriend("Bob", "Charlie")
            .addFriend("Charlie", "Eve")
            .addFriend("Eve", "Grace")
            .addFriend("Dave", "Frank")
            .addFriend("Heidi", "Ivan")
            .optimize();

    assert(x1.findCoverage({
                                   {"Alice", "Chemtrails"}
                           }) == (std::map<std::string, int>{
            {"Chemtrails", 7}
    }));

    assert(x1.findCoverage({
                                   {"Alice", "Chemtrails"},
                                   {"Grace", "5G"}
                           }) == (std::map<std::string, int>{
            {"5G",         2},
            {"Chemtrails", 4}
    }));

    assert(x1.findCoverage({
                                   {"Bob",   "5G"},
                                   {"Frank", "Chemtrails"},
                                   {"Ivan",  "Moon landing"}
                           }) == (std::map<std::string, int>{
            {"5G",           5},
            {"Chemtrails",   2},
            {"Moon landing", 2}
    }));

    assert(x1.findCoverage({
                                   {"Alice",   "Chemtrails"},
                                   {"Charlie", "5G"},
                                   {"Heidi",   "Moon landing"},
                                   {"Ivan",    "Flat Earth"}
                           }) == (std::map<std::string, int>{
            {"5G",           3},
            {"Chemtrails",   3},
            {"Flat Earth",   1},
            {"Moon landing", 1}
    }));

    assert(x1.findCoverage({
                                   {"Frank", "Chemtrails"},
                                   {"Grace", "5G"},
                                   {"Heidi", "Moon landing"},
                                   {"Ivan",  "Flat Earth"}
                           }) == (std::map<std::string, int>{
            {"5G",           3},
            {"Chemtrails",   3},
            {"Flat Earth",   1},
            {"Moon landing", 1}
    }));

    assert(x1.findCoverage({
                                   {"Alice",   "Chemtrails"},
                                   {"Bob",     "5G"},
                                   {"Charlie", "Moon landing"},
                                   {"Dave",    "Flat Earth"},
                                   {"Eve",     "Area 51"}
                           }) == (std::map<std::string, int>{
            {"5G",           1},
            {"Area 51",      2},
            {"Chemtrails",   1},
            {"Flat Earth",   2},
            {"Moon landing", 1}
    }));

    assert(x1.findCoverage({
                                   {"Judy", "5G"}
                           }) == (std::map<std::string, int>{
            {"5G", 1}
    }));

    assert(x1.findCoverage({
                                   {"Eve",   "5G"},
                                   {"Grace", "Chemtrails"},
                                   {"Judy",  "Moon landing"}
                           }) == (std::map<std::string, int>{
            {"5G",           6},
            {"Chemtrails",   1},
            {"Moon landing", 1}
    }));

    assert(x1.findCoverage({
                                   {"Alice",   "Chemtrails"},
                                   {"Charlie", "Chemtrails"},
                                   {"Frank",   "5G"},
                                   {"Grace",   "5G"},
                                   {"Judy",    "5G"}
                           }) == (std::map<std::string, int>{
            {"5G",         3},
            {"Chemtrails", 3}
    }));

    assert(x1.findCoverage({
                                   {"Bob",   "Chemtrails"},
                                   {"Dave",  "5G"},
                                   {"Eve",   "Moon landing"},
                                   {"Heidi", "Moon landing"},
                                   {"Ivan",  "Moon landing"},
                                   {"Judy",  "Moon landing"}
                           }) == (std::map<std::string, int>{
            {"5G",           2},
            {"Chemtrails",   1},
            {"Moon landing", 5}
    }));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
</code>
</hidden>