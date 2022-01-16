//
// Created by Chris Luttio on 1/9/22.
//

#include "utils.h"

sf::Vector2f compute_piece_position(BoardPosition position, Side orientation) {
    const float offset = constants::square_size / 2 - constants::piece_size / 2;
    sf::Vector2f pos;
    pos.x = (float)(orientation == White ? position.column : 7 - position.column) * constants::square_size + offset;
    pos.y = (float)(orientation == White ? position.row : 7 - position.row) * constants::square_size  + offset;
    return pos;
}