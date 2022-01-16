//
// Created by Chris Luttio on 12/17/21.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>

#include "../data_types.h"

struct Board {
    Board(): pieces(), piece_id(1), kings() {
        for (int i = 0; i < 3; i++)
            kings[i] = {-1, -1};
        _castled = {false, false, false};
    }

    static void setup(Board& board) {
        Pieces back_row[] = {
                Rook,
                Knight,
                Bishop,
                Queen,
                King,
                Bishop,
                Knight,
                Rook,
        };
        for (int i = 0; i < 8; i++) {
            board.set_piece_at(0, i, {back_row[i], Black});
            board.set_piece_at(1, i, {Pawn, Black});
            board.set_piece_at(7, i, {back_row[i], White});
            board.set_piece_at(6, i, {Pawn, White});
        }
    }

    [[nodiscard]] bool legal(Move& move) const {
        if (!logical_move(move))
            return false;
        move = classify_move(move);
        if (move.type == Unclassified)
            return false;
        Board imaginary = *this;
        imaginary.move(move);
        auto piece = get_piece_at(move.current);
        bool will_be_in_check = imaginary.king_in_check(piece.side);
        if (will_be_in_check)
            return false;
        if (!valid_move(move))
            return false;
        return true;
    }

    [[nodiscard]] bool can_defend(Move& move) const {
        move = classify_move(move);
        if (move.type == Unclassified)
            return false;
        Board imaginary = *this;
        imaginary.move(move);
        auto piece = get_piece_at(move.current);
        bool will_be_in_check = imaginary.king_in_check(piece.side);
        if (will_be_in_check)
            return false;
        if (!valid_move(move))
            return false;
        return true;
    }

    [[nodiscard]] bool logical_move(const Move &move) const;
    [[nodiscard]] bool valid_move(const Move&) const;

    Move classify_move(const Move&) const;

    void move(const Move&);

    [[nodiscard]] Piece get_piece_at(int row, int column) const {
        if (row < 0 || row >= 8 || column < 0 || column >= 8)
            return {};
        return pieces[row][column];
    }

    [[nodiscard]] Piece get_piece_at(BoardPosition pos) const {
        return get_piece_at(pos.row, pos.column);
    }

    void set_piece_at(int row, int column, Piece p = {}) {
        if (p.id == -1 && p.type != None)
            p.id = piece_id++;
        if (p.id != -1 && p.type == King)
            kings[p.side] = {row, column};
        pieces[row][column] = p;
    }

    void set_piece_at(BoardPosition pos, Piece p = {}) {
        set_piece_at(pos.row, pos.column, p);
    }

    [[nodiscard]] bool moved(const Piece& p) const {
        return std::any_of(moves.begin(), moves.end(), [&](const Move& move) {
            return p.id == move.piece_id;
        });
    }

    [[nodiscard]] BoardLine line_of_sight(BoardPosition, int, int) const;

    [[nodiscard]] std::vector<BoardPosition> get_threatened_positions(BoardPosition) const;

    [[nodiscard]] int threatened(BoardPosition, Side = NoSide) const;

    [[nodiscard]] bool king_in_check(Side side) const {
        return threatened(kings[side]);
    }

    [[nodiscard]] bool obstructed(const Move&) const;

    [[nodiscard]] bool checkmate() const;
    [[nodiscard]] bool stalemate(Side) const;
    [[nodiscard]] bool can_move(Side) const;

    [[nodiscard]] std::vector<Move> possible_moves(BoardPosition) const;

    [[nodiscard]] Side last_turn_color() const {
        if (moves.empty())
            return Black;
        return get_piece_at(moves.back().next).side;
    }

    [[nodiscard]] std::vector<BoardPosition> get_pieces(Side color) const {
        std::vector<BoardPosition> pieces;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                auto piece = get_piece_at(y, x);
                if (piece.type == None || piece.side != color)
                    continue;
                pieces.push_back({y, x});
            }
        }
        return pieces;
    }

    [[nodiscard]] bool castled(Side color) const {
        return _castled[color];
    }

    std::array<BoardPosition, 3> kings;
    std::vector<Move> moves;
    std::array<std::array<Piece, 8>, 8> pieces;
    int piece_id;
    Pieces last_piece_taken;
private:
    std::array<bool, 3> _castled;

    static std::vector<BoardPosition> merge_lines(const std::vector<BoardLine>&);
};

#endif //CHESS_BOARD_H
