//
// Created by Chris Luttio on 12/15/21.
//

#ifndef CHESS_EVENT_H
#define CHESS_EVENT_H

#include <SFML/Graphics.hpp>

enum Events {
    MouseDown,
    MouseMove,
    MouseUp,
    KeyDown,
    KeyUp,
    Wheel,
};

struct InputEvent {
    explicit InputEvent(Events type): type(type), _consumed(false) {}

    void consume() {
        _consumed = true;
    }

    [[nodiscard]] bool consumed() const {
        return _consumed;
    }

    virtual ~InputEvent() = default;

    Events type;
private:
    bool _consumed;
};

struct MouseButtonEvent: InputEvent {
    MouseButtonEvent(Events type, sf::Vector2i location, sf::Mouse::Button button): location(location), button(button), InputEvent(type) {}

    sf::Vector2i location;
    sf::Mouse::Button button;
};

struct MouseMoveEvent: InputEvent {
    MouseMoveEvent(sf::Vector2i location, sf::Mouse::Button button): location(location), button(button), InputEvent(MouseMove) {}

    sf::Vector2i location;
    sf::Mouse::Button button;
};


#endif //CHESS_EVENT_H
