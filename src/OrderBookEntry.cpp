#include "OrderBookEntry.h"
#include "Order.h"
#include <cassert>

void OrderBookEntry::addOrder(Order &O)
{
    if (entries.size() > 0)
    {
        assert(entries.back().time < O.time);
    }

    entries.push_back(O);
    total_qty += O.remaining_quantity;
}

Order &OrderBookEntry::getFront()
{
    return entries.front();
}

void OrderBookEntry::removeFront()
{
    total_qty -= getFront().remaining_quantity;
    entries.pop_front();
}

const size_t OrderBookEntry::size() const
{
    return entries.size();
}
