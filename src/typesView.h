#ifndef typesView_HEADER
#define typesView_HEADER

#include <gtkmm.h>
#include <jansson.h>

#include <inttypes.h>
#include <set>

class TypesView : public Gtk::ScrolledWindow {
    protected :

        class TypesColumns : public Gtk::TreeModel::ColumnRecord {
            public :
                TypesColumns () {
                    add(marketGroupID);
                    add(typeID);
                    add(name);
                }

                Gtk::TreeModelColumn <unsigned int> marketGroupID;
                Gtk::TreeModelColumn <unsigned int> typeID;
                Gtk::TreeModelColumn <Glib::ustring> name;
        };

        TypesColumns columns;

        Gtk::TreeView treeView;
        Glib::RefPtr<Gtk::TreeStore> treeModel;

        Gtk::Menu popupMenu;

        void init ();

    public :
        TypesView ();

        std::set <uint32_t> getChildrenTypeIDs (Gtk::TreeModel::Row row);
        std::set <uint32_t> getSelectedTypeIDs ();

        void launchSpreadWindow ();
        void launchBidOfferWindow ();

        bool on_button_press_event (GdkEventButton * event);
};

#endif