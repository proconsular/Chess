//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_MULTI_RECEIVER_H
#define CHESS_MULTI_RECEIVER_H

#include "receiver.h"
#include <list>
#include <memory>

struct MultiReceiver: Receiver {
    MultiReceiver(): receivers() {}
    explicit MultiReceiver(std::list<std::shared_ptr<Receiver>> receivers): receivers(std::move(receivers)) {}

    void push_back(const std::shared_ptr<Receiver>& receiver) {
        receivers.push_back(receiver);
    }

    void receive(InputEvent *event) override {
        for (auto iter = receivers.begin(); iter != receivers.end();) {
            if (iter->get()->alive()) {
                iter->get()->receive(event);
                ++iter;
            } else {
                iter = receivers.erase(iter);
            }
        }
    }

protected:
    std::list<std::shared_ptr<Receiver>> receivers;
};

#endif //CHESS_MULTI_RECEIVER_H
