#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include "database.h"
#include "dbdata.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    //void ScreenDataFromDB(const QTableWidget *widget, int typeRequest);
    void ScreenDataFromDB(QTableWidget *widget);
    void ScreenTableViewFromDB(QTableView *view);
    void ReceiveStatusConnectionToDB(bool status);
    void ReceiveQueryError(QSqlError err);


private slots:
    void on_act_addData_triggered();
    void on_act_connect_triggered();
    void on_pb_request_clicked();
    void on_pb_clear_clicked();


signals:
    void sig_RequestToDb(QString request, int modelType);

private:

    QVector<QString> dataForConnect; //Данные для подключения к БД.

    Ui::MainWindow *ui;
    DbData *dataDb;
    DataBase* dataBase;
    QMessageBox* msg;

    QString request;
    QTableWidget* tb_result2 = NULL;
    QTableView* tb_view = NULL;


};
#endif // MAINWINDOW_H
