//
// Created by Chris Luttio on 12/16/21.
//

#ifndef CHESS_PIECE_FACTORY_H
#define CHESS_PIECE_FACTORY_H

#include "factory.h"
#include "entity/piece_entity.h"

struct PieceFactory: Factory<std::shared_ptr<PieceEntity>> {
    explicit PieceFactory(std::shared_ptr<sf::Texture> texture):
        texture(std::move(texture)),
        position{0, 0},
        piece{}
        {}

    [[nodiscard]] std::shared_ptr<PieceEntity> make() override {
        auto entity = std::make_shared<PieceEntity>(piece, position);
        entity->renderer = std::make_shared<PieceRenderer>(entity->state, texture);
        return entity;
    }

    Piece piece;
    BoardPosition position;
    std::shared_ptr<sf::Texture> texture;
};

#endif //CHESS_PIECE_FACTORY_H
