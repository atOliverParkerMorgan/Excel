#include <string>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

class CHashSet {
private:

    class CItem {
    public:
        explicit CItem(std::string key)
                : m_Key(std::move(key)) {}

        std::string m_Key;
        CItem *m_NextHash = nullptr;
        CItem *m_NextOrder = nullptr;
        CItem *m_PrevOrder = nullptr;
    };

    friend class CHashSetTest;

public:
    size_t hash(const std::string &key) const {
        return std::hash<std::string>{}(key) % m_Size;
    }

    CHashSet(int m) : m_Table(nullptr), m_Size(m), m_First(nullptr), m_Last(nullptr) {
        m_Table = new CItem *[m];
        for (int i = 0; i < m; i++)
            m_Table[i] = nullptr;
    }

    ~CHashSet() {
        while (m_First) {
            CItem *tmp = m_First;
            m_First = m_First->m_NextOrder;
            delete tmp;
        }
    }

    CHashSet(CHashSet &) = delete;

    CHashSet &operator=(CHashSet &) = delete;

    bool insert(const std::string &key) {
        size_t index = hash(key);
        auto *newItem = new CItem(key);

        CItem **current = &m_Table[index];
        while (*current) {
            if ((*current)->m_Key == key) {
                delete newItem;
                return false;
            }
            current = &(*current)->m_NextHash;
        }

        if (!m_First) {
            m_First = newItem;
        } else {
            m_Last->m_NextOrder = newItem;

        }
        newItem->m_PrevOrder = m_Last;
        m_Last = newItem;

        *current = newItem;

        return true;
    }

    bool contains(const std::string &key) {
        size_t index = hash(key);

        CItem *tmp = m_Table[index];
        while (tmp) {
            if (tmp->m_Key == key) return true;
            tmp = tmp->m_NextHash;
        }
        return false;
    }

    bool remove(const std::string &key) {
        size_t index = hash(key);

        CItem **tmp = &m_Table[index];
        while (*tmp) {

            if ((*tmp)->m_Key == key) {
                if ((*tmp)->m_PrevOrder) {
                    (*tmp)->m_PrevOrder->m_NextOrder = (*tmp)->m_NextOrder;
                } else {
                    m_First = (*tmp)->m_NextOrder;
                }
                if ((*tmp)->m_NextOrder) {
                    (*tmp)->m_NextOrder->m_PrevOrder = (*tmp)->m_PrevOrder;
                } else {
                    m_Last = (*tmp)->m_PrevOrder;
                }
                CItem *cpy = *tmp;
                *tmp = (*tmp)->m_NextHash;
                delete cpy;
                return true;
            }
            tmp = &(*tmp)->m_NextHash;
        }
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, const CHashSet &table) {
        os << "{";
        CItem *tmp = table.m_First;
        while (tmp) {
            os << tmp->m_Key;
            if (tmp->m_NextOrder) {
                os << ", ";
            }
            tmp = tmp->m_NextOrder;
        }

        os << "}";
        return os;
    }


    struct CIterator {
        CIterator() : m_Item(nullptr) {};

        CIterator(CItem *item) : m_Item(item) {}

        CIterator &operator++() {
            if (m_Item)
                m_Item = m_Item->m_NextOrder;
            return *this;
        }

        CIterator operator++(int) {
            CIterator cpy = *this;
            operator++();
            return cpy;
        }

        CIterator &operator--() {
            if (m_Item)
                m_Item = m_Item->m_PrevOrder;
            return *this;
        }

        CIterator operator--(int) {
            CIterator cpy = *this;
            operator--();
            return cpy;
        }

        string &operator*() const {
            return m_Item->m_Key;
        }

        friend bool operator==(const CIterator &lhs, const CIterator &rhs) {
            return lhs.m_Item == rhs.m_Item;
        }

        friend bool operator!=(const CIterator &lhs, const CIterator &rhs) {
            return !(lhs == rhs);
        }


    private:
        CItem *m_Item;

        friend class CHashSetTest;

    };

    CHashSet::CIterator begin() const {
        return {m_First};
    }

    CHashSet::CIterator end() const {
        return {nullptr};
    }

    size_t m_Size;
    CItem *m_First = nullptr;
    CItem *m_Last = nullptr;
    CItem **m_Table = nullptr;
};

class CHashSetTest {
public:
    static void test() {
        CHashSet hs0(5);

        assert (hs0.insert("PA1"));
        assert (hs0.insert("PA2"));
        assert (hs0.insert("MA1"));
        assert (hs0.insert("MA2"));
        assert (hs0.insert("SAP"));
        assert (hs0.insert("DBS"));

        assert (hs0.contains("PA1"));
        assert (hs0.contains("PA2"));
        assert (hs0.contains("MA1"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

        assert (hs0.m_First->m_Key == "PA1");
        assert (hs0.m_First->m_NextOrder->m_Key == "PA2");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_Key == "MA1");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "MA2");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "SAP");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "DBS");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder ==
                nullptr);

        assert (hs0.m_Last->m_Key == "DBS");
        assert (hs0.m_Last->m_PrevOrder->m_Key == "SAP");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "MA2");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "MA1");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder ==
                nullptr);

#ifndef CUSTOM_HASH

        assert (hs0.m_First->m_NextHash->m_Key == "DBS");
        assert (hs0.m_First->m_NextHash->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash ==
                nullptr);

#endif

        assert (hs0.remove("MA1"));

        assert (hs0.contains("PA1"));
        assert (hs0.contains("PA2"));
        assert (!hs0.contains("MA1"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));


        assert (hs0.m_First->m_Key == "PA1");
        assert (hs0.m_First->m_NextOrder->m_Key == "PA2");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_Key == "MA2");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "SAP");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "DBS");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder == nullptr);

        assert (hs0.m_Last->m_Key == "DBS");
        assert (hs0.m_Last->m_PrevOrder->m_Key == "SAP");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "MA2");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert (hs0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder == nullptr);

#ifndef CUSTOM_HASH

        assert (hs0.m_First->m_NextHash->m_Key == "DBS");
        assert (hs0.m_First->m_NextHash->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash == nullptr);
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash == nullptr);

#endif

        assert (hs0.insert("test0"));
        assert (hs0.insert("test1"));
        assert (hs0.insert("test2"));
        assert (hs0.insert("test3"));
        assert (hs0.insert("test4"));
        assert (hs0.insert("test5"));
        assert (hs0.insert("test6"));
        assert (hs0.insert("test7"));

        assert (hs0.contains("test0"));
        assert (hs0.contains("test1"));
        assert (hs0.contains("test2"));
        assert (hs0.contains("test3"));
        assert (hs0.contains("test4"));
        assert (hs0.contains("test5"));
        assert (hs0.contains("test6"));
        assert (hs0.contains("test7"));

        assert (hs0.contains("PA1"));
        assert (hs0.contains("PA2"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

#ifndef CUSTOM_HASH

        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash->m_Key == "test4");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash->m_NextHash->m_Key == "test5");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash->m_NextHash->m_NextHash == nullptr);

#endif

        assert (hs0.remove("test4"));

        assert (hs0.contains("test0"));
        assert (hs0.contains("test1"));
        assert (hs0.contains("test2"));
        assert (hs0.contains("test3"));
        assert (!hs0.contains("test4"));
        assert (hs0.contains("test5"));
        assert (hs0.contains("test6"));
        assert (hs0.contains("test7"));

        assert (hs0.contains("PA1"));
        assert (hs0.contains("PA2"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

#ifndef CUSTOM_HASH

        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash->m_Key == "test5");
        assert (hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextHash->m_NextHash == nullptr);

#endif

        assert (hs0.remove("PA1"));

        assert (hs0.contains("test0"));
        assert (hs0.contains("test1"));
        assert (hs0.contains("test2"));
        assert (hs0.contains("test3"));
        assert (hs0.contains("test5"));
        assert (hs0.contains("test6"));
        assert (hs0.contains("test7"));

        assert (!hs0.contains("PA1"));
        assert (hs0.contains("PA2"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

#ifndef CUSTOM_HASH

        assert (hs0.m_Table[hs0.hash("PA1")]->m_Key == "DBS");
        assert (hs0.m_Table[hs0.hash("PA1")]->m_NextHash == nullptr);

        assert (hs0.m_First->m_NextOrder->m_NextHash->m_NextHash->m_Key == "test2");

#endif

        assert (hs0.remove("test2"));

        assert (hs0.contains("test0"));
        assert (hs0.contains("test1"));
        assert (!hs0.contains("test2"));
        assert (hs0.contains("test3"));
        assert (hs0.contains("test5"));
        assert (hs0.contains("test6"));
        assert (hs0.contains("test7"));

        assert (hs0.contains("PA2"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

#ifndef CUSTOM_HASH

        assert (hs0.m_First->m_NextOrder->m_NextHash->m_NextHash == nullptr);

#endif

        auto it0 = hs0.begin();

        assert (it0 == hs0.begin());
        assert (it0 != hs0.end());
        assert (it0.m_Item == hs0.m_First);
        assert (*it0 == hs0.m_First->m_Key);
        assert (*(++it0) == hs0.m_First->m_NextOrder->m_Key);
        assert (*(++it0) == hs0.m_First->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(++it0) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (++it0 == hs0.end());
        assert (it0.m_Item == nullptr);

        auto it1 = hs0.begin();

        assert (it1 == hs0.begin());
        assert (it1 != hs0.end());
        assert (it1.m_Item == hs0.m_First);
        assert (*it1 == hs0.m_First->m_Key);
        assert (*(it1++) == hs0.m_First->m_Key);
        assert (*it1 == hs0.m_First->m_NextOrder->m_Key);
        assert (*(it1++) == hs0.m_First->m_NextOrder->m_Key);
        assert (*it1 == hs0.m_First->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) == hs0.m_First->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) == hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*it1 ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (*(it1++) ==
                hs0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
        assert (it1 == hs0.end());
        assert (it1.m_Item == nullptr);

        auto it2 = hs0.begin();

        for (size_t i = 0; i < 4; ++i, ++it2);

        assert (*it2 == "test0");

        assert (hs0.remove("test5"));

        assert (hs0.contains("test0"));
        assert (hs0.contains("test1"));
        assert (hs0.contains("test3"));
        assert (!hs0.contains("test5"));
        assert (hs0.contains("test6"));
        assert (hs0.contains("test7"));

        assert (hs0.contains("PA2"));
        assert (hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

        assert (*it2 == "test0");

        hs0.remove("MA2");

        assert (hs0.contains("test0"));
        assert (hs0.contains("test1"));
        assert (hs0.contains("test3"));
        assert (hs0.contains("test6"));
        assert (hs0.contains("test7"));

        assert (hs0.contains("PA2"));
        assert (!hs0.contains("MA2"));
        assert (hs0.contains("SAP"));
        assert (hs0.contains("DBS"));

        assert (*it2 == "test0");
    }
};

int main() {
    CHashSetTest::test();

    return EXIT_SUCCESS;
}