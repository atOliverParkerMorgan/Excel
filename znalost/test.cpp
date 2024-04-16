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

int main ( void )
{
    string x =   "Network: FIT network\n"
                 "+-Host: progtest.fit.cvut.cz\n"
                 "| +-147.32.232.142\n"
                 "| +-CPU, 8 cores @ 2400MHz\n"
                 "| +-CPU, 8 cores @ 1200MHz\n"
                 "| +-HDD, 1500 GiB\n"
                 "| | +-[0]: 50 GiB, /\n"
                 "| | +-[1]: 5 GiB, /boot\n"
                 "| | \\-[2]: 1000 GiB, /var\n"
                 "| +-SSD, 60 GiB\n"
                 "| | \\-[0]: 60 GiB, /data\n"
                 "| +-Memory, 2000 MiB\n"
                 "| \\-Memory, 2000 MiB\n"
                 "+-Host: courses.fit.cvut.cz\n"
                 "| +-147.32.232.213\n"
                 "| +-CPU, 4 cores @ 1600MHz\n"
                 "| +-Memory, 4000 MiB\n"
                 "| \\-HDD, 2000 GiB\n"
                 "|   +-[0]: 100 GiB, /\n"
                 "|   \\-[1]: 1900 GiB, /data\n"
                 "\\-Host: imap.fit.cvut.cz\n"
                 "  +-147.32.232.238\n"
                 "  +-2001:718:2:2901::238\n"
                 "  +-CPU, 4 cores @ 2500MHz\n"
                 "  \\-Memory, 8000 MiB\n";
    std::cout << x << std::endl;
    return 0;
}