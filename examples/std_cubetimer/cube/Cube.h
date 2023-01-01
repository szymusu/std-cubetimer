#ifndef STD_CUBETIMER_CUBE_H
#define STD_CUBETIMER_CUBE_H

#include <array>

#include "Corner.h"
#include "Edge.h"
#include "Move.h"

struct CornerStickerPosition {
    int cornerNumber;
    unsigned char rotation;
};
struct EdgeStickerPosition {
    int edgeNumber;
    bool isOther;
};

struct Layer {
    CornerStickerPosition cornerPositions[4];
    EdgeStickerPosition edgePositions[4];
};

class Cube {
public:
    void makeMove(Move move) {
        Layer layer = layers[move.face];
        if (move.modifier == TWO) {
            Corner tmpCorner = corners[layer.cornerPositions[0].cornerNumber - 1];
            corners[layer.cornerPositions[0].cornerNumber - 1] = corners[layer.cornerPositions[2].cornerNumber - 1];
            corners[layer.cornerPositions[2].cornerNumber - 1] = tmpCorner;
            tmpCorner = corners[layer.cornerPositions[1].cornerNumber - 1];
            corners[layer.cornerPositions[1].cornerNumber - 1] = corners[layer.cornerPositions[3].cornerNumber - 1];
            corners[layer.cornerPositions[3].cornerNumber - 1] = tmpCorner;

            Edge tmpEdge = edges[layer.edgePositions[0].edgeNumber - 1];
            edges[layer.edgePositions[0].edgeNumber - 1] = edges[layer.edgePositions[2].edgeNumber - 1];
            edges[layer.edgePositions[2].edgeNumber - 1] = tmpEdge;
            tmpEdge = edges[layer.edgePositions[1].edgeNumber - 1];
            edges[layer.edgePositions[1].edgeNumber - 1] = edges[layer.edgePositions[3].edgeNumber - 1];
            edges[layer.edgePositions[3].edgeNumber - 1] = tmpEdge;
        }
        else if (move.modifier == NONE) {
            Corner tmpCorner = corners[layer.cornerPositions[0].cornerNumber - 1];
            corners[layer.cornerPositions[0].cornerNumber - 1] = corners[layer.cornerPositions[1].cornerNumber - 1];
            corners[layer.cornerPositions[1].cornerNumber - 1] = corners[layer.cornerPositions[2].cornerNumber - 1];
            corners[layer.cornerPositions[2].cornerNumber - 1] = corners[layer.cornerPositions[3].cornerNumber - 1];
            corners[layer.cornerPositions[3].cornerNumber - 1] = tmpCorner;

            Edge tmpEdge = edges[layer.edgePositions[0].edgeNumber - 1];
            edges[layer.edgePositions[0].edgeNumber - 1] = edges[layer.edgePositions[1].edgeNumber - 1];
            edges[layer.edgePositions[1].edgeNumber - 1] = edges[layer.edgePositions[2].edgeNumber - 1];
            edges[layer.edgePositions[2].edgeNumber - 1] = edges[layer.edgePositions[3].edgeNumber - 1];
            edges[layer.edgePositions[3].edgeNumber - 1] = tmpEdge;

            if (move.face != U && move.face != D) {
                corners[layer.cornerPositions[0].cornerNumber - 1].rotate(2);
                corners[layer.cornerPositions[1].cornerNumber - 1].rotate(1);
                corners[layer.cornerPositions[2].cornerNumber - 1].rotate(2);
                corners[layer.cornerPositions[3].cornerNumber - 1].rotate(1);
            }

            if (move.face == F || move.face == B) {
                for (int i = 0; i < 4; ++i) {
                    edges[layer.edgePositions[i].edgeNumber - 1].flip();
                }
            }
        }
        else if (move.modifier == PRIME) {
            Corner tmpCorner = corners[layer.cornerPositions[3].cornerNumber - 1];
            corners[layer.cornerPositions[3].cornerNumber - 1] = corners[layer.cornerPositions[2].cornerNumber - 1];
            corners[layer.cornerPositions[2].cornerNumber - 1] = corners[layer.cornerPositions[1].cornerNumber - 1];
            corners[layer.cornerPositions[1].cornerNumber - 1] = corners[layer.cornerPositions[0].cornerNumber - 1];
            corners[layer.cornerPositions[0].cornerNumber - 1] = tmpCorner;

            Edge tmpEdge = edges[layer.edgePositions[3].edgeNumber - 1];
            edges[layer.edgePositions[3].edgeNumber - 1] = edges[layer.edgePositions[2].edgeNumber - 1];
            edges[layer.edgePositions[2].edgeNumber - 1] = edges[layer.edgePositions[1].edgeNumber - 1];
            edges[layer.edgePositions[1].edgeNumber - 1] = edges[layer.edgePositions[0].edgeNumber - 1];
            edges[layer.edgePositions[0].edgeNumber - 1] = tmpEdge;

            if (move.face != U && move.face != D) {
                corners[layer.cornerPositions[0].cornerNumber - 1].rotate(2);
                corners[layer.cornerPositions[1].cornerNumber - 1].rotate(1);
                corners[layer.cornerPositions[2].cornerNumber - 1].rotate(2);
                corners[layer.cornerPositions[3].cornerNumber - 1].rotate(1);
            }

            if (move.face == F || move.face == B) {
                for (int i = 0; i < 4; ++i) {
                    edges[layer.edgePositions[i].edgeNumber - 1].flip();
                }
            }
        }
    }

    bool isSolved() {
        int mismatches = 0;
        for (size_t i = 0; i < corners.size(); ++i) {
            if (corners[i] != SOLVED_CORNERS[i]) {
                std::cout << "Mismatch corner " << i << std::endl;
                std::cout << "Rotation: " << static_cast<int>(corners[i].rotation) << std::endl;
                mismatches++;
            }
        }
        for (size_t i = 0; i < edges.size(); ++i) {
            if (edges[i] != SOLVED_EDGES[i]) {
                std::cout << "Mismatch edge " << i << std::endl;
                mismatches++;
            }
        }
        if (mismatches > 0) {
            std::cout << mismatches << " mismatches" << std::endl;
            return false;
        }
        else return true;
//        return corners == SOLVED_CORNERS && edges == SOLVED_EDGES;
    }

private:
    const Layer LAYER_U = {{
                {1, 0},
                {2, 0},
                {3, 0},
                {4, 0}},
                           {
                {1, false},
                {2, false},
                {3, false},
                {4, false}}
    };
    const Layer LAYER_D = {{
                {8, 0},
                {7, 0},
                {6, 0},
                {5, 0}},
                           {
                {3, false},
                {2, false},
                {1, false},
                {4, false}}
    };
    const Layer LAYER_F = {{
                {4, 1},
                {3, 2},
                {7, 1},
                {8, 2}},
                           {
                {3, true},
                {7, false},
                {11, true},
                {8, false}}
    };
    const Layer LAYER_B = {{
                {2, 1},
                {1, 2},
                {5, 1},
                {6, 2}},
                           {
                {1, true},
                {5, false},
                {9, true},
                {6, false}}
    };
    const Layer LAYER_R = {{
                {3, 1},
                {2, 2},
                {6, 1},
                {7, 2}},
                           {
                {2, true},
                {6, true},
                {10, true},
                {7, true}}
    };
    const Layer LAYER_L = {{
                {1, 1},
                {4, 2},
                {8, 1},
                {5, 2}},
                           {
                {4, true},
                {8, true},
                {12, true},
                {1, true}}
    };

    Layer layers[6] = {
            LAYER_U, LAYER_R, LAYER_F, LAYER_D, LAYER_L, LAYER_B
    };

    const std::array<Corner, 8> SOLVED_CORNERS = {
            Corner(U, L, B),
            Corner(U, B, R),
            Corner(U, R, F),
            Corner(U, F, L),
            Corner(D, B, L),
            Corner(D, R, B),
            Corner(D, F, R),
            Corner(D, L, F)
    };
    const std::array<Edge, 12> SOLVED_EDGES = {
            Edge(U, B),
            Edge(U, R),
            Edge(U, F),
            Edge(U, L),
            Edge(B, L),
            Edge(B, R),
            Edge(F, R),
            Edge(F, L),
            Edge(D, B),
            Edge(D, R),
            Edge(D, F),
            Edge(D, L)
    };
    std::array<Corner, 8> corners = {SOLVED_CORNERS[0], SOLVED_CORNERS[1], SOLVED_CORNERS[2], SOLVED_CORNERS[3], SOLVED_CORNERS[4], SOLVED_CORNERS[5], SOLVED_CORNERS[6], SOLVED_CORNERS[7]};
    std::array<Edge, 12> edges = {
            SOLVED_EDGES[0], SOLVED_EDGES[1], SOLVED_EDGES[2], SOLVED_EDGES[3], SOLVED_EDGES[4], SOLVED_EDGES[5],
            SOLVED_EDGES[6], SOLVED_EDGES[7], SOLVED_EDGES[8], SOLVED_EDGES[9], SOLVED_EDGES[10], SOLVED_EDGES[11]
    };
};

#endif //STD_CUBETIMER_CUBE_H
