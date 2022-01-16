//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_RECEIVER_H
#define CHESS_RECEIVER_H

#include "../event.h"
#include "../agent.h"

struct Receiver: Agent {
    virtual void receive(InputEvent*) = 0;
};

#endif //CHESS_RECEIVER_H
