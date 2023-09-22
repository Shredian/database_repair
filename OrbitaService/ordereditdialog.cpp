#include "ordereditdialog.h"
#include "ui_ordereditdialog.h"
#include "mainwindow.h"

OrderEditDialog::OrderEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderEditDialog)
{
    ui->setupUi(this);
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;
    idorder = mw->currentRow;


    if (mw->db->open()){
        QSqlQuery* query = new QSqlQuery(*(mw->db));
        query->setForwardOnly(true);

        if( !query->prepare(QString("select * from main_view where idorders = ?")))
        {
            qDebug() << "Error = " << mw->db->lastError().text();
        }
        else
        {
            query->addBindValue(mw->currentRow);
        }

        bool result = qSQLDbHelper_order->executeDelete(query);
        if( result )
            qDebug() << "Successful edited";
        else
            qDebug() << "Edit failed";
        query->next();

        ui->line_imei->setText(query->value(6).toString());
        ui->line_component_price->setText(query->value(11).toString());
        ui->line_condition->setText(query->value(8).toString());
        ui->line_master_price->setText(query->value(10).toString());
        ui->line_model->setText(query->value(5).toString());

        if (ui->line_condition->text() == "Diagnostic")
            ui->rb_diagnostic->setChecked(true);
        else if (ui->line_condition->text() == "Ready")
            ui->rb_ready->setChecked(true);
        else
            ui->rb_component->setChecked(true);
        ui->line_condition->setDisabled(true);


        //Client Fio
        QSqlQueryModel *que_model_client_fio = new QSqlQueryModel;
        que_model_client_fio->setQuery("select fio_client from client;");
        ui->line_client_fio->setModel(que_model_client_fio);
        ui->line_client_fio->clearEditText();
        ui->line_client_fio->setCurrentText(query->value(2).toString());
        QString client_fio = query->value(2).toString();


        // Type Technic
        QSqlQueryModel *que_model_type_technic = new QSqlQueryModel;
        que_model_type_technic->setQuery("select distinct type_technic from technic order by type_technic DESC;");
        ui->line_type->setModel(que_model_type_technic);
        ui->line_type->setCurrentText(query->value(3).toString());


        // Brand Technic brand
        QSqlQueryModel *que_model_brand_technic = new QSqlQueryModel;
        que_model_brand_technic->setQuery("select distinct brand from technic order by brand DESC;");
        ui->line_brand->setModel(que_model_brand_technic);
//            ui->line_brand->clearEditText();
//            ui->line_brand->currentText();
        ui->line_brand->setCurrentText(query->value(4).toString());


        // Master fio master_fio
        QSqlQueryModel *que_model_master_fio = new QSqlQueryModel;
        que_model_master_fio->setQuery("select fio_master from master;");
        ui->line_master->setModel(que_model_master_fio);
        ui->line_master->setCurrentText(query->value(12).toString());

        // Remind data
        int idorder = std::stod(query->value(0).toString().toStdString());


        //ISSUE
        if( !query->prepare(
        QString("select description_issue from issue where idissue in"
                "(select issue_idissue from technic_issue where technic_idtechnic in "
                                        "(select technicid from orders where idorders = ?));")))
        {
            qDebug() <<"Error = " << mw->db->lastError().text();
        }
        else
        {
            query->addBindValue(idorder);
        }

        result = qSQLDbHelper_order->executeInsert(query);
        if( result ){
            qDebug() << "Successful get data for edit";
        }
        else
            qDebug() << "Get data for edit failed";
        query->next();
        ui->line_issue->setText(query->value(0).toString());

        //TEL_NUM
        if( !query->prepare(
        QString("select tel_num from client where fio_client = ?;")))
        {
            qDebug() <<"Error = " << mw->db->lastError().text();
        }
        else
        {
            query->addBindValue(client_fio);
        }

        result = qSQLDbHelper_order->executeInsert(query);
        if( result ){
            qDebug() << "Successful get data for edit";
        }
        else
            qDebug() << "Get data for edit failed";
        query->next();
        ui->line_tel->setText(query->value(0).toString());

        //COMPONENT
        if( !query->prepare(
        QString("select description_component from component where idcomponent in"
                "(select component_idcomponent from technic_component where technic_idtechnic in "
                                        "(select technicid from orders where idorders = ?));")))
        {
            qDebug() <<"Error = " << mw->db->lastError().text();
        }
        else
        {
            query->addBindValue(idorder);
        }

        result = qSQLDbHelper_order->executeInsert(query);
        if( result ){
            qDebug() << "Successful get data for edit";
        }
        else
            qDebug() << "Get data for edit failed";
        query->next();
        ui->line_component->setText(query->value(0).toString());


        delete query;
        mw->currentRow = 0;
        qDebug() <<"END CONSTRUCT DIALOG OF EDIT ORDER\n";
    }
}

OrderEditDialog::~OrderEditDialog()
{
    delete ui;
}

void OrderEditDialog::on_line_tel_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_tel->text();

    QSqlQuery* query = new QSqlQuery(*(mw->db));
    query->setForwardOnly(true);

    if( !query->prepare(
    QString("UPDATE public.client"
            "   SET tel_num = ?"
            "   WHERE idclient in (select clientid from orders where idorders = ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(curr_text);
        query->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_model_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_model->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.technic"
            "   SET model = ?"
            "   WHERE idtechnic in (select technicid from orders where idorders = ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_imei_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_imei->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.technic"
            "   SET imei = ?"
            "   WHERE idtechnic in (select technicid from orders where idorders = ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
        qDebug() <<"HERE 1\n ";
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    qDebug() <<"HERE 2 = " << result << "\n";
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_issue_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_issue->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("select * from edit_order_issue(?, ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_component_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_component->text();
    QString price = ui->line_component_price->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("select * from edit_order_component(?, ?, ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
        query_edit->addBindValue(price);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_component_price_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_component->text();
    QString price = ui->line_component_price->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("select * from edit_order_component(?, ?, ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
        query_edit->addBindValue(price);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_master_price_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_master_price->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.master_orders"
            "   SET master_price = ?"
            "   WHERE orders_idorders = ?;")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_condition_editingFinished()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QString curr_text = ui->line_condition->text();

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.orders"
            "   SET condition_order = ?"
            "   WHERE idorders = ?;")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(curr_text);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}




void OrderEditDialog::on_pb_accept_clicked()
{
    this->close();
    qDebug() << "Editing end\n";
}

void OrderEditDialog::on_rb_diagnostic_clicked()
{
    ui->line_condition->setText("Diagnostic");
}

void OrderEditDialog::on_rb_component_clicked()
{
    ui->line_condition->setText("Waiting component");
}

void OrderEditDialog::on_rb_ready_clicked()
{
    ui->line_condition->setText("Ready");
}

void OrderEditDialog::on_line_condition_textChanged(const QString &arg1)
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.orders"
            "   SET condition_order = ?"
            "   WHERE idorders = ?;")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(arg1);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful update data for edit";
    }
    else
        qDebug() << "Update data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_brand_currentTextChanged(const QString &arg1)
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.technic"
            "   SET brand = ?"
            "   WHERE idtechnic in (select technicid from orders where idorders = ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(arg1);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful update data for edit";
    }
    else
        qDebug() << "Update data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_type_currentTextChanged(const QString &arg1)
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    if( !query_edit->prepare(
    QString("UPDATE public.technic"
            "   SET type_technic = ?"
            "   WHERE idtechnic in (select technicid from orders where idorders = ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(arg1);
        query_edit->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query_edit);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    delete query_edit;
    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_client_fio_currentTextChanged(const QString &arg1)
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;


    QSqlQuery* query = new QSqlQuery(*(mw->db));
    query->setForwardOnly(true);

    if( !query->prepare(
    QString("UPDATE public.client"
            "   SET fio_client = ?"
            "   WHERE idclient in (select clientid from orders where idorders = ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(arg1);
        query->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";


    if( !query->prepare(
    QString("select tel_num from client where fio_client = ?;")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(arg1);
    }

    result = qSQLDbHelper_order->executeInsert(query);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    query->next();
    ui->line_tel->setText(query->value(0).toString());

    mw->que_model->setQuery("select * from main_view;");
}

void OrderEditDialog::on_line_master_currentTextChanged(const QString &arg1)
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;


    QSqlQuery* query = new QSqlQuery(*(mw->db));
    query->setForwardOnly(true);

    if( !query->prepare(
    QString("UPDATE public.master_orders"
            "   SET master_idmaster = (select idmaster from master where fio_master = ?)"
            "   WHERE orders_idorders = ?;")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query->addBindValue(arg1);
        query->addBindValue(idorder);
    }

    bool result = qSQLDbHelper_order->executeUpdate(query);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    mw->que_model->setQuery("select * from main_view;");
}


