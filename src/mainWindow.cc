#include "mainWindow.h"

#include "config.h"
#include "eveDB.h"
#include "evePwnClient.h"

#include <iostream>

MainWindow * singleton;

MainWindow :: MainWindow () {
    singleton = this;

    add(mainPaned);

    mainPaned.add1(mapView);
    mainPaned.add2(typesView);

    show_all_children();
}


MainWindow * MainWindow :: get () {
    return singleton;
}