#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "mainwindow.h"


AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    MainWindow * mw = MainWindow::getMainWinPtr();
    qSQLDbHelper_order = mw->qSQLDbHelper;
    idorder = mw->currentRow;

    if (mw->db->open()){
        QSqlQuery* query = new QSqlQuery(*(mw->db));
        query->setForwardOnly(true);
        if( !query->prepare(
        QString("select * from rent where idrent = ?") ))
        {
            qDebug() <<"Error = " << mw->db->lastError().text();
        }
        else
        {
            query->addBindValue(2);
        }

        bool result = qSQLDbHelper_order->executeInsert(query);
        if( result ){
            qDebug() << "Successful get rent";
        }
        else
            qDebug() << "rent get failed";
        query->next();
        QString prem = query->value(1).toString();
        QString util = query->value(2).toString();
        QString advert = query->value(3).toString();
        ui->l_advertisement->setText(advert);
        ui->l_premises->setText(prem);
        ui->l_rent->setText(util);

        QSqlQuery* query_2 = new QSqlQuery(*(mw->db));
        query_2->setForwardOnly(true);

        if( !query_2->prepare(
        QString("select * from service where idservice = ?") ))
        {
            qDebug() <<"Error = " << mw->db->lastError().text();
        }
        else
        {
            query_2->addBindValue(2);
        }

        result = qSQLDbHelper_order->executeInsert(query_2);
        if( result ){
            qDebug() << "Successful get rent";
        }
        else
            qDebug() << "rent get failed";
        query_2->next();
        ui->l_address->setText(query_2->value(1).toString());

    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
