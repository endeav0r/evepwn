#include "eveDB.h"

#include <jansson.h>
#include <iostream>

#include "config.h"


EveDB * EveDBsingleton = NULL;



EveDB * EveDB :: get () {
    if (EveDBsingleton != NULL)
        return EveDBsingleton;

    EveDBsingleton = new EveDB();
    EveDB * eveDB = EveDBsingleton;



    // load types

    json_error_t error;

    std::string invTypes = std::string(BASEPATH) + "invTypes.json";
    json_t * root = json_load_file(invTypes.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading market groups from " << invTypes << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return NULL;
    }

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_marketGroupID = json_object_get(object, "marketGroupID");
        if (json_is_null(j_marketGroupID))
            continue;

        json_t * j_typeID      = json_object_get(object, "typeID");
        json_t * j_typeName    = json_object_get(object, "typeName");
        json_t * j_description = json_object_get(object, "description");
        json_t * j_volume      = json_object_get(object, "volume");

        uint32_t    typeID        = json_integer_value(j_typeID);
        std::string name          = json_string_value(j_typeName);
        std::string description   = json_is_null(j_description) ? "" : json_string_value(j_description);
        double      volume        = json_real_value(j_volume);
        uint32_t    marketGroupID = json_integer_value(j_marketGroupID);

        eveDB->types[typeID] = EveType(typeID,
                                       name,
                                       description,
                                       volume,
                                       marketGroupID);
    }

    json_decref(root);



    // load regions

    std::string mapRegions = std::string(BASEPATH) + "mapRegions.json";
    root = json_load_file(mapRegions.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading market groups from " << mapRegions << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return NULL;
    }

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_regionID   = json_object_get(object, "regionID");
        json_t * j_regionName = json_object_get(object, "regionName");

        uint32_t    regionID = json_integer_value(j_regionID);
        std::string name     = json_string_value(j_regionName);

        eveDB->regions[regionID] = EveRegion(regionID,
                                             name);
    }

    json_decref(root);



    // load soldar systems

    std::string mapSolarSystems = std::string(BASEPATH) + "mapSolarSystems.json";
    root = json_load_file(mapSolarSystems.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading market groups from " << mapSolarSystems << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return NULL;
    }

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_regionID        = json_object_get(object, "regionID");
        json_t * j_solarSystemID   = json_object_get(object, "solarSystemID");
        json_t * j_solarSystemName = json_object_get(object, "solarSystemName");
        json_t * j_security        = json_object_get(object, "security");

        uint32_t    regionID      = json_integer_value(j_regionID);
        uint32_t    solarSystemID = json_integer_value(j_solarSystemID);
        std::string name          = json_string_value(j_solarSystemName);
        double      security      = json_real_value(j_security);

        eveDB->solarSystems[solarSystemID] = EveSolarSystem(regionID,
                                                            solarSystemID,
                                                            name,
                                                            security);
    }

    json_decref(root);



    // load stations

    std::string staStations = std::string(BASEPATH) + "staStations.json";
    root = json_load_file(staStations.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading stations from " << staStations << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return NULL;;
    }

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_stationID     = json_object_get(object, "stationID");
        json_t * j_corporationID = json_object_get(object, "corporationID");
        json_t * j_solarSystemID = json_object_get(object, "solarSystemID");
        json_t * j_regionID      = json_object_get(object, "regionID");
        json_t * j_stationName   = json_object_get(object, "stationName");

        uint32_t    stationID     = json_integer_value(j_stationID);
        uint32_t    corporationID = json_integer_value(j_corporationID);
        uint32_t    solarSystemID = json_integer_value(j_solarSystemID);
        uint32_t    regionID      = json_integer_value(j_regionID);
        std::string name          = json_string_value(j_stationName);

        eveDB->stations[stationID] = EveStation(stationID,
                                                corporationID,
                                                solarSystemID,
                                                regionID,
                                                name);
    }

    json_decref(root);



    // load market groups

    std::string staMarketGroups = std::string(BASEPATH) + "staMarketGroups.json";
    root = json_load_file(staMarketGroups.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading stations from " << staMarketGroups << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return NULL;;
    }

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_marketGroupID   = json_object_get(object, "marketGroupID");
        json_t * j_parentGroupID   = json_object_get(object, "parentGroupID");
        json_t * j_marketGroupName = json_object_get(object, "marketGroupName");

        uint32_t    marketGroupID = json_integer_value(j_marketGroupID);
        uint32_t    parentGroupID = UNDEFINED;
        if (not json_is_null(j_parentGroupID))
            parentGroupID = json_integer_value(j_parentGroupID);
        std::string name = json_string_value(j_marketGroupName);

        eveDB->marketGroups[marketGroupID] = MarketGroup(marketGroupID,
                                                         parentGroupID,
                                                         name);
    }

    json_decref(root);


    return eveDB;
}


EveType & EveDB :: g_type (uint32_t typeID) {
    EveDB * eveDB = EveDB::get();
    return eveDB->types[typeID];
}


EveRegion & EveDB :: g_region (uint32_t regionID) {
    EveDB * eveDB = EveDB::get();
    return eveDB->regions[regionID];
}


EveSolarSystem & EveDB :: g_solarSystem (uint32_t solarSystemID) {
    EveDB * eveDB = EveDB::get();
    return eveDB->solarSystems[solarSystemID];
}


EveStation & EveDB :: g_station (uint32_t stationID) {
    EveDB * eveDB = EveDB::get();
    return eveDB->stations[stationID];
}


bool EveDB :: has_station (uint32_t stationID) {
    EveDB * eveDB = EveDB::get();
    return eveDB->stations.count(stationID) > 0 ? true : false;
}