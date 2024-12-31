#pragma once

#include <list>
#include "Order.h"

class OrderBookEntry {
    public:
        int total_qty;
        list<Order> entries;

        OrderBookEntry() : total_qty(0) {};
        void addOrder(Order& O);
        Order& getFront();
        void removeFront();
        void changeFrontQuantity(int qty);
        int size();
};