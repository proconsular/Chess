//
// Created by Chris Luttio on 12/16/21.
//

#ifndef CHESS_FACTORY_H
#define CHESS_FACTORY_H

template <typename T>
struct Factory {
    virtual T make() = 0;
};

#endif //CHESS_FACTORY_H
