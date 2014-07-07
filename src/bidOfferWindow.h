#ifndef bidOfferWindow_HEADER
#define bidOfferWindow_HEADER

#include <gtkmm.h>

#include <set>

#include "bidOfferView.h"


class BidOfferWindow : public Gtk::Window {
    protected :
        Gtk::VBox vbox;

        Gtk::Label typeNameLabel;
        
        BidOfferView bidOfferView;
        uint32_t typeID;

    public :
        BidOfferWindow (uint32_t typeID);

};

#endif