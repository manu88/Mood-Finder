#include "mainwindow.h"
#include <QApplication>

#include "commons.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName( ORG_NAME );
    QCoreApplication::setApplicationName( APP_NAME );

    MainWindow w;
    w.show();

    return a.exec();
}
