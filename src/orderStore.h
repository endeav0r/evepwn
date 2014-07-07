#ifndef orderStore_HEADER
#define orderStore_HEADER

#include "order.h"

#include <list>
#include <map>

class OrderStore {
    private :
        // orderID -> order
        std::map <uint64_t, OrderStack> orderStacks;
        // stationID, typeID, -> list(orderID)
        std::map <uint32_t, std::map <uint32_t, std::list <uint64_t>>> stations;
    public :
        void addStationOrders (uint32_t station,
                               uint32_t typeID,
                               std::list <Order> & orders);

        std::list <Order> ordersTypeID        (uint32_t typeID);
        std::list <Order> ordersStationID     (uint32_t stationID);
        std::list <Order> ordersStationTypeID (uint32_t stationID, uint32_t typeID);
};

#endif