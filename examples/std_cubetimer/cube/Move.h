#ifndef STD_CUBETIMER_MOVE_H
#define STD_CUBETIMER_MOVE_H

enum Face {
    U, R, F, D, L, B
};

enum Modifier {
    NONE, PRIME, TWO
};

struct Move {
    Face face;
    Modifier modifier;

    Move reverse() {
        if (modifier == TWO)
            return this;
        else if (modifier == NONE)
            return { this->face, PRIME };
        else
            return { this->face, NONE };
    }

    bool isOppositeFace(Move other) {
        return (this->face + 3 % 6) == other.face;
    }
};


#endif //STD_CUBETIMER_MOVE_H
