#ifndef __TRAINER__
#include <algorithm>
#include <cassert>

// struktura pro kvádr
// ...

// ... equalCuboids(..., ...) {}
#endif

void normalizeCuboid(...) {
    // ...
}

#ifndef __TRAINER__
int main () {
    Cuboid a{{2, 6, 4}};
    Cuboid b{{2, 4, 6}};
    normalizeCuboid(a);
    assert(equalCuboids(a, b));
    Cuboid c{{10, 6, 4}};
    normalizeCuboid(c);
    assert(equalCuboids(c, {{ 4, 6, 10 }}));
    return 0;
}
#endif