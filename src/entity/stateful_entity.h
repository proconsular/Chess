//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_STATEFUL_ENTITY_H
#define CHESS_STATEFUL_ENTITY_H

#include <memory>
#include "entity.h"

template <typename State>
struct StatefulEntity: Entity {
    std::shared_ptr<State> state;
};

#endif //CHESS_STATEFUL_ENTITY_H
