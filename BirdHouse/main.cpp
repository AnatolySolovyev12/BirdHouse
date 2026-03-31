#include "GeneralClass.h"
#include <QtWidgets/QApplication>
#include <Windows.h>
#include <clocale>
#include <iostream>

int main(int argc, char* argv[])
{
    SetConsoleCP(65001);        // UTF‑8 вход
    SetConsoleOutputCP(65001);  // UTF‑8 вывод
    setlocale(LC_ALL, "ru_RU.UTF-8");

    QApplication app(argc, argv);
    GeneralClass genClass;

    std::cout << "ПРивет";

    return app.exec();
}
