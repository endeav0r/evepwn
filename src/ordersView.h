#ifndef ordersView_HEADER
#define ordersView_HEADER

#include <gtkmm.h>
#include <list>

#include "order.h"

class OrdersView : public Gtk::ScrolledWindow {
    protected :

        class OrdersColumns : public Gtk::TreeModel::ColumnRecord {
            public :
                OrdersColumns () {
                    add(volPrice);
                    add(price);
                    add(volRemaining);
                    add(range);
                    add(orderID);
                    add(volEntered);
                    add(bid);
                    add(issueDate);
                    add(duration);
                    add(stationID);
                    add(stationName);
                    add(regionName);
                }

                Gtk::TreeModelColumn <uint64_t> volPrice;
                Gtk::TreeModelColumn <double> price;
                Gtk::TreeModelColumn <uint64_t> volRemaining;
                Gtk::TreeModelColumn <int> range;
                Gtk::TreeModelColumn <uint64_t> orderID;
                Gtk::TreeModelColumn <uint64_t> volEntered;
                Gtk::TreeModelColumn <bool> bid;
                Gtk::TreeModelColumn <time_t> issueDate;
                Gtk::TreeModelColumn <uint32_t> duration;
                Gtk::TreeModelColumn <uint32_t> stationID;
                Gtk::TreeModelColumn <std::string> stationName;
                Gtk::TreeModelColumn <std::string> regionName;
        };

        OrdersColumns columns;

        Gtk::TreeView treeView;
        Glib::RefPtr<Gtk::TreeStore> treeModel;

    public :
        OrdersView ();

        void setOrders (std::list <Order> orders);
};

#endif