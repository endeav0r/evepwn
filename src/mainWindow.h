#ifndef mainWindow_HEADER
#define mainWindow_HEADER

#include <gtkmm.h>

#include "bidOfferView.h"
#include "mapView.h"
#include "typesView.h"

class MainWindow : public Gtk::Window {
    protected :
        Gtk::Paned mainPaned;

        TypesView typesView;
        MapView mapView;

        std::set <Gtk::Window *> windows;

    public :
        MainWindow ();
        static MainWindow * get();

        void addWindow (Gtk::Window * window) {
            windows.insert(window);
        }

        void delWindow (Gtk::Window * window) {
            windows.erase(window);
        }
};

#endif