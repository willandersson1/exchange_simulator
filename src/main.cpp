#include <iostream>
#include <string>

#include "Order.h"
#include "consts.h"
#include "Exchange.h"
#include "OrderBook.h"

using namespace std;

int main() {
    string stockName = "STOCK";
    Order myOrder(stockName, 100, ORDER_SIZE, OrderDirection::BUY);
    Exchange exch;
    exch.place_order("me", myOrder);
}