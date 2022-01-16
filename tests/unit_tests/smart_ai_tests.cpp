//
// Created by Chris Luttio on 1/7/22.
//

#include "gtest/gtest.h"
#include "players/smart_ai_player.h"
#include "pure_states/board.h"

#include "scorers/control_scorer.h"
#include "scorers/development_scorer.h"

#include <vector>

using namespace std;

TEST(scorer_tests, control_scorer) {
    Board b1;
    Board::setup(b1);

    ControlScorer scorer;

    EXPECT_EQ(0, scorer.score(b1, White));

    Move move1{{6, 4}, {4, 4}, Pawn_DoubleMove}; // King's Pawn
    b1.move(move1);

    EXPECT_EQ(0, scorer.score(b1, White));

    Move move2{{1, 3}, {3, 3}, Pawn_DoubleMove}; // Queen's Pawn
    b1.move(move2);

    Move move3{{7, 1}, {5, 2}, Knight_Move}; // Queen's Knight
    b1.move(move3);

    EXPECT_EQ(0, scorer.score(b1, White));
    EXPECT_EQ(-25, scorer.score(b1, Black));

    Move move4{{5, 2}, {3, 3}, Knight_Move};
    b1.move(move4);

    auto white = b1.get_pieces(White);
    auto black = b1.get_pieces(Black);

    EXPECT_EQ(20, ControlScorer::score_take(b1, {3, 3}, White, white, black));

    EXPECT_EQ(0, scorer.score(b1, White));
}

TEST(scorer_tests, control_scorer_attack) {
    Board b1;

    b1.set_piece_at({3, 1}, {Queen, White});
    b1.set_piece_at({3, 4}, {Pawn, Black});
    b1.set_piece_at({2, 5}, {Pawn, Black});
    b1.set_piece_at({3, 7}, {Queen, Black});

    MaterialScorer material;
    ControlScorer scorer;

    EXPECT_EQ(0, scorer.score(b1, White));
    EXPECT_EQ(-2, material.score(b1, White));

    b1.move({{3, 1}, {3, 4}, Queen_Move});

    EXPECT_EQ(-25, scorer.score(b1, White));
    EXPECT_EQ(-1, material.score(b1, White));
}

TEST(scorer_tests, control_scorer_2) {
    Board b1;
    Board::setup(b1);

    ControlScorer scorer;
    AggregateScorer aggregate;
    aggregate.push_back(1, make_shared<DevelopmentScorer>());
    aggregate.push_back(1, make_shared<ControlScorer>());

    EXPECT_EQ(0, aggregate.score(b1, White));

    Move m1{{6, 3}, {4, 3}, Pawn_DoubleMove};
    b1.move(m1);

    EXPECT_EQ(0, scorer.score(b1, White));
    EXPECT_EQ(1, aggregate.score(b1, White));
    EXPECT_EQ(0, aggregate.score(b1, Black));

    Board b2 = b1;
    Move m2{{1, 7}, {2, 7}, Pawn_Move};
    b1.move(m2);

    EXPECT_EQ(0, scorer.score(b1, Black));
    EXPECT_EQ(0, aggregate.score(b1, Black));

    Move alt1{{1, 3}, {2, 3}, Pawn_Move};
    b2.move(alt1);

    EXPECT_EQ(0, scorer.score(b2, Black));
    EXPECT_EQ(1, aggregate.score(b2, Black));
}

TEST(scorer_tests, control_scorer_3) {
    Board b1;

    b1.set_piece_at({1, 2}, {Pawn, Black});
    b1.set_piece_at({3, 3}, {Knight, Black});
    b1.set_piece_at({1, 5}, {Bishop, Black});
    b1.set_piece_at({4, 2}, {Pawn, White});
    b1.set_piece_at({5, 5}, {Bishop, White});

    b1.move({{1, 2}, {2, 2}, Pawn_Move});

    ControlScorer scorer;

    EXPECT_EQ(-4, ControlScorer::score_take(b1, {3, 3}, Black, b1.get_pieces(White), b1.get_pieces(Black)));
    EXPECT_EQ(-4, scorer.score(b1, Black));
}

TEST(scorer_tests, control_scorer_take) {
    ControlScorer scorer;

    Board b1;

    b1.set_piece_at({0, 0}, {Queen, White});
    b1.set_piece_at({0, 4}, {Pawn, Black});

    EXPECT_EQ(1, scorer.score(b1, White));
    EXPECT_EQ(-1, scorer.score(b1, Black));

    b1.move({{0, 0}, {0, 4}, Queen_Move});

    EXPECT_EQ(2, scorer.score(b1, White));
    EXPECT_EQ(-2, scorer.score(b1, Black));
}

TEST(smart_ai_tests, score_position_control) {
    Board b1;

    b1.set_piece_at({0, 3}, {Rook, White});
    b1.set_piece_at({4, 3}, {Pawn, Black});
    b1.set_piece_at({4, 0}, {Rook, Black});

    auto white = b1.get_pieces(White);
    auto black = b1.get_pieces(Black);

    EXPECT_EQ(9, ControlScorer::score_take(b1, {4, 3}, Black, white, black));
    EXPECT_EQ(-9, ControlScorer::score_take(b1, {4, 3}, White, white, black));

    b1.set_piece_at({5, 4}, {Pawn, White});

    white = b1.get_pieces(White);
    black = b1.get_pieces(Black);

    EXPECT_EQ(-10, ControlScorer::score_take(b1, {4, 3}, Black, white, black));
    EXPECT_EQ(10, ControlScorer::score_take(b1, {4, 3}, White, white, black));

    Board b2;

    b2.set_piece_at({0, 0}, {Queen, White});
    b2.set_piece_at({0, 4}, {Pawn, Black});

    white = b2.get_pieces(White);
    black = b2.get_pieces(Black);

    EXPECT_EQ(-1, ControlScorer::score_take(b2, {0, 4}, Black, white, black));
    EXPECT_EQ(1, ControlScorer::score_take(b2, {0, 4}, White, white, black));
}

TEST(smart_ai_tests, compute_composite_score) {
    {
        vector<Piece> pieces{{Pawn, Black},
                             {Rook, White}};
        EXPECT_EQ(-1, ControlScorer::compute_composite_score(pieces));
    }
    {
        vector<Piece> pieces{{Pawn, Black},
                             {Rook, White},
                             {Rook, White}};
        EXPECT_EQ(-1, ControlScorer::compute_composite_score(pieces));
    }
    {
        vector<Piece> pieces{{Pawn, Black}, {Rook, White}, {Rook, Black}, {Queen, White}};
        EXPECT_EQ(-1, ControlScorer::compute_composite_score(pieces));
    }
    {
        vector<Piece> pieces{{Pawn, Black}, {Pawn, White}, {Rook, Black}, {Rook, White}};
        EXPECT_EQ(-10, ControlScorer::compute_composite_score(pieces));
    }
    {
        vector<Piece> pieces{{Pawn, Black}, {Pawn, White}, {Bishop, Black}, {Rook, White}, {Rook, Black}};
        EXPECT_EQ(5, ControlScorer::compute_composite_score(pieces));
    }
    {
        vector<Piece> pieces{{Pawn, White}, {Rook, Black}, {Pawn, White}, {Pawn, White}, {Queen, White}};
        EXPECT_EQ(9, ControlScorer::compute_composite_score(pieces));
    }
}

TEST(smart_ai_tests, development_scorer_castle) {
    DevelopmentScorer scorer;

    Board b1;

    b1.set_piece_at({0, 0}, {Rook, Black});
    b1.set_piece_at({0, 7}, {Rook, Black});
    b1.set_piece_at({0, 4}, {King, Black});

    EXPECT_EQ(4, scorer.score(b1, Black));

    Board b2 = b1;
    Move k_castle = {{0, 4}, {0, 6}, King_KingSideCastle};
    b2.move(k_castle);

    EXPECT_EQ(14, scorer.score(b2, Black));

    Board b3 = b1;
    Move q_castle = {{0, 4}, {0, 2}, King_QueenSideCastle};
    b3.move(q_castle);

    EXPECT_EQ(14, scorer.score(b3, Black));
}

TEST(smart_ai_tests, development_scorer) {
    DevelopmentScorer scorer;

    Board b1;
    Board::setup(b1);

    EXPECT_EQ(0, scorer.score(b1, White));

    Move king_knight_move{{7, 5}, {5, 4}, Knight_Move};
    b1.move(king_knight_move);

    EXPECT_EQ(3, scorer.score(b1, White));


}