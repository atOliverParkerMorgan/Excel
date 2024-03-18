#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

using namespace std;
#endif /* __PROGTEST__ */

class Contact {
public:
    std::string firstName;
    std::string lastName;
    std::string phone;


};


bool isPhoneValid(const std::string &phone) {
    return phone.length() == 9 && std::all_of(phone.begin(), phone.end(), [](const char &c) { return isdigit(c); })
           && phone[0] != '0';

}


bool report(const string &fileName, ostream &out) {
    std::string line;
    ifstream fileReader(fileName);

    if (!fileReader.is_open()) return false;

    std::vector<Contact> contactList;

    bool search = false;
    while (getline(fileReader, line)) {
        istringstream iss(line);
        Contact contact;
        std::string extra;

        if (!search) {
            if (line.empty()) {
                search = true;
                continue;
            }

            if ((!(iss >> contact.firstName >> contact.lastName >> contact.phone) || !isPhoneValid(contact.phone) ||
                 iss >> extra || !extra.empty())) {
                return false;
            }
            contactList.push_back(contact);
        } else {
            std::string nameSearch;
            if (!((iss >> nameSearch) || (iss >> extra) || extra.empty())) {
                return false;
            }
            int cnt = 0;
            for (const Contact &con: contactList) {
                if (con.firstName == nameSearch || con.lastName == nameSearch) {
                    out << con.firstName << " " << con.lastName << " " << con.phone << std::endl;

                    ++cnt;
                }
            }
            out << "-> " << cnt << std::endl;

        }
    }
    return true;
}

#ifndef __PROGTEST__

int main() {
    ostringstream oss;
    oss.str("");
    assert(isPhoneValid("776100661"));
    assert (report("/home/olixandr/CLionProjects/Pa2/hw_cv/cv_3/tests//test0_in.txt", oss) == true);
    assert (oss.str() ==
            "John Christescu 258452362\n"
            "John Harmson 861647702\n"
            "-> 2\n"
            "-> 0\n"
            "Josh Dakhov 264112084\n"
            "Dakhov Speechley 865216101\n"
            "-> 2\n"
            "John Harmson 861647702\n"
            "-> 1\n");

    oss.str("");
    assert (report("tests/test1_in.txt", oss) == false);
    return 0;
}

#endif /* __PROGTEST__ */
