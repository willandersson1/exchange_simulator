#pragma once

#include <stdexcept>

using namespace std;

typedef int Price;

enum class OrderDirection
{
    BUY,
    SELL
};

inline string od_to_string(OrderDirection od)
{
    switch (od)
    {
    case OrderDirection::BUY:
        return "BUY";
    case OrderDirection::SELL:
        return "SELL";
    default:
        throw invalid_argument("Unknown order direction");
    }
}