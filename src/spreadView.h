#ifndef spreadView_HEADER
#define spreadView_HEADER

#include <gtkmm.h>

#include <inttypes.h>
#include <map>

#include "order.h"

class SpreadView : public Gtk::ScrolledWindow {
    protected :

        class SpreadColumns : public Gtk::TreeModel::ColumnRecord {
            public :
                SpreadColumns () {
                    add(typeID);
                    add(name);
                    add(bid);
                    add(bid5);
                    add(bidVolume);
                    add(offer);
                    add(offer5);
                    add(offerVolume);
                    add(spread);
                    add(spread5);
                    add(spreadPrice);
                }

                Gtk::TreeModelColumn <unsigned int> typeID;
                Gtk::TreeModelColumn <Glib::ustring> name;
                Gtk::TreeModelColumn <double>   bid;
                Gtk::TreeModelColumn <double>   bid5;
                Gtk::TreeModelColumn <uint64_t> bidVolume;
                Gtk::TreeModelColumn <double>   offer;
                Gtk::TreeModelColumn <double>   offer5;
                Gtk::TreeModelColumn <uint64_t> offerVolume;
                Gtk::TreeModelColumn <double>   spread;
                Gtk::TreeModelColumn <double>   spread5;
                Gtk::TreeModelColumn <int64_t>  spreadPrice;
        };

        SpreadColumns columns;

        Gtk::TreeView treeView;
        Glib::RefPtr<Gtk::TreeStore> treeModel;

        std::map <uint32_t, Gtk::TreeModel::Row> typeIDRowMap;

        Gtk::Menu popupMenu;

    public :
        SpreadView ();

        void loadType (uint32_t typeID, std::list <Order> orders);

        void launchBidOfferWindow ();

        bool on_button_press_event (GdkEventButton * event);
};

#endif