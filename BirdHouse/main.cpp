#include "GeneralClass.h"
#include <QtWidgets/QApplication>
#include <Windows.h>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    QApplication app(argc, argv);
    GeneralClass genClass;
    return app.exec();
}
