#include "ordersView.h"

#include "eveDB.h"

#include <iostream>
#include <map>

OrdersView :: OrdersView () {
    add(treeView);

    treeModel = Gtk::TreeStore::create(columns);
    treeView.set_model(treeModel);

    treeView.append_column("volPrice", columns.volPrice);
    treeView.append_column("Price", columns.price);
    treeView.append_column("Volume", columns.volRemaining);
    treeView.append_column("Region", columns.regionName);
    treeView.append_column("Station", columns.stationName);

    Gtk::TreeView::Column * column;

    column = treeView.get_column(0);
    column->set_sort_column(columns.volPrice);
    column->set_resizable(true);

    column = treeView.get_column(1);
    column->set_sort_column(columns.price);
    column->set_resizable(true);
    
    column = treeView.get_column(2);
    column->set_sort_column(columns.volRemaining);
    column->set_resizable(true);
    
    column = treeView.get_column(3);
    column->set_sort_column(columns.regionName);
    column->set_resizable(true);
    
    column = treeView.get_column(4);
    column->set_sort_column(columns.stationName);
    column->set_resizable(true);

    show_all_children();
}


void OrdersView :: setOrders (std::list <Order> orders) {
    treeModel = Gtk::TreeStore::create(columns);
    treeView.set_model(treeModel);

    std::list <Order> :: iterator it;
    for (it = orders.begin(); it != orders.end(); it++) {
        Order & order = *it;

        Gtk::TreeModel::Row row = *(treeModel->append());

        row[columns.volPrice] = order.g_price() * order.g_volRemaining();
        row[columns.price] = order.g_price();
        row[columns.volRemaining] = order.g_volRemaining();
        row[columns.range] = order.g_range();
        row[columns.orderID] = order.g_orderID();
        row[columns.volEntered] = order.g_volEntered();
        row[columns.bid] = order.g_bid();
        row[columns.issueDate] = order.g_issueDate();
        row[columns.duration] = order.g_duration();
        row[columns.stationID] = order.g_stationID();
        row[columns.stationName] = EveDB::g_station(order.g_stationID()).g_name();
        EveRegion region = EveDB::g_region(EveDB::g_station(order.g_stationID()).g_regionID());
        row[columns.regionName] = region.g_name();
    }
}