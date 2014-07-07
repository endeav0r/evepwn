#include "orderStore.h"

#include <set>

void OrderStore :: addStationOrders (uint32_t stationID,
                                     uint32_t typeID,
                                     std::list <Order> & orders) {

    // add these orders to existing order stacks
    std::list <Order> :: iterator it;
    for (it = orders.begin(); it != orders.end(); it++) {
        Order order = *it;
        if (orderStacks.count(order.g_orderID()) == 0)
            orderStacks[order.g_orderID()] = OrderStack(order);
        else
            orderStacks[order.g_orderID()].addOrder(order);
    }

    // run through existing orders for this type at this station, and find
    // out which orders we're missing

    std::set <uint64_t> orderIDs = std::set <uint64_t> (stations[stationID][typeID].begin(), stations[stationID][typeID].end());
    for (it = orders.begin(); it != orders.end(); it++) {
        orderIDs.erase((*it).g_orderID());
    }

    std::set <uint64_t> :: iterator iit;
    for (iit = orderIDs.begin(); iit != orderIDs.end(); iit++) {
        orderStacks[*iit].incAbsentOrders();
    }
}