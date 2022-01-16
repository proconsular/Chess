//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_BOARD_RENDERER_H
#define CHESS_BOARD_RENDERER_H

#include "multi_renderer.h"
#include "../state/board_state.h"

struct BoardRenderer: MultiRenderer {
    explicit BoardRenderer(std::shared_ptr<BoardState> state): state(std::move(state)) {}

    void render(sf::RenderWindow &window, sf::RenderStates& states) override {
        sf::RenderStates transform;
        transform.transform = state->transform;
        MultiRenderer::render(window, transform);
    }

    std::shared_ptr<BoardState> state;
};

#endif //CHESS_BOARD_RENDERER_H
