//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_SHAPE_RENDERER_H
#define CHESS_SHAPE_RENDERER_H

#include "renderer.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct ShapeRenderer: Renderer {
    explicit ShapeRenderer(std::shared_ptr<sf::Shape> shape): shape(std::move(shape)) {}

    void render(sf::RenderWindow &window, sf::RenderStates &states) override {
        window.draw(*shape, states);
    }

    std::shared_ptr<sf::Shape> shape;
};

#endif //CHESS_SHAPE_RENDERER_H
