//
// Created by Chris Luttio on 12/14/21.
//

#ifndef CHESS_PIECE_RENDERER_H
#define CHESS_PIECE_RENDERER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "renderer.h"
#include "../data_types.h"
#include "../state/piece_state.h"
#include "constants.h"

struct PieceRenderer: Renderer {
    explicit PieceRenderer(const std::shared_ptr<PieceState>& state, const std::shared_ptr<sf::Texture>& texture): state(state) {
        shape = sf::Sprite(*texture);
        const float size = 333.0f;
        shape.setScale(constants::piece_size / size, constants::piece_size / size);
        shape.setPosition(state->position);
    }

    void set_texture(Pieces type) {
        const float size = 333.0f;
        int xo = 0;
        switch (type) {
            case Pawn:
                xo = 0;
                break;
            case Rook:
                xo = 1;
                break;
            case Knight:
                xo = 2;
                break;
            case Bishop:
                xo = 3;
                break;
            case Queen:
                xo = 4;
                break;
            case King:
                xo = 5;
                break;
            default:
                break;
        }
        xo = 5 - xo;
        int yo = state->piece.side == White ? 0 : 1;
        shape.setTextureRect(sf::IntRect(size * xo, size * yo, size, size));
    }

    void render(sf::RenderWindow& window, sf::RenderStates& states) override {
        shape.setPosition(state->position);
        set_texture(state->piece.type);
        window.draw(shape, states);
    }

    sf::Sprite shape;
    std::shared_ptr<PieceState> state;
};

#endif //CHESS_PIECE_RENDERER_H
