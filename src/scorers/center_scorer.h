//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_CENTER_SCORER_H
#define CHESS_CENTER_SCORER_H

#include "scorer.h"
#include <vector>

struct AccurateCenterScorer: Scorer {
    [[nodiscard]] int score(const Board &board, Side side) const override {
        auto pieces = board.get_pieces(side);

        int value = 0;

        const std::vector<BoardPosition> center{{3, 3}, {3, 4}, {4, 3}, {4, 4}};

        for (const auto& piece: pieces) {
            for (const auto& pos: center) {
                Move move{piece, pos};
                if (board.legal(move))
                    value += 1;
            }
        }

        return value;
    }
};

#endif //CHESS_CENTER_SCORER_H
