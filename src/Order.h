#pragma once
#include <string>

#include "consts.h"

using namespace std;

class Order
{
public:
    static int global_count;
    int time;
    string stock_name;
    Price price;
    int quantity;
    OrderDirection direction;
    int remaining_quantity;
    string client_id;

    Order() : direction(OrderDirection::BUY) {};
    Order(string cid, string s, Price p, int q, OrderDirection d);
    string order_to_string();
};