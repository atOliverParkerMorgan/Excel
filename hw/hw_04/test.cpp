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
    CDate(int y,
          int m,
          int d)
            : m_Y(y),
              m_M(m),
              m_D(d) {
    }

    std::strong_ordering operator<=>(const CDate &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
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
    static std::string normalize(const std::string &name) {
        std::vector<std::string> words;
        std::istringstream iss(name);
        std::string word;
        std::string output;

        while ((iss >> word)) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            words.push_back(word);
        }
        std::sort(words.begin(), words.end());
        for (const auto &w: words) {
            output += w + " ";
        }
        return output;
    }

    static bool contains(const std::string &keyString, const std::set<std::string> &subSet) {
        std::istringstream iss(keyString);
        std::string word;
        std::set<std::string> normalizedKeySet;

        while ((iss >> word)) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            normalizedKeySet.insert(word);
        }

        for (const auto &element: subSet) {
            if (normalizedKeySet.find(element) == normalizedKeySet.end()) return false;
        }
        return true;
    }


    CStudent(const std::string &name,
             const CDate &born,
             int enrolled) : normalizeName(CStudent::normalize(name)), name(name), born(born), enrolled(enrolled) {

    }

    friend bool operator<(const CStudent &lhs, const CStudent &rhs) {
        return std::tie(lhs.enrolled, lhs.born, lhs.name) < std::tie(rhs.enrolled, rhs.born, rhs.name);
    }

    bool operator==(const CStudent &other) const {
        return name == other.name && born == other.born && enrolled == other.enrolled;
    }

    bool operator!=(const CStudent &other) const {
        return !(*this == other);
    }

    const std::string &getName() const {
        return name;
    }

    const std::string &getNameNormalized() const {
        return normalizeName;
    }

    const CDate &getBorn() const {
        return born;
    }

    int getEnrolled() const {
        return enrolled;
    }

    size_t getId() const {
        return id;
    }

    void setId(size_t newId) {
        id = newId;
    }


private:
    // todo
    size_t id = 0;
    std::string normalizeName;
    const std::string name;
    const CDate born;
    int enrolled;
};


class CFilter {
public:
    CFilter() :
            startBornDate(CDate(INT16_MIN, 1, 1)),
            endBornDate(CDate(INT16_MAX, 12, 31)),
            startEnrolled(INT16_MIN), endEnrolled(INT16_MAX) {}

    CFilter &name(const std::string &name) {
        allNames.insert(CStudent::normalize(name));
        return *this;
    }

    CFilter &bornBefore(const CDate &date) {
        endBornDate = date;
        return *this;

    }

    CFilter &bornAfter(const CDate &date) {
        startBornDate = date;
        return *this;

    }

    CFilter &enrolledBefore(int year) {
        endEnrolled = year;
        return *this;
    }

    CFilter &enrolledAfter(int year) {
        startEnrolled = year;
        return *this;
    }


    bool match(const CStudent &student) const {
        return startEnrolled < student.getEnrolled() && student.getEnrolled() < endEnrolled &&
               startBornDate <=> student.getBorn() < 0 && student.getBorn() <=> endBornDate < 0;
    }

    int getStartEnrolled() const {
        return startEnrolled;
    }

    int getEndEnrolled() const {
        return endEnrolled;
    }

    const CDate &getStartBornDate() const {
        return startBornDate;
    }

    const CDate &getEndBornDate() const {
        return endBornDate;
    }

    const std::set<std::string> &getAllName() const {
        return allNames;
    }

private:
    std::set<std::string> allNames;
    CDate startBornDate;
    CDate endBornDate;
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

    CSort() = default;


    CSort &addKey(ESortKey key,
                  bool ascending) {
        allCriteria.emplace_back(ascending, key);
        return *this;
    }

    bool cmp(const CStudent &a, const CStudent &b) const {
        for (const Criteria criteria: allCriteria) {
            if (criteria.key == ESortKey::NAME) {
                if (a.getName() == b.getName()) continue;
                if (criteria.ascending) return a.getName() < b.getName();
                return a.getName() > b.getName();
            } else if (criteria.key == ESortKey::BIRTH_DATE) {
                auto birthDateComparison = a.getBorn() <=> b.getBorn();
                if (birthDateComparison == 0) continue;
                if (criteria.ascending) return birthDateComparison < 0;
                return birthDateComparison > 0;
            } else if (criteria.key == ESortKey::ENROLL_YEAR) {
                if (a.getEnrolled() == b.getEnrolled()) continue;
                if (criteria.ascending) return a.getEnrolled() < b.getEnrolled();
                return a.getEnrolled() > b.getEnrolled();
            }
        }
        return a.getId() < b.getId();
    }

private:
    std::vector<Criteria > allCriteria;
};


class CStudyDept {
public:
    CStudyDept() = default;

    bool addStudent(const CStudent &x) {
        CStudent s = x;
        s.setId(id_counter++);
        return students.insert(s).second;
    }

    bool delStudent(const CStudent &x) {
        return students.erase(x) == 1;
    }

    std::list<CStudent> search(const CFilter &flt,
                               const CSort &sortOpt) const {

        std::list<CStudent> output;
        const auto &allNames = flt.getAllName();

        for (const auto &student: students) {
            if (flt.match(student)) {
                if (allNames.empty()) {
                    output.push_back(student);
                    continue;
                }
                for (const auto &name: allNames) {
                    if (name == student.getNameNormalized()) {
                        output.push_back(student);;
                        break;
                    }
                }
            }
        }


        output.sort([& sortOpt](const CStudent &a, const CStudent &b) {
            return sortOpt.cmp(a, b);
        });

        return output;

    }

    std::set<std::string> suggest(const std::string &name) const {
        std::vector<std::string> tmp;
        std::set<std::string> output;
        std::istringstream iss(name);

        std::string word;
        while ((iss >> word)) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            word = word.append(" ");
            tmp.push_back(word);
        }

        for (const auto &student: students) {
            bool found = true;
            for (const std::string &element: tmp) {
                if (student.getNameNormalized().find(element) == std::string::npos) {
                    found = false;
                    break;
                }
            }
            if (found) {
                output.insert(student.getName());
            }
        }

        return output;
    }


private:
    std::set<CStudent> students;
    size_t id_counter = 0;
};


#ifndef __PROGTEST__

int main(void) {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) !=
              CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) !=
            CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) !=
            CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) !=
            CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) !=
            CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
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


    x0.search(CFilter(), CSort());
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
    assert (x0.search(
            CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
                    "james BOND"),
            CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
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
    assert (x0.search(
            CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
                    "james BOND"),
            CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
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
