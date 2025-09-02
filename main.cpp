#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // QStringList ds=QSqlDatabase::drivers();
    // foreach(QString drivername,ds)
    // {
    //     qDebug() << drivername;
    // }
    return a.exec();
}
