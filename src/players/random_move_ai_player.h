//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_RANDOM_MOVE_AI_PLAYER_H
#define CHESS_RANDOM_MOVE_AI_PLAYER_H

#include "player.h"
#include <set>
#include <vector>

struct RandomMoveAIPlayer: Player {
    explicit RandomMoveAIPlayer(Side color): color(color) {}

    [[nodiscard]] Move move(const Board& board) const override {
        if (board.king_in_check(color)) {
            auto king_pos = board.kings[color];
            auto moves = board.possible_moves(king_pos);
            if (!moves.empty()) {
                return moves.front();
            }
        } else {
            std::vector<BoardPosition> pieces;
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    auto piece = board.get_piece_at(y, x);
                    if (piece.type == None || piece.side != color)
                        continue;
                    pieces.push_back({y, x});
                }
            }
            std::vector<int> random_indices;
            std::set<int> used;
            for (auto _: pieces) {
                int random_no = rand() % pieces.size();
                while (used.find(random_no) != used.end())
                    random_no = rand() % pieces.size();
                used.insert(random_no);
                random_indices.push_back(random_no);
            }
            for (auto idx: random_indices) {
                auto moves = board.possible_moves(pieces[idx]);
                if (!moves.empty()) {
                    return moves[rand() & moves.size()];
                }
            }
        }
    }

private:
    Side color;
};

#endif //CHESS_RANDOM_MOVE_AI_PLAYER_H
