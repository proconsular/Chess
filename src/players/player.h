//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_PLAYER_H
#define CHESS_PLAYER_H

#include "../data_types.h"
#include "../pure_states/board.h"

struct Player {
    [[nodiscard]] virtual Move move(const Board&) const = 0;
};

#endif //CHESS_PLAYER_H
