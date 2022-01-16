//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_ENTITY_H
#define CHESS_ENTITY_H

#include <memory>

#include "../receivers/receiver.h"
#include "behaviors/behavior.h"
#include "../renderers/renderer.h"

struct Entity {
    Entity(): id(0) {}

    void terminate() const {
        if (receiver)
            receiver->terminate();
        if (behavior)
            behavior->terminate();
        if (renderer)
            renderer->terminate();
    }

    uint32_t id;
    std::shared_ptr<Receiver> receiver;
    std::shared_ptr<Behavior> behavior;
    std::shared_ptr<Renderer> renderer;
};

#endif //CHESS_ENTITY_H
