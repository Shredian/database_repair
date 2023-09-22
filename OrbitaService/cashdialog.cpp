#include "cashdialog.h"
#include "ui_cashdialog.h"
#include "mainwindow.h"

CashDialog::CashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CashDialog)
{
    ui->setupUi(this);
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;
    idorder = mw->currentRow;


    if (mw->db->open()){
        QSqlQueryModel *que_model_master_fio = new QSqlQueryModel;
        que_model_master_fio->setQuery("select fio_master from master;");
        ui->line_master->setModel(que_model_master_fio);
        ui->dateEdit_1->setDate(QDate(2022, 1, 1));
        ui->dateEdit_2->setDate(QDate(2022, 12, 12));
        qDebug() << ui->dateEdit_1->date().toString("yyyy-MM-dd");
    }

}

CashDialog::~CashDialog()
{
    delete ui;
}

void CashDialog::on_pb_cash_clicked()
{
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;

    QSqlQuery* query_edit = new QSqlQuery(*(mw->db));
    query_edit->setForwardOnly(true);

    QString date_1 = ui->dateEdit_1->date().toString("yyyy-MM-dd");
    QString date_2 = ui->dateEdit_2->date().toString("yyyy-MM-dd");
    qDebug() << date_1 << date_2;
    QString master = ui->line_master->currentText();
    if( !query_edit->prepare(
    QString("select idmaster from master where fio_master = ?") ))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit->addBindValue(master);
    }

    bool result = qSQLDbHelper_order->executeInsert(query_edit);
    if( result ){
        qDebug() << "Successful get master";
    }
    else
        qDebug() << "Master get failed";
    query_edit->next();
    QString idmaster = query_edit->value(0).toString();
    qDebug() << idmaster;
    delete query_edit;

    QSqlQuery* query_edit_2 = new QSqlQuery(*(mw->db));
    query_edit_2->setForwardOnly(true);

    if( !query_edit_2->prepare(
    QString("select * from cash_out_master (?, ?, ?);")))
    {
        qDebug() <<"Error = " << mw->db->lastError().text();
    }
    else
    {
        query_edit_2->addBindValue(idmaster);
        query_edit_2->addBindValue(date_1);
        query_edit_2->addBindValue(date_2);
    }

    result = qSQLDbHelper_order->executeUpdate(query_edit_2);
    if( result ){
        qDebug() << "Successful get data for edit";
    }
    else
        qDebug() << "Get data for edit failed";
    query_edit_2->next();
    qDebug() << "HERE 2";

    QString cash_out = query_edit_2->value(0).toString();
    qDebug() << "HERE 1";

    qDebug() << cash_out;

    QMessageBox replyy;
    replyy .setText(cash_out);
    replyy.exec();

    delete query_edit_2;

}
