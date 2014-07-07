#include <gtkmm.h>

#include "mainWindow.h"

int main (int argc, char * argv[]) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "tfpwn.evepwn");

    MainWindow mainWindow;
    mainWindow.set_default_size(320, 600);

    return app->run(mainWindow);
}