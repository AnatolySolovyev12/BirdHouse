#include "BirdHouse.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BirdHouse window;
    window.show();
    return app.exec();
}
