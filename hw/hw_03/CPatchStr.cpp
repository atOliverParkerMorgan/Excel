#include "CPatchStr.h"

CPatchStr::CPatchStr() : patchList(std::make_shared<PatchList>()) {}

CPatchStr::CPatchStr(const char *str) : patchList(std::make_shared<PatchList>()) {
    size_t length = strlen(str);
    char *newStr = new char[length + 1];
    std::strcpy(newStr, str);
    std::shared_ptr<char[]> newShared(newStr);
    push_back(newShared, 0, length);
}

CPatchStr::CPatchStr(const CPatchStr &other) : patchList(std::make_shared<PatchList>(*other.patchList)) {}

CPatchStr::~CPatchStr() {}

CPatchStr &CPatchStr::operator=(const CPatchStr &rhs) {
    if (this != &rhs) {
        patchList->size = rhs.patchList->size;
        patchList->capacity = rhs.patchList->capacity;
        patchList->strLen = rhs.patchList->strLen;
        if (rhs.patchList->nodes) {
            patchList->nodes = std::make_unique<Node[]>(patchList->capacity);
            for (size_t i = 0; i < patchList->size; ++i) {
                patchList->nodes[i] = rhs.patchList->nodes[i];
            }
        } else {
            patchList->nodes = nullptr;
        }
    }
    return *this;
}

CPatchStr CPatchStr::subStr(size_t from, size_t len) const {
    if (from + len > patchList->strLen) {
        throw std::out_of_range("Out of range error");
    }

    CPatchStr newCPatchStr;

    size_t totalLength = 0;
    size_t currentNodeIndex = 0;
    size_t startOffset = 0;

    for (size_t i = 0; i < patchList->size; ++i) {

        totalLength += patchList->nodes[i].len;
        if (totalLength > from) {
            currentNodeIndex = i;
            startOffset = from - (totalLength - patchList->nodes[i].len);
            break;
        }
    }

    while (len > 0 && currentNodeIndex < patchList->size) {
        size_t copyLength = std::min(patchList->nodes[currentNodeIndex].len - startOffset, len);
        newCPatchStr.push_back(patchList->nodes[currentNodeIndex].patch, patchList->nodes[currentNodeIndex].offset +
                                                                         startOffset, copyLength);
        len -= copyLength;
        startOffset = 0;
        ++currentNodeIndex;
    }

    return newCPatchStr;
}

CPatchStr &CPatchStr::append(const CPatchStr &src) {
    size_t length = src.patchList->size;
    for (size_t i = 0; i < length; ++i) {
        push_back(src.patchList->nodes[i].patch, src.patchList->nodes[i].offset, src.patchList->nodes[i].len);
    }
    return *this;
}

CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src) {
    if (pos > patchList->strLen) {
        throw std::out_of_range("Out of range error");
    }

    CPatchStr firstHalf = subStr(0, pos);
    CPatchStr secondHalf = subStr(pos, patchList->strLen - pos);

    // Create a new CPatchStr to hold the inserted substring
    CPatchStr insertedSubstr = src.subStr(0, src.patchList->strLen);

    // Append the first half, inserted substring, and the second half
    firstHalf.append(insertedSubstr);
    firstHalf.append(secondHalf);

    *this = firstHalf;
    return *this;
}

CPatchStr &CPatchStr::remove(size_t from, size_t len) {
    if (from + len > patchList->strLen) {
        throw std::out_of_range("Out of range error");
    }

    CPatchStr firstHalf = subStr(0, from);
    CPatchStr secondHalf = subStr(len + from, patchList->strLen - len - from);

    for (size_t i = 0; i < secondHalf.patchList->size; ++i) {
        firstHalf.push_back(secondHalf.patchList->nodes[i].patch, secondHalf.patchList->nodes[i].offset,
                            secondHalf.patchList->nodes[i].len);
    }

    *this = firstHalf;
    return *this;
}

char *CPatchStr::toStr() const {
    char *newStr = new char[patchList->strLen + 1]();
    for (size_t i = 0; i < patchList->size; ++i) {
        const char *string = patchList->nodes[i].patch.get();
        if (string != nullptr) {
            strncat(newStr, string + patchList->nodes[i].offset, patchList->nodes[i].len);
        }
    }
    return newStr;
}

void CPatchStr::push_back(std::shared_ptr<char[]> str, size_t offset, size_t length) {
    if (patchList->size >= patchList->capacity) {
        patchList->capacity = patchList->capacity * 2 + 8;
        std::unique_ptr<Node[]> newNodes(new Node[patchList->capacity]);
        for (size_t i = 0; i < patchList->size; ++i) {
            newNodes[i] = patchList->nodes[i];
        }
        patchList->nodes = std::move(newNodes);
    }
    patchList->nodes[patchList->size].patch = str;
    patchList->nodes[patchList->size].offset = offset;
    patchList->nodes[patchList->size].len = length;
    patchList->strLen += length;
    ++patchList->size;
}

CPatchStr::Node::Node() : patch(nullptr), offset(0), len(0) {}

CPatchStr::Node::Node(const CPatchStr::Node &node) : offset(0), len(0) {
    if (this != &node) {
        offset = node.offset;
        len = node.len;
        if (node.patch) {
            patch = node.patch;
        }
    }
}

CPatchStr::Node &CPatchStr::Node::operator=(const CPatchStr::Node &rhs) {
    if (this != &rhs) {
        len = rhs.len;
        offset = rhs.offset;
        if (rhs.patch) {
            patch = rhs.patch;
        }
    }
    return *this;
}

CPatchStr::PatchList::PatchList() : nodes(nullptr), capacity(0), size(0), strLen(0) {}

CPatchStr::PatchList::PatchList(const CPatchStr::PatchList &other) : nodes(nullptr), capacity(other.capacity),
                                                                     size(other.size), strLen(other.strLen) {
    if (other.nodes) {
        nodes = std::make_unique<Node[]>(capacity);
        for (size_t i = 0; i < size; ++i) {
            nodes[i] = other.nodes[i];
        }
    }
}

