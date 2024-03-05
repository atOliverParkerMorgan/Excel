#ifndef __TRAINER__

#include <string>
#include <utility>
#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#endif

// trida Student
class Student {
private:
    double _average;
    std::string _name;
public:
    Student(std::string name, double average) : _name(std::move(name)), _average(average) {}

    const std::string &name() const {
        return this->_name;
    }

    double average() const {
        return this->_average;
    }

};

// trida StudentDatabase
class StudentDatabase {
    std::vector<Student> students;

public:

    size_t getStudentsSize() const {
        return students.size();
    }

    bool addStudent(const Student &student) {
        if (!this->findStudent(student.name())) {
            students.push_back(student);
            return true;
        }
        return false;
    }

    void print(std::ostream &out) const {
        for (const Student &student: students) {
            std::cout << "Student { " << student.name() << ", " << std::setprecision(5) << student.average() << " }\n"
                      << std::flush;
            out << "Student { " << student.name() << ", " << std::setprecision(5) << student.average() << " }\n"
                << std::flush;

        }
    }

    const Student *findStudent(const std::string &cmp_name) const {
        for (const Student &s : students) {
            if (s.name() == cmp_name) {
                return &s;
            }
        }
        return nullptr;
    }

    bool

};

#ifndef __TRAINER__

int main() {
    std::string name = "John Doe";
    double average = 2.4;
    const Student st1(name, average);

    assert(st1.name() == name);
    assert(st1.average() == average);

    StudentDatabase db;
    assert((int) db.getStudentsSize() == 0);

    std::vector<Student> expectedStudents;
    std::vector<std::pair<std::string, double>> testStudentData = {{"John Doe",     2.4},
                                                                   {"abc",          3.4901},
                                                                   {"Jan Novak II", 1.0 + 1e-12},
                                                                   {"$(!+",         4.0},
                                                                   {"",             1.66666666}};

    for (const auto &testData: testStudentData) {
        expectedStudents.push_back({testData.first, testData.second});
        assert(db.addStudent({testData.first, testData.second}));
    }

    assert(db.getStudentsSize() == testStudentData.size());

    assert(!db.addStudent({"John Doe", 2.0}));

    std::vector<std::pair<Student, bool>> testFindData = {{{"John Doe",     2.4},         true},
                                                          {{"John Doee",    2.4},         false},
                                                          {{"Jan Novak II", 1.0 + 1e-12}, true}};

    for (const auto &findData: testFindData) {
        const Student *foundStudent = db.findStudent(findData.first.name());
        assert((foundStudent != nullptr) == findData.second);
        if (findData.second) {
            assert(foundStudent->name() == findData.first.name());
            assert(foundStudent->average() == findData.first.average());
        }
    }

    const StudentDatabase constDb = db;

    std::ostringstream oss;
    constDb.print(oss);
    std::string expectedPrint = "Student { John Doe, 2.4 }\nStudent { abc, 3.4901 }\nStudent { Jan Novak II, 1 }\nStudent { $(!+, 4 }\nStudent { , 1.6667 }\n";
    std::string actualPrint = oss.str();
    assert(actualPrint == expectedPrint);

    assert(constDb.getStudentsSize() == testStudentData.size());
    return 0;
}

#endif