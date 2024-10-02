#include <cassert>
#include <vector>
#include <memory>
#include <string>

using namespace std;

class CSocialNetwork {
public:
    CSocialNetwork(size_t size) : m_Size(size), m_Table(new CPerson *[m_Size]) {
        for (size_t i = 0; i < m_Size; ++i) {
            m_Table[i] = nullptr;
        }
    }

    CSocialNetwork(const CSocialNetwork &src) : m_Size(src.m_Size), m_Table(new CPerson *[m_Size]) {
        // TODO
    }

    CSocialNetwork &operator=(const CSocialNetwork &other) = delete;

    ~CSocialNetwork() {
        // TODO
    }

    void addPerson(const string &name) {
        size_t index = hashFunction(name);
        auto *newPerson = new CPerson(name, m_Table[index]);
        m_Table[index] = newPerson;
    }

    void addFriend(const string &name, const string &friendName) {
        CPerson *person = findPerson(name);
        CPerson *friendOfPerson = findPerson(friendName);
        person->m_Friends.push_back(friendOfPerson);
        friendOfPerson->m_Friends.push_back(person);
    }

    bool areFriends(const string &name1, const string &name2) {


        CPerson *person1 = findPerson(name1);
        CPerson *person2 = findPerson(name2);

        for (CPerson *personFriend: person1->m_Friends) {
            if (personFriend == person2) return true;
        }

        return false;
    }

private:
    struct CPerson {
        CPerson() = default;

        CPerson(const string &name, CPerson *next) : m_Name(name), m_Next(next) {}

        string m_Name;
        CPerson *m_Next = nullptr;
        vector<CPerson *> m_Friends;
    };

    size_t hashFunction(const string &name) const {
        size_t hash = 3001;
        for (char c: name) {
            hash = hash * 31 + c;
        }
        return hash % m_Size;
    }

    CPerson *findPerson(const string &name) const {
        size_t index = hashFunction(name);
        for (CPerson *tmp = m_Table[index]; tmp; tmp = tmp->m_Next)
            if (tmp->m_Name == name) return tmp;
        return nullptr;
    }

    size_t m_Size;
    CPerson **m_Table;

    friend class Tester;
};

class Tester {
public:
    static void testCopyConstructor() {
        CSocialNetwork network1(10);
        network1.addPerson("Alice");
        network1.addPerson("Bob");
        network1.addPerson("Charlie");
        network1.addFriend("Alice", "Bob");
        network1.addFriend("Bob", "Charlie");

        CSocialNetwork network2(network1);

        // Basic friendship check
        assert(network2.areFriends("Alice", "Bob") == true);
        assert(network2.areFriends("Bob", "Alice") == true); // Bilateral check
        assert(network2.areFriends("Bob", "Charlie") == true);
        assert(network2.areFriends("Charlie", "Bob") == true); // Bilateral check
        assert(network2.areFriends("Alice", "Charlie") == false);
        assert(network2.areFriends("Charlie", "Alice") == false); // Bilateral check

        // Ensure changes in original network do not affect the copy
        network1.addFriend("Alice", "Charlie");
        assert(network2.areFriends("Alice", "Charlie") == false);
        assert(network2.areFriends("Charlie", "Alice") == false); // Bilateral check

        // Ensure changes in the copy do not affect the original
        network2.addFriend("Bob", "Alice");
        assert(network1.areFriends("Bob", "Alice") == true); // Should be true already
        assert(network1.areFriends("Alice", "Bob") == true); // Bilateral check

        // Check if all persons are copied correctly
        assert(network2.areFriends("Alice", "Bob") == true);
        assert(network2.areFriends("Bob", "Alice") == true); // Bilateral check
        assert(network2.areFriends("Bob", "Charlie") == true);
        assert(network2.areFriends("Charlie", "Bob") == true); // Bilateral check
        assert(network2.areFriends("Alice", "Charlie") == false);
        assert(network2.areFriends("Charlie", "Alice") == false); // Bilateral check

        // Add more tests to ensure deep copy
        network1.addPerson("David");
        network1.addFriend("Charlie", "David");
        CSocialNetwork network3(network1);
        assert(network3.areFriends("Charlie", "David") == true);
        assert(network3.areFriends("David", "Charlie") == true); // Bilateral check
        assert(network3.areFriends("Alice", "Charlie") == true);
        assert(network3.areFriends("Charlie", "Alice") == true); // Bilateral check
        assert(network3.areFriends("Bob", "Alice") == true);
        assert(network3.areFriends("Alice", "Bob") == true); // Bilateral check

        // Verify that new person in original doesn't exist in copy
        network1.addPerson("Eve");
        assert(network3.findPerson("Eve") == nullptr);

        // Verify that friendship in original doesn't affect the copy
        network1.addFriend("Alice", "David");
        assert(network3.areFriends("Alice", "David") == false);
        assert(network3.areFriends("David", "Alice") == false); // Bilateral check

        // Verify new friendship in copy does not affect the original
        network3.addFriend("Charlie", "Alice");
        assert(network3.areFriends("Charlie", "Alice") == true);
        assert(network3.areFriends("Alice", "Charlie") == true); // Bilateral check

        // Ensure original network doesn't have new people added in copy
        network3.addPerson("Frank");
        assert(network1.findPerson("Frank") == nullptr);

        // Verify that the copied network has correct persons
        assert(network2.findPerson("Alice") != nullptr);
        assert(network2.findPerson("Bob") != nullptr);
        assert(network2.findPerson("Charlie") != nullptr);

        // Test with multiple persons having the same name
        CSocialNetwork n1(10);
        n1.addPerson("Alice");
        n1.addPerson("Peter");
        n1.addFriend("Alice", "Peter");
        n1.addPerson("Alice");

        CSocialNetwork n2(n1);
        assert(n2.findPerson("Alice")->m_Friends.empty());
        assert(n2.findPerson("Alice")->m_Next->m_Friends[0] == n2.findPerson("Peter"));
    }
};

int main() {
    Tester::testCopyConstructor();
    return 0;
}
