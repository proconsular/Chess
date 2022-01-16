#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "src/data_types.h"
#include "src/entity/board_entity.h"
#include "src/entity/piece_entity.h"

#include "src/receivers/multi_receiver.h"
#include "src/receivers/piece_drag_receiver.h"
#include "src/event.h"

#include "constants.h"
#include "players/smart_ai_player.h"
#include "players/autonomous_player.h"

using namespace std;
using namespace sf;

int main() {
    const int window_width = 720, window_height = 720;

    auto video_mode = sf::VideoMode(window_width, window_height);
    sf::RenderWindow window(video_mode, "Chess");
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("resources/fonts/Arial.ttf")) {
        std::cerr << "Error: Font not loaded.\n";
        exit(1);
    }

    auto pieces = std::make_shared<Texture>();
    if (!pieces->loadFromFile("resources/textures/chess.png")) {
        std::cerr << "Error: pieces.png not loaded.\n";
        exit(1);
    }
    pieces->setSmooth(true);

    SoundBuffer piece_move_sound;
    if (!piece_move_sound.loadFromFile("resources/sounds/chess_move.wav")) {
        std::cerr << "Error: chess_move.wav not loaded.\n";
        exit(1);
    }

    SoundBuffer in_check_sound;
    if (!in_check_sound.loadFromFile("resources/sounds/check.wav")) {
        std::cerr << "Error: check.wav not loaded.\n";
        exit(1);
    }

    const int board_width = 8, board_height = 8;

    const float board_total_width = constants::square_size * board_width;
    const float board_total_height = constants::square_size * board_height;

    const float board_x = (float)window_width / 2.0f - board_total_width / 2.0f;
    const float board_y = (float)window_height / 2.0f - board_total_height / 2.0f;

    sf::RectangleShape rect(sf::Vector2f(board_total_width, board_total_height));
    rect.setPosition(board_x, board_y);
    rect.setFillColor(sf::Color(255, 255, 255));

    bool color = false;
    vector<RectangleShape> tiles;
    for (int y = 0; y < board_height; y++) {
        for (int x = 0; x < board_width; x++) {
            RectangleShape tile(Vector2f(constants::square_size, constants::square_size));
            tile.setPosition(Vector2f((float)x * constants::square_size + board_x, (float)y * constants::square_size + board_y));
            tile.setFillColor(color ? Color(100, 100, 100) : Color(200, 200, 200));
            color = !color;
            tiles.push_back(tile);
        }
        color = !color;
    }



    auto factory = std::make_unique<PieceFactory>(pieces);

    auto transform = std::make_shared<Transform>();

    transform->translate(board_x, board_y);

    auto board_entity = std::make_shared<BoardEntity>(transform, std::move(factory));
    board_entity->piece_moved = std::make_shared<Sound>();
    board_entity->piece_moved->setBuffer(piece_move_sound);
    board_entity->in_check = std::make_shared<Sound>();
    board_entity->in_check->setBuffer(in_check_sound);

    Board board;
    Board::setup(board);

    board_entity->set(board);

    auto computer_player = std::make_shared<SmartAIPlayer>(Black);
    auto threaded_player = AutonomousPlayer(computer_player);

    auto receiver = std::make_shared<MultiReceiver>();
    for (auto& [key, value]: board_entity->state->pieces) {
        receiver->push_back(std::make_shared<PieceDragReceiver>(value->state, board_entity));
    }

    vector<unique_ptr<InputEvent>> events;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            switch (event.type) {
                case sf::Event::MouseButtonPressed: {
                    events.push_back(make_unique<MouseButtonEvent>(MouseDown, Vector2i(event.mouseButton.x, event.mouseButton.y), event.mouseButton.button));
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    events.push_back(make_unique<MouseButtonEvent>(MouseUp, Vector2i(event.mouseButton.x, event.mouseButton.y), event.mouseButton.button));
                    break;
                }
                case sf::Event::MouseMoved: {
                    events.push_back(make_unique<MouseMoveEvent>(Vector2i(event.mouseMove.x, event.mouseMove.y), event.mouseButton.button));
                    break;
                }
                case sf::Event::KeyReleased: {
                    if (event.key.code == Keyboard::F) {
                        board_entity->set_orientation(board_entity->state->orientation == White ? Black : White);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        for (auto& evt: events) {
            receiver->receive(evt.get());
        }
        events.clear();

        board_entity->behavior->update();

        auto current_board = board_entity->state->board;
        if (current_board.last_turn_color() == White) {
            if (!current_board.checkmate() || !current_board.stalemate(White) || current_board.stalemate(Black)) {
                if (!threaded_player.is_running() && !threaded_player.is_done()) {
                    threaded_player.start(current_board);
                }
            }
            if (threaded_player.is_done()) {
                Move move = threaded_player.get_move();
                board_entity->move(move);
            }
        }

        window.clear(Color::Blue);

        window.draw(rect);

        for (const auto& tile: tiles) {
            window.draw(tile);
        }

        RenderStates states;
        states.transform = *transform;
        board_entity->renderer->render(window, states);

        window.display();
    }

    return 0;
}
