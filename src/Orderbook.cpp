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
    best_bid = 0;
    best_ask = numeric_limits<int>::max();
};

void OrderBook::add_entry(Order& O) {
    // TODO make sure it's fine according to the min increment
    // TODO allow partial fills
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

    if (O.direction == OrderDirection::BUY) {
        partialFillMatch(O);
        return;
    } else {
        partialFillMatchSell(O);
        return;
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

void OrderBook::partialFillMatch(Order& O) {
    assert(O.direction == OrderDirection::BUY);
    vector<Order> matched_to;
    double fill_cost = 0;
    while (O.remaining_quantity > 0 && sell_entries.size() > 0 && best_ask <= O.price) {
        OrderBookEntry& entry_matching_to = sell_entries.at(best_ask);
        Order& order_matching_to = entry_matching_to.getFront();
        int qty_to_match = min(O.remaining_quantity, order_matching_to.remaining_quantity);
        fill_cost += qty_to_match * best_ask;

        order_matching_to.remaining_quantity -= qty_to_match;
        O.remaining_quantity -= qty_to_match;
        entry_matching_to.total_qty -= qty_to_match;
        if (order_matching_to.remaining_quantity == 0) {
            // Used it up so remove and recalculate
            matched_to.push_back(order_matching_to);
            entry_matching_to.removeFront();
            if (entry_matching_to.size() == 0) {
                // No more orders here so need to remove it from the dict and recalculate best ask
                sell_entries.erase(best_ask);
                if (sell_entries.size() > 0) {
                    auto minElement = *sell_entries.begin();
                    best_ask = minElement.first;
                } else {
                    best_ask = 1000.0; // TODO
                }
            }
        }
    }

    if (O.remaining_quantity == 0) {
        cout << "Filled, with cost " << fill_cost << endl;
        matched_to.push_back(O);
        return;
    }
    
    // Couldn't fully match so add this new order to the book with its remaining quantity
    if (buy_entries.find(O.price) != buy_entries.end()) {
        buy_entries.at(O.price).addOrder(O);
    } else {
        OrderBookEntry newEntry;
        newEntry.addOrder(O);
        buy_entries.insert({O.price, newEntry});
    }
    best_bid = max(best_bid, O.price);
}


// for selling
void OrderBook::partialFillMatchSell(Order& O) {
    assert(O.direction == OrderDirection::SELL);
    vector<Order> matched_to;
    double fill_cost = 0; 
    while (O.remaining_quantity > 0 && buy_entries.size() > 0 && O.price <= best_bid) {
        OrderBookEntry& entry_matching_to = buy_entries.at(best_bid);
        Order& order_matching_to = entry_matching_to.getFront();
        int qty_to_match = min(O.remaining_quantity, order_matching_to.remaining_quantity);
        fill_cost += qty_to_match * best_bid;

        order_matching_to.remaining_quantity -= qty_to_match;
        O.remaining_quantity -= qty_to_match;
        entry_matching_to.total_qty -= qty_to_match;
        if (order_matching_to.remaining_quantity == 0) {
            // Used it up so remove and recalculate
            matched_to.push_back(order_matching_to);
            entry_matching_to.removeFront();
            if (entry_matching_to.size() == 0) {
                // No more orders here so need to remove it from the dict and recalculate best ask
                buy_entries.erase(best_bid);
                if (buy_entries.size() > 0) {
                    auto maxElement = *(--buy_entries.end());
                    best_bid = maxElement.first;
                } else {
                    best_bid = 1; // TODO
                }
            }
        }
    }

    if (O.remaining_quantity == 0) {
        cout << "Filled, with cost " << fill_cost << endl;
        matched_to.push_back(O);
        return;
    }
    
    // Couldn't fully match so add this new order to the book with its remaining quantity
    if (sell_entries.find(O.price) != sell_entries.end()) {
        sell_entries.at(O.price).addOrder(O);
    } else {
        OrderBookEntry newEntry;
        newEntry.addOrder(O);
        sell_entries.insert({O.price, newEntry});
    }
    best_ask = min(best_ask, O.price);
}


void OrderBook::match(Order& O) {
    assert(O.quantity == ORDER_SIZE);
    map<double, OrderBookEntry>& map_to_use_ref = O.direction == OrderDirection::BUY ? 
                                                        sell_entries : 
                                                        buy_entries;

    // Because we don't allow partial filling (for now), 
    // it's always the best price
    Order matchedOrder;
    if (O.direction == OrderDirection::BUY) {
        matchedOrder = map_to_use_ref.at(best_ask).getFront();
        map_to_use_ref.at(best_ask).removeFront();
        if (map_to_use_ref.at(best_ask).size() == 0) {
            map_to_use_ref.erase(best_ask);
        }
        auto minElement = *map_to_use_ref.begin();
        best_ask = minElement.first;
    } else {
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
    assert(best_bid < best_ask);
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