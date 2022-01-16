//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_MULTI_BEHAVIOR_H
#define CHESS_MULTI_BEHAVIOR_H

#include "behavior.h"

#include <list>

struct MultiBehavior: Behavior {

    void push_back(const std::shared_ptr<Behavior>& behavior) {
        behaviors.push_back(behavior);
    }

    void update() override {
        for (auto iter = behaviors.begin(); iter != behaviors.end();) {
            if (iter->get()->alive()) {
                iter->get()->update();
                ++iter;
            } else {
                iter = behaviors.erase(iter);
            }
        }
    }

    std::list<std::shared_ptr<Behavior>> behaviors;
};

#endif //CHESS_MULTI_BEHAVIOR_H
