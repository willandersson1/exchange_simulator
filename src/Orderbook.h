#pragma once
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "Order.h"
#include "OrderBookEntry.h"
#include <vector>

class OrderBook {
    public:
        OrderBook(string name);

        string stock_name;
        map<double, OrderBookEntry> buy_entries;
        map<double, OrderBookEntry> sell_entries;

        double best_bid;
        double best_ask;

        void add_entry(Order O);
        void match(Order O);
        void displayBook();
};

