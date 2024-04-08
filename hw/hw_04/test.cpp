#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate {
public:

    CDate(int y, int m, int d) : m_Y(y), m_M(m), m_D(d) {
    }

    std::strong_ordering operator<=>(const CDate &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

    CDate &operator=(const CDate &rhs) = default;

    bool isStartOfTime() const {
        return m_Y == INT16_MIN && m_M == 1 && m_D == 1;
    }

    bool isEndOfTime() const {
        return m_Y == INT16_MAX && m_M == 12 && m_D == 31;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};
#endif /* __PROGTEST__ */

class CStudent {
public:


    CStudent(const std::string &name,
             const CDate &born,
             int enrolled) : name(name), born(born), enrolled(enrolled) {

    }


    bool operator==(const CStudent &other) const {
        return name == other.name && born == other.born && enrolled == other.enrolled;
    }

    bool operator!=(const CStudent &other) const {
        return !(*this == other);
    }

    std::string getName() const {
        return name;
    }

    CDate getBorn() const {
        return born;
    }

    int getEnrolled() const {
        return enrolled;
    }

    struct ComparatorByName {
        bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
            if (lhs->getName() == rhs->getName()) {
                if (lhs->getBorn() == rhs->getBorn()) {
                    return lhs->getEnrolled() < rhs->getEnrolled();
                }
                return lhs->getBorn() < rhs->getBorn();
            }
            return lhs->getName() < rhs->getName();
        }
    };

    struct ComparatorByBorn {
        bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
            if (lhs->getBorn() == rhs->getBorn()) {
                if (lhs->getName() == rhs->getName()) {
                    return lhs->getEnrolled() < rhs->getEnrolled();
                }
                return lhs->getName() < rhs->getName();
            }
            return lhs->getBorn() < rhs->getBorn();
        }
    };

    struct ComparatorByEnrolled {
        bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
            if (lhs->getEnrolled() == rhs->getEnrolled()) {
                if (lhs->getName() == rhs->getName()) {
                    return lhs->getBorn() < rhs->getBorn();
                }
                return lhs->getName() < rhs->getName();
            }
            return lhs->getEnrolled() < rhs->getEnrolled();
        }
    };

    struct ComparatorByNormalizedName {
        bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
            if (lhs->normalizeName == rhs->normalizeName) {
                if (lhs->getName() == rhs->getName()) {
                    if (lhs->getBorn() == rhs->getBorn()) {
                        return lhs->getEnrolled() < rhs->getEnrolled();
                    }
                    return lhs->getBorn() < rhs->getBorn();
                }
                return lhs->getName() < rhs->getName();
            }
            return lhs->normalizeName < rhs->normalizeName;
        }
    };


private:
    // todo
    int id;
    const std::string normalizeName;
    const std::string name;
    const CDate born;
    int enrolled;
};


class CFilter {
public:
    CFilter() : notInit(true), startEnrolled(INT16_MIN), endEnrolled(INT16_MAX),
                startBornDate(std::make_unique<CDate>(CDate(INT16_MIN, 1, 1))),
                endBornDate(std::make_unique<CDate>(CDate(INT16_MAX, 12, 31))) {}

    CFilter &name(const std::string &name) {
        notInit = false;
        allNames.insert(name);
        notInit = false;
        return *this;
    }

    CFilter &bornBefore(const CDate &date) {
        *endBornDate = date;
        notInit = false;
        return *this;

    }

    CFilter &bornAfter(const CDate &date) {
        *startBornDate = date;
        notInit = false;
        return *this;

    }

    CFilter &enrolledBefore(int year) {
        endEnrolled = year;
        notInit = false;
        return *this;
    }

    CFilter &enrolledAfter(int year) {
        startEnrolled = year;
        notInit = false;
        return *this;
    }


    bool match(const CStudent &student) const {
        return

        *startBornDate < student.getBorn() && student.getBorn() < *endBornDate;
    }

    int getStartEnrolled() const {
        return startEnrolled;
    }

    int getEndEnrolled() const {
        return endEnrolled;
    }

    const CDate &getStartBornDate() const {
        return *startBornDate;
    }

    const CDate &getEndBornDate() const {
        return *endBornDate;
    }

    const std::set<std::string> &getAllName() const {
        return allNames;
    }

private:
    bool notInit = true;
    std::set<std::string> allNames;
    std::unique_ptr<CDate> startBornDate;
    std::unique_ptr<CDate> endBornDate;
    int startEnrolled;
    int endEnrolled;
};


class CSort {

public:
    struct Criteria {
        bool ascending;
        ESortKey key;

        Criteria(bool acending, ESortKey key) : ascending(acending), key(key) {};
    };

    CSort() {};

    CSort &addKey(ESortKey key,
                  bool ascending) {
        allCriteria.push_back(new Criteria(ascending, key));
    }

private:
    std::vector<Criteria *> allCriteria;
};


class CStudyDept {
public:
    CStudyDept() {}

    bool addStudent(const CStudent &x) {
        std::shared_ptr<CStudent> sharedPtr = std::make_shared<CStudent>(x);

        if (!studentsName[x.getName()].insert(sharedPtr).second) {
            return false;
        }
        // studentNormalizedName[x.ge]
        studentsBorn[x.getBorn()].insert(sharedPtr);
        studentsEnrolled[x.getEnrolled()].insert(sharedPtr);
        return true;
    }

    bool delStudent(const CStudent &x) {
        std::shared_ptr<CStudent> sharedPtr = std::make_shared<CStudent>(x);

        if (studentsName[x.getName()].erase(sharedPtr) == 0) {
            return false;
        }
        studentsBorn[x.getBorn()].erase(sharedPtr);
        studentsEnrolled[x.getEnrolled()].erase(sharedPtr);
        return true;
    }

    std::list<CStudent> search(const CFilter &flt,
                               const CSort &sortOpt) const {

        std::list<CStudent> output;
        const auto &allNames = flt.getAllName();


        if (allNames.empty()) {
            const int startEnrolled = flt.getStartEnrolled();
            const int endEnrolled = flt.getEndEnrolled();
            const CDate &startBornDate = flt.getStartBornDate();
            const CDate &endBornDate = flt.getEndBornDate();


            if (startEnrolled != INT16_MIN) {
                const auto studentsLowerBound = studentsEnrolled.lower_bound(startEnrolled);
                const auto studentsUpperBound = studentsEnrolled.upper_bound(endEnrolled);
                for (auto it = studentsLowerBound; it != studentsUpperBound; it++) {
                    for (const auto &studentPtr: it->second) {
                        if (flt.match(*studentPtr)) {
                            output.push_back(*studentPtr);
                        }
                    }
                }
            } else if (!startBornDate.isStartOfTime()) {
                const auto studentsLowerBound = studentsBorn.lower_bound(startBornDate);
                const auto studentsUpperBound = studentsBorn.upper_bound(endBornDate);
                for (auto it = studentsLowerBound; it != studentsUpperBound; it++) {
                    for (const auto &studentPtr: it->second) {
                        if (flt.match(*studentPtr)) {
                            output.push_back(*studentPtr);
                        }
                    }
                }
            } else if (!endBornDate.isEndOfTime()) {
                const auto studentsLowerBound = studentsBorn.lower_bound(endBornDate);
                for (auto it = studentsLowerBound; it != studentsBorn.end(); it++) {
                    for (const auto &studentPtr: it->second) {
                        if (flt.match(*studentPtr)) {
                            output.push_back(*studentPtr);
                        }
                    }
                }
            } else if (endEnrolled != INT16_MAX) {
                const auto studentsLowerBound = studentsEnrolled.lower_bound(endEnrolled);
                for (auto it = studentsLowerBound; it != studentsEnrolled.end(); it++) {
                    for (const auto &studentPtr: it->second) {
                        if (flt.match(*studentPtr)) {
                            output.push_back(*studentPtr);
                        }
                    }
                }
            } else {
                for (const auto &it: studentsEnrolled) {
                    for (const auto &studentPtr: it.second) {
                        if (flt.match(*studentPtr)) {
                            output.push_back(*studentPtr);
                        }
                    }
                }
            }

        } else {
            for (const auto &name: allNames) {
                const auto &students = studentsName.find(name);
                if (students != studentsName.end()) {
                    for (const auto &studentPtr: students->second) {
                        if (flt.match(*studentPtr)) {
                            output.push_back(*studentPtr);
                        }
                    }
                }
            }
        }

        return output;

    }

    std::set<std::string> suggest(const std::string &name) const {

    }

    void print() const {
        std::cout << "Students in the department:" << std::endl;
        std::cout << "--- Name ---" << std::endl;
        for (const auto &pair: studentsName) {
            std::cout << "Key: " << pair.first << std::endl;
            for (const auto &studentPtr: pair.second) {
                const CStudent &student = *studentPtr;
                std::cout << "Name: " << student.getName() << ", Born: " << student.getBorn() << ", Enrolled: "
                          << student.getEnrolled() << std::endl;
            }
        }
        std::cout << "--- Born ---" << std::endl;
        for (const auto &pair: studentsBorn) {
            std::cout << "Key: " << pair.first << std::endl;
            for (const auto &studentPtr: pair.second) {
                const CStudent &student = *studentPtr;
                std::cout << "Name: " << student.getName() << ", Born: " << student.getBorn() << ", Enrolled: "
                          << student.getEnrolled() << std::endl;
            }
        }
        std::cout << "--- Enrolled ---" << std::endl;
        for (const auto &pair: studentsEnrolled) {
            std::cout << "Key: " << pair.first << std::endl;
            for (const auto &studentPtr: pair.second) {
                const CStudent &student = *studentPtr;
                std::cout << "Name: " << student.getName() << ", Born: " << student.getBorn() << ", Enrolled: "
                          << student.getEnrolled() << std::endl;
            }
        }
    }


private:
    std::map<std::string, std::set<std::shared_ptr<CStudent>>> studentsName;
    std::map<std::string, std::set<std::shared_ptr<CStudent>>> studentNormalizedName;
    std::map<CDate, std::set<std::shared_ptr<CStudent>>> studentsBorn;
    std::map<int, std::set<std::shared_ptr<CStudent>>> studentsEnrolled;
};


#ifndef __PROGTEST__

int main(void) {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));
    assert (x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));
    x0.print();
    x0.search(CFilter(), CSort());
    assert (x0.search(CFilter(), CSort()) == (std::list<CStudent>
            {
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, false)) == (std::list<CStudent>
            {
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter().name("james bond"),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012)
            }));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
            }));
    assert (x0.suggest("peter") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }));
    assert (x0.suggest("bond") == (std::set<std::string>
            {
                    "Bond James",
                    "James Bond"
            }));
    assert (x0.suggest("peter joHn") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (x0.suggest("peter joHn bond") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("pete") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("peter joHn PETER") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
