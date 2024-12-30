#include <set>
#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "Order.h"
#include "OrderBook.h"
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
    if (O.direction == OrderDirection::BUY && ask_quantities.size() == 0) {
        should_match = false; 
    }

    if (O.direction == OrderDirection::SELL && bid_quantities.size() == 0) {
        should_match = false; 
    }
    
    cout << "Should match = " << should_match << endl;
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
    map<double, int>& map_to_use_ref = O.direction == OrderDirection::BUY ? 
                                        bid_quantities : 
                                        ask_quantities;
    if (map_to_use_ref.find(O.price) != map_to_use_ref.end()) {
        map_to_use_ref.at(O.price) += O.quantity;
    } else {
        map_to_use_ref.insert({O.price, O.quantity});
    }

    assert(bid_quantities.size() == 0 || bid_quantities.at(best_bid) > 0);
    assert(ask_quantities.size() == 0 || ask_quantities.at(best_ask) > 0);
    displayBook();
};

double OrderBook::match(Order O) {
    assert(O.quantity == ORDER_SIZE);
    map<double, int>& map_to_use_ref = O.direction == OrderDirection::BUY ? 
                                                        ask_quantities : 
                                                        bid_quantities;


    // Because we don't allow partial filling (for now), 
    // it's always the best price
    double cost_to_fill = 0;
    if (O.direction == OrderDirection::BUY) {
        cost_to_fill += best_ask * O.quantity;
        map_to_use_ref.erase(best_ask);
        auto minElement = *map_to_use_ref.begin();
        best_ask = minElement.first;
    } else {
        cost_to_fill += best_bid * O.quantity;
        map_to_use_ref.erase(best_bid);
        auto maxElement = *(--map_to_use_ref.end());
        best_bid = maxElement.first;
    }

    cout << "cost to fill " << cost_to_fill << endl;
    displayBook();
    return cost_to_fill;
}

void OrderBook::displayBook() {
    cout << "Displaying book for " << stock_name << endl;
    cout << "Best bid: " << best_bid << endl;
    cout << "Best ask: " << best_ask << endl;
    cout << endl;

    for (auto iter = ask_quantities.rbegin(); iter != ask_quantities.rend(); iter++) {
        double price = iter -> first;
        int qty = iter -> second;
        cout << price << " | " << qty << endl;
    }
    cout << "-----" << endl;
    for (auto iter = bid_quantities.rbegin(); iter != bid_quantities.rend(); iter++) {
        double price = iter -> first;
        int qty = iter -> second;
        cout << price << " | " << qty << endl;
    }
}