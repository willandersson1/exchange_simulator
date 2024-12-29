#pragma once
#include <cassert>
#include <string>
#include <iostream>

#include "consts.h"

using namespace std;

class Order {
    public:
        string stock_name;
        double price;
        int quantity;
        OrderDirection direction;

        Order(string s, double p, int q, OrderDirection d) : stock_name(s), price(p), quantity(q), direction(d) {
            cout << "creating order" << endl;
            assert(q == ORDER_SIZE);
        }
};
