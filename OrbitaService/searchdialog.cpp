#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "mainwindow.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;
    idorder = mw->currentRow;

    if (mw->db->open()){
        QSqlQuery* query = new QSqlQuery(*(mw->db));
        query->setForwardOnly(true);

        QSqlQueryModel *que_model_master_fio = new QSqlQueryModel;
        que_model_master_fio->setQuery("select fio_master from master;");
        ui->line_master->setModel(que_model_master_fio);
        QSqlQueryModel *que_model_client_fio = new QSqlQueryModel;
        que_model_client_fio->setQuery("select fio_client from client;");
        ui->line_client->setModel(que_model_client_fio);
        ui->line_client->clearEditText();
        QSqlQueryModel *que_model_brand_technic = new QSqlQueryModel;
        que_model_brand_technic->setQuery("select distinct brand from technic order by brand DESC;");
        ui->line_brand->setModel(que_model_brand_technic);
        ui->line_brand->setDisabled(true);
        ui->line_client->setDisabled(true);
        ui->line_imei->setDisabled(true);
        ui->line_master->setDisabled(true);
        ui->line_tel->setDisabled(true);
        delete query;
    }

}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_rb_tel_clicked()
{
    ui->line_brand->setDisabled(true);
    ui->line_client->setDisabled(true);
    ui->line_imei->setDisabled(true);
    ui->line_master->setDisabled(true);
    ui->line_tel->setDisabled(false);
}

void SearchDialog::on_rb_client_clicked()
{
    ui->line_brand->setDisabled(true);
    ui->line_client->setDisabled(false);
    ui->line_imei->setDisabled(true);
    ui->line_master->setDisabled(true);
    ui->line_tel->setDisabled(true);
}

void SearchDialog::on_rb_brand_clicked()
{
    ui->line_brand->setDisabled(false);
    ui->line_client->setDisabled(true);
    ui->line_imei->setDisabled(true);
    ui->line_master->setDisabled(true);
    ui->line_tel->setDisabled(true);
}

void SearchDialog::on_rb_imei_clicked()
{
    ui->line_brand->setDisabled(true);
    ui->line_client->setDisabled(true);
    ui->line_imei->setDisabled(false);
    ui->line_master->setDisabled(true);
    ui->line_tel->setDisabled(true);
}

void SearchDialog::on_rb_master_clicked()
{
    ui->line_brand->setDisabled(true);
    ui->line_client->setDisabled(true);
    ui->line_imei->setDisabled(true);
    ui->line_master->setDisabled(false);
    ui->line_tel->setDisabled(true);
}

void SearchDialog::on_pb_accept_clicked()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;
    QSqlQuery* query = new QSqlQuery(*(mw->db));
    query->setForwardOnly(true);

    if(ui->rb_master->isChecked()){
        QString result_s = "select * from main_view where fio_master = '" + ui->line_master->currentText() + "';";
        qDebug() << result_s;
        mw->que_model->setQuery(result_s);
    }else if (ui->rb_brand->isChecked()){
        QString result_s = "select * from main_view where brand = '" + ui->line_brand->currentText() + "';";
        qDebug() << result_s;
        mw->que_model->setQuery(result_s);
    }else if (ui->rb_client->isChecked()){
        QString result_s = "select * from main_view where fio_client = '" + ui->line_client->currentText() + "';";
        qDebug() << result_s;
        mw->que_model->setQuery(result_s);
    }else if (ui->rb_imei->isChecked()){
        QString result_s = "select * from main_view where imei = '" + ui->line_imei->text() + "';";
        qDebug() << result_s;
        mw->que_model->setQuery(result_s);
    }else if (ui->rb_tel->isChecked()){
        QString result_s = "select * from main_view where fio_client = (select fio_client from public.client where tel_num  =  '"
                + ui->line_tel->text() + "');";
        qDebug() << result_s;
        mw->que_model->setQuery(result_s);
    }else{
        QMessageBox replyy;
        replyy .setText("Nothing selected!");
        replyy.exec();
    }
    this->accept();
}
