/**
* Implementujte metodu insert (vkládání), erase (mazání prvků
* ze stromu a zároveň je potřeba uchovat správné pořádí podle
* data vkládání) a vlastní destruktor, ostatní metody jsou
        * naimplentované. Kopírující konstruktor i operátor přiřazení
        * zakázán. Není povolené si přidat žádnou členskou proměnnou,
* ale lze naimplementovat vlastní pomocné metody.
*
* Za správnost šablony neručím :-).
*/

#include <string>
#include <cassert>
#include <utility>

using namespace std;

class CTree {
public:
    CTree() = default;

    ~CTree() {
        while (m_First) {
            CNode *tmp = m_First;
            m_First = m_First->m_NextOrder;
            delete tmp;
        }
    }

    CTree(const CTree &src) = delete;

    bool operator=(const CTree &other) = delete;

    bool isSet(const string &key) {
        CNode *node = m_Root;
        while (node) {
            if (key == node->m_Key) return true;
            if (key < node->m_Key) node = node->m_L;
            else node = node->m_R;
        }
        return false;
    }

    bool insert(const string &key) {
        CNode **node = &m_Root;
        while (*node) {
            if (key == (*node)->m_Key) return false;
            if (key < (*node)->m_Key) node = &(*node)->m_L;
            else node = &(*node)->m_R;
        }

        auto *newNode = new CNode(key);
        *node = newNode;

        CNode *prevNode = m_Last;
        if (!m_First) {
            m_First = newNode;
        } else {
            m_Last->m_NextOrder = newNode;
        }
        m_Last = newNode;
        m_Last->m_PrevOrder = prevNode;

        return true;
    }

    bool erase(const string &key) {
        CNode **node = &m_Root;
        while (*node) {
            if (key == (*node)->m_Key) {

                if ((*node)->m_L && (*node)->m_R) {
                    CNode *target = *node;
                    node = &(*node)->m_R;
                    while ((*node)->m_L) {
                        node = &(*node)->m_L;
                    }
                    target->m_Key = (*node)->m_Key;
                }

                CNode *toDelete = *node;

                if (toDelete->m_L) {
                    *node = (*node)->m_L;
                } else {
                    *node = (*node)->m_R;
                }
                toDelete->m_L = toDelete->m_R = nullptr;

                if (toDelete->m_PrevOrder) {
                    toDelete->m_PrevOrder->m_NextOrder = toDelete->m_NextOrder;
                } else {
                    m_First = toDelete->m_NextOrder;
                }
                if (toDelete->m_NextOrder) {
                    toDelete->m_NextOrder->m_PrevOrder = toDelete->m_PrevOrder;
                } else {
                    m_Last = toDelete->m_PrevOrder;
                }

                delete toDelete;

                return true;
            }


            if (key < (*node)->m_Key) node = &(*node)->m_L;
            else node = &(*node)->m_R;
        }

        return false;
    }


    class CNode {
    public:
        CNode(string key) : m_Key(std::move(key)) {}

        string m_Key;
        CNode *m_L = nullptr;
        CNode *m_R = nullptr;
        CNode *m_PrevOrder = nullptr;
        CNode *m_NextOrder = nullptr;
    };

public:
    struct Iterator {

        Iterator(CNode *node) : m_Item(node) {}

        Iterator &operator++() {
            if (m_Item) {
                m_Item = m_Item->m_NextOrder;
            }
            return *this;
        }

        Iterator operator++(int) {
            CNode *cpy = m_Item;
            if (m_Item) {
                m_Item = m_Item->m_NextOrder;
            }
            return cpy;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.m_Item == rhs.m_Item;
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
            return !(lhs == rhs);
        }

        string &operator*() const {
            return m_Item->m_Key;
        }


        CNode *m_Item;
    };

    Iterator begin() {
        return Iterator(m_First);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    CNode *m_Root = nullptr;
    CNode *m_First = nullptr;
    CNode *m_Last = nullptr;
};

class CTester : public CTree {
public:
    CTester() = default;

    void test() {
        {
            CTester t0;
            assert(t0.insert("PA1") == true);
            assert(t0.m_First->m_Key == "PA1" && t0.m_First->m_NextOrder == nullptr);
            assert(t0.isSet("PA1") == true);
            assert(t0.insert("UOS") == true);
            assert(t0.insert("PA2") == true);
            assert(t0.isSet("PA2") == true);
            assert(t0.isSet("PA3") == false);

            assert(t0.insert("PA2") == false);
            assert(t0.insert("CAO") == true);
            assert(t0.insert("LIN") == true);
            assert(t0.insert("AAG") == true);
            assert(t0.insert("AG1") == true);
            assert(t0.insert("ZDM") == true);

            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "UOS"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
                   &&
                   t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                   "AG1"
                   &&
                   t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                   "ZDM");

            assert(t0.m_Last->m_Key == "ZDM"
                   && t0.m_Last->m_PrevOrder->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "AAG"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                   &&
                   t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                   "UOS"
                   &&
                   t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                   "PA1");

            assert(t0.erase("") == false);

            assert(t0.erase("ZDM") == true);
            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "UOS"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                      "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "AAG"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
                   &&
                   t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                   "PA1");
            assert(t0.isSet("ZDM") == false);

            assert(t0.erase("AAG") == true);
            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "UOS"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert(t0.isSet("AAG") == false);

            assert(t0.erase("CAO") == true);
            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "UOS"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert(t0.isSet("CAO") == false);

            assert(t0.erase("UOS") == true);
            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert(t0.isSet("UOS") == false);

            assert(t0.erase("UOS") == false);
            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert(t0.isSet("UOS") == false);

            assert(t0.erase("LIN") == true);
            assert(t0.m_First->m_Key == "PA1"
                   && t0.m_First->m_NextOrder->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "PA2"
                   && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert(t0.isSet("LIN") == false);

            assert(t0.erase("PA1") == true);
            assert(t0.m_First->m_Key == "PA2"
                   && t0.m_First->m_NextOrder->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1"
                   && t0.m_Last->m_PrevOrder->m_Key == "PA2");
            assert(t0.isSet("PA1") == false);

            assert(t0.erase("PA2") == true);
            assert(t0.m_First->m_Key == "AG1");
            assert(t0.m_Last->m_Key == "AG1");
            assert(t0.isSet("PA2") == false);

            assert(t0.erase("AG1") == true);
            assert(t0.m_First == t0.m_Last);
            assert(t0.isSet("AG1") == false);
        };
        {
            CTester t0;
            assert (t0.insert("PA1") == true);
            assert (t0.m_First->m_Key == "PA1" && t0.m_First->m_NextOrder == nullptr);
            assert (t0.isSet("PA1") == true);
            assert (t0.insert("UOS") == true);
            assert (t0.insert("PA2") == true);
            assert (t0.isSet("PA2") == true);
            assert (t0.isSet("PA3") == false);

            assert (t0.insert("PA2") == false);
            assert (t0.insert("CAO") == true);
            assert (t0.insert("LIN") == true);
            assert (t0.insert("AAG") == true);
            assert (t0.insert("AG1") == true);
            assert (t0.insert("ZDM") == true);

            assert (t0.m_First->m_Key == "PA1"
                    && t0.m_First->m_NextOrder->m_Key == "UOS"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                       "AAG"
                    &&
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                    "AG1"
                    &&
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                    "ZDM");

            assert (t0.m_Last->m_Key == "ZDM"
                    && t0.m_Last->m_PrevOrder->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "AAG"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                       "PA2"
                    &&
                    t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                    "UOS"
                    &&
                    t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                    "PA1");

            assert (t0.erase("") == false);

            assert (t0.erase("ZDM") == true);
            assert (t0.m_First->m_Key == "PA1"
                    && t0.m_First->m_NextOrder->m_Key == "UOS"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                       "AAG"
                    &&
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                    "AG1");
            assert (t0.m_Last->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_Key == "AAG"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                       "UOS"
                    &&
                    t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                    "PA1");
            assert (t0.isSet("ZDM") == false);

            assert (t0.erase("AAG") == true);
            assert (t0.m_First->m_Key == "PA1"
                    && t0.m_First->m_NextOrder->m_Key == "UOS"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                       "AG1");
            assert (t0.m_Last->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
                       "PA1");
            assert (t0.isSet("AAG") == false);

            assert (t0.erase("CAO") == true);
            assert (t0.m_First->m_Key == "PA1"
                    && t0.m_First->m_NextOrder->m_Key == "UOS"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert (t0.m_Last->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert (t0.isSet("CAO") == false);

            assert (t0.erase("UOS") == true);
            assert (t0.m_First->m_Key == "PA1"
                    && t0.m_First->m_NextOrder->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert (t0.m_Last->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_Key == "LIN"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert (t0.isSet("UOS") == false);


            assert (t0.erase("LIN") == true);
            assert (t0.m_First->m_Key == "PA1"
                    && t0.m_First->m_NextOrder->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "AG1");
            assert (t0.m_Last->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_Key == "PA2"
                    && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
            assert (t0.isSet("LIN") == false);

            assert (t0.erase("PA1") == true);
            assert (t0.m_First->m_Key == "PA2"
                    && t0.m_First->m_NextOrder->m_Key == "AG1");
            assert (t0.m_Last->m_Key == "AG1"
                    && t0.m_Last->m_PrevOrder->m_Key == "PA2");
            assert (t0.isSet("PA1") == false);

            assert (t0.erase("PA2") == true);
            assert (t0.m_First->m_Key == "AG1");
            assert (t0.m_Last->m_Key == "AG1");
            assert (t0.isSet("PA2") == false);

            assert (t0.erase("AG1") == true);
            assert (t0.m_First == t0.m_Last);
            assert (t0.isSet("AG1") == false);


            assert (t0.erase("PA2") == false);

            assert (t0.insert("PA1") == true);
            assert (t0.insert("UOS") == true);
            assert (t0.insert("PA2") == true);
            assert (t0.insert("PA2") == false);
            assert (t0.insert("CAO") == true);
            assert (t0.insert("LIN") == true);
            assert (t0.insert("AAG") == true);
            assert (t0.insert("AG1") == true);
            assert (t0.insert("ZDM") == true);
            assert (t0.insert("FOO") == true);
            assert (t0.insert("1a") == true);


            auto it0 = t0.begin();

            assert (it0 == t0.begin());
            assert (it0 != t0.end());
            assert (it0.m_Item == t0.m_First);
            assert (*it0 == t0.m_First->m_Key);
            assert (*(++it0) == t0.m_First->m_NextOrder->m_Key);
            assert (*(++it0) == t0.m_First->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(++it0) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (++it0 == t0.end());
            assert (it0.m_Item == nullptr);

            auto it1 = t0.begin();

            assert (it1 == t0.begin());
            assert (it1 != t0.end());
            assert (it1.m_Item == t0.m_First);
            assert (*it1 == t0.m_First->m_Key);
            assert (*(it1++) == t0.m_First->m_Key);
            assert (*it1 == t0.m_First->m_NextOrder->m_Key);
            assert (*(it1++) == t0.m_First->m_NextOrder->m_Key);
            assert (*it1 == t0.m_First->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) == t0.m_First->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) == t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*it1 ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (*(it1++) ==
                    t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key);
            assert (it1 == t0.end());
            assert (it1.m_Item == nullptr);

            auto it2 = t0.begin();

            for (size_t i = 0; i < 4; ++i, ++it2);

            assert (t0.erase("CAO") == true);
            assert (t0.erase("CANNOT_ERASED") == false);
            assert (t0.erase("PA1") == true);
            assert (t0.erase("CAO") == false);
            assert (t0.erase("LIN") == true);
            assert (t0.erase("AAG") == true);
            assert (t0.erase("AG1") == true);
            assert (t0.erase("FOO") == true);
            assert (t0.erase("PA2") == true);
            assert (t0.erase("ZDM") == true);
            assert (t0.erase("UOS") == true);

            assert (t0.insert("PA1") == true);
            assert (t0.insert("UOS") == true);
            assert (t0.insert("PA2") == true);
            assert (t0.insert("PA2") == false);
            assert (t0.insert("CAO") == true);
            assert (t0.insert("LIN") == true);
            assert (t0.insert("AAG") == true);
            assert (t0.insert("AG1") == true);
            assert (t0.insert("ZDM") == true);

            assert (t0.erase("UOS") == true);
            assert (t0.erase("PA2") == true);
            assert (t0.erase("ZDM") == true);
            assert (t0.erase("PA1") == true);
            assert (t0.erase("AAG") == true);
            assert (t0.erase("AG1") == true);
            assert (t0.erase("LIN") == true);
            assert (t0.erase("CAO") == true);

            assert (t0.insert("PA1") == true);
            assert (t0.insert("CAO") == true);
            assert (t0.insert("UOS") == true);
            assert (t0.insert("PA2") == true);
            assert (t0.insert("PA2") == false);
            assert (t0.insert("LIN") == true);
            assert (t0.insert("AAG") == true);
            assert (t0.insert("AG1") == true);
            assert (t0.insert("ZDM") == true);

            assert (t0.erase("PA1") == true);
            assert (t0.erase("ZDM") == true);
            assert (t0.erase("UOS") == true);
            assert (t0.erase("PA2") == true);
            assert (t0.erase("AG1") == true);
            assert (t0.erase("CAO") == true);
            assert (t0.erase("AAG") == true);
            assert (t0.erase("LIN") == true);


            assert (t0.insert("20") == true);
            assert (t0.insert("10") == true);
            assert (t0.insert("5") == true);
            assert (t0.insert("15") == true);
            assert (t0.insert("17") == true);
            assert (t0.insert("13") == true);
            assert (t0.insert("14") == true);
            assert (t0.insert("11") == true);
            assert (t0.insert("12") == true);
            assert (t0.insert("3") == true);
            assert (t0.insert("7") == true);
            assert (t0.insert("25") == true);

            assert (t0.erase("10") == true);
            assert (t0.erase("25") == true);
            assert (t0.erase("20") == true);
            assert (t0.erase("17") == true);

        };
    };
};

int main() {
    CTester t;
    t.test();

    return EXIT_SUCCESS;
}

