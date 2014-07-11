#include "typesView.h"

#include <iostream>
#include <map>

#include "bidOfferWindow.h"
#include "mainWindow.h"
#include "spreadWindow.h"

#include "eveDB.h"

#include "config.h"

TypesView :: TypesView () {
    add(treeView);

    treeModel = Gtk::TreeStore::create(columns);
    treeView.set_model(treeModel);

    treeView.append_column("Name", columns.name);

    Gtk::MenuItem * item = Gtk::manage(new Gtk::MenuItem("_Spreads", true));
    item->signal_activate().connect(sigc::mem_fun(*this, &TypesView::launchSpreadWindow));
    popupMenu.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Bid Offer", true));
    item->signal_activate().connect(sigc::mem_fun(*this, &TypesView::launchBidOfferWindow));
    popupMenu.append(*item);

    popupMenu.accelerate(*this);
    popupMenu.show_all();

    treeView.signal_button_press_event().connect(sigc::mem_fun(*this, &TypesView::on_button_press_event), false);

    init();

    show_all_children();
}


std::set <uint32_t> TypesView :: getChildrenTypeIDs (Gtk::TreeModel::Row row) {
    std::set <uint32_t> typeIDs;

    Gtk::TreeModel::Children children = row.children();
    Gtk::TreeModel::Children::iterator it;
    for (it = children.begin(); it != children.end(); it++) {
        Gtk::TreeModel::Row row = *it;
        if (row[columns.typeID] != UNDEFINED)
            typeIDs.insert(row[columns.typeID]);
        std::set <uint32_t> childrenTypeIDs = getChildrenTypeIDs(row);
        typeIDs.insert(childrenTypeIDs.begin(), childrenTypeIDs.end());
    }

    return typeIDs;
}


void TypesView :: launchSpreadWindow () {
    std::set <uint32_t> typeIDs;

    Glib::RefPtr<Gtk::TreeSelection> selection = treeView.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();

    if (iter) {
        Gtk::TreeModel::Row row = *iter;

        typeIDs = getChildrenTypeIDs(row);

        if (row[columns.typeID] != UNDEFINED)
            typeIDs.insert(row[columns.typeID]);
    }

    std::cout << typeIDs.size() << " typeIDs" << std::endl;

    SpreadWindow * spreadWindow = new SpreadWindow(typeIDs);
    spreadWindow->show();

    MainWindow::get()->addWindow(spreadWindow);
}


void TypesView :: launchBidOfferWindow () {
    std::set <uint32_t> typeIDs;

    Glib::RefPtr<Gtk::TreeSelection> selection = treeView.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();

    if (iter) {
        Gtk::TreeModel::Row row = *iter;

        if (row[columns.typeID] != UNDEFINED) {
            BidOfferWindow * bidOfferWindow = new BidOfferWindow(row[columns.typeID]);
            bidOfferWindow->show();
        }
    }
}


bool TypesView :: on_button_press_event (GdkEventButton * event) {
    bool return_value = ScrolledWindow::on_button_press_event(event);

    if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
        popupMenu.popup(event->button, event->time);
    }

    return return_value;
}


std::set <uint32_t> TypesView :: getSelectedTypeIDs () {
    std::set <uint32_t> typeIDs;

    Glib::RefPtr<Gtk::TreeSelection> selection = treeView.get_selection();
    std::vector <Gtk::TreeModel::Path> selections = selection->get_selected_rows();

    std::vector <Gtk::TreeModel::Path> :: iterator it;
    for (it = selections.begin(); it != selections.end(); it++) {
        Gtk::TreeModel::iterator iter = treeModel->get_iter(*it);
        if (not iter) continue;

        Gtk::TreeModel::Row row = *iter;
        typeIDs.insert(row[columns.typeID]);
    }

    return typeIDs;
}


void TypesView :: init () {
    const std::map <uint32_t, EveMarketGroup> & marketGroups = EveDB::g_marketGroups();
    std::map <uint32_t, EveMarketGroup> :: const_iterator mmgit;
    std::list <EveMarketGroup> mgList;

    for (mmgit = marketGroups.begin(); mmgit != marketGroups.end(); mmgit++) {
        mgList.push_back(mmgit->second);
    }

    std::map <unsigned int, Gtk::TreeModel::Row> marketGroupMap;

    std::list <EveMarketGroup> :: iterator lmgit = mgList.begin();

    while (mgList.size() > 0) {
        const EveMarketGroup & marketGroup = *lmgit;
        if (marketGroup.g_parentGroupID() == UNDEFINED) {
            uint32_t id = marketGroup.g_marketGroupID();
            marketGroupMap[id] = *(treeModel->append());
            marketGroupMap[id][columns.marketGroupID] = id;
            marketGroupMap[id][columns.typeID] = UNDEFINED;
            marketGroupMap[id][columns.name] = marketGroup.g_name();
            lmgit = mgList.erase(lmgit);
            continue;
        }
        else if (marketGroupMap.count(marketGroup.g_parentGroupID()) > 0) {
            uint32_t id = marketGroup.g_marketGroupID();
            marketGroupMap[id] = *(treeModel->append(marketGroupMap[marketGroup.g_parentGroupID()].children()));
            marketGroupMap[id][columns.marketGroupID] = id;
            marketGroupMap[id][columns.typeID] = UNDEFINED;
            marketGroupMap[id][columns.name] = marketGroup.g_name();
            lmgit = mgList.erase(lmgit);
            continue;
        }

        lmgit++;
    }

    const std::map <uint32_t, EveType> & types = EveDB::g_types();
    std::map <uint32_t, EveType> :: const_iterator tit;

    for (tit = types.begin(); tit != types.end(); tit++) {
        const EveType & type = tit->second;

        if (type.g_marketGroupID() == UNDEFINED)
            continue;

        Gtk::TreeModel::Row row = *(treeModel->append(marketGroupMap[type.g_marketGroupID()].children()));
        row[columns.marketGroupID] = UNDEFINED;
        row[columns.typeID] = type.g_typeID();
        row[columns.name] = type.g_name();
    }
}