#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    query = new QSqlQuery();
    //tableWidget = new QTableWidget();
}

DataBase::~DataBase()
{
    delete dataBase;
    qDebug() << "\tdataBase inside DataBase class instance deleted";
    delete query;
    qDebug() << "\tquery inside DataBase class instance deleted";
    //delete tableWidget;
    delete querymodel;
    delete tablemodel;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
    //querymodel = new QSqlQueryModel(this);


}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    //querymodel = new QSqlQueryModel();
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */

void DataBase::ReadQueryResult(QString request, int reqType){
    //switch(ui->cb_category->currentIndex()){
    QStringList headers;
    headers << "Название фильма" << "Описание фильма";

    //QSqlTableModel *model = new QSqlTableModel(this, *dataBase);
    //QTableView *view = new QTableView;

    switch(reqType){
    case requestAllFilms:
        {
        //tablemodel = new QSqlTableModel(this); //DOESN'T WORK, TABLE VIEW IS EMPTY
        tablemodel = new QSqlTableModel(this, *dataBase);
        tablemodel->setTable("film");
        tablemodel->select();

        for(int n = 1; n < headers.size() + 1; n++){
            tablemodel->setHeaderData(n, Qt::Horizontal, headers[n -1]);
        };
        QTableView *view = new QTableView;
        view->setModel(tablemodel);
        //qDebug() << "Tableview has " << view->model()->rowCount() << " rows";
        view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //NO EFFECT((
        //view->resizeColumnsToContents();
        view->resizeColumnToContents(1);
        view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        view->setColumnHidden(0, true);
        for(int n = 3; n < view->model()->columnCount(); n++){
            view->setColumnHidden(n, true);
        }
        // view->reset();
        // view->update();
        // view->show();
        emit sig_SendTableView(view);
    }
        break;
    case requestComedy:
        {
            querymodel = new QSqlQueryModel();
            //model->setQuery(request); //DOESN'T WORK, TABLE VIEW IS EMPTY
            querymodel->setQuery(request, *dataBase);
            headers << "Категория";
            for(int n = 0; n < headers.size(); n++){
                //qDebug()  << headers[n];
                querymodel->setHeaderData(n, Qt::Horizontal, headers[n]);
            };
            QTableView *view = new QTableView;
            view->setModel(querymodel);
            //qDebug() << "Tableview has " << view->model()->rowCount() << " rows";
            view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);//NO EFFECT((
            //view->resizeColumnsToContents();
            view->resizeColumnToContents(0);
            view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            view->setColumnHidden(2, true);
            //view->show();
            emit sig_SendTableView(view);
    }
    break;
    case requestHorrors:
        //query = new QSqlQuery(); //DOESN'T WORK
        query = new QSqlQuery(*dataBase);
        bool querystatus = query->exec(request);
        if(querystatus){
            //qDebug() << "Query status is true";
        }else{
            //qDebug() << "Query status is false";
            //return query->lastError();
            emit sig_SendQueryError(query->lastError());
        }

        const int num_cols = 3;
        //tableWidget = new QTableWidget;
        QTableWidget* tableWidget = new QTableWidget;
        tableWidget->setColumnCount(num_cols);
        tableWidget->setRowCount(0);
        headers << "Категория";
        tableWidget->setHorizontalHeaderLabels(headers);

        uint32_t rowcounter = 0;

        while(query->next()){
            tableWidget->insertRow(rowcounter);
            for(int col = 0; col < num_cols; ++col){
                QString itemText;
                itemText  = query->value(col).toString();
                //qDebug() << itemText;
                QTableWidgetItem* item = new QTableWidgetItem(itemText);
                tableWidget->setItem(tableWidget->rowCount()-1, col, item);
            }
            ++rowcounter;
        }
        //tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //NO EFFECT((
        //tableWidget->resizeColumnsToContents();
        tableWidget->resizeColumnToContents(0);
        tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tableWidget->setColumnHidden(2, true);
        emit sig_SendDataFromDB(tableWidget);
        break;
    }
    //delete model;
    //delete view;
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
