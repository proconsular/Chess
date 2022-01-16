//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_SCORER_H
#define CHESS_SCORER_H

#include "data_types.h"
#include "pure_states/board.h"

struct Scorer {
    [[nodiscard]] virtual int score(const Board&, Side) const = 0;
};

#endif //CHESS_SCORER_H
