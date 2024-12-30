#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include "consts.h"
#include "Order.h"

using namespace std;

int Order::global_count = 0;

string Order::order_to_string() {
    stringstream ss;
    ss << "#" << time << ": " << od_to_string(direction) << " " << stock_name << ": " << quantity << " @ " << price << ", from " << client_id;
    return ss.str();
}

Order::Order(string cid, string s, double p, int q, OrderDirection d) 
        : time(++Order::global_count), client_id(cid), stock_name(s), price(p), quantity(q), direction(d) {
    assert(q == ORDER_SIZE);
    cout << order_to_string() << endl;
}