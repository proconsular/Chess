//
// Created by Chris Luttio on 1/10/22.
//

#ifndef CHESS_AGGREGATE_SCORER_H
#define CHESS_AGGREGATE_SCORER_H

#include "scorer.h"

#include "development_scorer.h"
#include "control_scorer.h"
#include "center_scorer.h"
#include "rim_scorer.h"
#include "material_scorer.h"

#include <vector>

struct AggregateScorer: Scorer {
    void push_back(int weight, const std::shared_ptr<Scorer>& scorer) {
        scorers.push_back(scorer);
        weights.push_back(weight);
    }

    [[nodiscard]] int score(const Board &board, Side side) const override {
        int value = 0;
        for (int i = 0; i < scorers.size(); i++) {
            value += weights[i] * scorers[i]->score(board, side);
        }
        return value;
    }

    std::vector<std::shared_ptr<Scorer>> scorers;
    std::vector<int> weights;
};

#endif //CHESS_AGGREGATE_SCORER_H
