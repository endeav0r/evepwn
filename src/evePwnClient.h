#ifndef evePwnClient_HEADER
#define evePwnClient_HEADER

#include "order.h"
#include <zmq.hpp>
#include <msgpack.hpp>
#include <list>
#include <map>

class EvePwnClient {
    private :
        zmq::context_t context;
        zmq::socket_t socket;

        std::list <Order> ordersRequest (msgpack::sbuffer & buffer);

    public :

        EvePwnClient();
        std::list <Order> ordersStationID (uint32_t stationID);
        std::list <Order> ordersTypeID    (uint32_t typeID);
        std::map <uint32_t, std::list <Order>> ordersTypeIDs (std::set <uint32_t> & typeIDs);
};

#endif