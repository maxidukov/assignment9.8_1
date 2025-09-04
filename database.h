#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>

#include <QHeaderView>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

//Типы запросов
enum requestType{

    requestAllFilms = 1,
    requestComedy   = 2,
    requestHorrors  = 3

};



class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(QString request, int reqType);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void ReadQueryResult(QString request, int reqType);


signals:

   void sig_SendDataFromDB(QTableWidget *tableWg);
   void sig_SendTableView(QTableView *view);

   void sig_SendStatusConnection(bool);
   void sig_SendQueryError(QSqlError err);



private:

    QSqlDatabase* dataBase;
    QSqlQuery* query;
    //QTableWidget* tableWidget;
    QSqlTableModel *tablemodel = new QSqlTableModel;
    QSqlQueryModel *querymodel = new QSqlQueryModel;


};

#endif // DATABASE_H
