#pragma once
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "Order.h"

class OrderBook {
    public:
        OrderBook(string name);

        string stock_name;
        map<double, int> bid_quantities;
        map<double, int> ask_quantities;

        double best_bid;
        double best_ask;

        // TODO should actually return a matching (# buys matched, # asks matched)
        void add_entry(Order O);
        double match(Order O);
};

