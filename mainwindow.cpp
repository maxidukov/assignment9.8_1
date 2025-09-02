#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tb_result2(new QTableWidget(this->centralWidget()))
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);
    //tb_result2 = new QTableWidget();
    //tableWidget = new QTableWidget(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    connect(dataDb, &DbData::sig_sendDataAndConnect, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
        dataDb->close();
        on_act_connect_triggered();
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
     connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

     connect(dataBase, &DataBase::sig_SendTableView, this, &MainWindow::ScreenTableViewFromDB);

    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(this, &MainWindow::sig_RequestToDb, dataBase, &DataBase::ReadQueryResult);

    connect(dataBase, &DataBase::sig_SendQueryError, this, &MainWindow::ReceiveQueryError);

}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "UI deleted";
    delete dataDb;
    qDebug() << "dataDB deleted";
    delete dataBase;
    qDebug() << "database (DataBase class instance) deleted";
    delete msg;
    //qDebug() << "Got to this point, all objects but tb_result2 deleted";
    //tb_result2->deleteLater();
    //tb_result2->deleteLater(); //CAUSES CRASH. NO NEED TO DELETE IT, IT'S ALREADY DELETED WHEN DELETING UI, RIGHT ?
    qDebug() << "Got to this point, all objects deleted";
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");
       ui->act_addData->setEnabled(false);


       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);

        ui->act_addData->setEnabled(true);
    }

}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{

    ///Тут должен быть код ДЗ
    request = "SELECT title, description, c.name  FROM film f "
              "JOIN film_category fc on f.film_id = fc.film_id "
              "JOIN category c on c.category_id  = fc.category_id";
    switch(ui->cb_category->currentIndex()+1){
    //switch(typeRequest){
    case requestAllFilms:
        //request = "SELECT title, description FROM film;";
        break;
    case requestComedy: request += " WHERE c.name = 'Comedy';";
        break;
    case requestHorrors: request += " WHERE c.name = 'Horror';";
        break;
    }
    qDebug() << "SQL query is this: " << request;

    emit sig_RequestToDb(request, ui->cb_category->currentIndex()+1);

}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
//void MainWindow::ScreenDataFromDB(const QTableWidget *widget, int typeRequest)
void MainWindow::ScreenDataFromDB(QTableWidget *widget)
{
    ///Тут должен быть код ДЗ
    //switch(typeRequest){
    //case requestAllFilms:
        // QTableView *view = new QTableView;
        // view->setModel(model);
        // view->show();
    //    break;
    //case requestComedy:
    //case requestHorrors:
        // ui->tb_result->setColumnCount(widget->columnCount());
        // ui->tb_result->setRowCount(widget->rowCount());
        // QStringList headers;

        // for(int i = 0; i < widget->columnCount(); i++){
        //     headers << widget->horizontalHeaderItem(i)->text();
        // }
        // ui->tb_result->setHorizontalHeaderLabels(headers);

        // for(int i = 0; i < widget->rowCount(); i++){
        //     for(int j = 0; j < widget->columnCount(); j++){
        //         ui->tb_result->setItem(i, j, widget->item(i,j)->clone());
        //     }
        // }
        // ui->tb_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // ui->tb_result->update();
    //break;
    //}

    //ui->tb_result->deleteLater();
    //QTableWidget* tb_result2 = new QTableWidget;
    //tb_result2 = new QTableWidget(this->centralWidget());
    tb_result2 = widget;
    // QVBoxLayout* layout_table = new QVBoxLayout;
    // layout_table->addWidget(tableWidget);
    //ui->gridLayout->addLayout(layout_table,1,0);
    ui->gridLayout->replaceWidget(ui->tb_result, tb_result2);
    //ui->gridLayout->addWidget(tb_result2,1,0);
    ui->gridLayout->update();
}
    void MainWindow::ScreenTableViewFromDB(QTableView *view)
    {

        ///Тут должен быть код ДЗ
        tb_view = view;
        ui->gridLayout->replaceWidget(ui->tb_result, tb_view);
        ui->gridLayout->update();
    }

/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);

        //ui->act_addData->setEnabled(false);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");

        ui->act_addData->setEnabled(true);

        msg->exec();
    }

}

void MainWindow::ReceiveQueryError(QSqlError err){
    dataBase->DisconnectFromDataBase(DB_NAME);
    ui->lb_statusConnect->setText("Отключено");
    ui->act_connect->setText("Подключиться");
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    msg->setIcon(QMessageBox::Critical);
    msg->setText("Sql query ERROR: " + err.text());
    msg->exec();
}

void MainWindow::on_pb_clear_clicked(){
    // if(ui->tb_result != NULL){
    //     qDebug() << "tb->result is not NULL";
    //     ui->tb_result->clear();
    // }
    //if(tb_result2 != NULL) tb_result2->clear();
    //ui->tb_result->clear();
    tb_result2->clear();
    if(tb_view != NULL){
        tb_view->setModel(NULL);
    }
}



