//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_BOARD_STATE_H
#define CHESS_BOARD_STATE_H

#include <utility>

#include "../pure_states/board.h"
#include "../pure_states/board.h"
#include "../data_types.h"
#include "../entity/piece_entity.h"

struct BoardState {
    explicit BoardState(std::shared_ptr<sf::Transform> transform): transform(std::move(transform)), orientation(White), hovering(false) {}

    std::shared_ptr<sf::Transform> transform;
    std::map<int, std::shared_ptr<PieceEntity>> pieces;
    Board board;

    Side orientation;

    BoardPosition hover_position;
    bool hovering;
};

#endif //CHESS_BOARD_STATE_H
