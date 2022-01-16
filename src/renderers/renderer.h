//
// Created by Chris Luttio on 12/14/21.
//

#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "../agent.h"

struct Renderer: Agent {
    virtual void render(sf::RenderWindow&, sf::RenderStates&) = 0;
};

#endif //CHESS_RENDERER_H
