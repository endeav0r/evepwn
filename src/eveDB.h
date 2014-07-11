#ifndef eveDB_HEADER
#define eveDB_HEADER

#include "config.h"

#include <inttypes.h>
#include <map>
#include <string>

class EveMarketGroup {
    private :
        uint32_t marketGroupID;
        uint32_t parentGroupID;
        std::string name;

    public :
        EveMarketGroup (uint32_t marketGroupID,
                     uint32_t parentGroupID,
                     std::string name)
            : marketGroupID (marketGroupID),
              parentGroupID (parentGroupID),
              name (name) {}
        EveMarketGroup () {}

        uint32_t    g_marketGroupID () const { return marketGroupID; }
        uint32_t    g_parentGroupID () const { return parentGroupID; }
        std::string g_name          () const { return name; }
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

        uint32_t    g_typeID        () const { return typeID; }
        std::string g_name          () const { return name; }
        std::string g_description   () const { return description; }
        double      g_volume        () const { return volume; }
        uint32_t    g_marketGroupID () const { return marketGroupID; }
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

        uint32_t    g_regionID () const { return regionID; }
        std::string g_name     () const { return name; }
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

        uint32_t    g_regionID      () const { return regionID; }
        uint32_t    g_solarSystemID () const { return solarSystemID; }
        std::string g_name          () const { return name; }
        double      g_security      () const { return security; }
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

        uint32_t    g_stationID     () const { return stationID; }
        uint32_t    g_corporationID () const { return corporationID; }
        uint32_t    g_solarSystemID () const { return solarSystemID; }
        uint32_t    g_regionID      () const { return regionID; }
        std::string g_name          () const { return name; }
};


class EveDB {
    private :
        std::map <uint32_t, EveType>        types;
        std::map <uint32_t, EveRegion>      regions;
        std::map <uint32_t, EveSolarSystem> solarSystems;
        std::map <uint32_t, EveStation>     stations;
        std::map <uint32_t, EveMarketGroup> marketGroups;

        static EveDB * get ();

    public :
        static const EveType &        g_type        (uint32_t typeID);
        static const EveRegion &      g_region      (uint32_t regionID);
        static const EveSolarSystem & g_solarSystem (uint32_t solarSystemID);
        static const EveStation &     g_station     (uint32_t stationID);

        static const std::map <uint32_t, EveType>         & g_types ();
        static const std::map <uint32_t, EveRegion>       & g_regions ();
        static const std::map <uint32_t, EveSolarSystem>  & g_solarSystems ();
        static const std::map <uint32_t, EveStation>      & g_stations     ();
        static const std::map <uint32_t, EveMarketGroup > & g_marketGroups ();

        static bool             has_region    (uint32_t regionID);
        static bool             has_station   (uint32_t stationID);
};

#endif