#ifndef STD_CUBETIMER_MOVE_H
#define STD_CUBETIMER_MOVE_H

enum Face {
    U, R, F, D, L, B
};

enum Modifier {
    NONE, PRIME, TWO
};

class Move {
public:
    Face face;
    Modifier modifier;

    Move reverse() {
        if (modifier == TWO)
            return *this;
        else if (modifier == NONE)
            return { this->face, PRIME };
        else
            return { this->face, NONE };
    }

    bool isOppositeFace(Move other) {
        return (this->face + 3 % 6) == other.face;
    }

    char faceChar() {
        switch (face) {
            case U:
                return 'U';
            case R:
                return 'R';
            case F:
                return 'F';
            case D:
                return 'D';
            case L:
                return 'L';
            case B:
                return 'B';
            default:
                return 'X';
        }
    }
};


#endif //STD_CUBETIMER_MOVE_H
