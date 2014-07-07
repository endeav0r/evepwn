#include "mainWindow.h"

#include "config.h"
#include "eveDB.h"
#include "evePwnClient.h"

#include <iostream>

MainWindow * singleton;

MainWindow :: MainWindow () {
    singleton = this;

    add(typesView);
//    add(mainPaned);

//    mainPaned.add1(mapView);
//    mainPaned.add2(typesView);

    typesView.loadMarket("/home/user/code/evepwn-gtk/");
//    mapView.loadMap("/home/user/code/evepwn-gtk/");

    show_all_children();
}


MainWindow * MainWindow :: get () {
    return singleton;
}