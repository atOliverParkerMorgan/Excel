#ifndef __TRAINER__

#include <iostream>
#include <cassert>

#endif

struct Node {
    Node(int m_val);

    int m_val;
    Node *m_next;
};

Node::Node(int val) {
    m_val = val;
    m_next = nullptr;
}

class CQueue {
public:
    // konstruktory, destruktory
    CQueue();

    CQueue(const CQueue &queue);

    ~CQueue();

    void push(int x);

    int pop();
    CQueue& operator=(const CQueue &queue);

    bool isEmpty() const;

    size_t size() const;

    const int &front() const;

    const int &back() const;

private:
    Node *m_head;
    Node *m_tail;
    size_t m_size;
};


void CQueue::push(int x) {
    if (!m_head) {
        m_head = m_tail = new Node(x);
    } else {
        m_tail->m_next = new Node(x);
        m_tail = m_tail->m_next;
    }
    m_size++;
}


int CQueue::pop() {
    if (!m_head) return -1;

    const int val = m_head->m_val;
    Node *tmp = m_head->m_next;
    delete m_head;
    m_head = tmp;
    m_size--;

    return val;
}

bool CQueue::isEmpty() const {
    return m_size == 0;
}

const int &CQueue::front() const {
    return m_head->m_val;
}

const int &CQueue::back() const {
    return m_tail->m_val;
}

CQueue::CQueue() {
    m_size = 0;
    m_head = m_tail = nullptr;
}

size_t CQueue::size() const {
    return m_size;
}

CQueue::~CQueue() {
    Node *current = m_head;
    while (current != nullptr) {
        Node *next = current->m_next;
        delete current;
        current = next;
    }
}

CQueue::CQueue(const CQueue &queue) {
    if (!queue.m_head) {
        m_head = nullptr;
        m_tail = nullptr;
        m_size = 0;
    } else {
        // Kopírování prvního uzlu fronty
        m_head = new Node(queue.m_head->m_val);
        // Průchod zbytkem fronty a kopírování uzlů
        Node *sourceNode = queue.m_head->m_next;
        Node *targetNode = m_head;
        while (sourceNode) {
            targetNode->m_next = new Node(sourceNode->m_val);
            targetNode = targetNode->m_next;
            sourceNode = sourceNode->m_next;
        }
        m_tail = targetNode; // Nastavení ukazatele na poslední uzel
        m_size = queue.m_size; // Nastavení velikosti fronty
    }
}

CQueue &CQueue::operator=(const CQueue &queue) {
    if
}


#ifndef __TRAINER__

int main() {
    CQueue queue;
    assert(queue.isEmpty());
    assert(queue.size() == 0);

    queue.push(1);
    assert(queue.size() == 1);

    queue.push(2);
    queue.push(3);
    assert(!queue.isEmpty());
    assert(queue.size() == 3);
    assert(queue.front() == 1);
    assert(queue.back() == 3);

    assert(queue.pop() == 1);
    assert(queue.size() == 2);
    assert(queue.front() == 2);
    assert(queue.back() == 3);

    queue.push(4);
    assert(queue.back() == 4);

    assert(queue.pop() == 2);
    assert(queue.pop() == 3);
    assert(queue.pop() == 4);
    assert(queue.isEmpty());
    assert(queue.size() == 0);

    // kopirujici konstruktor
    CQueue a;
    a.push(10);
    a.push(20);
    CQueue b = a;
    b.push(30);
    assert(a.size() == 2);
    assert(b.size() == 3);

    assert(a.pop() == 10);
    assert(a.size() == 1);
    assert(b.size() == 3);
    assert(!a.isEmpty());
    assert(!b.isEmpty());

    assert(a.pop() == 20);
    assert(a.isEmpty());

    assert(b.pop() == 10);
    return 0;
}

#endif



