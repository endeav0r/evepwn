#ifndef mapView_HEADER
#define mapView_HEADER

#include <set>

#include <gtkmm.h>
#include <jansson.h>

class MapView : public Gtk::ScrolledWindow {
    protected :

        class MapColumns : public Gtk::TreeModel::ColumnRecord {
            public :
                MapColumns () {
                    add(regionID);
                    add(solarSystemID);
                    add(stationID);
                    add(name);
                }

                Gtk::TreeModelColumn <unsigned int> regionID;
                Gtk::TreeModelColumn <unsigned int> solarSystemID;
                Gtk::TreeModelColumn <unsigned int> stationID;
                Gtk::TreeModelColumn <Glib::ustring> name;
        };

        MapColumns columns;

        Gtk::TreeView treeView;
        Glib::RefPtr<Gtk::TreeStore> treeModel;

    public :
        MapView ();

        void loadMap (std::string basepath);
};

#endif