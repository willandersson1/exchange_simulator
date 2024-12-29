#include <string>

#include "Order.h"
#include "Exchange.h"
#include "OrderBook.h"

using namespace std;

void Exchange::place_order(string client_id, Order O) {
    cout << "placing order" << endl;
    if (orderbooks.find(O.stock_name) == orderbooks.end()) {
        orderbooks.emplace(O.stock_name, O.stock_name);
    }
    orderbooks.at(O.stock_name).add_entry(O);
}
