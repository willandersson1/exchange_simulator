#pragma once

#include <stdexcept>

using namespace std;

inline double MAX_PRICE = 50;
inline int ORDER_SIZE = 10;

enum OrderDirection {
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