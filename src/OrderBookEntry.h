#pragma once

#include <list>
#include "Order.h"

class OrderBookEntry {
    public:
        int total_qty;
        list<Order> entries;

        OrderBookEntry() : total_qty(0) {};
        void add_order(Order O);
};