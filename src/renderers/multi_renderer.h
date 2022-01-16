//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_MULTI_RENDERER_H
#define CHESS_MULTI_RENDERER_H

#include "renderer.h"

#include <list>
#include <memory>
#include <utility>

struct MultiRenderer: Renderer {
    MultiRenderer(): renderers() {}
    explicit MultiRenderer(std::list<std::weak_ptr<Renderer>> renderers): renderers(std::move(renderers)) {}

    void push_back(const std::weak_ptr<Renderer>& renderer) {
        renderers.push_back(renderer);
    }

    void render(sf::RenderWindow &window, sf::RenderStates& states) override {
        for (auto iter = renderers.begin(); iter != renderers.end();) {
            if (auto ptr = iter->lock(); ptr && ptr->alive()) {
                ptr->render(window, states);
                ++iter;
            } else {
                iter = renderers.erase(iter);
            }
        }
    }

protected:
    std::list<std::weak_ptr<Renderer>> renderers;
};

#endif //CHESS_MULTI_RENDERER_H
