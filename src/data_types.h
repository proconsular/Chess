//
// Created by Chris Luttio on 12/14/21.
//

#ifndef CHESS_DATA_TYPES_H
#define CHESS_DATA_TYPES_H

#include <array>
#include <vector>

enum Pieces {
    None,
    Pawn,
    Rook,
    Bishop,
    Knight,
    Queen,
    King,
};

enum Side {
    NoSide,
    White,
    Black
};

struct BoardPosition {
    int row, column;

    [[nodiscard]] bool logical() const {
        return row >= 0 && row < 8 && column >= 0 && column < 8;
    }

    bool operator==(BoardPosition other) const {
        return row == other.row && column == other.column;
    }

    bool operator!=(BoardPosition other) const {
        return !(*this == other);
    }
};


struct Piece {
    Piece() = default;
    Piece(Pieces type, Side side): type(type), side(side) {}
    Piece(Pieces type, Side side, int id): type(type), side(side), id(id) {}

    Pieces type = None;
    Side side = NoSide;
    int id = -1;
};

struct BoardLine {
    std::vector<BoardPosition> positions;
    std::vector<size_t> piece_indices;

    BoardPosition get_piece_position(size_t idx) const {
        return positions[piece_indices[idx]];
    }
};

enum move_type {
    Unclassified,
    Pawn_Move,
    Pawn_Attack,
    Pawn_DoubleMove,
    Pawn_EnPassant,
    Pawn_Promotion,
    Rook_Move,
    Knight_Move,
    Bishop_Move,
    Queen_Move,
    King_Move,
    King_KingSideCastle,
    King_QueenSideCastle,
};

struct Move {
    Move() {}
    Move(BoardPosition current, BoardPosition next): current(current), next(next) {}
    Move(BoardPosition current, BoardPosition next, move_type type): current(current), next(next), type(type) {}
    Move(BoardPosition current, BoardPosition next, move_type type, int id): current(current), next(next), type(type), piece_id(id) {}

    BoardPosition current, next;
    move_type type{Unclassified};
    int piece_id{0};
    Pieces promotion{Queen};
    Pieces piece_type{None};

    [[nodiscard]] bool diagonal() const {
        const int dy = abs(vertical_movement());
        const int dx = abs(horizontal_movement());
        return dx > 0 && dy > 0 && dx == dy;
    }

    [[nodiscard]] bool vertical() const {
        return abs(current.row - next.row) > 0 && abs(current.column - next.column) == 0;
    }

    [[nodiscard]] bool horizontal() const {
        return abs(current.row - next.row) == 0 && abs(current.column - next.column) > 0;
    }

    [[nodiscard]] int vertical_movement() const {
        return next.row - current.row;
    }

    [[nodiscard]] int horizontal_movement() const {
        return next.column - current.column;
    }

    [[nodiscard]] int vertical_direction() const {
        return vertical_movement() == 0 ? 0 : vertical_movement() / abs(vertical_movement());
    }

    [[nodiscard]] int horizontal_direction() const {
        return horizontal_movement() == 0 ? 0 : horizontal_movement() / abs(horizontal_movement());
    }
};

#endif //CHESS_DATA_TYPES_H
