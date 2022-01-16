//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_PIECE_TRANSLATION_BEHAVIOR_H
#define CHESS_PIECE_TRANSLATION_BEHAVIOR_H

#include "behavior.h"

#include <utility>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "../utils.h"
#include "../state/piece_state.h"

struct PieceTranslationBehavior: Behavior {
    PieceTranslationBehavior(std::shared_ptr<PieceState> state, float speed): state(std::move(state)), to({-1, -1}), percent(0), moving(false), speed(speed), percent_speed(0) {}

    void update() override {
        if (moving) {
            auto to_pos = compute_piece_position(to, state->orientation);
            percent = std::clamp(percent + compute_ease_movement(percent) * percent_speed, 0.f, 1.f);
            auto interpolation = (to_pos - from_pos) * percent + from_pos;
            state->position = interpolation;
            if (percent >= 1)
                moving = false;
        }
    }

    static float compute_ease_movement(float percent) {
        return (-2.0f * powf(percent - 0.5f, 2) + 0.5f) + 0.1f;
    }

    void move(BoardPosition to) {
        this->to = to;
        from_pos = state->position;
        auto to_pos = compute_piece_position(to, state->orientation);
        auto diff = to_pos - from_pos;
        auto distance = sqrt(diff.x * diff.x + diff.y * diff.y);
        percent_speed = speed / distance;
        moving = true;
        percent = 0;
    }

private:
    sf::Vector2f from_pos;
    BoardPosition to;
    float percent;
    float percent_speed;
    bool moving;

    float speed;

    std::shared_ptr<PieceState> state;
};

#endif //CHESS_PIECE_TRANSLATION_BEHAVIOR_H
