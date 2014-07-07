#include "evePwnClient.h"

#include <iostream>
#include <map>
#include <vector>

#include "config.h"


EvePwnClient :: EvePwnClient ()
    : context (zmq::context_t(1)),
      socket (zmq::socket_t(context, ZMQ_REQ)) {
    socket.connect(EVEPWNHOST);
}


std::map <std::string, msgpack::object> msgpackMap (msgpack::object obj) {
    std::map <std::string, msgpack::object> m;
    obj.convert(&m);
    return m;
}


std::vector <msgpack::object> msgpackVec (msgpack::object obj) {
    std::vector <msgpack::object> v;
    obj.convert(&v);
    return v;
}


std::string msgpackString (msgpack::object obj) {
    std::string s;
    obj.convert(&s);
    return s;
}


uint32_t msgpackUint32 (msgpack::object obj) {
    uint32_t u;
    obj.convert(&u);
    return u;
}


double msgpackDouble (msgpack::object obj) {
    double d;
    obj.convert(&d);
    return d;
}


uint64_t msgpackUint64 (msgpack::object obj) {
    uint64_t u;
    obj.convert(&u);
    return u;
}


int msgpackInt (msgpack::object obj) {
    int i;
    obj.convert(&i);
    return i;
}


bool msgpackBool (msgpack::object obj) {
    bool b;
    obj.convert(&b);
    return b;
}


Order unpackOrder (msgpack::object obj) {
    std::vector <msgpack::object> orderArray = msgpackVec(obj);
    std::string emdr = msgpackString(orderArray[0]);

    return Order((uint8_t *) emdr.c_str(),
                 msgpackUint32(orderArray[1]),
                 msgpackDouble(orderArray[2]),
                 msgpackUint64(orderArray[3]),
                 msgpackInt(orderArray[4]),
                 msgpackUint64(orderArray[5]),
                 msgpackUint64(orderArray[6]),
                 msgpackUint64(orderArray[7]),
                 msgpackBool(orderArray[8]),
                 msgpackUint32(orderArray[9]),
                 msgpackUint32(orderArray[10]),
                 msgpackUint32(orderArray[11]),
                 msgpackUint32(orderArray[12]),
                 msgpackUint32(orderArray[13])
                 );
}


std::list <Order> EvePwnClient :: ordersRequest (msgpack::sbuffer & buffer) {
    zmq::message_t request(buffer.size());
    memcpy(request.data(), buffer.data(), request.size());

    socket.send(request);

    zmq::message_t response;
    socket.recv(&response);

    std::cout << "received " << response.size() << " bytes" << std::endl;

    msgpack::unpacked msg;
    msgpack::unpack(&msg, (const char *) response.data(), response.size());

    std::map <std::string, msgpack::object> msgmap = msgpackMap(msg.get());

    std::vector <msgpack::object> orderVec = msgpackVec(msgmap["orders"]);

    std::list <Order> orders;

    std::vector <msgpack::object> :: iterator it;
    for (it = orderVec.begin(); it != orderVec.end(); it++) {
        orders.push_back(unpackOrder(*it));
    }

    return orders;
}


std::list <Order> EvePwnClient :: ordersStationID (uint32_t stationID) {
    msgpack::sbuffer buffer;
    msgpack::packer <msgpack::sbuffer> pk(&buffer);

    pk.pack_map(2);
    pk.pack(std::string("action"));
    pk.pack(std::string("orders.stationID"));
    pk.pack(std::string("stationID"));
    pk.pack_uint32(stationID);

    return ordersRequest(buffer);
}


std::list <Order> EvePwnClient :: ordersTypeID (uint32_t typeID) {
    msgpack::sbuffer buffer;
    msgpack::packer <msgpack::sbuffer> pk(&buffer);

    pk.pack_map(2);
    pk.pack(std::string("action"));
    pk.pack(std::string("orders.typeID"));
    pk.pack(std::string("typeID"));
    pk.pack_uint32(typeID);

    return ordersRequest(buffer);
}


std::map <uint32_t, std::list <Order>> EvePwnClient :: ordersTypeIDs (std::set <uint32_t> & typeIDs) {
    msgpack::sbuffer buffer;
    msgpack::packer <msgpack::sbuffer> pk(&buffer);

    pk.pack_map(2);
    pk.pack(std::string("action"));
    pk.pack(std::string("orders.typeIDs"));
    pk.pack(std::string("typeIDs"));
    pk.pack_array(typeIDs.size());

    std::set <uint32_t> :: iterator it;
    for (it = typeIDs.begin(); it != typeIDs.end(); it++) {
        pk.pack_uint32(*it);
    }
    std::cout << std::endl;

    zmq::message_t request(buffer.size());
    memcpy(request.data(), buffer.data(), request.size());

    socket.send(request);

    zmq::message_t response;
    socket.recv(&response);

    std::cout << "received " << response.size() << " bytes" << std::endl;

    msgpack::unpacked msg;
    msgpack::unpack(&msg, (const char *) response.data(), response.size());

    std::map <std::string, msgpack::object> msgmap = msgpackMap(msg.get());

    std::map <uint32_t, msgpack::object> typeIDsmap;
    msgmap["orders"].convert(&typeIDsmap);

    std::map <uint32_t, std::list <Order>> ordersMap;

    std::map <uint32_t, msgpack::object> :: iterator tit;
    for (tit = typeIDsmap.begin(); tit != typeIDsmap.end(); tit++) {
        // instantiates the row for empty orders
        ordersMap[tit->first].size();

        std::vector <msgpack::object> orderVec = msgpackVec(tit->second);
        std::vector <msgpack::object> :: iterator vit;
        for (vit = orderVec.begin(); vit != orderVec.end(); vit++) {
            ordersMap[tit->first].push_back(unpackOrder(*vit));
        }
    }

    return ordersMap;
}