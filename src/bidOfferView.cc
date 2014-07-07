#include "bidOfferView.h"


BidOfferView :: BidOfferView () {
    pack_start(bidsBox);
    pack_start(offersBox);

    bidsLabel.set_text("Bids");
    offersLabel.set_text("Offers");

    bidsBox.pack_start(bidsLabel, false, true, 2);
    bidsBox.pack_start(bidsView, true, true, 2);

    offersBox.pack_start(offersLabel, false, true, 2);
    offersBox.pack_start(offersView, true, true, 2);
}


void BidOfferView :: updateOrders (std::list <Order> orders) {
    std::list <Order> bids;
    std::list <Order> offers;

    std::list <Order> :: iterator it;

    for (it = orders.begin(); it != orders.end(); it++) {
        if ((*it).g_bid())
            bids.push_back(*it);
        else
            offers.push_back(*it);
    }

    bidsView.setOrders(bids);
    offersView.setOrders(offers);
}