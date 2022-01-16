//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_SMART_AI_PLAYER_H
#define CHESS_SMART_AI_PLAYER_H

#include "player.h"

#include <map>
#include <vector>
#include <list>
#include <utils.h>
#include <queue>

#include "scorers/scorer.h"
#include "scorers/aggregate_scorer.h"
#include "scorers/material_scorer.h"
#include "scorers/development_scorer.h"
#include "scorers/control_scorer.h"
#include "scorers/center_scorer.h"
#include "scorers/checkmate_scorer.h"

struct SmartAIPlayer: Player {
    static const int LOWEST_SCORE = -2147483648;
    static const int HIGHEST_SCORE = 2147483647;

    explicit SmartAIPlayer(Side color): color(color) {
        auto aggregate = std::make_shared<AggregateScorer>();
        aggregate->push_back(1, std::make_shared<AccurateCenterScorer>());
        aggregate->push_back(3, std::make_shared<ControlScorer>());
        aggregate->push_back(1, std::make_shared<DevelopmentScorer>());
//        aggregate->push_back(1, std::make_shared<MaterialScorer>());
        aggregate->push_back(1, std::make_shared<CheckmateScorer>());
        scorer = aggregate;
    }

    [[nodiscard]] Move move(const Board& board) const override {
        return find_best_score(board, color, 0).second;
    }

    [[nodiscard]] static Side other_side(Side side) {
        return side == White ? Black : White;
    }

    Side color;
    std::shared_ptr<Scorer> scorer;
private:

//    [[nodiscard]] Move find_best(const Board& board, Side side, int depth) const {
//        auto compare = [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
//            return a.first < b.first;
//        };
//
//        std::priority_queue<std::pair<int, Move>, std::vector<std::pair<int, Move>>, decltype(compare)> priority(compare);
//
//        auto pieces = board.get_pieces(side);
//        for (const auto& piece: pieces) {
//            auto moves = board.possible_moves(piece);
//            for (const auto& move: moves) {
//                auto next = board;
//                auto mv = next.classify_move(move);
//                next.move(mv);
//                int score = scorer->score(next, color) ;
//                priority.push({score, mv});
//            }
//        }
//
//        std::vector<Move> best_moves;
//        best_moves.reserve(10);
//        for (int i = 0; i < 10; i++) {
//            best_moves.push_back(priority.top().second);
//            priority.pop();
//        }
//
//        for (const auto& move: best_moves) {
//            auto next = board;
//            auto mv = next.classify_move(move);
//            next.move(mv);
//
//        }
//    }

    [[nodiscard]] std::pair<int, Move> find_best_score(const Board& board, Side side, int depth) const {
        if (depth >= 1)
            return {0, Move{}};

        auto compare = [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
            return a.first < b.first;
        };

        const int num = 0;

        std::priority_queue<std::pair<int, Move>, std::vector<std::pair<int, Move>>, decltype(compare)> priority(compare);

        auto pieces = board.get_pieces(side);
        for (const auto& piece: pieces) {
            auto moves = board.possible_moves(piece);
            for (const auto& move: moves) {
                auto next = board;
                auto mv = next.classify_move(move);
                next.move(mv);
                int score = scorer->score(next, color) ;
                priority.push({score, mv});
            }
        }

        std::vector<std::pair<int, Move>> best_moves;
        best_moves.reserve(num);
        for (int i = 0; i < num || !priority.empty(); i++) {
            best_moves.push_back(priority.top());
            priority.pop();
        }

        int best_score = -100000000;
        Move best_move;

        for (auto& pair: best_moves) {
            auto next = board;
            auto mv = next.classify_move(pair.second);
            next.move(mv);
            int score = find_best_score(next, other_side(side), depth + 1).first;
            pair.first += score;
            if (pair.first > best_score) {
                best_score = pair.first;
                best_move = pair.second;
            }
        }

        return {best_score, best_move};
    }
};

#endif //CHESS_SMART_AI_PLAYER_H
