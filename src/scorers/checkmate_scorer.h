//
// Created by Chris Luttio on 1/12/22.
//

#ifndef CHESS_CHECKMATE_SCORER_H
#define CHESS_CHECKMATE_SCORER_H

#include "scorer.h"
#include "data_types.h"

struct CheckmateScorer: Scorer {
    [[nodiscard]] int score(const Board &board, Side side) const override {
        if (board.checkmate()) {
            return board.last_turn_color() == side ? 100000 : -100000;
        }
        return 0;
    }
};

#endif //CHESS_CHECKMATE_SCORER_H
