//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_PIECE_ENTITY_H
#define CHESS_PIECE_ENTITY_H

#include "../behaviors/multi_behavior.h"

#include "stateful_entity.h"
#include "../state/piece_state.h"
#include "../renderers/piece_renderer.h"

struct PieceEntity: StatefulEntity<PieceState> {
    PieceEntity(Piece piece, BoardPosition position) {
        state = std::make_shared<PieceState>(piece, position);
        behavior = std::make_shared<MultiBehavior>();
    }
};

#endif //CHESS_PIECE_ENTITY_H
