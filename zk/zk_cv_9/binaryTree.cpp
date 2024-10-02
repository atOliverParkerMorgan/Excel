#include<iostream>
#include<string>
#include<cassert>
#include<sstream>
#include <utility>

using namespace std;

class CTree {

private:
    class CNode {
    public:
        CNode(string key, string val)
                : m_Key(std::move(key)), m_Val(std::move(val)) {}

        string m_Key, m_Val;
        CNode *m_L = nullptr, *m_R = nullptr;
        CNode *m_NextOrder = nullptr;
    };

    class Iterator {
    public:
        explicit Iterator(CNode *current) : m_Current(current) {};

        bool friend operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.m_Current == rhs.m_Current;
        }

        Iterator operator++() {
            if (m_Current)
                m_Current = m_Current->m_NextOrder;
            return *this;
        }

        pair<string, string> operator*() const {
            return {m_Current->m_Key, m_Current->m_Val};
        }

        CNode *m_Current;
    };

public:
    CTree() = default;

    CTree(const CTree &src) = delete;

    CTree &operator=(const CTree &src) = delete;

    ~CTree() {
        while (m_First) {
            CNode *tmp = m_First;
            m_First = m_First->m_NextOrder;
            delete tmp;
        }
    }

    bool isSet(const string &key) {
        CNode **currentNode = &m_Root;
        while (*currentNode) {
            if ((*currentNode)->m_Key == key) {
                return true;
            } else if ((*currentNode)->m_Key > key) {
                currentNode = &(*currentNode)->m_L;
            } else if ((*currentNode)->m_Key < key) {
                currentNode = &(*currentNode)->m_R;
            }
        }
        return false;
    }

    bool insert(const string &key, const string &val) {

        CNode **currentNode = &m_Root;
        while (*currentNode) {
            if ((*currentNode)->m_Key == key) {
                return false;
            } else if ((*currentNode)->m_Key > key) {
                currentNode = &(*currentNode)->m_L;
            } else if ((*currentNode)->m_Key < key) {
                currentNode = &(*currentNode)->m_R;
            }
        }

        *currentNode = new CNode(key, val);
        if (!m_First) {
            m_First = *currentNode;
        } else {
            m_Last->m_NextOrder = *currentNode;
        }
        m_Last = *currentNode;

        return true;
    }


    friend ostream &operator<<(ostream &os, const CTree &src) {
        CNode *tmp = src.m_First;
        os << "{";
        for (auto it = src.begin(); it != src.end(); ++it) {
            os << tmp->m_Key << " => " << tmp->m_Val;
            if (tmp->m_NextOrder)
                os << ", ";
            tmp = tmp->m_NextOrder;
        }
        os << "}";
        return os;
    }

    CNode *m_Root = nullptr;
    CNode *m_First = nullptr, *m_Last = nullptr;

    Iterator begin() const {
        return Iterator(m_First);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
};

int main(void) {
    CTree t;
    stringstream ss;
    ss << t;
    assert(ss.str() == "{}");
    ss.clear();
    ss.str("");
    assert(t.insert("PA1", "done"));
    assert(t.m_First == t.m_Last);
    assert(t.m_First->m_Key == "PA1");
    assert(!t.isSet("UOS"));
    assert(t.insert("PA2", "fail"));
    assert(t.insert("UOS", "funny"));

    ss << t;
    // cout << t << endl;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny}");
    ss.clear();
    ss.str("");


    assert(t.m_Root->m_L == nullptr);
    assert(t.m_Last->m_Key == "UOS");
    assert(t.m_Root->m_R->m_Key == "PA2");
    assert(t.m_Root->m_R->m_L == nullptr);
    assert(t.m_Root->m_R->m_R->m_Key == "UOS");
    assert(t.m_First->m_NextOrder->m_NextOrder == t.m_Last);
    assert(t.isSet("PA2"));

    assert(t.insert("CAO", "lul"));
    assert(t.insert("LIN", "F"));
    assert(t.m_Root->m_L->m_Key == "CAO");
    assert(t.m_Root->m_L->m_L == nullptr);
    assert(t.m_Root->m_L->m_R->m_Key == "LIN");
    assert(t.m_Last == t.m_Root->m_L->m_R);
    assert(t.m_Root->m_L->m_R->m_L == nullptr);
    assert(t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder = t.m_Root->m_L->m_R);
    assert(t.isSet("LIN"));

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F}");
    ss.clear();
    ss.str("");

    assert(t.insert("SAP", "shit"));
    assert(t.m_Root->m_R->m_R->m_L->m_Key == "SAP");
    assert(t.m_Last == t.m_Root->m_R->m_R->m_L);


    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit}");
    ss.clear();
    ss.str("");

    assert(!t.isSet("PA3"));
    assert(t.isSet("LIN"));
    assert(t.isSet("SAP"));

    return 0;
}