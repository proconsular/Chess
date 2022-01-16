//
// Created by Chris Luttio on 1/4/22.
//

#ifndef CHESS_UTILS_H
#define CHESS_UTILS_H

#include <vector>

#include "constants.h"
#include "data_types.h"
#include <SFML/Graphics.hpp>

sf::Vector2f compute_piece_position(BoardPosition position, Side orientation);

template <typename T>
std::vector<T> zip(const std::vector<T>& first, const std::vector<T>& second) {
    std::vector<T> result;
    int n = 0;
    int m = 0;
    while (n < first.size() || m < second.size()) {
        if (n < first.size())
            result.push_back(first[n++]);
        if (m < second.size())
            result.push_back(second[m++]);
    }
    return result;
}

[[nodiscard]] static int get_piece_value(Pieces type) {
    static std::map<Pieces, int> piece_values = {
            {Pawn, 1},
            {Knight, 5},
            {Bishop, 5},
            {Rook, 10},
            {Queen, 25},
            {King, 100},
    };
    return piece_values[type];
}

#endif //CHESS_UTILS_H
