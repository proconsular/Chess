//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_BOARD_ENTITY_H
#define CHESS_BOARD_ENTITY_H

#include <set>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "stateful_entity.h"
#include "constants.h"
#include "../state/board_state.h"
#include "../renderers/multi_renderer.h"
#include "../factory.h"
#include "../piece_factory.h"
#include "../renderers/shape_renderer.h"
#include "../behaviors/piece_translation_behavior.h"

struct BoardEntity: StatefulEntity<BoardState> {
     explicit BoardEntity(const std::shared_ptr<sf::Transform>& transform, std::unique_ptr<PieceFactory>&& factory) {
         this->factory = std::move(factory);
         const float outline_thickness = 5.f;
         tile_highlight = std::make_shared<ShapeRenderer>(std::make_shared<sf::RectangleShape>(sf::Vector2f(constants::square_size - outline_thickness * 2, constants::square_size - outline_thickness * 2)));
         tile_highlight->shape->setFillColor(sf::Color(100, 150, 200, 100));
         tile_highlight->shape->setPosition(-100, -100);
         tile_highlight->shape->setOutlineThickness(outline_thickness);
         tile_highlight->shape->setOutlineColor(sf::Color(100, 150, 200, 200));
         tile_highlight->shape->setOrigin((constants::square_size - outline_thickness * 2) / 2, (constants::square_size - outline_thickness * 2) / 2);
         state = std::make_shared<BoardState>(transform);
         auto container = std::make_shared<MultiRenderer>();
         container->push_back(tile_highlight);
         renderer = container;
         behavior = std::make_shared<MultiBehavior>();
     }

     void set_orientation(Side side) {
         if (state->orientation == side)
             return;
         state->orientation = side;
         for (const auto& [key, value]: state->pieces) {
             value->state->set_orientation(side);
         }
     }

     void set_hover(BoardPosition pos) {
         tile_highlight->shape->setPosition((float)pos.column * constants::square_size + constants::square_size / 2, (float)pos.row * constants::square_size + constants::square_size / 2);
     }

     void move(const Move& move) {
         Move m = move;
         if (state->board.legal(m) && state->board.last_turn_color() != state->board.get_piece_at(m.current).side) {
             state->board.move(m);
//             piece_moved->play();
//             if (state->board.king_in_check(White) || state->board.king_in_check(Black)) {
//                 in_check->play();
//             }
         }
         set(state->board);
     }

     void set(const Board& board) {
        std::set<int> ids_found;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                auto piece = board.get_piece_at(y, x);
                if (piece.type == None)
                    continue;
                ids_found.insert(piece.id);
                if (state->pieces.find(piece.id) == state->pieces.end()) {
                    factory->piece = piece;
                    factory->position = {y, x};
                    auto entity = factory->make();
                    entity->state->transform = state->transform;
                    state->pieces[piece.id] = entity;
                    if (auto container = std::dynamic_pointer_cast<MultiRenderer>(renderer)) {
                        container->push_back(entity->renderer);
                    }
                    if (auto container = std::dynamic_pointer_cast<MultiBehavior>(entity->behavior)) {
                        container->push_back(std::make_shared<PieceTranslationBehavior>(entity->state, 20.0f));
                    }
                    if (auto container = std::dynamic_pointer_cast<MultiBehavior>(behavior)) {
                        container->push_back(entity->behavior);
                    }
                    entity->state->set_position(y, x);
                }
                auto entity = state->pieces[piece.id];
                if (auto container = std::dynamic_pointer_cast<MultiBehavior>(entity->behavior)) {
                    if (auto translator = std::dynamic_pointer_cast<PieceTranslationBehavior>(container->behaviors.front())) {
                        translator->move({y, x});
                    }
                }
                entity->state->piece = piece;
                entity->state->board_position = {y, x};
            }
        }
        std::vector<int> removed;
        for (const auto& [key, value]: state->pieces) {
            if (ids_found.find(key) == ids_found.end())
                removed.push_back(key);
        }
        for (const auto& id: removed) {
            state->pieces[id]->terminate();
            state->pieces.erase(id);
        }
        state->board = board;
    }

    std::shared_ptr<sf::Sound> piece_moved, in_check;
private:
    std::shared_ptr<ShapeRenderer> tile_highlight;
    std::unique_ptr<PieceFactory> factory;
};

#endif //CHESS_BOARD_ENTITY_H
