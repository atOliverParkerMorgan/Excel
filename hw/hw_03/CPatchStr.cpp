//
// Created by olixandr on 28.3.24.
//

#include "CPatchStr.h"

CPatchStr::CPatchStr() {
    head = tail = nullptr;
}

CPatchStr::CPatchStr(const char *string) {
    len = strlen(string);
    auto *node = new CNode(string);
    head = tail = node;
}

CPatchStr::CPatchStr(const CPatchStr &other) : head(nullptr), tail(nullptr), len(other.len) {
    if (other.head) {
        head = tail = new CNode(other.head->sharedPtr);
        CNode *otherCurrentPtr = other.head;
        otherCurrentPtr = otherCurrentPtr->next;

        while (otherCurrentPtr) {
            tail->next = new CNode(otherCurrentPtr->sharedPtr);
            tail = tail->next;

            otherCurrentPtr = otherCurrentPtr->next;
        }
    }
}

CPatchStr::~CPatchStr() {
    while (head) {
        CNode *tmp = head->next;
        head->sharedPtr.reset();
        head = tmp;
    }
}


CPatchStr &CPatchStr::operator=(const CPatchStr &rhs) {
    if (this != &rhs) {
        CPatchStr tmp(rhs);
        std::swap(head, tmp.head);
        std::swap(tail, tmp.tail);
        len = rhs.len;
    }
    return *this;
}


CPatchStr CPatchStr::subStr(size_t from, size_t len) const {

}

CPatchStr &CPatchStr::append(const CPatchStr &src) {
    if (src.head) {
        CNode *srcCurrentPtr = src.head;

        while (srcCurrentPtr) {
            CNode *newNode = new CNode(srcCurrentPtr->sharedPtr);
            if (!head) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = tail->next;
            }
            len += newNode->len - newNode->ofs;
            srcCurrentPtr = srcCurrentPtr->next;
        }
    }

    return *this;
}


CPatchStr::CNode *CPatchStr::getCStrOnPos(size_t pos) {
    CNode *tmp = head;
    size_t current_length = head->len - head->ofs;
    while (pos < current_length) {
        current_length += tmp->len - tmp->ofs;
        tmp = tmp->next;
    }
    return tmp;
}

CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src) {


    if (pos > len) {
        throw std::out_of_range("Insert out of range");
    }
    if (!head)
        return *this;

    if (pos == len) {
        append(src);
    } else {

        CNode *insertPos = getCStrOnPos(pos);
        CNode *afterInsertPos = insertPos->next;
        insertPos->next = src.head;
        src.tail->next = afterInsertPos;

        if (!afterInsertPos) {
            tail = src.tail;
        }

        len += src.len;
    }
    return *this;
}

CPatchStr &CPatchStr::remove(size_t from, size_t len) {
    return *this;
}

char *CPatchStr::toStr() const {
    if (len == 0)
        return nullptr;

    char *result = new char[len + 1];
    char *currentPointer = result;

    CNode *tmp = head;
    while (tmp) {
        std::copy(tmp->sharedPtr.get() + tmp->ofs, tmp->sharedPtr.get() + tmp->ofs + tmp->len, currentPointer);
        currentPointer += tmp->len - tmp->ofs;
        tmp = tmp->next;
    }
    result[len] = '\0';
    return result;
}

void CPatchStr::print() {
    CNode *tmp = head;
    while (tmp) {
        std::cout << tmp->ofs << " - " << tmp->len << " " << tmp->sharedPtr.get() << std::endl;
        tmp = tmp->next;
    }
}


CPatchStr::CNode::CNode() : len(0) {};

CPatchStr::CNode::CNode(const std::shared_ptr<char> &ptr) : sharedPtr(ptr), len(strlen(sharedPtr.get())) {}

CPatchStr::CNode::~CNode() {
    sharedPtr.reset();
}

CPatchStr::CNode::CNode(const char *string) {
    len = strlen(string);
    char *stringCopy = new char[len + 1];
    std::strcpy(stringCopy, string);

    sharedPtr = std::shared_ptr<char>(stringCopy, std::default_delete<char[]>());
    std::cout << "Here" << sharedPtr.get() << std::endl;
}



