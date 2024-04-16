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
    virtual void print(std::ostream &os) const = 0;

    virtual ~CComponent() = default;

    friend std::ostream &operator<<(std::ostream &os, const CComponent &component) {
        component.print(os);
        return os;
    }

    virtual std::shared_ptr<CComponent> getShared() const = 0;

    size_t m_Print = 0;

};

class CCPU : public CComponent {
public:
    CCPU(int numberOfCores, int frequency) : m_NumberOFCores(numberOfCores), m_Frequency(frequency) {};

    void print(std::ostream &ostream) const override {
        ostream << "CPU, " << m_NumberOFCores << " cores @ " << m_Frequency << "MHz\n";
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

    void print(std::ostream &ostream) const override {
        ostream << "Memory, " << m_Memory << " MiB\n";
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
        CPartition(int size, std::string id) : m_Size(size), m_Id(std::move(id)) {};
        std::string m_Id;
        int m_Size;
    };


    void print(std::ostream &ostream) const override {
        ostream << m_Type << ", " << m_Size << " GiB\n";
        size_t cnt = 0;
        size_t count = m_Partitions.size();
        for (const std::shared_ptr<CPartition> &partition: m_Partitions) {
            for (int i = 0; i < 2; ++i) {
                if (i < m_Print) {
                    ostream << "| ";
                } else {
                    ostream << "  ";
                }
            }
            if (++cnt == count) {
                ostream << "\\-";
            } else {
                ostream << "+-";
            }

            ostream << "[" << cnt - 1 << "]: " << partition->m_Size << " GiB, " << partition->m_Id << "\n";
        }
    }

    CDisk &addPartition(int size, std::string id) {
        std::shared_ptr<CPartition> sharedPtr = std::make_shared<CPartition>(CPartition(size, std::move(id)));
        m_Partitions.push_back(sharedPtr);
        return *this;
    }


    std::shared_ptr<CComponent> getShared() const override {
        auto sharedPtr = std::make_shared<CDisk>(*this);
        sharedPtr->m_Partitions.clear();
        for (const auto &partition: m_Partitions) {
            sharedPtr->m_Partitions.push_back(std::make_shared<CPartition>(*partition));
        }
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

class CComputer {
public:
    CComputer(std::string name) : m_Name(std::move(name)) {};

    CComputer &addComponent(const CComponent &component) {
        m_Components.push_back(component.getShared());
        return *this;
    }


    CComputer &addAddress(const std::string &address) {
        const std::string &newString = address;
        m_Address.push_back(newString);
        return *this;
    }

    const std::string &getName() {
        return m_Name;
    }

    std::shared_ptr<CComputer> getShared() const {
        auto sharePtr = std::make_shared<CComputer>(*this);
        sharePtr->m_Components.clear();
        for (const auto &component: m_Components) {
            sharePtr->m_Components.push_back(component->getShared());
        }

        return sharePtr;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const CComputer &computer) {
        ostream << "Host: " << computer.m_Name << "\n";
        size_t cnt = 0;
        size_t count = computer.m_Components.size() + computer.m_Address.size();
        for (const std::string &address: computer.m_Address) {
            if (computer.maxDepth != computer.currentDepth) {
                ostream << "| ";
            } else if (computer.maxDepth != 0) {
                ostream << "  ";
            }
            if (++cnt == count) {
                ostream << "\\-";
            } else {
                ostream << "+-";
            }
            ostream << address << "\n";
        }

        cnt = 0;
        count = computer.m_Components.size();

        for (const std::shared_ptr<CComponent> &component: computer.m_Components) {
            if (computer.maxDepth != computer.currentDepth) {
                ostream << "| ";
            } else if (computer.maxDepth != 0) {
                ostream << "  ";
            }

            if (++cnt == count) {
                ostream << "\\-";
            } else {
                ostream << "+-";
            }
            component->m_Print = computer.maxDepth - computer.currentDepth;
            ostream << *component;
        }
        return ostream;
    }

    std::vector<std::shared_ptr<CComponent>> getComponents() const {
        return m_Components;
    }

    void setPrintDepth(const size_t depth, const size_t max) {
        currentDepth = depth;
        maxDepth = max;
    }

private:
    std::string m_Name;
    std::vector<std::shared_ptr<CComponent>> m_Components;
    std::vector<std::string> m_Address;
    size_t currentDepth = 0;
    size_t maxDepth = 0;
};

class CNetwork {
public:
    CNetwork(std::string name) : m_Name(std::move(name)), counter(0) {}

    CNetwork &addComputer(const CComputer &computer) {
        m_Computers.push_back(new CComputer(computer));
        counter++;
        return *this;
    }

    CComputer *findComputer(const std::string &name) {
        for (int i = 0; i < counter; i++) {
            if (m_Computers[i]->getName() == name) {
                return m_Computers[i];
            }
        }
        return nullptr;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const CNetwork &network) {
        ostream << "Network: " << network.m_Name << "\n";
        size_t cnt = 0;

        for (int i = 0; i < network.counter; i++) {

            cnt++;
            network.m_Computers[i]->setPrintDepth(cnt, network.counter);

            if (cnt == network.counter) {
                ostream << "\\-";
            } else {
                ostream << "+-";
            }
            ostream << network.m_Computers[i];

            network.m_Computers[i]->setPrintDepth(0, 0);
        }
        return ostream;
    }

private:
    std::vector<CComputer> m_Computers;
    std::string m_Name;
    std::size_t counter;
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
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
