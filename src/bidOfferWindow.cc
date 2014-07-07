#include "bidOfferWindow.h"

#include "config.h"
#include "eveDB.h"
#include "evePwnClient.h"
#include "mainWindow.h"

BidOfferWindow :: BidOfferWindow (uint32_t typeID) {
    this->typeID = typeID;

    this->set_default_size(800, 600);

    add(vbox);

    typeNameLabel.set_text(EveDB::g_type(typeID).g_name());

    vbox.pack_start(typeNameLabel, false, true, 2);
    vbox.pack_start(bidOfferView, true, true, 2);

    show_all_children();

    MainWindow::get()->addWindow(this);

    show();

    EvePwnClient evePwnClient;
    bidOfferView.updateOrders(stackOrders(evePwnClient.ordersTypeID(typeID)));
}