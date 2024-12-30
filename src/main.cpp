#include <iostream>
#include <string>

#include "Order.h"
#include "consts.h"
#include "Exchange.h"
#include "OrderBook.h"

using namespace std;

int main() {
    string stockName = "STOCK";
    string clientName = "client";
    Exchange exch;
    exch.place_order(Order(clientName, stockName, 100, ORDER_SIZE, OrderDirection::BUY));
    exch.place_order(Order(clientName, stockName, 90, ORDER_SIZE, OrderDirection::BUY));
    exch.place_order(Order(clientName, stockName, 110, ORDER_SIZE, OrderDirection::SELL));
    exch.place_order(Order(clientName, stockName, 120, ORDER_SIZE, OrderDirection::SELL));
    exch.place_order(Order(clientName, stockName, 100, ORDER_SIZE, OrderDirection::SELL));
    exch.place_order(Order(clientName, stockName, 200, ORDER_SIZE, OrderDirection::BUY));
    exch.place_order(Order(clientName, stockName, 300, ORDER_SIZE, OrderDirection::SELL));
    exch.place_order(Order(clientName, stockName, 300, ORDER_SIZE, OrderDirection::SELL));
}