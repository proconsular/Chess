//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_CONTROL_SCORER_H
#define CHESS_CONTROL_SCORER_H

#include "scorer.h"
#include "utils.h"

struct ControlScorer: Scorer {
    [[nodiscard]] int score(const Board &board, Side side) const override {
        int value = 0;
        auto white = board.get_pieces(White);
        auto black = board.get_pieces(Black);
        auto our_pieces = side == White ? white : black;
        auto enemy_s_pieces = side == White ? black : white;
        auto our_turn = board.last_turn_color() != side;
        /*
         * Look at each of the enemy's pieces, if we are attacking them, that is good.
         * If we are attacking them, how good is our attack? If it is a poor attack, then it's a neutral position (maybe slightly better).
         * If we are attacking them well, then that's a doubly good position.
         */
        if (our_turn) {
            for (const auto& piece: enemy_s_pieces) {
                auto score = score_take(board, piece, side, white, black);
                if (score > 0) {
                    value += score;
                }
            }
        }
        /*
         * Look at each one of our pieces, if our pieces are being attacked, how good is our defence?
         * Being attacked is bad, it should be avoided. But if we are attacked, how good is our defence?
         * Being attacked and having poor defence is doubly bad. Being attacked, but having good defence is neutral.
         */
        for (const auto& piece: our_pieces) {
            auto score = score_take(board, piece, side, white, black);
            if (score < 0) {
                value += score;
            }
        }

        /*
         * If we had taken a piece last move, we should get credit for that.
         */

//        if (!board.moves.empty()) {
//            value += (our_turn ? -1 : 1) * get_piece_value(board.last_piece_taken) * 2;
//        }

        return value;
    }

    /*
     * Given a position with a piece, how well attacked or defended is it? If the side is equal to the piece's side, how well defended is this side's piece?
     * If not, how well attacked?
     */
    [[nodiscard]] static int score_take(const Board& board, BoardPosition position, Side side, const std::vector<BoardPosition>& white, const std::vector<BoardPosition>& black) {
        auto piece = board.get_piece_at(position);
        auto color = piece.side;

        auto possible_attackers = color == White ? black : white;
        auto possible_defenders = color == White ? white : black;

        std::vector<Piece> attackers, defenders;

        for (const auto& pos: possible_attackers) {
            Move move{pos, position};
            if (board.legal(move)) {
                attackers.push_back(board.get_piece_at(pos));
            }
        }
        for (const auto& pos: possible_defenders) {
            Move move{pos, position};
            if (board.can_defend(move)) {
                defenders.push_back(board.get_piece_at(pos));
            }
        }

        auto sort_by_value = [&](const Piece& a, const Piece& b) {
            auto pa = get_piece_value(a.type);
            auto pb = get_piece_value(b.type);
            return pa < pb;
        };
        std::sort(attackers.begin(), attackers.end(), sort_by_value);
        std::sort(defenders.begin(), defenders.end(), sort_by_value);

        defenders.insert(defenders.begin(), piece);
        auto merged = zip(defenders, attackers);

        return compute_composite_score(merged) * (side == color ? 1 : -1);
    }

    [[nodiscard]] static int compute_composite_score(const std::vector<Piece>& pieces) {
        int value = 0;
        int normal = -1;
        for (int i = 0; i < pieces.size() - 1; i++) {
            auto piece = pieces[i];
            if (i + 1 < pieces.size() && pieces[i + 1].side == piece.side)
                break;
            int val = get_piece_value(piece.type);
            value += val * normal;
            normal = normal == 1 ? -1 : 1;
            if (i > 0) {
                int pre_val = get_piece_value(pieces[i - 1].type);
                if (pre_val > val) {
                    break;
                }
            }
        }
        return value;
    }
};

#endif //CHESS_CONTROL_SCORER_H
