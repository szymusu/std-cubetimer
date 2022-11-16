#ifndef STD_CUBETIMER_SCRAMBLE_H
#define STD_CUBETIMER_SCRAMBLE_H

#include <array>

#include "Move.h"

class Scramble {
private:
    std::array<Move, 25> moves;

    void generate() {
        moves.fill({F, TWO});
    }

public:
    Scramble() {
        generate();
    }
};


#endif //STD_CUBETIMER_SCRAMBLE_H
