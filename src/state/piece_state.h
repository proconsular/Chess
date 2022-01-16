//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_PIECE_STATE_H
#define CHESS_PIECE_STATE_H

#include "../data_types.h"
#include "constants.h"

struct PieceState {
    PieceState(Piece piece, BoardPosition position): piece(piece), board_position(position), orientation(White) {
        set_position(position.row, position.column);
        size = sf::Vector2f(constants::piece_size, constants::piece_size);
    }

    void set_orientation(Side side) {
        orientation = side;
        set_position(board_position.row, board_position.column);
    }

    void set_position(int row, int column) {
        board_position = {row, column};
        const float offset = constants::square_size / 2 - constants::piece_size / 2;
        position.x = (float)(orientation == White ? column : 7 - column) * constants::square_size + offset;
        position.y = (float)(orientation == White ? row : 7 - row) * constants::square_size  + offset;
    }

    Piece piece;
    BoardPosition board_position;
    sf::Vector2f position, size;
    std::shared_ptr<sf::Transform> transform;

    Side orientation;
};

#endif //CHESS_PIECE_STATE_H
