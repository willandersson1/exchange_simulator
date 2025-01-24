#pragma once
#include <string>

#include "consts.h"

using namespace std;

class Order
{
public:
    static unsigned int global_count;
    unsigned int time;
    const string stock_name;
    Price price;
    size_t quantity;
    OrderDirection direction;
    size_t remaining_quantity;
    const string client_id;

    Order() : direction(OrderDirection::BUY) {};
    Order(const string cid, const string s, Price p, size_t q, OrderDirection d);
    string order_to_string() const;
};