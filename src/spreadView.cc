#include "spreadView.h"

#include "bidOfferWindow.h"
#include "config.h"
#include "eveDB.h"

#include <cstdio>

#include <algorithm>

SpreadView :: SpreadView () {
    add(treeView);

    treeModel = Gtk::TreeStore::create(columns);
    treeView.set_model(treeModel);

    treeView.append_column("Name",         columns.name);
    treeView.append_column("Bid",          columns.bid);
    treeView.append_column("Bid 5%",       columns.bid5);
    treeView.append_column("Bid Volume",   columns.bidVolume);
    treeView.append_column("Offer",        columns.offer);
    treeView.append_column("Offer 5%",     columns.offer5);
    treeView.append_column("Offer Volume", columns.offerVolume);
    treeView.append_column("Spread",       columns.spread);
    treeView.append_column("Spread 5%",    columns.spread5);
    treeView.append_column("SpreadPrice",  columns.spreadPrice);

    Gtk::TreeView::Column * column;
    column = treeView.get_column(0);
    column->set_sort_column(columns.name);
    column->set_resizable(true);

    column = treeView.get_column(1);
    column->set_sort_column(columns.bid);
    column->set_resizable(true);

    column = treeView.get_column(2);
    column->set_sort_column(columns.bid5);
    column->set_resizable(true);

    column = treeView.get_column(3);
    column->set_sort_column(columns.bidVolume);
    column->set_resizable(true);

    column = treeView.get_column(4);
    column->set_sort_column(columns.offer);
    column->set_resizable(true);

    column = treeView.get_column(5);
    column->set_sort_column(columns.offer5);
    column->set_resizable(true);

    column = treeView.get_column(6);
    column->set_sort_column(columns.offerVolume);
    column->set_resizable(true);

    column = treeView.get_column(7);
    column->set_sort_column(columns.spread);
    column->set_resizable(true);

    column = treeView.get_column(8);
    column->set_sort_column(columns.spread5);
    column->set_resizable(true);

    column = treeView.get_column(9);
    column->set_sort_column(columns.spreadPrice);
    column->set_resizable(true);

    show_all_children();

    Gtk::MenuItem * item = Gtk::manage(new Gtk::MenuItem("_Bid Offers", true));
    item->signal_activate().connect(sigc::mem_fun(*this, &SpreadView::launchBidOfferWindow));
    popupMenu.append(*item);

    popupMenu.accelerate(*this);
    popupMenu.show_all();

    treeView.signal_button_press_event().connect(sigc::mem_fun(*this, &SpreadView::on_button_press_event), false);
}


void SpreadView :: launchBidOfferWindow () {
    Glib::RefPtr<Gtk::TreeSelection> selection = treeView.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();

    if (iter) {
        Gtk::TreeModel::Row row = *iter;

        BidOfferWindow * bidOfferWindow = new BidOfferWindow(row[columns.typeID]);
        bidOfferWindow->show();
    }
}


bool SpreadView :: on_button_press_event (GdkEventButton * event) {
    bool return_value = ScrolledWindow::on_button_press_event(event);

    if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
        popupMenu.popup(event->button, event->time);
    }

    return return_value;
}


bool sortOffers (const Order lhs, const Order rhs) {
    return (lhs.g_price() < rhs.g_price());
}


bool sortBids (const Order lhs, const Order rhs) {
    return (lhs.g_price() > rhs.g_price());
}


void SpreadView :: loadType (uint32_t typeID, std::list <Order> orders) {
    std::vector <Order> offers;
    std::vector <Order> bids;

    uint64_t bidVolume = 0;
    uint64_t offerVolume = 0;

    std::list <Order> :: iterator it;
    for (it = orders.begin(); it != orders.end(); it++) {
        Order & order = *it;
        if (order.g_bid()) {
            bidVolume += order.g_volRemaining();
            bids.push_back(order);
        }
        else {
            offerVolume += order.g_volRemaining();
            offers.push_back(order);
        }
    }

    std::sort(bids.begin(), bids.end(), sortBids);
    std::sort(offers.begin(), offers.end(), sortOffers);

    int64_t bidVolume5 = bidVolume / 20;
    int64_t offerVolume5 = offerVolume / 20;

    double bid5 = 0;
    for (size_t i = 0; i < bids.size(); i++) {
        bidVolume5 -= bids[i].g_volRemaining();
        if ((bidVolume5 <= 0) && (bid5 == 0)) {
            bid5 = bids[i].g_price();
            break;
        }
    }

    double offer5 = 0;
    for (size_t i = 0; i < offers.size(); i++) {
        offerVolume5 -= offers[i].g_volRemaining();
        if ((offerVolume5 <= 0) && (offer5 == 0)) {
            offer5 = offers[i].g_price();
            break;
        }
    }

    double offer, bid, spread, spread5, spreadPrice;

    if (offers.size() == 0) {
        offer = 0;
        spread = 0;
        spreadPrice = 0;
        spread5 = 0;
    }
    else
        offer = offers.front().g_price();

    if (bids.size() == 0) {
        bid = 0;
        spread = 0;
        spreadPrice = 0;
        spread5 = 0;
    }
    else
        bid = bids.front().g_price();

    if ((offers.size() > 0) && (bids.size() > 0)) {
        offer       = offers.front().g_price();
        bid         = bids.front().g_price();
        spread      = (offer - bid) / offer;
        if (offer5 != 0)
            spread5 = (offer5 - bid5) / offer5;
        spreadPrice = bid - offer;
    }

    if (typeIDRowMap.count(typeID) == 0)
        typeIDRowMap[typeID] = *(treeModel->append());

    typeIDRowMap[typeID][columns.typeID] = typeID;
    typeIDRowMap[typeID][columns.name] = EveDB::g_type(typeID).g_name();
    typeIDRowMap[typeID][columns.bid] = bid;
    typeIDRowMap[typeID][columns.bid5] = bid5;
    typeIDRowMap[typeID][columns.bidVolume] = bidVolume;
    typeIDRowMap[typeID][columns.offer] = offer;
    typeIDRowMap[typeID][columns.offer5] = offer5;
    typeIDRowMap[typeID][columns.offerVolume] = offerVolume;
    typeIDRowMap[typeID][columns.spread] = spread;
    typeIDRowMap[typeID][columns.spread5] = spread5;
    typeIDRowMap[typeID][columns.spreadPrice] = spreadPrice;
}