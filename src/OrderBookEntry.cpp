#include "OrderBookEntry.h"
#include "Order.h"
#include <cassert>

void OrderBookEntry::add_order(Order O) {
    if (entries.size() > 0) {
        assert(entries.back().time < O.time);
    }

    entries.push_back(O);
    total_qty += O.quantity;
}