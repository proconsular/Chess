//
// Created by Chris Luttio on 1/6/22.
//

#include "benchmark.h"

#include "data_types.h"
#include "pure_states/board.h"
#include "players/smart_ai_player.h"

static void BM_smart_ai_move(benchmark::State& state) {
    Board board;
    Board::setup(board);
    Move king_pawn_move{{6, 4}, {4, 4}};
    king_pawn_move = board.classify_move(king_pawn_move);
    board.move(king_pawn_move);
    SmartAIPlayer player(Black);
    for (auto _: state) {
        Move m = player.move(board);
    }
}

BENCHMARK(BM_smart_ai_move);

static void BM_smart_ai_score_position(benchmark::State& state) {
    Board board;
    Board::setup(board);
//    Move king_pawn_move{{6, 4}, {4, 4}};
//    king_pawn_move = board.classify_move(king_pawn_move);
//    board.move(king_pawn_move);
    SmartAIPlayer player(Black);
    for (auto _: state) {
        int score = player.score_position(board);
    }
}

BENCHMARK(BM_smart_ai_score_position);