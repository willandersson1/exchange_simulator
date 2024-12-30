#include <set>
#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "Order.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"
#include <limits>

using namespace std;

OrderBook::OrderBook(string name) {
    stock_name = name;
    best_bid = numeric_limits<double>::epsilon();
    best_ask = numeric_limits<double>::max();
};

void OrderBook::add_entry(Order O) {
    // TODO make sure it's fine according to the min increment
    // TODO allow partial fills
    assert(O.quantity == ORDER_SIZE);

    bool is_best_bid = O.direction == OrderDirection::BUY && O.price > best_bid;
    bool is_best_ask = O.direction == OrderDirection::SELL && O.price < best_ask;

    bool should_match = (is_best_bid && O.price >= best_ask) || (is_best_ask && O.price <= best_bid);
    
    // Check that there is actually something in the book
    if (O.direction == OrderDirection::BUY && sell_entries.size() == 0) {
        should_match = false; 
    }

    if (O.direction == OrderDirection::SELL && buy_entries.size() == 0) {
        should_match = false; 
    }
    
    if (should_match) {
        match(O);
        return;
    }

    if (is_best_bid) {
        best_bid = O.price;
    } else if (is_best_ask) {
        best_ask = O.price;
    }

    // Add to map
    // TODO turn this get map to use into a method
    map<double, OrderBookEntry>& map_to_use_ref = O.direction == OrderDirection::BUY ? 
                                        buy_entries : 
                                        sell_entries;
    if (map_to_use_ref.find(O.price) != map_to_use_ref.end()) {
        map_to_use_ref.at(O.price).addOrder(O);
    } else {
        OrderBookEntry newEntry;
        newEntry.addOrder(O);
        map_to_use_ref.insert({O.price, newEntry});
    }

    assert(buy_entries.size() == 0 || buy_entries.at(best_bid).total_qty > 0);
    assert(sell_entries.size() == 0 || sell_entries.at(best_ask).total_qty > 0);
};

void OrderBook::match(Order O) {
    assert(O.quantity == ORDER_SIZE);
    map<double, OrderBookEntry>& map_to_use_ref = O.direction == OrderDirection::BUY ? 
                                                        sell_entries : 
                                                        buy_entries;

    // Because we don't allow partial filling (for now), 
    // it's always the best price
    double cost_to_fill = 0;
    Order matchedOrder;
    if (O.direction == OrderDirection::BUY) {
        cost_to_fill = best_ask * O.quantity;
        matchedOrder = map_to_use_ref.at(best_ask).getFront();
        map_to_use_ref.at(best_ask).removeFront();
        if (map_to_use_ref.at(best_ask).size() == 0) {
            map_to_use_ref.erase(best_ask);
        }
        auto minElement = *map_to_use_ref.begin();
        best_ask = minElement.first;
    } else {
        cost_to_fill = best_bid * O.quantity;
        matchedOrder = map_to_use_ref.at(best_bid).getFront();
        map_to_use_ref.at(best_bid).removeFront();
        if (map_to_use_ref.at(best_bid).size() == 0) {
            map_to_use_ref.erase(best_bid);
        }
        auto maxElement = *(--map_to_use_ref.end());
        best_bid = maxElement.first;
    }

    cout << "matched " << O.order_to_string() << " with " << matchedOrder.order_to_string() << endl;
}

void OrderBook::displayBook() {
    cout << "Displaying book for " << stock_name << endl;
    cout << "Best bid: " << best_bid << endl;
    cout << "Best ask: " << best_ask << endl;
    cout << endl;

    for (auto iter = sell_entries.rbegin(); iter != sell_entries.rend(); iter++) {
        double price = iter -> first;
        int qty = iter -> second.total_qty;
        int n_entries = iter -> second.size();
        cout << price << " | " << qty << " (" << n_entries << " order(s))" << endl;
    }
    cout << "-----" << endl;
    for (auto iter = buy_entries.rbegin(); iter != buy_entries.rend(); iter++) {
        double price = iter -> first;
        auto snd = iter -> second;
        int qty = iter -> second.total_qty;
        int n_entries = iter -> second.size();
        cout << price << " | " << qty << " (" << n_entries << " order(s))" << endl;
    }
}