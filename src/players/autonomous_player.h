//
// Created by Chris Luttio on 1/5/22.
//

#ifndef CHESS_AUTONOMOUS_PLAYER_H
#define CHESS_AUTONOMOUS_PLAYER_H

#include <memory>
#include <thread>
#include <iostream>

#include "player.h"

struct AutonomousPlayer {
    explicit AutonomousPlayer(std::shared_ptr<Player> player): player(std::move(player)), running{false}, done{false}, alive{true} {
        main = std::thread(&AutonomousPlayer::run, this);
    }

    void start(const Board& b) {
        auto lock = std::unique_lock<std::mutex>(mtx);
        board = b;
        running = true;
        cv.notify_one();
    }

    [[nodiscard]] Move get_move() {
        auto lock = std::unique_lock<std::mutex>(mtx);
        done = false;
        return move;
    }

    [[nodiscard]] bool is_running() const {
        return running;
    }

    [[nodiscard]] bool is_done() const {
        return done;
    }

    ~AutonomousPlayer() {
        alive = false;
        cv.notify_one();
        main.join();
    }

private:
    void run() {
        while (alive) {
            auto lock = std::unique_lock<std::mutex>(mtx);
            while (!running) {
                if (!alive)
                    return;
                cv.wait(lock);
            }
            move = player->move(board);
            running = false;
            done = true;
        }
    }

    Board board;
    Move move;
    std::thread main;
    std::shared_ptr<Player> player;
    std::condition_variable cv;
    std::mutex mtx;
    bool running, done, alive;
};

#endif //CHESS_AUTONOMOUS_PLAYER_H
