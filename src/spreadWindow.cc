#include "spreadWindow.h"

#include "config.h"
#include "eveDB.h"
#include "evePwnClient.h"

#include <iostream>

SpreadWindow :: SpreadWindow (std::set <uint32_t> typeIDs) {
    this->typeIDs = typeIDs;

    this->set_default_size(800, 600);

    add(spreadView);

    show_all_children();

    EvePwnClient evePwnClient;
    std::map <uint32_t, std::list <Order>> ordersMap;
    ordersMap = evePwnClient.ordersTypeIDs(typeIDs);

    uint64_t totalOrders = 0;
    std::map <uint32_t, std::list <Order>> :: iterator it;
    for (it = ordersMap.begin(); it != ordersMap.end(); it++) {
        spreadView.loadType(it->first,
        					stackOrders(it->second));
        totalOrders += it->second.size();
    }

    std::cout << "totalOrders=" << totalOrders << std::endl;
}