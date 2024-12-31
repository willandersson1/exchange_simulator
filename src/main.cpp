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
    exch.place_order(Order(clientName, stockName, 90, 10, OrderDirection::SELL));
    exch.place_order(Order(clientName, stockName, 100, 15, OrderDirection::BUY));
    exch.place_order(Order(clientName, stockName, 80, 5, OrderDirection::SELL));
}