#ifndef order_HEADER
#define order_HEADER

#include <cstring>
#include <list>
#include <inttypes.h>


class Order {
    private :
        uint8_t  emdr[16];
        uint32_t typeID;
        double price;           // 8 8
        uint64_t volRemaining;  // 8 16
        int range;              // 4 20
        uint64_t orderID;       // 8 28
        uint64_t volEntered;    // 8 36
        uint64_t minVolume;     // 8 44
        bool bid;               // 4 48
        uint32_t issueDate;       // 4 52
        uint32_t duration;      // 4 56
        uint32_t stationID;     // 4 60
        uint32_t generatedAt;
        uint32_t currentTime;
    public :
        Order (uint8_t * emdr,
               uint32_t typeID,
               double price,
               uint64_t volRemaining,
               int range,
               uint64_t orderID,
               uint64_t volEntered,
               uint64_t minVolume,
               bool bid,
               uint32_t issueDate,
               uint32_t duration,
               uint32_t stationID,
               uint32_t generatedAt,
               uint32_t currentTime)
            :
                typeID (typeID),
                price (price),
                volRemaining (volRemaining),
                range (range),
                orderID (orderID),
                volEntered (volEntered),
                minVolume (minVolume),
                bid (bid),
                issueDate (issueDate),
                duration (duration),
                stationID (stationID),
                generatedAt (generatedAt),
                currentTime (currentTime) {
                    memcpy(this->emdr, emdr, 16);
                }
        Order () {}

        const uint8_t * g_emdr   () const { return emdr; }
        uint32_t  g_typeID       () const { return typeID; }
        double    g_price        () const { return price; }
        uint64_t  g_volRemaining () const { return volRemaining; }
        int       g_range        () const { return range; }
        uint64_t  g_orderID      () const { return orderID; }
        uint64_t  g_volEntered   () const { return volEntered; }
        uint64_t  g_minVolume    () const { return minVolume; }
        bool      g_bid          () const { return bid; }
        uint32_t  g_issueDate    () const { return issueDate; }
        uint32_t  g_duration     () const { return duration; }
        uint32_t  g_stationID    () const { return stationID; }
        uint32_t  g_generatedAt  () const { return generatedAt; }
        uint32_t  g_currentTime  () const { return currentTime; }
};

class OrderStack {
    private :
        std::list <Order> orders;
        Order medianOrder;
        int absentOrders;

        void updateMedianOrder ();

    public :
        OrderStack (const Order & order) {
            orders.push_back(order);
            medianOrder = order;
            absentOrders = 0;
        }
        OrderStack () {
            absentOrders = 0;
        }

        void         addOrder (const Order & order);

        unsigned int numOrders       () { return orders.size(); }
        Order &      g_medianOrder   () { return medianOrder; }
        void         incAbsentOrders () { absentOrders++; }
        unsigned int g_absentOrders  () { return absentOrders; }
};


std::list <Order> stackOrders (const std::list <Order> & orders);

#endif