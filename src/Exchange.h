#pragma once

#include <string>
#include "Order.h"
#include <unordered_map>
#include "OrderBook.h"

class Exchange {
    public: 
        void place_order(string client_id, Order O);
    private:
        unordered_map<string, OrderBook> orderbooks;
};
