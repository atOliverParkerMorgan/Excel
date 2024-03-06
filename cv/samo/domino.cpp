#include <iostream>
#include <vector>
#include <algorithm>

struct Domino {
    int x, y;

    // Konstruktor pro pohodlné vytváření kostky
    Domino(){};
    Domino(int x, int y) : x(x), y(y) {}

    // Funkce pro otáčení kostky
    void rotate() {
        std::swap(x, y);
    }

    // Operátor < pro porovnávání kostek
    bool operator<(const Domino& other) const {
        return std::make_pair(x, y) < std::make_pair(other.x, other.y);
    }
};

// Funkce pro načítání dominových kostek ze vstupu
bool readDomino(Domino& domino) {
    char d1,d2,d3;
    if (std::cin >> d1 >> domino.x >> d2 >> domino.y >> d3) {
        return d1=='[' && d2 == ',' && d3 == ']' && std::cin.good();
    } else {
        return false;
    }
}
// Non-member equality operator for Domino
bool operator==(const Domino& lhs, const Domino& rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y) || (lhs.x == rhs.y && lhs.y == rhs.x);
}

int main() {
    std::vector<Domino> dominos;
    Domino tempDomino;

    // Načítání dominových kostek real ukládání do dynamického pole
    while (readDomino(tempDomino)) {
        // Otáčení kostky tak, aby x <= y
        if (tempDomino.x > tempDomino.y) {
            tempDomino.rotate();
        }

        // Kontrola, zda kostka již není v poli
        if (std::find(dominos.begin(), dominos.end(), tempDomino) == dominos.end()) {
            dominos.push_back(tempDomino);
        }
    }

    // Seřazení pole kostek
    std::sort(dominos.begin(), dominos.end());

    // Výpis počtu unikátních kostek
    std::cout << "Pocet unikatnich kostek: " << dominos.size() << std::endl;

    return 0;
}
