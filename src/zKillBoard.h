#ifndef zKillBoard_HEADER
#define zKillBoard_HEADER


#include <jansson.h>

#include <inttypes.h>
#include <list>
#include <map>
#include <string>


class ZKBItem {
    private :
        uint32_t typeID;
        uint64_t qtyDropped;
        uint64_t qtyDestroyed;
    public :
        ZKBItem (uint32_t typeID,
                 uint32_t qtyDropped,
                 uint32_t qtyDestroyed)
            : typeID (typeID),
              qtyDropped (qtyDropped),
              qtyDestroyed (qtyDestroyed) {}
        ZKBItem (json_t * root);
        ZKBItem () {}

        uint32_t g_typeID () const { return typeID; }
        uint64_t g_qtyDropped () const { return qtyDropped; }
        uint64_t q_qtyDestroyed () const { return qtyDestroyed; }
};


class ZKBKill {
    private :
        uint32_t    solarSystemID;
        uint32_t    shipTypeID;
        std::string characterName;
        uint32_t    characterID;
        std::string corporationName;
        uint32_t    corporationID;
        std::list <ZKBItem> items;
    public :
        ZKBKill () {}
        ZKBKill (json_t * root);

        uint32_t    g_solarSystemID () const { return solarSystemID; }
        uint32_t    g_shipTypeID () const { return shipTypeID; }
        std::string g_characterName () const { return characterName; }
        uint32_t    g_characterID () const { return characterID; }
        std::string g_corporationName () const { return corporationName; }
        uint32_t    g_corporationID () const { return corporationID; }
        std::list <ZKBItem> & g_items () const { return items; }
};


class ZKBSolarSystem {
    private :
        std::list <ZKBKill> kills;
    public :
        ZKBSystem (uint32_t solarSystemID);

        const std::list <ZKBItem> g_itemsGrouped();
};


class ZKillBoard {
    private :
        std::map <uint32_t, std::list <ZKBKill>> solarSystemKills;

    public :
        const ZKBSolarSystem & g_solarSystem (uint32_t solarSystemID);
};

#endif