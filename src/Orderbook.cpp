#include <string>
#include <map>
#include <iostream>
#include <cassert>

#include "Order.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"

using namespace std;

void OrderBook::submitOrder(Order& O) {
    // Do partial filling and add to orderbook if not able to fully match
    // TODO output or communicate the matched orders
    vector<Order> ordersFullyMatched;
    bool isBuy = O.direction == OrderDirection::BUY;
    map<double, OrderBookEntry>& entriesToUse = isBuy ? 
                                    sell_entries : 
                                    buy_entries;

    while (shouldContinueMatching(O)) {
        double bestBidOrAsk = isBuy ? best_ask : best_bid;
        OrderBookEntry& entryMatchingTo = entriesToUse.at(bestBidOrAsk);
        Order& orderMatchingTo = entryMatchingTo.getFront();
        int quantityToMatch = min(O.remaining_quantity, orderMatchingTo.remaining_quantity);

        orderMatchingTo.remaining_quantity -= quantityToMatch;
        O.remaining_quantity -= quantityToMatch;
        entryMatchingTo.total_qty -= quantityToMatch;

        if (orderMatchingTo.remaining_quantity == 0) {
            // Used it up so remove and recalculate
            ordersFullyMatched.push_back(orderMatchingTo);
            entryMatchingTo.removeFront();
            if (entryMatchingTo.size() == 0) {
                if (isBuy) removeBestAskEntry();
                else removeBestBidEntry();
            }
        }
    }

    if (O.remaining_quantity == 0) {
        ordersFullyMatched.push_back(O);
        return;
    }
    
    // Couldn't fully match 
    addEntry(O);
};


void OrderBook::updateBestBidAskFromOrder(Order& O) {
    if (O.direction == OrderDirection::BUY) best_bid = max(best_bid, O.price);
    else best_ask = min(best_ask, O.price);
}

bool OrderBook::shouldContinueMatching(Order& O) {
    if (O.direction == OrderDirection::BUY) {
        return O.remaining_quantity > 0 && sell_entries.size() > 0 && best_ask <= O.price;
    } else {
        return O.remaining_quantity > 0 && buy_entries.size() > 0 && O.price <= best_bid;
    }
}

void OrderBook::addEntry(Order& O) {
    map<double, OrderBookEntry>& entriesAddingTo = O.direction == OrderDirection::BUY ? 
                                                    buy_entries :
                                                    sell_entries;
    if (entriesAddingTo.find(O.price) != entriesAddingTo.end()) {
        entriesAddingTo.at(O.price).addOrder(O);
    } else {
        OrderBookEntry newEntry;
        newEntry.addOrder(O);
        entriesAddingTo.insert({O.price, newEntry});
    }
    updateBestBidAskFromOrder(O);
}

void OrderBook::removeBestBidEntry() {
    buy_entries.erase(best_bid);
    if (buy_entries.size() > 0) {
        auto maxElement = *(--buy_entries.end());
        best_bid = maxElement.first;
    } else {
        best_bid = LOWEST_BID;
    }
}

void OrderBook::removeBestAskEntry() {
    sell_entries.erase(best_ask);
    if (sell_entries.size() > 0) {
        auto minElement = *sell_entries.begin();
        best_ask = minElement.first;
    } else {
        best_ask = HIGHEST_ASK;
    }
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