#include "order.h"

#include <algorithm>
#include <map>
#include <vector>


std::list <Order> stackOrders (const std::list <Order> & orders) {
    std::map <uint64_t, OrderStack> ordersMap;

    std::list <Order> :: const_iterator it;
    for (it = orders.begin(); it != orders.end(); it++) {
        const Order & order = *it;
        if (ordersMap.count(order.g_orderID() == 0))
            ordersMap[order.g_orderID()] = OrderStack(order);
        else
            ordersMap[order.g_orderID()].addOrder(order);
    }

    std::list <Order> medianOrders;
    std::map <uint64_t, OrderStack> :: iterator mit;
    for (mit = ordersMap.begin(); mit != ordersMap.end(); mit++) {
        medianOrders.push_back(mit->second.g_medianOrder());
    }

    return medianOrders;
}


void OrderStack :: addOrder (const Order & order) {
    orders.push_back(order);
    updateMedianOrder();
}


void OrderStack :: updateMedianOrder () {
    if (orders.size() == 1)
        medianOrder = orders.front();

    std::vector <double> price;
    std::vector <int> range;
    std::vector <uint64_t> volEntered;
    std::vector <uint32_t> issueDate;
    std::vector <uint32_t> duration;
    std::vector <uint32_t> stationID;
    std::vector <uint32_t> generatedAt;
    std::vector <uint32_t> currentTime;

    std::list <Order> :: iterator it;
    for (it = orders.begin(); it != orders.end(); it++) {
        Order order = *it;
        price.push_back(order.g_price());
        range.push_back(order.g_range());
        volEntered.push_back(order.g_volEntered());
        issueDate.push_back(order.g_issueDate());
        duration.push_back(order.g_duration());
        stationID.push_back(order.g_stationID());
        generatedAt.push_back(order.g_generatedAt());
        currentTime.push_back(order.g_currentTime());
    }

    std::sort(price.begin(), price.end());
    std::sort(range.begin(), range.end());
    std::sort(volEntered.begin(), volEntered.end());
    std::sort(issueDate.begin(), issueDate.end());
    std::sort(duration.begin(), duration.end());
    std::sort(stationID.begin(), stationID.end());
    std::sort(generatedAt.begin(), generatedAt.end());
    std::sort(currentTime.begin(), currentTime.end());

    uint8_t emdr[16];
    memset(emdr, 0, 16);

    medianOrder = Order(emdr,
                        orders.front().g_typeID(),
                        price[price.size() / 2],
                        orders.back().g_volRemaining(),
                        range[range.size() / 2],
                        orders.front().g_orderID(),
                        volEntered[volEntered.size() / 2],
                        orders.back().g_minVolume(),
                        orders.front().g_bid(),
                        issueDate[issueDate.size() / 2],
                        duration[duration.size() / 2],
                        stationID[stationID.size() / 2],
                        generatedAt[generatedAt.size() / 2],
                        currentTime[currentTime.size() / 2]);
}