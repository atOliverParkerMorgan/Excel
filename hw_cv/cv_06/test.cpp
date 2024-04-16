#ifndef __PROGTEST__
#include <cstring>
using namespace std;
class CLinkedSetTester;
#endif /* __PROGTEST__ */

class CLinkedSet {
private:
    struct CNode {
        CNode(const char *value) : m_value(value) {}
        ~CNode() { delete[] m_value; }

        const char *Value() const { return m_value; }

        CNode *m_Next = nullptr;

    private:
        const char *m_value;
    };

    size_t size = 0;
    CNode *m_Begin = nullptr;

public:
    // default constructor
    CLinkedSet() = default;

    // copy constructor (deep copy)
    CLinkedSet(const CLinkedSet &other) {
        CNode *otherNode = other.m_Begin;
        CNode *prev = nullptr;
        while (otherNode) {
            CNode *newNode = new CNode(strdup(otherNode->Value())); // Deep copy of value
            if (!m_Begin) {
                m_Begin = newNode;
            } else {
                prev->m_Next = newNode;
            }
            prev = newNode;
            otherNode = otherNode->m_Next;
        }
        size = other.size;
    }

    // operator= (deep copy)
    CLinkedSet &operator=(const CLinkedSet &rhs) {
        if (this != &rhs) {
            // Clear current list
            Clear();

            // Deep copy elements from rhs
            CNode *otherNode = rhs.m_Begin;
            CNode *prev = nullptr;
            while (otherNode) {
                CNode *newNode = new CNode(strdup(otherNode->Value())); // Deep copy of value
                if (!m_Begin) {
                    m_Begin = newNode;
                } else {
                    prev->m_Next = newNode;
                }
                prev = newNode;
                otherNode = otherNode->m_Next;
            }
            size = rhs.size;
        }
        return *this;
    }

    // destructor
    ~CLinkedSet() { Clear(); }

    bool Insert(const char *value) {
        // Check if value already exists
        if (Contains(value)) {
            return false;
        }

        // Create a new node
        CNode *newNode = new CNode(strdup(value)); // Deep copy of value

        // Insert at the beginning of the list if empty
        if (!m_Begin) {
            m_Begin = newNode;
        } else {
            // Traverse the list to find the appropriate position for insertion
            CNode *prev = nullptr;
            CNode *current = m_Begin;
            while (current && strcmp(value, current->Value()) > 0) {
                prev = current;
                current = current->m_Next;
            }

            // Insert the new node
            newNode->m_Next = current;
            if (prev) {
                prev->m_Next = newNode;
            } else {
                m_Begin = newNode;
            }
        }

        size++;
        return true;
    }

    bool Remove(const char *value) {
        // Empty list
        if (size == 0) return false;

        // Remove from the beginning
        if (strcmp(value, m_Begin->Value()) == 0) {
            CNode *newBegin = m_Begin->m_Next;
            delete m_Begin;
            m_Begin = newBegin;
            size--;
            return true;
        }

        // Remove from other positions
        CNode *prev = m_Begin;
        CNode *current = m_Begin->m_Next;
        while (current) {
            if (strcmp(value, current->Value()) == 0) {
                prev->m_Next = current->m_Next;
                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->m_Next;
        }

        return false; // Value not found
    }

    bool Empty() const { return size == 0; }

    size_t Size() const { return size; }

    bool Contains(const char *value) const {
        CNode *current = m_Begin;
        while (current) {
            if (strcmp(value, current->Value()) == 0) {
                return true;
            }
            current = current->m_Next;
        }
        return false;
    }

private:
    // Helper function to clear the list and deallocate memory
    void Clear() {
        CNode *current = m_Begin;
        while (current) {
            CNode *next = current->m_Next;
            delete current;
            current = next;
        }
        m_Begin = nullptr;
        size = 0;
    }

    friend class CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester {
    static void test0() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        assert(!x0.Empty());
    }

    static void test1() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CLinkedSet x1(x0);
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test2() {
        CLinkedSet x0;
        CLinkedSet x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }
};

int main() {
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */
