#pragma once
#include <string>

#include "consts.h"

using namespace std;

class Order {
    public:
        static int global_count;
        int time;
        string stock_name;
        double price;
        int quantity;
        OrderDirection direction;
        string client_id;

        Order() : direction(OrderDirection::BUY) {};
        Order(string cid, string s, double p, int q, OrderDirection d);
        string order_to_string();
};