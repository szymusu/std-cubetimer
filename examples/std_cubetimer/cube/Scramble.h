#ifndef STD_CUBETIMER_SCRAMBLE_H
#define STD_CUBETIMER_SCRAMBLE_H

#include <array>
#include <string>

#include "Move.h"

class Scramble {
private:
    std::array<Move, 25> moves;
    std::string movesNotation;

    void generate() {
        moves.fill({F, TWO});
    }

    void generateNotation() {
        for (size_t i = 0; i < moves.size(); i++) {
            movesNotation[2*i] = moves[i].faceChar();
            char suffix = ' ';
            if (moves[i].modifier == PRIME)
                suffix = '\'';
            else if (moves[i].modifier == TWO)
                suffix = '2';
            movesNotation[2*i + 1] = suffix;
        }
    }

public:
    char* getNotationChars() {
        return movesNotation.data();
    }

    Scramble() {
        generate();
        generateNotation();
        printf("%s\n", getNotationChars());
    }
};


#endif //STD_CUBETIMER_SCRAMBLE_H
