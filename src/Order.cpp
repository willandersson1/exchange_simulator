#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include "consts.h"
#include "Order.h"

using namespace std;

unsigned int Order::global_count = 0;

string Order::order_to_string() const
{
    stringstream ss;
    ss << "#" << time << ": " << od_to_string(direction) << " " << stock_name << ": " << remaining_quantity << " @ " << price << ", from " << client_id;
    return ss.str();
}

Order::Order(const string cid, const string s, Price p, size_t q, OrderDirection d)
    : time(++Order::global_count), client_id(cid), stock_name(s), price(p), quantity(q), direction(d), remaining_quantity(q)
{
    cout << order_to_string() << endl;
}