#pragma once
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <cassert>
#include <limits>

#include "Order.h"
#include "OrderBookEntry.h"
#include "consts.h"
#include <vector>

class OrderBook {
    public:
        OrderBook(string name) : stock_name(name), best_bid(LOWEST_BID), best_ask(HIGHEST_ASK) {};

        string stock_name;
        map<Price, OrderBookEntry> buy_entries;
        map<Price, OrderBookEntry> sell_entries;

        Price best_bid;
        Price best_ask;

        void submitOrder(Order& O);
        void displayBook();
    private:
        static constexpr Price LOWEST_BID = 1;
        static constexpr Price HIGHEST_ASK = numeric_limits<int>::max();
        void updateBestBidAskFromOrder(Order& O);
        bool shouldContinueMatching(Order& O);
        void addEntry(Order& O);
        void removeBestBidEntry();
        void removeBestAskEntry();
};

