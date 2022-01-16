//
// Created by Chris Luttio on 12/17/21.
//

#include "gtest/gtest.h"
#include "pure_states/board.h"

TEST(board_tests, defaults) {
    Board board;

    board.set_piece_at(0, 0, Piece{Rook, White});

    auto p = board.get_piece_at(0, 0);
    ASSERT_EQ(Rook, p.type);
    ASSERT_EQ(White, p.side);

    auto p2 = board.get_piece_at(0, 1);
    ASSERT_EQ(None, p2.type);
    ASSERT_EQ(NoSide, p2.side);

    for (int row = 0; row < 8; row++) {
        for (int col = 1; col < 8; col++) {
            auto pp = board.get_piece_at(row, col);
            ASSERT_EQ(None, pp.type);
            ASSERT_EQ(NoSide, pp.side);
        }
    }
}

TEST(board_tests, copy) {
    Board a;

    a.set_piece_at(0, 0, {Rook, White});

    Board b = a;

    ASSERT_EQ(Rook, b.get_piece_at(0, 0).type);

    a.set_piece_at(0, 1, {Knight, White});

    ASSERT_EQ(None, b.get_piece_at(0, 1).type);

    a.set_piece_at(1, 0, {Pawn, White});

    ASSERT_EQ(Pawn, a.get_piece_at(1, 0).type);

    Board c = a;

    ASSERT_EQ(Pawn, c.get_piece_at(1, 0).type);
    ASSERT_EQ(None, b.get_piece_at(1, 0).type);

    a.move(Move{{0, 1}, {2, 2}, Knight_Move});

    ASSERT_EQ(None, a.get_piece_at(0, 1).type);
    ASSERT_EQ(Knight, a.get_piece_at(2, 2).type);

    ASSERT_EQ(1, a.moves.size());

    Board d = a;

    ASSERT_EQ(1, d.moves.size());

    a.moves.push_back(Move{{0, 2}, {0, 3}});

    ASSERT_EQ(2, a.moves.size());
    ASSERT_EQ(1, d.moves.size());
}

TEST(board_tests, logical_move) {
    Board board;

    Move out_of_bounds1 {{-1, 0}, {0, 0}};
    EXPECT_FALSE(board.logical_move(out_of_bounds1));

    Move out_of_bounds2 {{8, 0}, {7, 0}};
    EXPECT_FALSE(board.logical_move(out_of_bounds2));

    Move out_of_bounds3 {{0, -1}, {0, 0}};
    EXPECT_FALSE(board.logical_move(out_of_bounds3));

    Move out_of_bounds4 {{0, 8}, {0, 7}};
    EXPECT_FALSE(board.logical_move(out_of_bounds4));

    Move no_piece {{1, 0}, {1, 1}};
    EXPECT_FALSE(board.logical_move(no_piece));

    Move no_piece2 {{4, 4}, {5, 4}};
    EXPECT_FALSE(board.logical_move(no_piece2));

    board.set_piece_at(0, 0, {Rook, White});
    board.set_piece_at(0, 1, {Rook, White});

    Move same_side {{0, 0}, {0, 1}};
    EXPECT_FALSE(board.logical_move(same_side));

    Move same_side2 {{0, 1}, {0, 0}};
    EXPECT_FALSE(board.logical_move(same_side2));

    Move valid {{0, 0}, {1, 0}};
    EXPECT_TRUE(board.logical_move(valid));

    Move valid2 {{0, 0}, {7, 0}};
    EXPECT_TRUE(board.logical_move(valid2));

    board.set_piece_at(1, 0, {Rook, Black});

    Move valid3 {{0, 0}, {1, 0}};
    EXPECT_TRUE(board.logical_move(valid3));
}

TEST(board_tests, classify_pawn_move_1) {
    Board board;

    BoardPosition starting_place{6, 3};
    board.set_piece_at(starting_place, {Pawn, White});

    Move pawn_move{starting_place, {5, starting_place.column}};
    EXPECT_EQ(Unclassified, pawn_move.type);

    EXPECT_TRUE(board.logical_move(pawn_move));
    EXPECT_EQ(Pawn_Move, board.classify_move(pawn_move).type);

    Move pawn_double_move{starting_place, {4, starting_place.column}};
    EXPECT_TRUE(board.logical_move(pawn_double_move));
    EXPECT_EQ(Pawn_DoubleMove, board.classify_move(pawn_double_move).type);

    Board attack_left_board = board;
    attack_left_board.set_piece_at(5, 2, {Pawn, Black});

    Move pawn_attack_left{starting_place, {5, starting_place.column - 1}};
    EXPECT_EQ(Pawn_Attack, attack_left_board.classify_move(pawn_attack_left).type);

    Board attack_right_board = board;
    attack_right_board.set_piece_at(5, 4, {Pawn, Black});

    Move pawn_attack_right{starting_place, {5, starting_place.column + 1}};
    EXPECT_EQ(Pawn_Attack, attack_right_board.classify_move(pawn_attack_right).type);

    Board en_passant;
    en_passant.set_piece_at(3, 3, {Pawn, Black});
    en_passant.set_piece_at(3, 4, {Pawn, White});
    en_passant.moves.push_back(Move{{1, 3}, {3, 3}, Pawn_DoubleMove});

    Move en_passant_move{{3, 4}, {2, 3}};
    EXPECT_EQ(Pawn_EnPassant, en_passant.classify_move(en_passant_move).type);

    Board promotion;
    promotion.set_piece_at(1, 3, {Pawn, White});

    Move promotion_move{{1, 3}, {0, 3}};
    EXPECT_EQ(Pawn_Promotion, promotion.classify_move(promotion_move).type);
}

TEST(board_tests, classify_pawn_moves_2) {
    Board board;

    board.set_piece_at(6, 4, {Pawn, White});

    Move back_move{{6, 4}, {7, 4}};
    EXPECT_TRUE(board.logical_move(back_move));
    EXPECT_EQ(Unclassified, board.classify_move(back_move).type);

    Move side_move_left{{6, 4}, {6, 3}};
    EXPECT_TRUE(board.logical_move(side_move_left));
    EXPECT_EQ(Unclassified, board.classify_move(side_move_left).type);

    Move side_move_right{{6, 4}, {6, 5}};
    EXPECT_TRUE(board.logical_move(side_move_right));
    EXPECT_EQ(Unclassified, board.classify_move(side_move_right).type);

    Move triple_move{{6, 4}, {3, 4}};
    EXPECT_TRUE(board.logical_move(triple_move));
    EXPECT_EQ(Unclassified, board.classify_move(triple_move).type);

    Move left_attack{{6, 4}, {5, 3}};
    EXPECT_TRUE(board.logical_move(left_attack));
    EXPECT_EQ(Unclassified, board.classify_move(left_attack).type);

    Move right_attack{{6, 4}, {5, 5}};
    EXPECT_TRUE(board.logical_move(right_attack));
    EXPECT_EQ(Unclassified, board.classify_move(right_attack).type);

    Move left_double_attack{{6, 4}, {4, 2}};
    EXPECT_TRUE(board.logical_move(left_double_attack));
    EXPECT_EQ(Unclassified, board.classify_move(left_double_attack).type);

    Move left_back_attack{{6, 4}, {7, 3}};
    EXPECT_TRUE(board.logical_move(left_back_attack));
    EXPECT_EQ(Unclassified, board.classify_move(left_back_attack).type);

    for (int i = 0; i < 50; i++) {
        int dx = rand() % 3 - 1;
        int dy = rand() % 3 - 1;
        Move random{{6, 4}, {6 + dx * (3 + rand() % 8), 4 + dy * (3 + rand() % 8)}};
        if (board.logical_move(random)) {
            EXPECT_EQ(Unclassified, board.classify_move(random).type);
        }
    }

    Board double_attack;
    double_attack.set_piece_at(5, 3, {Pawn, White});

    Move double_attack_move{{5, 3}, {3, 3}};
    EXPECT_TRUE(double_attack.logical_move(double_attack_move));
    EXPECT_EQ(Unclassified, double_attack.classify_move(double_attack_move).type);

    Board en_passant;
    en_passant.set_piece_at(3, 3, {Pawn, Black});
    en_passant.set_piece_at(3, 4, {Pawn, White});
    en_passant.moves.push_back(Move{{1, 3}, {3, 3}, Pawn_DoubleMove});

    Move en_passant_move{{3, 4}, {2, 3}};
    EXPECT_EQ(Pawn_EnPassant, en_passant.classify_move(en_passant_move).type);

    Board en_passant_no_enemy;
    en_passant_no_enemy.set_piece_at(3, 4, {Pawn, White});
    en_passant_no_enemy.moves.push_back(Move{{1, 3}, {3, 3}, Pawn_DoubleMove});

    Move en_passant_move_1{{3, 4}, {2, 3}};
    EXPECT_EQ(Unclassified, en_passant_no_enemy.classify_move(en_passant_move_1).type);

    Board en_passant_no_move;
    en_passant_no_move.set_piece_at(3, 3, {Pawn, Black});
    en_passant_no_move.set_piece_at(3, 4, {Pawn, White});

    Move en_passant_move_2{{3, 4}, {2, 3}};
    EXPECT_EQ(Unclassified, en_passant_no_move.classify_move(en_passant_move_2).type);
}

TEST(board_tests, classify_rook_move) {
    Board board;

    board.set_piece_at(4, 4, {Rook, White});

    Move horizontal_1 {{4, 4}, {4, 7}};
    EXPECT_EQ(Rook_Move, board.classify_move(horizontal_1).type);

    Move horizontal_2 {{4, 4}, {4, 0}};
    EXPECT_EQ(Rook_Move, board.classify_move(horizontal_2).type);

    Move vertical_1 {{4, 4}, {7, 4}};
    EXPECT_EQ(Rook_Move, board.classify_move(vertical_1).type);

    Move vertical_2 {{4, 4}, {0, 4}};
    EXPECT_EQ(Rook_Move, board.classify_move(vertical_2).type);

    Move diagonal_1 {{4, 4}, {5, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_1).type);

    Move diagonal_2 {{4, 4}, {3, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_2).type);

    Move diagonal_3 {{4, 4}, {5, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_3).type);

    Move diagonal_4 {{4, 4}, {3, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_4).type);

    Move knight {{4, 4}, {3, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(knight).type);
}

TEST(board_tests, classify_bishop_move) {
    Board board;

    board.set_piece_at(4, 4, {Bishop, White});

    Move diagonal_1 {{4, 4}, {2, 2}};
    EXPECT_EQ(Bishop_Move, board.classify_move(diagonal_1).type);

    Move diagonal_2 {{4, 4}, {6, 6}};
    EXPECT_EQ(Bishop_Move, board.classify_move(diagonal_2).type);

    Move diagonal_3 {{4, 4}, {2, 6}};
    EXPECT_EQ(Bishop_Move, board.classify_move(diagonal_3).type);

    Move diagonal_4 {{4, 4}, {6, 2}};
    EXPECT_EQ(Bishop_Move, board.classify_move(diagonal_4).type);

    Move horizontal_1 {{4, 4}, {4, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(horizontal_1).type);

    Move horizontal_2 {{4, 4}, {4, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(horizontal_2).type);

    Move vertical_1 {{4, 4}, {5, 4}};
    EXPECT_EQ(Unclassified, board.classify_move(vertical_1).type);

    Move vertical_2 {{4, 4}, {3, 4}};
    EXPECT_EQ(Unclassified, board.classify_move(vertical_2).type);

    Move knight {{4, 4}, {6, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(knight).type);
}

TEST(board_tests, classify_knight_move) {
    Board board;

    board.set_piece_at(4, 4, {Knight, White});

    Move top_high_left {{4, 4}, {6, 3}};
    EXPECT_EQ(Knight_Move, board.classify_move(top_high_left).type);

    Move top_low_left {{4, 4}, {5, 2}};
    EXPECT_EQ(Knight_Move, board.classify_move(top_low_left).type);

    Move bottom_high_left {{4, 4}, {3, 2}};
    EXPECT_EQ(Knight_Move, board.classify_move(bottom_high_left).type);

    Move bottom_low_left {{4, 4}, {2, 3}};
    EXPECT_EQ(Knight_Move, board.classify_move(bottom_low_left).type);

    Move bottom_low_right {{4, 4}, {2, 5}};
    EXPECT_EQ(Knight_Move, board.classify_move(bottom_low_right).type);

    Move bottom_high_right {{4, 4}, {3, 6}};
    EXPECT_EQ(Knight_Move, board.classify_move(bottom_high_right).type);

    Move top_low_right {{4, 4}, {5, 6}};
    EXPECT_EQ(Knight_Move, board.classify_move(top_low_right).type);

    Move top_high_right {{4, 4}, {6, 5}};
    EXPECT_EQ(Knight_Move, board.classify_move(top_high_right).type);

    Move horizontal_left {{4, 4}, {4, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(horizontal_left).type);

    Move horizontal_right {{4, 4}, {4, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(horizontal_right).type);

    Move vertical_top {{4, 4}, {3, 4}};
    EXPECT_EQ(Unclassified, board.classify_move(vertical_top).type);

    Move vertical_bottom {{4, 4}, {5, 4}};
    EXPECT_EQ(Unclassified, board.classify_move(vertical_bottom).type);

    Move diagonal_top_left {{4, 4}, {3, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_top_left).type);

    Move diagonal_bottom_left {{4, 4}, {5, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_bottom_left).type);

    Move diagonal_top_right {{4, 4}, {3, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_top_right).type);

    Move diagonal_bottom_right {{4, 4}, {5, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(diagonal_bottom_right).type);
}

TEST(board_tests, classify_queen_move) {
    Board board;

    board.set_piece_at(4, 4, {Queen, White});

    Move diagonal_1 {{4, 4}, {2, 2}};
    EXPECT_EQ(Queen_Move, board.classify_move(diagonal_1).type);

    Move diagonal_2 {{4, 4}, {6, 6}};
    EXPECT_EQ(Queen_Move, board.classify_move(diagonal_2).type);

    Move diagonal_3 {{4, 4}, {2, 6}};
    EXPECT_EQ(Queen_Move, board.classify_move(diagonal_3).type);

    Move diagonal_4 {{4, 4}, {6, 2}};
    EXPECT_EQ(Queen_Move, board.classify_move(diagonal_4).type);

    Move horizontal_1 {{4, 4}, {4, 7}};
    EXPECT_EQ(Queen_Move, board.classify_move(horizontal_1).type);

    Move horizontal_2 {{4, 4}, {4, 0}};
    EXPECT_EQ(Queen_Move, board.classify_move(horizontal_2).type);

    Move vertical_1 {{4, 4}, {7, 4}};
    EXPECT_EQ(Queen_Move, board.classify_move(vertical_1).type);

    Move vertical_2 {{4, 4}, {0, 4}};
    EXPECT_EQ(Queen_Move, board.classify_move(vertical_2).type);

    Move top_high_left {{4, 4}, {6, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(top_high_left).type);

    Move top_low_left {{4, 4}, {5, 2}};
    EXPECT_EQ(Unclassified, board.classify_move(top_low_left).type);

    Move bottom_high_left {{4, 4}, {3, 2}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_high_left).type);

    Move bottom_low_left {{4, 4}, {2, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_low_left).type);

    Move bottom_low_right {{4, 4}, {2, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_low_right).type);

    Move bottom_high_right {{4, 4}, {3, 6}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_high_right).type);

    Move top_low_right {{4, 4}, {5, 6}};
    EXPECT_EQ(Unclassified, board.classify_move(top_low_right).type);

    Move top_high_right {{4, 4}, {6, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(top_high_right).type);
}

TEST(board_tests, classify_king_move_1) {
    Board board;

    board.set_piece_at(4, 4, {King, White});

    Move diagonal_1 {{4, 4}, {3, 3}};
    EXPECT_EQ(King_Move, board.classify_move(diagonal_1).type);

    Move diagonal_2 {{4, 4}, {5, 5}};
    EXPECT_EQ(King_Move, board.classify_move(diagonal_2).type);

    Move diagonal_3 {{4, 4}, {3, 5}};
    EXPECT_EQ(King_Move, board.classify_move(diagonal_3).type);

    Move diagonal_4 {{4, 4}, {5, 3}};
    EXPECT_EQ(King_Move, board.classify_move(diagonal_4).type);

    Move horizontal_1 {{4, 4}, {4, 5}};
    EXPECT_EQ(King_Move, board.classify_move(horizontal_1).type);

    Move horizontal_2 {{4, 4}, {4, 3}};
    EXPECT_EQ(King_Move, board.classify_move(horizontal_2).type);

    Move vertical_1 {{4, 4}, {5, 4}};
    EXPECT_EQ(King_Move, board.classify_move(vertical_1).type);

    Move vertical_2 {{4, 4}, {3, 4}};
    EXPECT_EQ(King_Move, board.classify_move(vertical_2).type);

    Move top_high_left {{4, 4}, {6, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(top_high_left).type);

    Move top_low_left {{4, 4}, {5, 2}};
    EXPECT_EQ(Unclassified, board.classify_move(top_low_left).type);

    Move bottom_high_left {{4, 4}, {3, 2}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_high_left).type);

    Move bottom_low_left {{4, 4}, {2, 3}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_low_left).type);

    Move bottom_low_right {{4, 4}, {2, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_low_right).type);

    Move bottom_high_right {{4, 4}, {3, 6}};
    EXPECT_EQ(Unclassified, board.classify_move(bottom_high_right).type);

    Move top_low_right {{4, 4}, {5, 6}};
    EXPECT_EQ(Unclassified, board.classify_move(top_low_right).type);

    Move top_high_right {{4, 4}, {6, 5}};
    EXPECT_EQ(Unclassified, board.classify_move(top_high_right).type);

    Move horizontal_1_2 {{4, 4}, {4, 6}};
    EXPECT_EQ(Unclassified, board.classify_move(horizontal_1_2).type);

    Move horizontal_2_2 {{4, 4}, {4, 2}};
    EXPECT_EQ(Unclassified, board.classify_move(horizontal_2_2).type);

    Move vertical_1_2 {{4, 4}, {6, 4}};
    EXPECT_EQ(Unclassified, board.classify_move(vertical_1_2).type);

    Move vertical_2_2 {{4, 4}, {2, 4}};
    EXPECT_EQ(Unclassified, board.classify_move(vertical_2_2).type);
}

TEST(board_tests, moved) {
    Board board;

    board.set_piece_at(4, 4, {Queen, White});

    Board moved1 = board;

    EXPECT_EQ(1, moved1.get_piece_at(4, 4).id);

    Move m1 {{4, 4}, {4, 5}, Queen_Move};
    moved1.move(m1);

    auto move = moved1.moves.back();
    EXPECT_EQ(1, move.piece_id);

    auto piece = moved1.get_piece_at(4, 5);
    EXPECT_EQ(Queen, piece.type);
    EXPECT_TRUE(moved1.moved(piece));

    Board moved2 = board;

    moved2.set_piece_at(0, 0, {Rook, White});

    moved2.move(m1);
    moved2.move({{4, 5}, {0, 5}, Queen_Move});

    auto queen = moved2.get_piece_at(0, 5);
    EXPECT_EQ(Queen, queen.type);
    EXPECT_EQ(1, queen.id);
    EXPECT_TRUE(moved2.moved(queen));

    EXPECT_EQ(1, moved2.moves[0].piece_id);
    EXPECT_EQ(1, moved2.moves[1].piece_id);

    auto rook = moved2.get_piece_at(0, 0);
    EXPECT_EQ(Rook, rook.type);
    EXPECT_EQ(2, rook.id);
    EXPECT_FALSE(moved2.moved(rook));
}

TEST(board_tests, classify_king_king_castle) {
    Board board;

    board.set_piece_at(7, 4, {King, White});
    board.set_piece_at(7, 7, {Rook, White});
    board.set_piece_at(7, 0, {Rook, White});

    Move king_side_castle {{7, 4}, {7, 6}};
    EXPECT_EQ(King_KingSideCastle, board.classify_move(king_side_castle).type);

    Move queen_side_castle {{7, 4}, {7, 2}};
    EXPECT_EQ(King_QueenSideCastle, board.classify_move(queen_side_castle).type);

    Board king_moves = board;
    king_moves.move({{7, 4}, {6, 4}, King_Move});
    king_moves.move({{6, 4}, {7, 4}, King_Move});

    EXPECT_EQ(Unclassified, king_moves.classify_move(king_side_castle).type);
    EXPECT_EQ(Unclassified, king_moves.classify_move(queen_side_castle).type);

    Board kings_rook_moves = board;
    kings_rook_moves.move({{7, 7}, {6, 7}, King_Move});
    kings_rook_moves.move({{6, 7}, {7, 7}, King_Move});

    EXPECT_EQ(Unclassified, kings_rook_moves.classify_move(king_side_castle).type);

    Board queens_rook_moves = board;
    queens_rook_moves.move({{7, 0}, {6, 0}, Rook_Move});
    queens_rook_moves.move({{6, 0}, {7, 0}, Rook_Move});

    EXPECT_EQ(Unclassified, queens_rook_moves.classify_move(queen_side_castle).type);
}

TEST(board_tests, line_of_sight) {
    Board board;

    auto list = board.line_of_sight({0, 0}, 0, 1);
    EXPECT_EQ(7, list.positions.size());
    EXPECT_EQ(0, list.piece_indices.size());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(0, list.positions[i].row);
        EXPECT_EQ(i + 1, list.positions[i].column);
    }

    auto list2 = board.line_of_sight({0, 4}, 0, 1);
    EXPECT_EQ(3, list2.positions.size());

    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(0, list2.positions[i].row);
        EXPECT_EQ(5 + i, list2.positions[i].column);
    }

    auto list3 = board.line_of_sight({0, 4}, 0, -1);
    EXPECT_EQ(4, list3.positions.size());

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(0, list3.positions[i].row);
        EXPECT_EQ(3 - i, list3.positions[i].column);
    }

    auto vertical_1 = board.line_of_sight({0, 0}, 1, 0);
    EXPECT_EQ(7, vertical_1.positions.size());
    EXPECT_TRUE(vertical_1.piece_indices.empty());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(i + 1, vertical_1.positions[i].row);
        EXPECT_EQ(0, vertical_1.positions[i].column);
    }

    auto diagonal_1 = board.line_of_sight({0, 0}, 1, 1);
    EXPECT_EQ(7, diagonal_1.positions.size());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(i + 1, diagonal_1.positions[i].row);
        EXPECT_EQ(i + 1, diagonal_1.positions[i].column);
    }

    auto diagonal_2 = board.line_of_sight({4, 4}, -1, -1);
    EXPECT_EQ(4, diagonal_2.positions.size());

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(3 - i, diagonal_2.positions[i].row);
        EXPECT_EQ(3 - i, diagonal_2.positions[i].column);
    }

    board.set_piece_at({2, 5}, {Rook, White});
    board.set_piece_at({2, 7}, {Pawn, Black});
    auto pieces_on_line = board.line_of_sight({2, 0}, 0, 1);
    EXPECT_EQ(7, pieces_on_line.positions.size());
    EXPECT_EQ(2, pieces_on_line.piece_indices.size());

    {
        auto p1 = board.get_piece_at(pieces_on_line.get_piece_position(0));
        EXPECT_EQ(Rook, p1.type);
        EXPECT_EQ(White, p1.side);

        auto p2 = board.get_piece_at(pieces_on_line.get_piece_position(1));
        EXPECT_EQ(Pawn, p2.type);
        EXPECT_EQ(Black, p2.side);
    }
}

TEST(board_tests, get_threatened_positions_pawn) {
    Board board;

    board.set_piece_at({6, 3}, {Pawn, White});

    auto t1 = board.get_threatened_positions({6, 3});
    EXPECT_EQ(2, t1.size());

    {
        auto p1 = t1[0];
        EXPECT_EQ(5, p1.row);
        EXPECT_EQ(2, p1.column);
        auto p2 = t1[1];
        EXPECT_EQ(5, p2.row);
        EXPECT_EQ(4, p2.column);
    }

    Board b2;

    b2.set_piece_at({6, 0}, {Pawn, White});

    auto t2 = b2.get_threatened_positions({6, 0});
    EXPECT_EQ(1, t2.size());

    {
        auto p1 = t2[0];
        EXPECT_EQ(5, p1.row);
        EXPECT_EQ(1, p1.column);
    }

    Board b3;

    b3.set_piece_at({6, 7}, {Pawn, White});

    auto t3 = b3.get_threatened_positions({6, 7});
    EXPECT_EQ(1, t3.size());

    {
        auto p1 = t3[0];
        EXPECT_EQ(5, p1.row);
        EXPECT_EQ(6, p1.column);
    }

    Board b4;

    b4.set_piece_at({0, 4}, {Pawn, White});

    auto t4 = b4.get_threatened_positions({0, 4});
    EXPECT_EQ(0, t4.size());
}

TEST(board_tests, get_threatened_positions_rook) {
    Board board;

    board.set_piece_at({0, 0}, {Rook, White});

    auto t1 = board.get_threatened_positions({0, 0});
    EXPECT_EQ(14, t1.size());

    for (int i = 0; i < 7; i++) {
        // right
        EXPECT_EQ(0, t1[i].row);
        EXPECT_EQ(i + 1, t1[i].column);
        // down
        EXPECT_EQ(i + 1, t1[i + 7].row);
        EXPECT_EQ(0, t1[i + 7].column);
    }

    Board b2 = board;

    b2.set_piece_at({0, 3}, {Rook, Black});

    auto t2 = b2.get_threatened_positions({0, 0});
    EXPECT_EQ(7 + 3, t2.size());

    // right
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(0, t2[i].row);
        EXPECT_EQ(i + 1, t2[i].column);
    }

    // down
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(i + 1, t2[i + 3].row);
        EXPECT_EQ(0, t2[i + 3].column);
    }

    auto p1 = t2[2];
    EXPECT_EQ(0, p1.row);
    EXPECT_EQ(3, p1.column);

    auto pc1 = b2.get_piece_at(p1);
    EXPECT_EQ(Rook, pc1.type);
    EXPECT_EQ(Black, pc1.side);

    Board b3;

    b3.set_piece_at({4, 4}, {Rook, White});

    auto t3 = b3.get_threatened_positions({4, 4});
    EXPECT_EQ(4 + 3 + 4 + 3, t3.size());

    // left
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(4, t3[i].row);
        EXPECT_EQ(3 - i, t3[i].column);
    }

    // right
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(4, t3[i + 4].row);
        EXPECT_EQ(i + 5, t3[i + 4].column);
    }

    // up
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(3 - i, t3[i + 4 + 3].row);
        EXPECT_EQ(4, t3[i + 4 + 3].column);
    }

    // down
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(i + 5, t3[i + 4 + 3 + 4].row);
        EXPECT_EQ(4, t3[i + 4 + 3 + 4].column);
    }
}

TEST(board_tests, get_threatened_position_bishop) {
    Board board;

    board.set_piece_at({0, 0}, {Bishop, White});

    auto t1 = board.get_threatened_positions({0, 0});
    EXPECT_EQ(7, t1.size());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(i + 1, t1[i].row);
        EXPECT_EQ(i + 1, t1[i].column);
    }

    board.set_piece_at({4, 4}, {Pawn, White});

    auto t2 = board.get_threatened_positions({0, 0});
    EXPECT_EQ(4, t2.size());

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(i + 1, t2[i].row);
        EXPECT_EQ(i + 1, t2[i].column);
    }

    Board b2;

    b2.set_piece_at({7, 7}, {Bishop, White});

    auto t3 = b2.get_threatened_positions({7, 7});
    EXPECT_EQ(7, t3.size());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(6 - i, t3[i].row);
        EXPECT_EQ(6 - i, t3[i].column);
    }

    Board b3;

    b3.set_piece_at({7, 0}, {Bishop, White});

    auto t4 = b3.get_threatened_positions({7, 0});
    EXPECT_EQ(7, t4.size());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(6 - i, t4[i].row);
        EXPECT_EQ(i + 1, t4[i].column);
    }

    Board b4;

    b4.set_piece_at({0, 7}, {Bishop, White});

    auto t5 = b4.get_threatened_positions({0, 7});
    EXPECT_EQ(7, t5.size());

    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(i + 1, t5[i].row);
        EXPECT_EQ(6 - i, t5[i].column);
    }
}

TEST(board_tests, get_threatened_position_knight) {
    Board board;

    board.set_piece_at({4, 4}, {Knight, White});

    auto t1 = board.get_threatened_positions({4, 4});
    EXPECT_EQ(8, t1.size());

    // -2, -1
    EXPECT_EQ(2, t1[0].row);
    EXPECT_EQ(3, t1[0].column);

    // -1, -2
    EXPECT_EQ(3, t1[1].row);
    EXPECT_EQ(2, t1[1].column);

    // -2, 1
    EXPECT_EQ(2, t1[2].row);
    EXPECT_EQ(5, t1[2].column);

    // -1, 2
    EXPECT_EQ(3, t1[3].row);
    EXPECT_EQ(6, t1[3].column);

    // 2, -1
    EXPECT_EQ(6, t1[4].row);
    EXPECT_EQ(3, t1[4].column);

    // 1, -2
    EXPECT_EQ(5, t1[5].row);
    EXPECT_EQ(2, t1[5].column);

    // 2, 1
    EXPECT_EQ(6, t1[6].row);
    EXPECT_EQ(5, t1[6].column);

    // 1, 2
    EXPECT_EQ(5, t1[7].row);
    EXPECT_EQ(6, t1[7].column);
}

TEST(board_tests, get_threatened_positions_king) {
    Board board;

    board.set_piece_at({4, 4}, {King, White});

    auto t1 = board.get_threatened_positions({4, 4});
    EXPECT_EQ(8, t1.size());

    EXPECT_EQ(3, t1[0].row);
    EXPECT_EQ(3, t1[0].column);

    EXPECT_EQ(3, t1[1].row);
    EXPECT_EQ(4, t1[1].column);

    EXPECT_EQ(3, t1[2].row);
    EXPECT_EQ(5, t1[2].column);

    EXPECT_EQ(4, t1[3].row);
    EXPECT_EQ(3, t1[3].column);

    EXPECT_EQ(4, t1[4].row);
    EXPECT_EQ(5, t1[4].column);
}

TEST(board_tests, threatened) {
    Board board;

    board.set_piece_at({6, 4}, {Pawn, White});

    EXPECT_EQ(0, board.threatened({6, 4}));

    board.set_piece_at({0, 4}, {Rook, Black});

    EXPECT_EQ(1, board.threatened({6, 4}));
    EXPECT_EQ(0, board.threatened({0, 4}));

    board.set_piece_at({0, 5}, {Pawn, White});
    board.set_piece_at({0, 7}, {King, White});

    EXPECT_EQ(1, board.threatened({0, 5}));
    EXPECT_EQ(0, board.threatened({0, 7}));

    board.set_piece_at({7, 0}, {Queen, Black});

    EXPECT_EQ(1, board.threatened({0, 7}));

    Board b2;

    b2.set_piece_at({0, 0}, {Rook, White});
    b2.set_piece_at({0, 4}, {Rook, Black});
    b2.set_piece_at({4, 0}, {Rook, Black});

    EXPECT_EQ(2, b2.threatened({0, 0}));
    EXPECT_EQ(1, b2.threatened({0, 4}));
    EXPECT_EQ(1, b2.threatened({4, 0}));

    b2.set_piece_at({4, 4}, {Rook, Black});

    EXPECT_EQ(1, b2.threatened({0, 4}, White));
}

TEST(board_tests, valid_move) {
    Board board;

    board.set_piece_at({0, 2}, {Rook, White});
    board.set_piece_at({0, 5}, {Rook, Black});

    EXPECT_FALSE(board.valid_move({{0, 2}, {0, 7}}));
    EXPECT_TRUE(board.valid_move({{0, 2}, {0, 5}}));
    EXPECT_FALSE(board.valid_move({{0, 2}, {0, 6}}));

    board.set_piece_at({4, 2}, {Pawn, Black});

    EXPECT_FALSE(board.valid_move({{0, 2}, {5, 2}}));
    EXPECT_TRUE(board.valid_move({{0, 2}, {4, 2}}));

    board.set_piece_at({5, 5}, {Bishop, White});

    EXPECT_TRUE(board.valid_move({{5, 5}, {7, 7}}));

    board.set_piece_at({6, 6}, {Pawn, Black});

    EXPECT_FALSE(board.valid_move({{5, 5}, {7, 7}}));
    EXPECT_TRUE((board.valid_move({{5, 5}, {2, 2}})));

    board.set_piece_at({3, 3}, {Pawn, Black});

    EXPECT_FALSE((board.valid_move({{5, 5}, {2, 2}})));
    EXPECT_TRUE((board.valid_move({{5, 5}, {3, 3}})));
    EXPECT_TRUE(board.logical_move({{5, 5}, {3, 3}}));

    board.set_piece_at({3, 3}, {Pawn, White});

    EXPECT_TRUE((board.valid_move({{5, 5}, {3, 3}})));
    EXPECT_FALSE(board.logical_move({{5, 5}, {3, 3}}));

    Board b2;

    b2.set_piece_at({6, 3}, {Pawn, White});

    EXPECT_TRUE(b2.valid_move({{6, 3}, {4, 3}, Pawn_DoubleMove}));

    b2.set_piece_at({5, 3}, {Pawn, White});

    EXPECT_FALSE(b2.valid_move({{6, 3}, {4, 3}, Pawn_DoubleMove}));

    b2.set_piece_at({7, 4}, {King, White});
    b2.set_piece_at({7, 7}, {Rook, White});

    EXPECT_TRUE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    b2.set_piece_at({7, 5}, {Rook, White});

    EXPECT_FALSE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    b2.set_piece_at({7, 5});

    EXPECT_TRUE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    b2.set_piece_at({5, 4}, {Rook, Black});

    EXPECT_FALSE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    b2.set_piece_at({5, 4}, {Rook, White});

    EXPECT_TRUE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));
    EXPECT_FALSE(b2.threatened({7, 5}, White));

    b2.set_piece_at({5, 4});
    b2.set_piece_at({5, 5}, {Rook, Black});

    EXPECT_FALSE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    b2.set_piece_at({5, 5}, {Rook, White});

    EXPECT_TRUE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    b2.set_piece_at({5, 6}, {Rook, Black});

    EXPECT_FALSE(b2.valid_move({{7, 4}, {7, 6}, King_KingSideCastle}));

    Board b3;

    b3.set_piece_at({7, 0}, {Rook, White});
    b3.set_piece_at({7, 4}, {King, White});

    EXPECT_TRUE(b3.valid_move({{7, 4}, {7, 2}, King_QueenSideCastle}));

    b3.set_piece_at({7, 1}, {Rook, White});

    EXPECT_FALSE(b3.valid_move({{7, 4}, {7, 2}, King_QueenSideCastle}));

    b3.set_piece_at({7, 1});
    b3.set_piece_at({7, 2}, {Rook, White});

    EXPECT_FALSE(b3.valid_move({{7, 4}, {7, 2}, King_QueenSideCastle}));

    b3.set_piece_at({7, 2});
    b3.set_piece_at({4, 3}, {Rook, Black});

    EXPECT_FALSE(b3.valid_move({{7, 4}, {7, 2}, King_QueenSideCastle}));

    b3.set_piece_at({4, 3});
    b3.set_piece_at({6, 5}, {Pawn, White});
    b3.set_piece_at({4, 7}, {Bishop, Black});

    EXPECT_FALSE(b3.valid_move({{6, 3}, {5, 3}, Pawn_Move}));
}

TEST(board_tests, move) {
    Board b1;

    // general move
    b1.set_piece_at({3, 0}, {Rook, White});

    Move rook_move {{3, 0}, {3, 7}};
    EXPECT_TRUE(b1.legal(rook_move));
    b1.move(rook_move);

    {
        auto p1 = b1.get_piece_at({3, 7});
        EXPECT_EQ(Rook, p1.type);
        EXPECT_EQ(White, p1.side);
        auto p2 = b1.get_piece_at({3, 0});
        EXPECT_EQ(None, p2.type);
        EXPECT_EQ(NoSide, p2.side);
    }

    // special move #1: King Side Castle
    Board b2;

    b2.set_piece_at({7, 4}, {King, White});
    b2.set_piece_at({7, 7}, {Rook, White});

    Move king_move {{7, 4}, {7, 6}};
    EXPECT_TRUE(b2.legal(king_move));
    b2.move(king_move);

    {
        auto p1 = b2.get_piece_at({7, 6});
        EXPECT_EQ(King, p1.type);
        EXPECT_EQ(White, p1.side);
        auto p2 = b2.get_piece_at({7, 5});
        EXPECT_EQ(Rook, p2.type);
        EXPECT_EQ(White, p2.side);
    }

    // special move #2: Queen Side Castle
    Board b3;

    b3.set_piece_at({7, 4}, {King, White});
    b3.set_piece_at({7, 0}, {Rook, White});

    Move king_move_2 {{7, 4}, {7, 2}};
    EXPECT_TRUE(b3.legal(king_move_2));
    b3.move(king_move_2);

    {
        auto p1 = b3.get_piece_at({7, 2});
        EXPECT_EQ(King, p1.type);
        EXPECT_EQ(White, p1.side);
        auto p2 = b3.get_piece_at({7, 3});
        EXPECT_EQ(Rook, p2.type);
        EXPECT_EQ(White, p2.side);
    }

    // special move #3: En Passant

    Board b4;

    b4.set_piece_at({6, 3}, {Pawn, White});
    b4.set_piece_at({4, 4}, {Pawn, Black});

    Move pawn_move_1 {{6, 3}, {4, 3}};
    EXPECT_TRUE(b4.legal(pawn_move_1));
    b4.move(pawn_move_1);

    {
        auto p1 = b4.get_piece_at({4, 3});
        EXPECT_EQ(Pawn, p1.type);
        EXPECT_EQ(White, p1.side);
    }

    Move pawn_move_2 {{4, 4}, {5, 3}};
    EXPECT_TRUE(b4.legal(pawn_move_2));
    b4.move(pawn_move_2);

    {
        auto p1 = b4.get_piece_at({5, 3});
        EXPECT_EQ(Pawn, p1.type);
        EXPECT_EQ(Black, p1.side);
        auto p2 = b4.get_piece_at({4, 3});
        EXPECT_EQ(None, p2.type);
    }

    // special move #4: Promotion

    Board b5;

    b5.set_piece_at({1, 4}, {Pawn, White});

    Move promotion {{1, 4}, {0, 4}};
    promotion.promotion = Queen;
    EXPECT_TRUE(b5.legal(promotion));
    b5.move(promotion);

    {
        auto p1 = b5.get_piece_at({0, 4});
        EXPECT_EQ(Queen, p1.type);
        EXPECT_EQ(White, p1.side);
    }
}

TEST(board_tests, checkmate) {
    Board b1;

    b1.set_piece_at({0, 2}, {King, Black});
    b1.set_piece_at({2, 2}, {Queen, White});
    b1.set_piece_at({0, 0}, {Rook, White});

    EXPECT_TRUE(b1.checkmate());

    Board b2;

    b2.set_piece_at({0, 2}, {King, Black});
    b2.set_piece_at({1, 2}, {Queen, White});

    EXPECT_FALSE(b2.checkmate());

    Board b3;

    b3.set_piece_at({0, 0}, {Rook, White});
    b3.set_piece_at({1, 0}, {Rook, White});
    b3.set_piece_at({0, 4}, {King, Black});

    EXPECT_TRUE(b3.checkmate());

    Board b4;

    b4.set_piece_at({6, 5}, {Pawn, White});
    b4.set_piece_at({6, 6}, {Pawn, White});
    b4.set_piece_at({6, 7}, {Pawn, White});
    b4.set_piece_at({7, 6}, {King, White});
    b4.set_piece_at({6, 4}, {Bishop, White});
    b4.set_piece_at({7, 0}, {Rook, Black});

    EXPECT_FALSE(b4.checkmate());

    b4.set_piece_at({6, 4});

    EXPECT_TRUE(b4.checkmate());

    Board b5;

    b5.set_piece_at({0, 4}, {King, Black});
    b5.set_piece_at({1, 4}, {Queen, White});
    b5.set_piece_at({2, 4}, {Rook, White});

    EXPECT_TRUE(b5.checkmate());

    Board b6;

    b6.set_piece_at({6, 5}, {Pawn, White});
    b6.set_piece_at({5, 6}, {Pawn, White});
    b6.set_piece_at({6, 7}, {Pawn, White});
    b6.set_piece_at({7, 6}, {King, White});
    b6.set_piece_at({0, 0}, {Bishop, Black});
    b6.set_piece_at({7, 0}, {Rook, Black});

    EXPECT_TRUE(b6.checkmate());

    Board b7;

    b7.set_piece_at({1, 7}, {Pawn, White});
    b7.set_piece_at({1, 6}, {Pawn, White});
    b7.set_piece_at({0, 7}, {King, White});
    b7.set_piece_at({3, 3}, {Bishop, Black});
    b7.set_piece_at({2, 6}, {Knight, Black});

    EXPECT_TRUE(b7.checkmate());

    b7.set_piece_at({2, 0}, {Rook, White});

    EXPECT_FALSE(b7.checkmate());
}

TEST(board_tests, stalemate) {
    Board b1;

    b1.set_piece_at({0, 4}, {King, Black});
    b1.set_piece_at({1, 4}, {Pawn, White});
    b1.set_piece_at({2, 4}, {King, White});

    EXPECT_FALSE(b1.checkmate());
    EXPECT_TRUE(b1.stalemate(Black));

    Board b2;

    b2.set_piece_at({7, 0}, {King, White});
    b2.set_piece_at({6, 0}, {Pawn, White});
    b2.set_piece_at({6, 1}, {Pawn, White});
    b2.set_piece_at({5, 0}, {Knight, Black});
    b2.set_piece_at({4, 3}, {Bishop, Black});

    EXPECT_FALSE(b2.checkmate());
    EXPECT_TRUE(b2.stalemate(White));
}
