#include "orderdialog.h"
#include "ui_orderdialog.h"
#include "mainwindow.h"


OrderDialog::OrderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderDialog)
{
    ui->setupUi(this);
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    if (mw->db->open()){
        QSqlQuery* query = new QSqlQuery(*(mw->db));
        query->setForwardOnly(true);
        ui->line_imei->setText("123321");
        ui->line_component->setText("Diod-1");
        ui->line_component_price->setText("0");
        ui->line_condition->setText("Diagnostic");
        ui->rb_diagnostic->setChecked(true);
        ui->line_issue->setText("BP");
        ui->line_master_price->setText("0");
        ui->line_model->setText("A123123");


        //Client Fio
        QSqlQueryModel *que_model_client_fio = new QSqlQueryModel;
        QString client_fio = "select fio_client from client;";
        que_model_client_fio->setQuery(client_fio);
        ui->line_client_fio->setModel(que_model_client_fio);
//        ui->line_client_fio->clearEditText();
//        ui->line_client_fio->setCurrentText("Kasyan");

        // Type Technic
        QSqlQueryModel *que_model_type_technic = new QSqlQueryModel;
        QString type_technic = "select distinct type_technic from technic;";
        que_model_type_technic->setQuery(type_technic);
        ui->line_type->setModel(que_model_type_technic);
        ui->line_type->clearEditText();
        ui->line_type->currentText();

        // Brand Technic brand
        QSqlQueryModel *que_model_brand_technic = new QSqlQueryModel;
        QString brand_technic = "select distinct brand from technic order by brand DESC;";
        que_model_brand_technic->setQuery(brand_technic);
        ui->line_brand->setModel(que_model_brand_technic);
        ui->line_brand->clearEditText();
        ui->line_brand->currentText();

        // Master fio master_fio
        QSqlQueryModel *que_model_master_fio = new QSqlQueryModel;
        QString master_fio = "select fio_master from master;";
        que_model_master_fio->setQuery(master_fio);
        ui->line_master->setModel(que_model_master_fio);
        ui->line_master->clearEditText();
        ui->line_master->currentText();

        delete query;
        qDebug() <<"END CONSTRUCT DIALOG OF NEW ORDER\n";
    }
}

OrderDialog::~OrderDialog()
{
    delete ui;
}

void OrderDialog::on_pb_accept_clicked()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    QString fio = ui->line_client_fio->currentText();
    QString tel = ui->line_tel->text();
    QString brand = ui->line_brand->currentText();
    QString type = ui->line_type->currentText();
    QString model = ui->line_model->text();
    QString imei = ui->line_imei->text();
    QString issue = ui->line_issue->text();
    QString condition = ui->line_condition->text();
    QString component = ui->line_condition->text();
    int component_price = std::stod(ui->line_component_price->text().toStdString());
    QString master = ui->line_master->currentText();
    int master_price = std::stod(ui->line_master_price->text().toStdString());
    QSqlQuery* query = new QSqlQuery(*(mw->db));
    query->setForwardOnly(true);
    if (fio != "" && tel != "" && brand != ""){
    //MASTER
    if( !query->prepare(
    QString("select idmaster from master where fio_master = ?") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(master);
    }

    bool result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful insert";
    }
    else
        qDebug() << "Insert failed";
    query->next();
    QString idmaster = query->value(0).toString();


//    qDebug() <<"HERE 1";
//    query->prepare(QString("select * from make_order_client(?, ?)"));
//    qDebug() <<"HERE 2";
//    query->addBindValue(fio);
//    query->addBindValue(tel);
//    qSQLDbHelper_order->executeInsert(query);
//    qDebug() <<"HERE 3";
//    qDebug() <<"Error = " << mw->db->lastError().text();

    //CLIENT
    if( !query->prepare(
    QString("select * from make_order_client(?, ?)") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(fio);
        query->addBindValue(tel);
    }

    result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful insert";
    }
    else
        qDebug() << "Insert failed";
    query->next();
    QString idclient = query->value(0).toString();



    //TECHNIC
    if( !query->prepare(
    QString("select * from make_order_technic(?, ?, ?, ?)") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(type);
        query->addBindValue(model);
        query->addBindValue(brand);
        query->addBindValue(imei);
    }

    result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful insert";
    }
    else
        qDebug() << "Insert failed";
    query->next();
    QString idtechnic = query->value(0).toString();

    //ISSUE
    if( !query->prepare(
    QString("select * from make_order_issue(?, ?)") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(issue);
        query->addBindValue(std::stod(idtechnic.toStdString()));
    }

    result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful insert";
    }
    else
        qDebug() << "Insert failed";

    //ORDERS
    if( !query->prepare(
    QString("select * from make_order_orders(?, ?, ?, ?, ?)") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue((std::stod(idtechnic.toStdString())));
        query->addBindValue((std::stod(idclient.toStdString())));
        query->addBindValue(condition);
        query->addBindValue((std::stod(idmaster.toStdString())));
        query->addBindValue(master_price);
    }

    result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful insert";
    }
    else
        qDebug() << "Insert failed";
    query->next();
    idorder = query->value(0).toString();

    //COMPONENT
    if( !query->prepare(
    QString("select * from make_order_component(?, ?, ?)") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(component);
        query->addBindValue(std::stod(idtechnic.toStdString()));
        query->addBindValue(component_price);
    }

    result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful insert";
    }
    else
        qDebug() << "Insert failed";


    this->close();
    delete query;

    mw->que_model->setQuery("select * from main_view;");
    qDebug() << "Order create\n";
    }else {
        QMessageBox replyy;
        replyy .setText("Заполните все поля!");
        replyy.exec();

        this->close();
        delete query;

        mw->que_model->setQuery("select * from main_view;");

    }
}



void OrderDialog::on_rb_diagnostic_clicked()
{
    ui->line_condition->setText("Diagnostic");
}

void OrderDialog::on_rb_component_clicked()
{
    ui->line_condition->setText("Waiting component");
}

void OrderDialog::on_rb_ready_clicked()
{
    ui->line_condition->setText("Ready");
}

void OrderDialog::on_line_client_fio_currentTextChanged(const QString &arg1)
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));

    if( !query_edit->prepare(
    QString("select tel_num from public.client where fio_client  = ?;")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(arg1);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    query_edit->first();
    qDebug() << query_edit->first();
    ui->line_tel->setText(query_edit->value(0).toString());

    delete query_edit;
}
