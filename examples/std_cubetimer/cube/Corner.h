#ifndef STD_CUBETIMER_CORNER_H
#define STD_CUBETIMER_CORNER_H

#include <array>

#include "Move.h"

class Corner {
public:
    std::array<Face, 3> stickers;
    unsigned char rotation = 0;

    Corner(Face sticker1, Face sticker2, Face sticker3) {
        stickers[0] = sticker1;
        stickers[1] = sticker2;
        stickers[2] = sticker3;
        rotation = 0;
    }

    void rotate(unsigned char times) {
        rotation = (rotation+times) % 3;
    }

    bool operator== (const Corner &other) const {
        return this->stickers == other.stickers && this->rotation == other.rotation;
    }
};

#endif //STD_CUBETIMER_CORNER_H
