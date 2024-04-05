#include "CPatchStr.h"

CPatchStr::CPatchStr() : patchList(std::make_shared<PatchList>()) {

}

CPatchStr::CPatchStr(const char *str) : patchList(std::make_shared<PatchList>()) {
    push_back(str, strlen(str));
}

CPatchStr::CPatchStr(const CPatchStr &other) : patchList(std::make_shared<PatchList>()) {
    if (this != &other) {
        patchList->size = other.patchList->size;
        patchList->capacity = other.patchList->capacity;
        patchList->strLen = other.patchList->strLen;
        std::unique_ptr<Node[]> newNodes(new Node[patchList->size]);
        patchList->nodes = std::move(newNodes);

        for (int i = 0; i < other.patchList->size; ++i) {
            patchList->nodes[i] = other.patchList->nodes[i];
        }
    }
}

CPatchStr::~CPatchStr() {};


CPatchStr &CPatchStr::operator=(CPatchStr &rhs) {
    if (this != &rhs) {

    }
}

CPatchStr CPatchStr::subStr(size_t from, size_t len) const {
    CPatchStr newCPatchStr;

    size_t totalLength = 0; // Track the total length processed so far
    size_t startNodeIndex = 0; // Index of the node where the substring starts
    size_t startOffset = 0; // Offset within the start node

    // Find the starting node and offset within that node for the substring
    for (size_t i = 0; i < patchList->size; ++i) {
        totalLength += patchList->nodes[i].len;
        if (totalLength > from) {
            startNodeIndex = i;
            startOffset = from - (totalLength - patchList->nodes[i].len);
            break;
        }
    }

    size_t remainingLength = len; // Remaining length of the substring to extract
    size_t currentNodeIndex = startNodeIndex; // Index of the current node being processed
    size_t currentOffset = startOffset; // Offset within the current node

    // Iterate through the nodes to extract the substring
    while (remainingLength > 0 && currentNodeIndex < patchList->size) {
        size_t copyLength = std::min(patchList->nodes[currentNodeIndex].len - currentOffset, remainingLength);
        newCPatchStr.push_back(
                patchList->nodes[currentNodeIndex].patch.get() + patchList->nodes[currentNodeIndex].offset +
                currentOffset, copyLength);
        remainingLength -= copyLength;
        currentOffset = 0; // After the first iteration, offset will always be 0 for subsequent nodes
        ++currentNodeIndex;
    }

    return newCPatchStr;
}

CPatchStr &CPatchStr::append(const CPatchStr &src) {
    const char *str = src.toStr();
    push_back(str, strlen(str));
    delete[] str;

}

CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src) {
}

CPatchStr &CPatchStr::remove(size_t from, size_t len) {
}

char *CPatchStr::toStr() const {
    char *newStr = new char[patchList->strLen + 1]();
    for (int i = 0; i < patchList->size; ++i) {
        strncat(newStr, patchList->nodes[i].patch.get() + patchList->nodes[i].offset, patchList->nodes[i].len);
    }
    return newStr;
}

void CPatchStr::push_back(const char *str, size_t length) {


    if (patchList->size >= patchList->capacity) {
        patchList->capacity = patchList->capacity * 2 + 8;

        std::unique_ptr<Node[]> newNodes(new Node[patchList->capacity]);

        for (int i = 0; i < patchList->size; ++i) {
            newNodes[i] = patchList->nodes[i];
        }

        patchList->nodes = std::move(newNodes);

    }

    patchList->nodes[patchList->size].patch.reset(new char[length + 1]);
    std::memcpy(patchList->nodes[patchList->size].patch.get(), str, length);
    patchList->nodes[patchList->size].patch[length] = '\0';
    patchList->nodes[patchList->size].offset = 0;
    patchList->nodes[patchList->size].len = length;
    ++patchList->size;
}

CPatchStr::Node::Node() : patch(nullptr), offset(0), len(0) {

}

CPatchStr::Node::Node(const CPatchStr::Node &node) : offset(0), len(0) {
    if (this != &node) {
        offset = node.offset;
        len = node.len;
        patch.reset(new char[node.len + 1]);
        strcpy(patch.get(), node.patch.get());
    }
}

CPatchStr::Node &CPatchStr::Node::operator=(const CPatchStr::Node &rhs) {
    if (this != &rhs) {
        len = rhs.len;
        offset = rhs.offset;
        if (rhs.patch) {
            char *newPatch = new char[len + 1];
            strcpy(newPatch, rhs.patch.get());
            patch.reset(newPatch);
        } else {
            patch.reset();
        }
    }
    return *this;
}

CPatchStr::PatchList::PatchList() : nodes(nullptr), size(0), capacity(0), strLen(0) {}
