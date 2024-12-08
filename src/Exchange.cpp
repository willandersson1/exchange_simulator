#include <string>

#include "Order.h"
#include "Exchange.h"

using namespace std;

void Exchange::place_order(string client_id, Order O) {
    assert(orderbooks.find(O.stock_name) != orderbooks.end());

    orderbooks[O.stock_name].add_entry(O);
}
