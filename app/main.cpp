#include "MainWindow.h"
#include <exception>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<WorkerInput>("WorkerInput");
    qRegisterMetaType<WorkerOutput>("WorkerOutput");
    qRegisterMetaType<std::exception_ptr>("std::exception_ptr");

    MainWindow w;
    w.show();

    return a.exec();
}
