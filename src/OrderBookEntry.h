#pragma once

#include <list>
#include "Order.h"

class OrderBookEntry
{
public:
    size_t total_qty;
    list<Order> entries;

    OrderBookEntry() : total_qty(0) {};
    void addOrder(Order &O);
    Order &getFront();
    void removeFront();
    const size_t size() const;
};