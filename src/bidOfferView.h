#ifndef bidOfferView_HEADER
#define bidOfferView_HEADER

#include <gtkmm.h>
#include <list>

#include "ordersView.h"

class BidOfferView : public Gtk::HBox {
    protected :
        std::list <Order> orders;

        Gtk::VBox bidsBox;
        Gtk::VBox offersBox;

        Gtk::Label bidsLabel;
        Gtk::Label offersLabel;

        OrdersView bidsView;
        OrdersView offersView;
        
    public :
        BidOfferView ();

        void updateOrders (std::list <Order> orders);
};

#endif