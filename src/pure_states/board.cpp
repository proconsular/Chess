//
// Created by Chris Luttio on 12/17/21.
//

#include "board.h"

using namespace std;

bool Board::logical_move(const Move &move) const {
    auto p1 = get_piece_at(move.current);
    auto p2 = get_piece_at(move.next);
    if (p1.type == None || p1.side == NoSide)
        return false;
    if (p1.side == p2.side)
        return false;
    return move.next.logical();
}

/*
 * Classifies logical moves, does not determine validity.
 * A classified move is a move that can be labeled because it fits a specific definition (ie Pawn Attack).
 * Classified moves can be divided into two categories, general and special.
 * A general move would be a move that simply moves the piece from the current position to the next, taking a piece or not.
 * All pieces but the pawn have general moves.
 * A special move is one that is not a general move, (ie a Pawn Move can't attack)
 * General move classifications only serve to prove that the move is valid for that type of piece (ie a rook is moving like a rook not a bishop).
 * However, even if a general move is classified, there is no guarantee that it is valid, the only guarantee is that it is correct for that piece.
 *
 * If we start with a random move,
 *  if we prove it is logical,
 *      then that means it is on the board,
 *      it is moving a piece,
 *      it is not moving into a piece of the same color.
 *  if we classify it as a general move,
 *      then that means it is the right move for that piece,
 *      (ie a pawn is not moving like a knight)
 *
 * Special moves are more specific and have more requirements to be matched.
 * Unlike general moves, each special move is specific and has different guarantees.
 * For instance, to classify a pawn move, by definition a pawn move can't attack, therefore it is required to be moving into
 *  an empty space to fit the definition. If a pawn move is classified, then it is more likely to be valid because there
 *  are less potential conditions that could invalidate it (ie compared to a rook move, where there could be a piece in the middle of the move blocking it, thereby making it invalid)
 *
 */
Move Board::classify_move(const Move &move) const {
    Move classified = move;
    move_type type = Unclassified;

    auto piece = get_piece_at(move.current);
    auto next_piece = get_piece_at(move.next);

    bool is_empty_space = next_piece.type == None;
    int starting_row = piece.side == White ? 6 : 1;

    int side_direction = piece.side == White ? -1 : 1;

    switch (piece.type) {
        case Pawn: {
            if (move.vertical_direction() == side_direction) {
                if (move.vertical()) {
                    if (is_empty_space) {
                        if (move.vertical_movement() == side_direction) {
                            int back_row = piece.side == White ? 0 : 7;
                            if (move.next.row == back_row) {
                                type = Pawn_Promotion;
                            } else {
                                type = Pawn_Move;
                            }
                        } else if (move.vertical_movement() == side_direction * 2) {
                            if (move.current.row == starting_row) {
                                type = Pawn_DoubleMove;
                            }
                        }
                    }
                } else if (move.diagonal() && move.vertical_movement() == side_direction) {
                    if (!is_empty_space) {
                        type = Pawn_Attack;
                    } else {
                        if (!moves.empty()) {
                            auto last = moves.back();
                            if (last.type == Pawn_DoubleMove) {
                                if (last.next.column == move.next.column && last.next.row == move.current.row) {
                                    if (auto p = get_piece_at(last.next); p.type != None && p.side != piece.side) {
                                        type = Pawn_EnPassant;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        case Rook: {
            if (move.vertical() || move.horizontal()) {
                type = Rook_Move;
            }
            break;
        }
        case Bishop: {
            if (move.diagonal()) {
                type = Bishop_Move;
            }
            break;
        }
        case Knight: {
            if ((abs(move.vertical_movement()) == 2 && abs(move.horizontal_movement()) == 1) ||
                (abs(move.vertical_movement()) == 1 && abs(move.horizontal_movement()) == 2)) {
                type = Knight_Move;
            }
            break;
        }
        case Queen: {
            if (move.diagonal() || move.vertical() || move.horizontal()) {
                type = Queen_Move;
            }
            break;
        }
        case King: {
            if (move.diagonal() || move.vertical() || move.horizontal()) {
                if ((move.vertical() && abs(move.vertical_movement()) == 1) ||
                    (move.horizontal() && abs(move.horizontal_movement()) == 1) ||
                    (move.diagonal() && abs(move.vertical_movement()) == 1 && abs(move.horizontal_movement()) == 1)) {
                    type = King_Move;
                }
                int first_row = piece.side == White ? 7 : 0;
                if (move.horizontal() && abs(move.horizontal_movement()) == 2 && move.current.row == first_row && is_empty_space) {
                    if (!moved(piece)) {
                        if (move.horizontal_movement() > 0) {
                            if (auto rook = get_piece_at(first_row, 7);
                                rook.type == Rook && rook.side == piece.side && !moved(rook)) {
                                type = King_KingSideCastle;
                            }
                        } else {
                            if (auto rook = get_piece_at(first_row, 0);
                                rook.type == Rook && rook.side == piece.side && !moved(rook)) {
                                type = King_QueenSideCastle;
                            }
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    classified.type = type;

    return classified;
}

/*
 * Move is assumed to be logical and classified.
 *
 * To validate a move, all moves, regardless of type, have to meet one check:
 *  1. Will this move cause the king to be in check?
 *
 * For most moves, we need to check if any piece is blocking their path.
 *
 */
bool Board::valid_move(const Move &move) const {
    auto piece = get_piece_at(move.current);

//    Board imaginary = *this;
//    imaginary.move(move);
//    bool will_be_in_check = imaginary.king_in_check(piece.side);
//    if (will_be_in_check)
//        return false;

    int side_direction = piece.side == White ? -1 : 1;

    switch (piece.type) {
        case Pawn: {
            switch (move.type) {
                case Pawn_DoubleMove: {
                    auto other = get_piece_at({move.current.row + side_direction, move.current.column});
                    return other.type == None;
                }
                case Pawn_Move:
                case Pawn_Attack:
                case Pawn_EnPassant:
                case Pawn_Promotion:
                    return true;
                default:
                    break;
            }
            break;
        }
        case King: {
            switch (move.type) {
                case King_Move:
                    return !obstructed(move);
                case King_KingSideCastle: {
                    if (obstructed(move))
                        return false;
                    if (king_in_check(piece.side))
                        return false;
                    if (threatened({move.current.row, move.current.column + 1}, piece.side))
                        return false;
                    return true;
                }
                case King_QueenSideCastle: {
                    if (obstructed(move))
                        return false;
                    if (king_in_check(piece.side))
                        return false;
                    if (threatened({move.current.row, move.current.column - 1}, piece.side))
                        return false;
                    auto line = line_of_sight(move.current, 0, -1);
                    if (line.piece_indices.size() > 1)
                        return false;
                    return true;
                }
                default:
                    break;
            }
            return true;
        }
        case Rook:
        case Bishop:
        case Queen: {
            return !obstructed(move);
        }
        case Knight:
            return true;
        default:
            break;
    }

    return false;
}

/*
 * Move is assumed to be logical, classified, and valid.
 * Therefore, if p1 is the piece at move.current,
 * then p1.type is never None, and p1.side is never NoSide.
 * If p2 is the piece at move.next,
 * then p2.type can be anything, and p2.side is never p1.type,
 * thus either an empty space or the enemy's piece.
 *
 * Both positions are on the board.
 */
void Board::move(const Move &move) {
    Move m = move;
    auto piece = get_piece_at(m.current);
    auto next_piece = get_piece_at(m.next);
    last_piece_taken = next_piece.type;
    switch (m.type) {
        case Pawn_Move:
        case Pawn_DoubleMove:
        case Pawn_Attack:
        case Rook_Move:
        case Bishop_Move:
        case Knight_Move:
        case Queen_Move:
        case King_Move: {
            set_piece_at(m.next, piece);
            set_piece_at(m.current);
            break;
        }
        case King_KingSideCastle: {
            set_piece_at(m.next, piece);
            set_piece_at(m.current);
            int rook_id = get_piece_at(m.current.row, 7).id;
            set_piece_at({m.current.row, m.next.column - 1}, {Rook, piece.side, rook_id});
            set_piece_at({m.current.row, 7});
            _castled[piece.side] = true;
            break;
        }
        case King_QueenSideCastle: {
            set_piece_at(m.next, piece);
            set_piece_at(m.current);
            int rook_id = get_piece_at(m.current.row, 0).id;
            set_piece_at({m.current.row, m.next.column + 1}, {Rook, piece.side, rook_id});
            set_piece_at({m.current.row, 0});
            _castled[piece.side] = true;
            break;
        }
        case Pawn_EnPassant: {
            set_piece_at(m.next, piece);
            set_piece_at(m.current);
            set_piece_at({m.current.row, m.next.column});
            break;
        }
        case Pawn_Promotion: {
            set_piece_at(m.next, {m.promotion, piece.side, piece.id});
            set_piece_at(m.current);
            break;
        }
        case Unclassified:
            return;
    }
    m.piece_id = piece.id;
    m.piece_type = piece.type;
    moves.push_back(m);
}

/*
 * O(1)
 */
BoardLine Board::line_of_sight(BoardPosition position, int dr, int dc) const {
    BoardLine line;
    for (int i = 1; i < 8; i++) {
        int row = dr * i + position.row;
        int col = dc * i + position.column;
        BoardPosition pos {row, col};
        if (!pos.logical())
            break;
        auto piece = get_piece_at(pos);
        if (piece.type != None)
            line.piece_indices.push_back(line.positions.size());
        line.positions.push_back(pos);
    }
    return line;
}

bool Board::obstructed(const Move &move) const {
    auto line = line_of_sight(move.current, move.vertical_direction(), move.horizontal_direction());
    if (!line.piece_indices.empty()) {
        for (const auto& idx: line.piece_indices) {
            auto pos = line.positions[idx];
            auto dr = pos.row - move.current.row;
            auto dc = pos.column - move.current.column;
            if (move.vertical_direction() != 0)
                dr /= move.vertical_direction();
            else
                dr = 0;
            if (move.horizontal_direction() != 0)
                dc /= move.horizontal_direction();
            else
                dc = 0;
            if (dr < abs(move.vertical_movement()) || dc < abs(move.horizontal_movement()))
                return true;
        }
    }
    return false;
}

/*
 * O(n), but n is 0<n<8 so O(1)
 */
std::vector<BoardPosition> Board::merge_lines(const std::vector<BoardLine> &lines) {
    std::vector<BoardPosition> positions;
    for (const auto& list: lines) {
        if (!list.positions.empty()) {
            auto end = list.positions.end();
            if (!list.piece_indices.empty()) {
                end = list.positions.begin() + list.piece_indices[0] + 1;
            }
            positions.insert(positions.end(), list.positions.begin(), end);
        }
    }
    return positions;
}

/*
 * O(32)
 */
std::vector<BoardPosition> Board::get_threatened_positions(BoardPosition position) const {
    auto piece = get_piece_at(position);
    std::vector<BoardPosition> positions;
    int color_direction = piece.side == White ? -1 : 1;
    switch (piece.type) {
        // O(1)
        case Pawn: {
            BoardPosition left {position.row + color_direction, position.column - 1};
            BoardPosition right {position.row + color_direction, position.column + 1};
            if (left.logical())
                positions.push_back(left);
            if (right.logical())
                positions.push_back(right);
            break;
        }
        // O(16)
        case Rook: {
            auto left = line_of_sight(position, 0, -1);
            auto right = line_of_sight(position, 0, 1);
            auto up = line_of_sight(position, -1, 0);
            auto down = line_of_sight(position, 1, 0);
            return merge_lines({left, right, up, down});
        }
        // O(16)
        case Bishop: {
            auto upper_left = line_of_sight(position, -1, -1);
            auto upper_right = line_of_sight(position, -1, 1);
            auto lower_left = line_of_sight(position, 1, -1);
            auto lower_right = line_of_sight(position, 1, 1);
            return merge_lines({upper_left, upper_right, lower_left, lower_right});
        }
        // O(32)
        case Queen: {
            auto left = line_of_sight(position, 0, -1);
            auto right = line_of_sight(position, 0, 1);
            auto up = line_of_sight(position, -1, 0);
            auto down = line_of_sight(position, 1, 0);
            auto upper_left = line_of_sight(position, -1, -1);
            auto upper_right = line_of_sight(position, -1, 1);
            auto lower_left = line_of_sight(position, 1, -1);
            auto lower_right = line_of_sight(position, 1, 1);
            return merge_lines({
                left, right, up, down,
                upper_left, upper_right, lower_left, lower_right
            });
        }
        // O(8)
        case Knight: {
            for (const auto& [y, x]: vector<tuple<int, int>>{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}) {
                for (const auto& [a, b]: vector<tuple<int, int>>{{2, 1}, {1, 2}}) {
                    auto r = y * a + position.row;
                    auto c = x * b + position.column;
                    BoardPosition pos{r, c};
                    if (pos.logical()) {
                        positions.push_back(pos);
                    }
                }
            }
            break;
        }
        // O(8)
        case King: {
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if (x == 1 && y == 1)
                        continue;
                    BoardPosition pos{y + position.row - 1, x + position.column - 1};
                    if (pos.logical()) {
                        positions.push_back(pos);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
    return positions;
}

/*
 * O(63)?
 */
int Board::threatened(BoardPosition position, Side side) const {
    auto piece = get_piece_at(position);
    int count = 0;
    Side color = piece.side;
    if (side != NoSide)
        color = side;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (x == position.column && y == position.row)
                continue;
            BoardPosition p{y, x};
            auto pp = get_piece_at(p);
            if (pp.type == None)
                continue;
            if (pp.side == color)
                continue;
            auto positions = get_threatened_positions(p);
            for (const auto& pos: positions)
                if (pos == position)
                    count++;
        }
    }
    return count;
}

/*
 * 1. Check if any king is in check.
 * 2. If a king is in check, determine if he can legally move.
 * 3. If he can't, then see if any of his pieces can block the attack.
 */
bool Board::checkmate() const {
    Side side = NoSide;
    if (king_in_check(White))
        side = White;
    if (king_in_check(Black))
        side = Black;
    if (side == NoSide)
        return false;
    return !can_move(side);
}

/*
 * Check if one of the players can't move
 */
bool Board::stalemate(Side side) const {
    return !king_in_check(side) && !can_move(side);
}

bool Board::can_move(Side side) const {
    auto position = kings[side];
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (x == 1 && y == 1)
                continue;
            Move move {position, {position.row + y - 1, position.column + x - 1}};
            if (legal(move))
                return true;
        }
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            BoardPosition pos {y, x};
            auto piece = get_piece_at(pos);
            if (piece.side != side || piece.type == King)
                continue;
            auto possible = possible_moves(pos);
            if (!possible.empty())
                return true;
        }
    }
    return false;
}

vector<Move> Board::possible_moves(BoardPosition position) const {
    vector<Move> possible;

    auto piece = get_piece_at(position);
    int side_direction = piece.side == White ? -1 : 1;
    int back_row = piece.side == White ? 7 : 0;

    switch (piece.type) {
        case Pawn: {
            Move pawn_move {position, {position.row + side_direction, position.column}};
            if (legal(pawn_move))
                possible.push_back(pawn_move);
            Move pawn_double_move {position, {position.row + 2 * side_direction, position.column}};
            if (legal(pawn_double_move))
                possible.push_back(pawn_double_move);
            Move left_attack {position, {position.row + side_direction, position.column - 1}};
            if (legal(left_attack))
                possible.push_back(left_attack);
            Move right_attack {position, {position.row + side_direction, position.column + 1}};
            if (legal(right_attack))
                possible.push_back(right_attack);
            break;
        }
        case Rook: {
            for (int x = 0; x < 8; x++) {
                Move horizontal {position, {position.row, x}};
                if (legal(horizontal))
                    possible.push_back(horizontal);
            }
            for (int y = 0; y < 8; y++) {
                Move vertical {position, {y, position.column}};
                if (legal(vertical))
                    possible.push_back(vertical);
            }
            break;
        }
        case Bishop: {
            for (const auto& [y, x]: vector<tuple<int, int>>{{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}) {
                for (int i = 0; i < 8; i++) {
                    Move move {position, {position.row + y * i, position.column + x * i}};
                    if (legal(move))
                        possible.push_back(move);
                }
            }
            break;
        }
        case Queen: {
            for (int x = 0; x < 8; x++) {
                Move horizontal {position, {position.row, x}};
                if (legal(horizontal))
                    possible.push_back(horizontal);
            }
            for (int y = 0; y < 8; y++) {
                Move vertical {position, {y, position.column}};
                if (legal(vertical))
                    possible.push_back(vertical);
            }
            for (const auto& [y, x]: vector<tuple<int, int>>{{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}) {
                for (int i = 0; i < 8; i++) {
                    Move move {position, {position.row + y * i, position.column + x * i}};
                    if (legal(move))
                        possible.push_back(move);
                }
            }
            break;
        }
        case Knight: {
            for (const auto& [y, x]: vector<tuple<int, int>>{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}) {
                for (const auto& [a, b]: vector<tuple<int, int>>{{2, 1}, {1, 2}}) {
                    auto r = y * a + position.row;
                    auto c = x * b + position.column;
                    BoardPosition pos{r, c};
                    Move move {position, pos};
                    if (legal(move)) {
                        possible.push_back(move);
                    }
                }
            }
            break;
        }
        case King: {
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if (x == 1 && y == 1)
                        continue;
                    Move move {position, {position.row + y - 1, position.column + x - 1}};
                    if (legal(move))
                        possible.push_back(move);
                }
            }
            if (position.row == back_row && position.column == 4) {
                Move move1 {position, {back_row, 6}};
                if (legal(move1))
                    possible.push_back(move1);
                Move move2 {position, {back_row, 2}};
                if (legal(move2))
                    possible.push_back(move2);
            }
            break;
        }
        default:
            break;
    }

    return possible;
}