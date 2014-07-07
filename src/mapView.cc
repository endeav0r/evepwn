#include "mapView.h"

#include <iostream>
#include <map>

#include "config.h"

MapView :: MapView () {
    add(treeView);

    treeModel = Gtk::TreeStore::create(columns);
    treeView.set_model(treeModel);

    treeView.append_column("Name", columns.name);

    show_all_children();
}


void MapView :: loadMap (std::string basepath) {
    json_error_t error;

    std::string mapRegions = std::string(BASEPATH) + "mapRegions.json";
    std::string mapSolarSystems = std::string(BASEPATH) + "mapSolarSystems.json";
    std::string staStations = std::string(BASEPATH) + "staStations.json";

    json_t * root = json_load_file(mapRegions.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading regions from " << mapRegions << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return;
    }

    std::map <unsigned int, Gtk::TreeModel::Row> regionMap;

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);
        json_t * j_regionID   = json_object_get(object, "regionID");
        json_t * j_regionName = json_object_get(object, "regionName");

        unsigned int regionID = json_integer_value(j_regionID);
        Glib::ustring regionName = json_string_value(j_regionName);

        regionMap[regionID] = *(treeModel->append());

        regionMap[regionID][columns.regionID] = regionID;
        regionMap[regionID][columns.solarSystemID] = -1;
        regionMap[regionID][columns.stationID] = -1;
        regionMap[regionID][columns.name] = regionName;
    }

    json_decref(root);

    root = json_load_file(mapSolarSystems.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading solarSystems from " << mapSolarSystems << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return;
    }

    std::map <unsigned int, Gtk::TreeModel::Row> solarSystemMap;

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_regionID = json_object_get(object, "regionID");
        json_t * j_solarSystemID = json_object_get(object, "solarSystemID");
        json_t * j_solarSystemName = json_object_get(object, "solarSystemName");

        if (j_solarSystemName == NULL)
            std::cerr << "solarSystemName == NULL" << std::endl;

        unsigned int regionID = json_integer_value(j_regionID);
        unsigned int solarSystemID = json_integer_value(j_solarSystemID);
        Glib::ustring solarSystemName = json_string_value(j_solarSystemName);

        solarSystemMap[solarSystemID] = *(treeModel->append(regionMap[regionID].children()));

        solarSystemMap[solarSystemID][columns.regionID] = regionID;
        solarSystemMap[solarSystemID][columns.solarSystemID] = solarSystemID;
        solarSystemMap[solarSystemID][columns.stationID] = -1;
        solarSystemMap[solarSystemID][columns.name] = solarSystemName;
    }

    json_decref(root);

    root = json_load_file (staStations.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading stations from " << staStations << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return;
    }

    for (size_t root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_regionID = json_object_get(object, "regionID");
        json_t * j_solarSystemID = json_object_get(object, "solarSystemID");
        json_t * j_stationID = json_object_get(object, "stationID");
        json_t * j_stationName = json_object_get(object, "stationName");

        unsigned int regionID = json_integer_value(j_regionID);
        unsigned int solarSystemID = json_integer_value(j_solarSystemID);
        unsigned int stationID = json_integer_value(j_stationID);
        Glib::ustring stationName = json_string_value(j_stationName);

        Gtk::TreeModel::Row row = *(treeModel->append(solarSystemMap[solarSystemID].children()));

        row[columns.regionID] = regionID;
        row[columns.solarSystemID] = solarSystemID;
        row[columns.stationID] = stationID;
        row[columns.name] = stationName;
    }

    json_decref(root);
}