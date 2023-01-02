#ifndef STD_CUBETIMER_SCRAMBLE_H
#define STD_CUBETIMER_SCRAMBLE_H

#include <array>
#include <string>
#include <ctime>
#include <random>

#include "Move.h"

class Scramble {
public:
    std::array<Move, 25> moves;

    void generate() {
        for (size_t i = 0; i < moves.size(); ++i) {
            int face = randomInt(0, 5);
            int modifier = randomInt(0, 2);
            moves[i] = {(Face)face, (Modifier)modifier};
            if (i > 0 && moves[i].face == moves[i-1].face) {
                i--;
                continue;
            }
            if (i > 1 && moves[i].face == moves[i-2].face && moves[i].isOppositeFace(moves[i-1])) {
                i--;
                continue;
            }
        }
    }

    int randomInt(int from, int to) {
        return from + rand() % (to - from);
    }

    Scramble() {
        srand(time(nullptr));
        generate();
    }
};


#endif //STD_CUBETIMER_SCRAMBLE_H
