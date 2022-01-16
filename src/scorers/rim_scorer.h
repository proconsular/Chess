//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_RIM_SCORER_H
#define CHESS_RIM_SCORER_H

#include "scorer.h"

struct PiecesOnRimScorer: Scorer {
    [[nodiscard]] int score(const Board &board, Side side) const override {
        auto pieces = board.get_pieces(side);
        int value = 0;
        for (const auto& pos: pieces) {
            auto piece = board.get_piece_at(pos);
            if (piece.type == Knight || piece.type == Bishop || piece.type == Queen) {
                if (!(pos.row == 0 || pos.row == 7 || pos.column == 0 || pos.column == 7)) {
                    value += 1;
                }
            }
        }
        return value;
    }
};

#endif //CHESS_RIM_SCORER_H
