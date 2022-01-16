//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_MATERIAL_SCORER_H
#define CHESS_MATERIAL_SCORER_H

#include "scorer.h"
#include "utils.h"

struct MaterialScorer: Scorer {
    [[nodiscard]] int score(const Board &board, Side color) const override {
        int piece_score = 0;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                auto piece = board.get_piece_at(y, x);
                if (piece.type == None)
                    continue;
                int side = (piece.side == color ? 1 : -1);
                piece_score += side * get_piece_value(piece.type);
            }
        }
        return piece_score;
    }
};

#endif //CHESS_MATERIAL_SCORER_H
