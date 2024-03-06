#ifndef __TRAINER__

#include <iostream>

#endif

struct Cuboid {
    int sides[3];
};

int readCuboid(Cuboid &cuboid) {
    std::cin >> cuboid.sides[0] >> cuboid.sides[1] >> cuboid.sides[2];

    return std::cin && cuboid.sides[0] > 0 && cuboid.sides[1] > 0 && cuboid.sides[2] > 0;
}

#ifndef __TRAINER__

int main() {
    Cuboid cuboid;
    if (!readCuboid(cuboid)) {
        std::cout << "Nespravny vstup." << std::endl;
        return 1;
    }
    std::cout << cuboid.sides[0] << ' ' << cuboid.sides[1] << ' ' << cuboid.sides[2] << std::endl;
    return 0;
}

#endif