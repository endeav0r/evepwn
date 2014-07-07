#ifndef eveDB_HEADER
#define eveDB_HEADER

#include "config.h"

#include <inttypes.h>
#include <map>
#include <string>

class MarketGroup {
    private :
        uint32_t marketGroupID;
        uint32_t parentGroupID;
        std::string name;

    public :
        MarketGroup (uint32_t marketGroupID,
                     uint32_t parentGroupID,
                     std::string name)
            : marketGroupID (marketGroupID),
              parentGroupID (parentGroupID),
              name (name) {}
        MarketGroup () {}

        uint32_t g_marketGroupID () { return marketGroupID; }
        uint32_t g_parentGroupID () { return parentGroupID; }
        std::string g_name () { return name; }
};


class EveType {
    private :
        uint32_t    typeID;
        std::string name;
        std::string description;
        double      volume;
        uint32_t    marketGroupID;
    public :
        EveType (uint32_t    typeID,
                 std::string name,
                 std::string description,
                 double      volume,
                 uint32_t    marketGroupID)
            : typeID (typeID),
              name (name),
              description (description),
              volume (volume),
              marketGroupID (marketGroupID) {}
        EveType () {}

        uint32_t    g_typeID        () { return typeID; }
        std::string g_name          () { return name; }
        std::string g_description   () { return description; }
        double      g_volume        () { return volume; }
        uint32_t    g_marketGroupID () { return marketGroupID; }
};


class EveRegion {
    private :
        uint32_t    regionID;
        std::string name;
    public :
        EveRegion (uint32_t regionID, std::string name)
            : regionID (regionID),
              name (name) {}
        EveRegion () {}

        uint32_t    g_regionID () { return regionID; }
        std::string g_name     () { return name; }
};


class EveSolarSystem {
    private :
        uint32_t regionID;
        uint32_t solarSystemID;
        std::string name;
        double security;
    public :
        EveSolarSystem (uint32_t regionID,
                        uint32_t solarSystemID,
                        std::string name,
                        double security)
            : regionID (regionID),
              solarSystemID (solarSystemID),
              name (name),
              security (security) {}
        EveSolarSystem () {}

        uint32_t    g_regionID      () { return regionID; }
        uint32_t    g_solarSystemID () { return solarSystemID; }
        std::string g_name          () { return name; }
        double      g_security      () { return security; }
};


class EveStation {
    private :
        uint32_t stationID;
        uint32_t corporationID;
        uint32_t solarSystemID;
        uint32_t regionID;
        std::string name;
    public :
        EveStation (uint32_t stationID,
                    uint32_t corporationID,
                    uint32_t solarSystemID,
                    uint32_t regionID,
                    std::string name)
            : stationID (stationID),
              corporationID (corporationID),
              solarSystemID (solarSystemID),
              regionID (regionID),
              name (name) {}
        EveStation () {}

        uint32_t    g_stationID     () { return stationID; }
        uint32_t    g_corporationID () { return corporationID; }
        uint32_t    g_solarSystemID () { return solarSystemID; }
        uint32_t    g_regionID      () { return regionID; }
        std::string g_name          () { return name; }
};


class EveDB {
    private :
        std::map <uint32_t, EveType>        types;
        std::map <uint32_t, EveRegion>      regions;
        std::map <uint32_t, EveSolarSystem> solarSystems;
        std::map <uint32_t, EveStation>     stations;
        std::map <uint32_t, MarketGroup>    marketGroups;

        static EveDB * get ();

    public :
        static EveType &        g_type        (uint32_t typeID);
        static EveRegion &      g_region      (uint32_t regionID);
        static EveSolarSystem & g_solarSystem (uint32_t solarSystemID);
        static EveStation &     g_station     (uint32_t stationID);

        static bool             has_region    (uint32_t regionID);
        static bool             has_station   (uint32_t stationID);
};

#endif