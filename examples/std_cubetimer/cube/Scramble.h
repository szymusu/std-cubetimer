#ifndef STD_CUBETIMER_SCRAMBLE_H
#define STD_CUBETIMER_SCRAMBLE_H

#include <array>
#include <string>

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

    char* toString() {
        std::string out;
        for (Move& move : this->moves) {
            out.insert(0, 1, move.faceChar());
            if (move.modifier == PRIME)
                out.insert(0, 1, '\'');
            else if (move.modifier == TWO)
                out.insert(0, 1, '2');
            out.insert(0, 1, ' ');
        }
        return out.data();
    }
};


#endif //STD_CUBETIMER_SCRAMBLE_H
