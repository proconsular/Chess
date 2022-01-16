//
// Created by Chris Luttio on 1/6/22.
//

#include "benchmark.h"

#include "pure_states/board.h"
#include "data_types.h"

static void BM_board_init(benchmark::State& state) {
    for (auto _: state) {
        Board board;
    }
}

BENCHMARK(BM_board_init);

static void BM_board_copy(benchmark::State& state) {
    for (auto _: state) {
        Board board;
        board.set_piece_at({0, 0}, {Rook, White});
        Board b2 = board;
    }
}

BENCHMARK(BM_board_copy);

static void BM_get_pieces(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        auto pieces = board.get_pieces(White);
    }
}

BENCHMARK(BM_get_pieces);

static void BM_checkmate(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        bool ok = board.checkmate();
    }
}

BENCHMARK(BM_checkmate);

static void BM_stalemate(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        bool ok = board.stalemate(White);
    }
}

BENCHMARK(BM_stalemate);

static void BM_possible_moves_pawn(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        auto moves = board.possible_moves({1, 0});
    }
}

BENCHMARK(BM_possible_moves_pawn);

static void BM_legal_move(benchmark::State& state) {
    Board board;
    Board::setup(board);
    Move king_pawn_move{{6, 4}, {4, 4}};
    for (auto _: state) {
        auto ok = board.legal(king_pawn_move);
    }
}

BENCHMARK(BM_legal_move);

static void BM_logical_move(benchmark::State& state) {
    Board board;
    Board::setup(board);
    Move king_pawn_move{{6, 4}, {4, 4}};
    for (auto _: state) {
        auto ok = board.logical_move(king_pawn_move);
    }
}

BENCHMARK(BM_logical_move);

static void BM_classify_move(benchmark::State& state) {
    Board board;
    Board::setup(board);
    Move king_pawn_move{{6, 4}, {4, 4}};
    for (auto _: state) {
        auto ok = board.classify_move(king_pawn_move);
    }
}

BENCHMARK(BM_classify_move);

static void BM_valid_move(benchmark::State& state) {
    Board board;
    Board::setup(board);
    Move king_pawn_move{{6, 4}, {4, 4}, Pawn_DoubleMove};
    for (auto _: state) {
        auto ok = board.valid_move(king_pawn_move);
    }
}

BENCHMARK(BM_valid_move);

static void BM_king_in_check(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        auto ok = board.king_in_check(White);
    }
}

BENCHMARK(BM_king_in_check);

static void BM_threatened(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        int count = board.threatened({1, 0});
    }
}

BENCHMARK(BM_threatened);

static void BM_get_threatened_positions_pawn(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        auto pos = board.get_threatened_positions({1, 0});
    }
}

BENCHMARK(BM_get_threatened_positions_pawn);

static void BM_get_threatened_positions_rook(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        auto pos = board.get_threatened_positions({0, 0});
    }
}

BENCHMARK(BM_get_threatened_positions_rook);

static void BM_get_threatened_positions_queen(benchmark::State& state) {
    Board board;
    Board::setup(board);
    for (auto _: state) {
        auto pos = board.get_threatened_positions({0, 3});
    }
}

BENCHMARK(BM_get_threatened_positions_queen);

