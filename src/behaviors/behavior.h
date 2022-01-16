//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_BEHAVIOR_H
#define CHESS_BEHAVIOR_H

#include "agent.h"

struct Behavior: Agent {
    virtual void update() = 0;
};

#endif //CHESS_BEHAVIOR_H
