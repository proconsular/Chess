//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_DEVELOPMENT_SCORER_H
#define CHESS_DEVELOPMENT_SCORER_H

#include "scorer.h"

struct DevelopmentScorer: Scorer {
    [[nodiscard]] int score(const Board &board, Side side) const override {
        int value = 0;
        int rank = side == White ? 7 : 0;
        bool castled = board.castled(side);
//        BoardPosition king_king_side{rank, 6};
//        BoardPosition king_queen_side{rank, 2};
//        BoardPosition king_side_rook{rank, 5};
//        BoardPosition queen_side_rook{rank, 3};
//        auto king_king = board.get_piece_at(king_king_side);
//        auto king_rook = board.get_piece_at(king_side_rook);
//        auto king_queen = board.get_piece_at(king_queen_side);
//        auto queen_rook = board.get_piece_at(queen_side_rook);
//        bool castled = false;
//        if (king_king.type == King && king_king.side == side && king_rook.type == Rook && king_rook.side == side) {
//            castled = true;
//        }
//        if (king_queen.type == King && king_queen.side == side && queen_rook.type == Rook && queen_rook.side == side) {
//            castled = true;
//        }
        if (castled) {
            value += 10;
        } else {
            if (board.moved(board.get_piece_at(board.kings[side]))) {
                value -= 100;
            }
        }

        auto pieces = board.get_pieces(side);
        for (const auto& pos: pieces) {
            auto piece = board.get_piece_at(pos);
            if (piece.type == Knight) {
                if (!(pos == BoardPosition{rank, 1} || pos == BoardPosition{rank, 6})) {
                    value += 3;
                }
            } else if (piece.type == Bishop) {
                if (!(pos == BoardPosition{rank, 2} || pos == BoardPosition{rank, 5})) {
                    value += 3;
                }
            } else if (piece.type == Rook) {
                if (!castled) {
                    if (board.moved(piece)) {
                        value -= 100;
                    }
                }
            } else if (piece.type == Queen) {
                if (castled) {
                    if (pos.row != rank) {
                        value += 1;
                    }
                }
            }
        }

        if (castled) {
            int second_rank = side == White ? 6 : 1;
            std::vector<BoardPosition> pawns{{second_rank, 1}, {second_rank, 3}, {second_rank, 4}, {second_rank, 6}};
            for (const auto& pos: pawns) {
                auto piece = board.get_piece_at(pos);
                if (piece.type != Pawn || piece.side != side) {
                    value += 1;
                }
            }
        }

        return value;
    }
};

#endif //CHESS_DEVELOPMENT_SCORER_H
