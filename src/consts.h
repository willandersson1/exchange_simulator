#pragma once

#include <stdexcept>

using namespace std;

constexpr double MAX_PRICE = 50;
constexpr int ORDER_SIZE = 10;

enum class OrderDirection {
    BUY,
    SELL
};

inline string od_to_string(OrderDirection od) {
    switch (od) {
        case OrderDirection::BUY: return "BUY";
        case OrderDirection::SELL: return "SELL";
        default: throw invalid_argument("Unknown order direction");
    }
}