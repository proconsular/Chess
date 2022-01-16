//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_PIECE_DRAG_RECEIVER_H
#define CHESS_PIECE_DRAG_RECEIVER_H

#include "receiver.h"
#include "../state/piece_state.h"
#include <iostream>
#include <cmath>
#include "constants.h"

struct PieceDragReceiver: Receiver {
    explicit PieceDragReceiver(std::shared_ptr<PieceState> state, std::shared_ptr<BoardEntity> board): state(std::move(state)), board(std::move(board)), held(false) {}

    void receive(InputEvent *event) override {
        auto bounds = sf::FloatRect(state->transform->transformPoint(state->position), state->size);
        int column = floor((state->position.x + constants::piece_size / 2.f) / constants::square_size);
        int row = floor((state->position.y + constants::piece_size / 2.f) / constants::square_size);
        switch (event->type) {
            case MouseDown: {
                if (auto* mouse_event = dynamic_cast<MouseButtonEvent*>(event)) {
                    if (bounds.contains((float)mouse_event->location.x, (float)mouse_event->location.y)) {
                        held = true;
                        offset = -sf::Vector2f(constants::piece_size, constants::piece_size) / 2.0f;
                    }
                }
                break;
            }
            case MouseMove: {
                if (held) {
                    if (auto* mouse_event = dynamic_cast<MouseMoveEvent*>(event)) {
                        auto vec = sf::Vector2f((float)mouse_event->location.x, (float)mouse_event->location.y);
                        state->position.x = vec.x + offset.x;
                        state->position.y = vec.y + offset.y;
                        board->set_hover({row, column});
                    }
                }
                break;
            }
            case MouseUp: {
                if (held) {
                    row = state->orientation == White ? row : 7 - row;
                    column = state->orientation == White ? column : 7 - column;
                    board->set_hover({-1, -1});
                    Move move{
                        state->board_position,
                        BoardPosition{row, column},
                    };
                    board->move(move);
                }
                held = false;
                break;
            }
            default:
                break;
        }
    }

    sf::Vector2f offset;
    bool held;
    std::shared_ptr<BoardEntity> board;
    std::shared_ptr<PieceState> state;
};

#endif //CHESS_PIECE_DRAG_RECEIVER_H
