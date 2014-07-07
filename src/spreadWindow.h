#ifndef spreadWindow_HEADER
#define spreadWindow_HEADER

#include <gtkmm.h>

#include <set>

#include "spreadView.h"


class SpreadWindow : public Gtk::Window {
    protected :
        SpreadView spreadView;

        std::set <uint32_t> typeIDs;

    public :
        SpreadWindow (std::set <uint32_t> typeIDs);

};

#endif