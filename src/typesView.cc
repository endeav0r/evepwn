#include "typesView.h"

#include <iostream>
#include <map>

#include "bidOfferWindow.h"
#include "mainWindow.h"
#include "spreadWindow.h"

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


void TypesView :: loadMarket (std::string basepath) {
    json_error_t error;

    std::string invMarketGroups = std::string(BASEPATH) + "invMarketGroups.json";
    std::string invTypes = std::string(BASEPATH) + "invTypes.json";
    json_t * root = json_load_file(invMarketGroups.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading market groups from " << invMarketGroups << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return;
    }

    std::map <unsigned int, Gtk::TreeModel::Row> marketGroupMap;

    size_t root_i = 0;
    while (json_array_size(root) > 0) {
        if (root_i >= json_array_size(root)) {
            root_i = 0;
            continue;
        }

        json_t * object = json_array_get(root, root_i);
        json_t * j_parentGroupID   = json_object_get(object, "parentGroupID");
        json_t * j_marketGroupID   = json_object_get(object, "marketGroupID");
        json_t * j_marketGroupName = json_object_get(object, "marketGroupName");

        unsigned int marketGroupID = json_integer_value(j_marketGroupID);

        if (json_is_null(j_parentGroupID)) {
            Glib::ustring marketGroupName = json_string_value(j_marketGroupName);
            marketGroupMap[marketGroupID] = *(treeModel->append());
            marketGroupMap[marketGroupID][columns.marketGroupID] = marketGroupID;
            marketGroupMap[marketGroupID][columns.typeID] = UNDEFINED;
            marketGroupMap[marketGroupID][columns.name] = marketGroupName;

            json_array_remove(root, root_i);
            continue;
        }
        else {
            unsigned int parentGroupID = json_integer_value(j_parentGroupID);
            if (marketGroupMap.count(parentGroupID) > 0) {
                Glib::ustring marketGroupName = json_string_value(j_marketGroupName);
                marketGroupMap[marketGroupID] = *(treeModel->append(marketGroupMap[parentGroupID].children()));
                marketGroupMap[marketGroupID][columns.marketGroupID] = marketGroupID;
                marketGroupMap[marketGroupID][columns.typeID] = UNDEFINED;
                marketGroupMap[marketGroupID][columns.name] = marketGroupName;

                json_array_remove(root, root_i);
                continue;
            }
        }

        root_i++;
    }

    json_decref(root);

    root = json_load_file(invTypes.c_str(), 0, &error);

    if (root == NULL) {
        std::cerr << "error loading market groups from " << invTypes << std::endl;
        std::cerr << "error text: " << error.text << std::endl;
        std::cerr << "error source: " << error.source << std::endl;
        std::cerr << "error line: " << error.line << std::endl;
        std::cerr << "error column: " << error.column << std::endl;
        std::cerr << "error position: " << error.position << std::endl;

        return;
    }

    for (root_i = 0; root_i < json_array_size(root); root_i++) {
        json_t * object = json_array_get(root, root_i);

        json_t * j_marketGroupID = json_object_get(object, "marketGroupID");
        if (json_is_null(j_marketGroupID))
            continue;

        json_t * j_typeID   = json_object_get(object, "typeID");
        json_t * j_typeName = json_object_get(object, "typeName");

        unsigned int typeID = json_integer_value(j_typeID);
        unsigned int marketGroupID = json_integer_value(j_marketGroupID);
        Glib::ustring typeName = json_string_value(j_typeName);

        Gtk::TreeModel::Row row = *(treeModel->append(marketGroupMap[marketGroupID].children()));
        row[columns.marketGroupID] = marketGroupID;
        row[columns.typeID] = typeID;
        row[columns.name] = typeName;
    }

    json_decref(root);
}