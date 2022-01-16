//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_AGENT_H
#define CHESS_AGENT_H

struct Agent {
    [[nodiscard]] bool alive() const {
        return _alive;
    }

    void terminate() {
        _alive = false;
    }
protected:
    bool _alive{true};
};

#endif //CHESS_AGENT_H
