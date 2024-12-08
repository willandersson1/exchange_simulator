#pragma once
#include <cassert>
#include <string>


#include "consts.h"

using namespace std;

class Order {
    public:
        string stock_name;
        double price;
        int quantity;
        OrderDirection direction;

        Order(double p, int q, OrderDirection d) : price(p), quantity(q), direction(d) {
            assert(q == ORDER_SIZE);
        }
};
