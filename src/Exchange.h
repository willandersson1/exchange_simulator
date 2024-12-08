#pragma once

#include <string>
#include "Order.h"
#include <map>
#include "Orderbook.h"

class Exchange {
    public: 
        void place_order(string client_id, Order O);
    private:
        map<string, Orderbook> orderbooks;
};
