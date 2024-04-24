#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include <cstring>

#endif /* __PROGTEST__ */

class CComponent {
public:
    virtual void print(std::ostream &ostream, bool isLastComputer, bool isLastComponent, bool isOnlyComputer) const = 0;

    virtual ~CComponent() = default;

    virtual std::shared_ptr<CComponent> getShared() const = 0;

};

class CCPU : public CComponent {
public:
    CCPU(int numberOfCores, int frequency) : m_NumberOFCores(numberOfCores), m_Frequency(frequency) {};

    void print(std::ostream &ostream, bool isLastComputer, bool isLastComponent, bool isOnlyComputer) const override {
        if (!isOnlyComputer) {
            ostream << (isLastComputer ? "  " : "| ");
        }
        ostream << (isLastComponent ? "\\-" : "+-") << "CPU, " << m_NumberOFCores
                << " cores @ " << m_Frequency << "MHz\n";
    }

    std::shared_ptr<CComponent> getShared() const override {
        return std::make_shared<CCPU>(*this);
    }

private:
    int m_NumberOFCores;
    int m_Frequency;
};

class CMemory : public CComponent {
public:
    CMemory(int memory) : m_Memory(memory) {};

    void print(std::ostream &ostream, bool isLastComputer, bool isLastComponent, bool isOnlyComputer) const override {
        if (!isOnlyComputer) {
            ostream << (isLastComputer ? "  " : "| ");
        }
        ostream << (isLastComponent ? "\\-" : "+-") << "Memory, " << m_Memory
                << " MiB\n";
    }

    std::shared_ptr<CComponent> getShared() const override {
        return std::make_shared<CMemory>(*this);
    }

private:
    int m_Memory;
};

class CDisk : public CComponent {
public:
    CDisk(std::string type, int size) : m_Type(std::move(type)), m_Size(size) {};

    struct CPartition {
        CPartition(int size, std::string id) : m_Id(std::move(id)), m_Size(size) {};
        std::string m_Id;
        int m_Size;
    };


    void print(std::ostream &ostream, bool isLastComputer, bool isLastComponent, bool isOnlyComputer) const override {
        if (!isOnlyComputer) {
            ostream << (isLastComputer ? "  " : "| ");
        }
        ostream << (isLastComponent ? "\\-" : "+-") << m_Type << ", " << m_Size << " GiB\n";
        size_t cnt = 0;
        for (const std::shared_ptr<CPartition> &partition: m_Partitions) {
            cnt++;
            if (!isOnlyComputer) {
                ostream << (isLastComputer ? "  " : "| ");
            }
            ostream << (isLastComponent ? "  " : "| ")
                    << (cnt == m_Partitions.size() ? "\\-" : "+-") << "[" << cnt - 1 << "]: "
                    << partition->m_Size << " GiB, " << partition->m_Id << "\n";
        }
    }

    CDisk &addPartition(int size, std::string id) {
        std::shared_ptr<CPartition> sharedPtr = std::make_shared<CPartition>(CPartition(size, std::move(id)));
        m_Partitions.push_back(sharedPtr);
        return *this;
    }


    std::shared_ptr<CComponent> getShared() const override {
        auto sharedPtr = std::make_shared<CDisk>(*this);
        return sharedPtr;
    }

    static const std::string SSD;
    static const std::string MAGNETIC;


private:
    std::string m_Type;
    int m_Size;
    std::vector<std::shared_ptr<CPartition>> m_Partitions;
};

const std::string CDisk::SSD = "SSD";
const std::string CDisk::MAGNETIC = "HDD";

class CComputer : public CComponent {
public:
    CComputer(std::string name) : m_Name(std::move(name)) {};

    CComputer &addComponent(const CComponent &component) {
        m_Components.push_back(component.getShared());
        return *this;
    }

    std::shared_ptr<CComponent> getShared() const override {
        auto sharedPtr = std::make_shared<CComputer>(*this);
        return sharedPtr;
    }


    CComputer &addAddress(const std::string &address) {
        const std::string &newString = address;
        m_Address.push_back(newString);
        return *this;
    }

    const std::string &getName() {
        return m_Name;
    }


    friend std::ostream &operator<<(std::ostream &ostream, const CComputer &computer) {
        computer.print(ostream, false, false, true);
        return ostream;
    }

    void print(std::ostream &ostream, bool isLastComputer, bool isLastComponent, bool isOnlyComputer) const override {
        if (!isOnlyComputer) ostream << (isLastComputer ? "\\-" : "+-");
        ostream << "Host: " << m_Name << "\n";
        size_t cnt = 0;
        for (const std::string &address: m_Address) {
            if (!isOnlyComputer)ostream << (isLastComputer ? "  " : "| ");
            ostream << (++cnt == m_Components.size() + m_Address.size() ? "\\-" : "+-") << address << "\n";
        }
        cnt = 0;
        for (const std::shared_ptr<CComponent> &component: m_Components) {
            component->print(ostream, isLastComputer, ++cnt == m_Components.size(), isOnlyComputer);
        }
    }

private:
    std::string m_Name;
    std::vector<std::shared_ptr<CComponent>> m_Components;
    std::vector<std::string> m_Address;
};

class CNetwork : public CComponent {
public:
    CNetwork(std::string name) : m_Name(std::move(name)) {}

    std::shared_ptr<CComponent> getShared() const override {
        return std::make_shared<CNetwork>(*this);
    }

    CNetwork &addComputer(const CComputer &computer) {
        m_Computers.push_back(computer);
        return *this;
    }

    CComputer *findComputer(const std::string &name) {
        for (size_t i = 0; i < m_Computers.size(); i++) {
            if (m_Computers[i].getName() == name) {
                return &m_Computers[i];
            }
        }
        return nullptr;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const CNetwork &network) {
        network.print(ostream, false, false, false);
        return ostream;
    }

    void print(std::ostream &ostream, bool isLastComputer, bool isLastComponent, bool isOnlyComputer) const override {
        ostream << "Network: " << m_Name << "\n";
        for (size_t i = 0; i < m_Computers.size(); i++) {
            m_Computers[i].print(ostream, i + 1 == m_Computers.size(), false, false);
        }
    }

private:
    std::vector<CComputer> m_Computers;
    std::string m_Name;
};


#ifndef __PROGTEST__

template<typename T_>
std::string toString(const T_ &x) {
    std::ostringstream oss;
    oss << x;
    std::cout << x << std::endl;

    return oss.str();
}

int main() {
    CNetwork n("FIT network");
    n.addComputer(
            CComputer("progtest.fit.cvut.cz").
                    addAddress("147.32.232.142").
                    addComponent(CCPU(8, 2400)).
                    addComponent(CCPU(8, 1200)).
                    addComponent(CDisk(CDisk::MAGNETIC, 1500).
                    addPartition(50, "/").
                    addPartition(5, "/boot").
                    addPartition(1000, "/var")).
                    addComponent(CDisk(CDisk::SSD, 60).
                    addPartition(60, "/data")).
                    addComponent(CMemory(2000)).
                    addComponent(CMemory(2000))).
            addComputer(
            CComputer("courses.fit.cvut.cz").
                    addAddress("147.32.232.213").
                    addComponent(CCPU(4, 1600)).
                    addComponent(CMemory(4000)).
                    addComponent(CDisk(CDisk::MAGNETIC, 2000).
                    addPartition(100, "/").
                    addPartition(1900, "/data"))).
            addComputer(
            CComputer("imap.fit.cvut.cz").
                    addAddress("147.32.232.238").
                    addComponent(CCPU(4, 2500)).
                    addAddress("2001:718:2:2901::238").
                    addComponent(CMemory(8000)));
    assert (toString(n) ==
            "Network: FIT network\n"
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
            "  \\-Memory, 8000 MiB\n");
    CNetwork x = n;
    auto c = x.findComputer("imap.fit.cvut.cz");
    assert (toString(*c) ==
            "Host: imap.fit.cvut.cz\n"
            "+-147.32.232.238\n"
            "+-2001:718:2:2901::238\n"
            "+-CPU, 4 cores @ 2500MHz\n"
            "\\-Memory, 8000 MiB\n");
    c->addComponent(CDisk(CDisk::MAGNETIC, 1000).
            addPartition(100, "system").
            addPartition(200, "WWW").
            addPartition(700, "mail"));
    assert (toString(x) ==
            "Network: FIT network\n"
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
            "  +-Memory, 8000 MiB\n"
            "  \\-HDD, 1000 GiB\n"
            "    +-[0]: 100 GiB, system\n"
            "    +-[1]: 200 GiB, WWW\n"
            "    \\-[2]: 700 GiB, mail\n");
    assert (toString(n) ==
            "Network: FIT network\n"
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
            "  \\-Memory, 8000 MiB\n");
    n.addComputer(CComputer("vm01.fit.cvut.cz").
            addAddress("147.32.232.232").
            addComponent(CCPU(32, 4000)).
            addComponent(CMemory(32768)).
            addComponent(CDisk(CDisk::MAGNETIC, 3000).
            addPartition(500, "SYSTEM").
            addPartition(2000, "DATA").
            addPartition(500, "BACKUP")).
            addComponent(CNetwork("dummy00").
            addComputer(CComputer("testing.fit.cvut.cz").
            addAddress("192.168.1.1").
            addComponent(CCPU(1, 300)).
            addComponent(CMemory(256)))).
            addComponent(CMemory(16384)).
            addComponent(CNetwork("vnet00")));
    assert (toString(n) ==
            "Network: FIT network\n"
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
            "+-Host: imap.fit.cvut.cz\n"
            "| +-147.32.232.238\n"
            "| +-2001:718:2:2901::238\n"
            "| +-CPU, 4 cores @ 2500MHz\n"
            "| \\-Memory, 8000 MiB\n"
            "\\-Host: vm01.fit.cvut.cz\n"
            "  +-147.32.232.232\n"
            "  +-CPU, 32 cores @ 4000MHz\n"
            "  +-Memory, 32768 MiB\n"
            "  +-HDD, 3000 GiB\n"
            "  | +-[0]: 500 GiB, SYSTEM\n"
            "  | +-[1]: 2000 GiB, DATA\n"
            "  | \\-[2]: 500 GiB, BACKUP\n"
            "  +-Network: dummy00\n"
            "  | \\-Host: testing.fit.cvut.cz\n"
            "  |   +-192.168.1.1\n"
            "  |   +-CPU, 1 cores @ 300MHz\n"
            "  |   \\-Memory, 256 MiB\n"
            "  +-Memory, 16384 MiB\n"
            "  \\-Network: vnet00\n");
    auto vmnet = n.findNetwork("vnet00");
    vmnet->addComputer(CComputer("monitor1.fit.cvut.cz").
            addAddress("147.32.232.254").
            addComponent(CCPU(2, 1500)).
            addComponent(CMemory(4096)).
            addComponent(CDisk(CDisk::MAGNETIC, 750).
            addPartition(100, "root").
            addPartition(600, "log")));
    assert (toString(*vmnet) ==
            "Network: vnet00\n"
            "\\-Host: monitor1.fit.cvut.cz\n"
            "  +-147.32.232.254\n"
            "  +-CPU, 2 cores @ 1500MHz\n"
            "  +-Memory, 4096 MiB\n"
            "  \\-HDD, 750 GiB\n"
            "    +-[0]: 100 GiB, root\n"
            "    \\-[1]: 600 GiB, log\n");
    assert (toString(n) ==
            "Network: FIT network\n"
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
            "+-Host: imap.fit.cvut.cz\n"
            "| +-147.32.232.238\n"
            "| +-2001:718:2:2901::238\n"
            "| +-CPU, 4 cores @ 2500MHz\n"
            "| \\-Memory, 8000 MiB\n"
            "\\-Host: vm01.fit.cvut.cz\n"
            "  +-147.32.232.232\n"
            "  +-CPU, 32 cores @ 4000MHz\n"
            "  +-Memory, 32768 MiB\n"
            "  +-HDD, 3000 GiB\n"
            "  | +-[0]: 500 GiB, SYSTEM\n"
            "  | +-[1]: 2000 GiB, DATA\n"
            "  | \\-[2]: 500 GiB, BACKUP\n"
            "  +-Network: dummy00\n"
            "  | \\-Host: testing.fit.cvut.cz\n"
            "  |   +-192.168.1.1\n"
            "  |   +-CPU, 1 cores @ 300MHz\n"
            "  |   \\-Memory, 256 MiB\n"
            "  +-Memory, 16384 MiB\n"
            "  \\-Network: vnet00\n"
            "    \\-Host: monitor1.fit.cvut.cz\n"
            "      +-147.32.232.254\n"
            "      +-CPU, 2 cores @ 1500MHz\n"
            "      +-Memory, 4096 MiB\n"
            "      \\-HDD, 750 GiB\n"
            "        +-[0]: 100 GiB, root\n"
            "        \\-[1]: 600 GiB, log\n");
    vmnet->addComputer(n.findComputer("monitor1.fit.cvut.cz")
                               ->duplicate({std::pair<std::string, std::string>("monitor1.fit.cvut.cz",
                                                                                "monitor2.fit.cvut.cz")}));
    assert (toString(*vmnet) ==
            "Network: vnet00\n"
            "+-Host: monitor1.fit.cvut.cz\n"
            "| +-147.32.232.254\n"
            "| +-CPU, 2 cores @ 1500MHz\n"
            "| +-Memory, 4096 MiB\n"
            "| \\-HDD, 750 GiB\n"
            "|   +-[0]: 100 GiB, root\n"
            "|   \\-[1]: 600 GiB, log\n"
            "\\-Host: monitor2.fit.cvut.cz\n"
            "  +-147.32.232.254\n"
            "  +-CPU, 2 cores @ 1500MHz\n"
            "  +-Memory, 4096 MiB\n"
            "  \\-HDD, 750 GiB\n"
            "    +-[0]: 100 GiB, root\n"
            "    \\-[1]: 600 GiB, log\n");
    n.addComputer(n.findComputer("vm01.fit.cvut.cz")
                          ->duplicate(
                                  {
                                          std::pair<std::string, std::string>("monitor1.fit.cvut.cz",
                                                                              "monitor3.fit.cvut.cz"),
                                          std::pair<std::string, std::string>("monitor2.fit.cvut.cz",
                                                                              "monitor4.fit.cvut.cz"),
                                          std::pair<std::string, std::string>("vm01.fit.cvut.cz", "vm02.fit.cvut.cz")
                                  }));
    assert (toString(n) ==
            "Network: FIT network\n"
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
            "+-Host: imap.fit.cvut.cz\n"
            "| +-147.32.232.238\n"
            "| +-2001:718:2:2901::238\n"
            "| +-CPU, 4 cores @ 2500MHz\n"
            "| \\-Memory, 8000 MiB\n"
            "+-Host: vm01.fit.cvut.cz\n"
            "| +-147.32.232.232\n"
            "| +-CPU, 32 cores @ 4000MHz\n"
            "| +-Memory, 32768 MiB\n"
            "| +-HDD, 3000 GiB\n"
            "| | +-[0]: 500 GiB, SYSTEM\n"
            "| | +-[1]: 2000 GiB, DATA\n"
            "| | \\-[2]: 500 GiB, BACKUP\n"
            "| +-Network: dummy00\n"
            "| | \\-Host: testing.fit.cvut.cz\n"
            "| |   +-192.168.1.1\n"
            "| |   +-CPU, 1 cores @ 300MHz\n"
            "| |   \\-Memory, 256 MiB\n"
            "| +-Memory, 16384 MiB\n"
            "| \\-Network: vnet00\n"
            "|   +-Host: monitor1.fit.cvut.cz\n"
            "|   | +-147.32.232.254\n"
            "|   | +-CPU, 2 cores @ 1500MHz\n"
            "|   | +-Memory, 4096 MiB\n"
            "|   | \\-HDD, 750 GiB\n"
            "|   |   +-[0]: 100 GiB, root\n"
            "|   |   \\-[1]: 600 GiB, log\n"
            "|   \\-Host: monitor2.fit.cvut.cz\n"
            "|     +-147.32.232.254\n"
            "|     +-CPU, 2 cores @ 1500MHz\n"
            "|     +-Memory, 4096 MiB\n"
            "|     \\-HDD, 750 GiB\n"
            "|       +-[0]: 100 GiB, root\n"
            "|       \\-[1]: 600 GiB, log\n"
            "\\-Host: vm02.fit.cvut.cz\n"
            "  +-147.32.232.232\n"
            "  +-CPU, 32 cores @ 4000MHz\n"
            "  +-Memory, 32768 MiB\n"
            "  +-HDD, 3000 GiB\n"
            "  | +-[0]: 500 GiB, SYSTEM\n"
            "  | +-[1]: 2000 GiB, DATA\n"
            "  | \\-[2]: 500 GiB, BACKUP\n"
            "  +-Network: dummy00\n"
            "  | \\-Host: testing.fit.cvut.cz\n"
            "  |   +-192.168.1.1\n"
            "  |   +-CPU, 1 cores @ 300MHz\n"
            "  |   \\-Memory, 256 MiB\n"
            "  +-Memory, 16384 MiB\n"
            "  \\-Network: vnet00\n"
            "    +-Host: monitor3.fit.cvut.cz\n"
            "    | +-147.32.232.254\n"
            "    | +-CPU, 2 cores @ 1500MHz\n"
            "    | +-Memory, 4096 MiB\n"
            "    | \\-HDD, 750 GiB\n"
            "    |   +-[0]: 100 GiB, root\n"
            "    |   \\-[1]: 600 GiB, log\n"
            "    \\-Host: monitor4.fit.cvut.cz\n"
            "      +-147.32.232.254\n"
            "      +-CPU, 2 cores @ 1500MHz\n"
            "      +-Memory, 4096 MiB\n"
            "      \\-HDD, 750 GiB\n"
            "        +-[0]: 100 GiB, root\n"
            "        \\-[1]: 600 GiB, log\n");
    vmnet->addComputer(n.findComputer("vm02.fit.cvut.cz")
                               ->duplicate(
                                       {
                                               std::pair<std::string, std::string>("monitor3.fit.cvut.cz",
                                                                                   "monitor5.fit.cvut.cz"),
                                               std::pair<std::string, std::string>("monitor4.fit.cvut.cz",
                                                                                   "monitor6.fit.cvut.cz"),
                                               std::pair<std::string, std::string>("vm02.fit.cvut.cz",
                                                                                   "vm03.fit.cvut.cz"),
                                               std::pair<std::string, std::string>("vnet00", "vnet01")
                                       }));
    assert (toString(*vmnet) ==
            "Network: vnet00\n"
            "+-Host: monitor1.fit.cvut.cz\n"
            "| +-147.32.232.254\n"
            "| +-CPU, 2 cores @ 1500MHz\n"
            "| +-Memory, 4096 MiB\n"
            "| \\-HDD, 750 GiB\n"
            "|   +-[0]: 100 GiB, root\n"
            "|   \\-[1]: 600 GiB, log\n"
            "+-Host: monitor2.fit.cvut.cz\n"
            "| +-147.32.232.254\n"
            "| +-CPU, 2 cores @ 1500MHz\n"
            "| +-Memory, 4096 MiB\n"
            "| \\-HDD, 750 GiB\n"
            "|   +-[0]: 100 GiB, root\n"
            "|   \\-[1]: 600 GiB, log\n"
            "\\-Host: vm03.fit.cvut.cz\n"
            "  +-147.32.232.232\n"
            "  +-CPU, 32 cores @ 4000MHz\n"
            "  +-Memory, 32768 MiB\n"
            "  +-HDD, 3000 GiB\n"
            "  | +-[0]: 500 GiB, SYSTEM\n"
            "  | +-[1]: 2000 GiB, DATA\n"
            "  | \\-[2]: 500 GiB, BACKUP\n"
            "  +-Network: dummy00\n"
            "  | \\-Host: testing.fit.cvut.cz\n"
            "  |   +-192.168.1.1\n"
            "  |   +-CPU, 1 cores @ 300MHz\n"
            "  |   \\-Memory, 256 MiB\n"
            "  +-Memory, 16384 MiB\n"
            "  \\-Network: vnet01\n"
            "    +-Host: monitor5.fit.cvut.cz\n"
            "    | +-147.32.232.254\n"
            "    | +-CPU, 2 cores @ 1500MHz\n"
            "    | +-Memory, 4096 MiB\n"
            "    | \\-HDD, 750 GiB\n"
            "    |   +-[0]: 100 GiB, root\n"
            "    |   \\-[1]: 600 GiB, log\n"
            "    \\-Host: monitor6.fit.cvut.cz\n"
            "      +-147.32.232.254\n"
            "      +-CPU, 2 cores @ 1500MHz\n"
            "      +-Memory, 4096 MiB\n"
            "      \\-HDD, 750 GiB\n"
            "        +-[0]: 100 GiB, root\n"
            "        \\-[1]: 600 GiB, log\n");
    n.findComputer("vm02.fit.cvut.cz")
            ->findNetwork("vnet00")
            ->addComputer(n.findComputer("vm01.fit.cvut.cz")
                                  ->duplicate(
                                          {
                                                  std::pair<std::string, std::string>("monitor1.fit.cvut.cz",
                                                                                      "monitor11.fit.cvut.cz"),
                                                  std::pair<std::string, std::string>("monitor2.fit.cvut.cz",
                                                                                      "monitor12.fit.cvut.cz"),
                                                  std::pair<std::string, std::string>("vm01.fit.cvut.cz",
                                                                                      "vm11.fit.cvut.cz"),
                                                  std::pair<std::string, std::string>("vm03.fit.cvut.cz",
                                                                                      "vm13.fit.cvut.cz"),
                                                  std::pair<std::string, std::string>("vnet00", "vnet10"),
                                                  std::pair<std::string, std::string>("vnet01", "vnet11")
                                          }));
    assert (toString(n) ==
            "Network: FIT network\n"
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
            "+-Host: imap.fit.cvut.cz\n"
            "| +-147.32.232.238\n"
            "| +-2001:718:2:2901::238\n"
            "| +-CPU, 4 cores @ 2500MHz\n"
            "| \\-Memory, 8000 MiB\n"
            "+-Host: vm01.fit.cvut.cz\n"
            "| +-147.32.232.232\n"
            "| +-CPU, 32 cores @ 4000MHz\n"
            "| +-Memory, 32768 MiB\n"
            "| +-HDD, 3000 GiB\n"
            "| | +-[0]: 500 GiB, SYSTEM\n"
            "| | +-[1]: 2000 GiB, DATA\n"
            "| | \\-[2]: 500 GiB, BACKUP\n"
            "| +-Network: dummy00\n"
            "| | \\-Host: testing.fit.cvut.cz\n"
            "| |   +-192.168.1.1\n"
            "| |   +-CPU, 1 cores @ 300MHz\n"
            "| |   \\-Memory, 256 MiB\n"
            "| +-Memory, 16384 MiB\n"
            "| \\-Network: vnet00\n"
            "|   +-Host: monitor1.fit.cvut.cz\n"
            "|   | +-147.32.232.254\n"
            "|   | +-CPU, 2 cores @ 1500MHz\n"
            "|   | +-Memory, 4096 MiB\n"
            "|   | \\-HDD, 750 GiB\n"
            "|   |   +-[0]: 100 GiB, root\n"
            "|   |   \\-[1]: 600 GiB, log\n"
            "|   +-Host: monitor2.fit.cvut.cz\n"
            "|   | +-147.32.232.254\n"
            "|   | +-CPU, 2 cores @ 1500MHz\n"
            "|   | +-Memory, 4096 MiB\n"
            "|   | \\-HDD, 750 GiB\n"
            "|   |   +-[0]: 100 GiB, root\n"
            "|   |   \\-[1]: 600 GiB, log\n"
            "|   \\-Host: vm03.fit.cvut.cz\n"
            "|     +-147.32.232.232\n"
            "|     +-CPU, 32 cores @ 4000MHz\n"
            "|     +-Memory, 32768 MiB\n"
            "|     +-HDD, 3000 GiB\n"
            "|     | +-[0]: 500 GiB, SYSTEM\n"
            "|     | +-[1]: 2000 GiB, DATA\n"
            "|     | \\-[2]: 500 GiB, BACKUP\n"
            "|     +-Network: dummy00\n"
            "|     | \\-Host: testing.fit.cvut.cz\n"
            "|     |   +-192.168.1.1\n"
            "|     |   +-CPU, 1 cores @ 300MHz\n"
            "|     |   \\-Memory, 256 MiB\n"
            "|     +-Memory, 16384 MiB\n"
            "|     \\-Network: vnet01\n"
            "|       +-Host: monitor5.fit.cvut.cz\n"
            "|       | +-147.32.232.254\n"
            "|       | +-CPU, 2 cores @ 1500MHz\n"
            "|       | +-Memory, 4096 MiB\n"
            "|       | \\-HDD, 750 GiB\n"
            "|       |   +-[0]: 100 GiB, root\n"
            "|       |   \\-[1]: 600 GiB, log\n"
            "|       \\-Host: monitor6.fit.cvut.cz\n"
            "|         +-147.32.232.254\n"
            "|         +-CPU, 2 cores @ 1500MHz\n"
            "|         +-Memory, 4096 MiB\n"
            "|         \\-HDD, 750 GiB\n"
            "|           +-[0]: 100 GiB, root\n"
            "|           \\-[1]: 600 GiB, log\n"
            "\\-Host: vm02.fit.cvut.cz\n"
            "  +-147.32.232.232\n"
            "  +-CPU, 32 cores @ 4000MHz\n"
            "  +-Memory, 32768 MiB\n"
            "  +-HDD, 3000 GiB\n"
            "  | +-[0]: 500 GiB, SYSTEM\n"
            "  | +-[1]: 2000 GiB, DATA\n"
            "  | \\-[2]: 500 GiB, BACKUP\n"
            "  +-Network: dummy00\n"
            "  | \\-Host: testing.fit.cvut.cz\n"
            "  |   +-192.168.1.1\n"
            "  |   +-CPU, 1 cores @ 300MHz\n"
            "  |   \\-Memory, 256 MiB\n"
            "  +-Memory, 16384 MiB\n"
            "  \\-Network: vnet00\n"
            "    +-Host: monitor3.fit.cvut.cz\n"
            "    | +-147.32.232.254\n"
            "    | +-CPU, 2 cores @ 1500MHz\n"
            "    | +-Memory, 4096 MiB\n"
            "    | \\-HDD, 750 GiB\n"
            "    |   +-[0]: 100 GiB, root\n"
            "    |   \\-[1]: 600 GiB, log\n"
            "    +-Host: monitor4.fit.cvut.cz\n"
            "    | +-147.32.232.254\n"
            "    | +-CPU, 2 cores @ 1500MHz\n"
            "    | +-Memory, 4096 MiB\n"
            "    | \\-HDD, 750 GiB\n"
            "    |   +-[0]: 100 GiB, root\n"
            "    |   \\-[1]: 600 GiB, log\n"
            "    \\-Host: vm11.fit.cvut.cz\n"
            "      +-147.32.232.232\n"
            "      +-CPU, 32 cores @ 4000MHz\n"
            "      +-Memory, 32768 MiB\n"
            "      +-HDD, 3000 GiB\n"
            "      | +-[0]: 500 GiB, SYSTEM\n"
            "      | +-[1]: 2000 GiB, DATA\n"
            "      | \\-[2]: 500 GiB, BACKUP\n"
            "      +-Network: dummy00\n"
            "      | \\-Host: testing.fit.cvut.cz\n"
            "      |   +-192.168.1.1\n"
            "      |   +-CPU, 1 cores @ 300MHz\n"
            "      |   \\-Memory, 256 MiB\n"
            "      +-Memory, 16384 MiB\n"
            "      \\-Network: vnet10\n"
            "        +-Host: monitor11.fit.cvut.cz\n"
            "        | +-147.32.232.254\n"
            "        | +-CPU, 2 cores @ 1500MHz\n"
            "        | +-Memory, 4096 MiB\n"
            "        | \\-HDD, 750 GiB\n"
            "        |   +-[0]: 100 GiB, root\n"
            "        |   \\-[1]: 600 GiB, log\n"
            "        +-Host: monitor12.fit.cvut.cz\n"
            "        | +-147.32.232.254\n"
            "        | +-CPU, 2 cores @ 1500MHz\n"
            "        | +-Memory, 4096 MiB\n"
            "        | \\-HDD, 750 GiB\n"
            "        |   +-[0]: 100 GiB, root\n"
            "        |   \\-[1]: 600 GiB, log\n"
            "        \\-Host: vm13.fit.cvut.cz\n"
            "          +-147.32.232.232\n"
            "          +-CPU, 32 cores @ 4000MHz\n"
            "          +-Memory, 32768 MiB\n"
            "          +-HDD, 3000 GiB\n"
            "          | +-[0]: 500 GiB, SYSTEM\n"
            "          | +-[1]: 2000 GiB, DATA\n"
            "          | \\-[2]: 500 GiB, BACKUP\n"
            "          +-Network: dummy00\n"
            "          | \\-Host: testing.fit.cvut.cz\n"
            "          |   +-192.168.1.1\n"
            "          |   +-CPU, 1 cores @ 300MHz\n"
            "          |   \\-Memory, 256 MiB\n"
            "          +-Memory, 16384 MiB\n"
            "          \\-Network: vnet11\n"
            "            +-Host: monitor5.fit.cvut.cz\n"
            "            | +-147.32.232.254\n"
            "            | +-CPU, 2 cores @ 1500MHz\n"
            "            | +-Memory, 4096 MiB\n"
            "            | \\-HDD, 750 GiB\n"
            "            |   +-[0]: 100 GiB, root\n"
            "            |   \\-[1]: 600 GiB, log\n"
            "            \\-Host: monitor6.fit.cvut.cz\n"
            "              +-147.32.232.254\n"
            "              +-CPU, 2 cores @ 1500MHz\n"
            "              +-Memory, 4096 MiB\n"
            "              \\-HDD, 750 GiB\n"
            "                +-[0]: 100 GiB, root\n"
            "                \\-[1]: 600 GiB, log\n");
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */

#endif /* __PROGTEST__ */
