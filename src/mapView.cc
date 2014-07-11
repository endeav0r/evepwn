#include "mapView.h"

#include <iostream>
#include <map>

#include "config.h"
#include "eveDB.h"

MapView :: MapView () {
    add(treeView);

    treeModel = Gtk::TreeStore::create(columns);
    treeView.set_model(treeModel);

    treeView.append_column("Name", columns.name);

    Gtk::TreeView::Column * column;

    column = treeView.get_column(0);
    column->set_sort_column(columns.name);

    init();

    show_all_children();
}


void MapView :: init () {
    const std::map <uint32_t, EveRegion> & regions = EveDB::g_regions();
    std::map <uint32_t, EveRegion> :: const_iterator rit;

    std::map <uint32_t, Gtk::TreeModel::Row> regionMap;

    for (rit = regions.begin(); rit != regions.end(); rit++) {
        const EveRegion & region = rit->second;

        regionMap[region.g_regionID()] = *(treeModel->append());
        regionMap[region.g_regionID()][columns.solarSystemID] = UNDEFINED;
        regionMap[region.g_regionID()][columns.stationID] = UNDEFINED;
        regionMap[region.g_regionID()][columns.name] = region.g_name();
    }


    const std::map <uint32_t, EveSolarSystem> & solarSystems = EveDB::g_solarSystems();
    std::map <uint32_t, EveSolarSystem> :: const_iterator ssit;

    std::map <uint32_t, Gtk::TreeModel::Row> solarSystemMap;

    for (ssit = solarSystems.begin(); ssit != solarSystems.end(); ssit++) {
        const EveSolarSystem & solarSystem = ssit->second;

        solarSystemMap[solarSystem.g_solarSystemID()] = *(treeModel->append(regionMap[solarSystem.g_regionID()].children()));
        solarSystemMap[solarSystem.g_solarSystemID()][columns.regionID] = solarSystem.g_regionID();
        solarSystemMap[solarSystem.g_solarSystemID()][columns.solarSystemID] = solarSystem.g_solarSystemID();
        solarSystemMap[solarSystem.g_solarSystemID()][columns.stationID] = UNDEFINED;
        solarSystemMap[solarSystem.g_solarSystemID()][columns.name] = solarSystem.g_name();
    }


    const std::map <uint32_t, EveStation> & stations = EveDB::g_stations();
    std::map <uint32_t, EveStation> :: const_iterator sit;

    for (sit = stations.begin(); sit != stations.end(); sit++) {
        const EveStation & station = sit->second;

        Gtk::TreeModel::Row row = *(treeModel->append(solarSystemMap[station.g_solarSystemID()].children()));
        row[columns.regionID] = station.g_stationID();
        row[columns.solarSystemID] = station.g_solarSystemID();
        row[columns.stationID] = station.g_stationID();
        row[columns.name] = station.g_name();
    }
}