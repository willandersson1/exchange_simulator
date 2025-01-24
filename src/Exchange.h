#pragma once

#include <string>
#include "Order.h"
#include <unordered_map>
#include "OrderBook.h"
#include <vector>

class Exchange
{
public:
    void place_order(Order O);

private:
    unordered_map<string, OrderBook> orderbooks;
};
