#ifndef STD_CUBETIMER_EDGE_H
#define STD_CUBETIMER_EDGE_H

#include <array>

#include "Move.h"

class Edge {
    std::array<Face, 2> stickers;
    bool flipped = false;

public:
    Edge(Face sticker1, Face sticker2) {
        stickers[0] = sticker1;
        stickers[1] = sticker2;
    }

    void flip() {
        flipped = !flipped;
    }

    bool operator== (const Edge &other) const {
        return this->stickers == other.stickers && this->flipped == other.flipped;
    }
};

#endif //STD_CUBETIMER_EDGE_H
