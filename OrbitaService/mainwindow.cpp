#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow * MainWindow::pMainWindow = nullptr;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMainWindow = this;
    const char* driverName = "QPSQL";
    qSQLDbHelper = new QSQLDbHelper(driverName);
    db = qSQLDbHelper->connect("localhost", "repairdb", "postgres", "mnbvcxz");


    if (db->open()){
        ui->statusbar->showMessage("Open");
        query = new QSqlQuery(*db);
        query->setForwardOnly(true);

        model = new QSqlTableModel(this, *db);
        que_model = new QSqlQueryModel;

        QString result = "select * from main_view;";
        que_model->setQuery(result);
        ui->tableView->setModel(que_model);
//      ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSortingEnabled(true);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//        ui->tableView->resizeColumnsToContents();
        que_model->setHeaderData(0, Qt::Horizontal, tr("Order id"));
        que_model->setHeaderData(1, Qt::Horizontal, tr("Date receipt"));
        que_model->setHeaderData(2, Qt::Horizontal, tr("Client"));
        que_model->setHeaderData(3, Qt::Horizontal, tr("Type"));
        que_model->setHeaderData(4, Qt::Horizontal, tr("Brand"));
        que_model->setHeaderData(5, Qt::Horizontal, tr("Model"));
        que_model->setHeaderData(6, Qt::Horizontal, tr("IMEI/S"));
        que_model->setHeaderData(7, Qt::Horizontal, tr("Issue"));
        que_model->setHeaderData(8, Qt::Horizontal, tr("Condition"));
        que_model->setHeaderData(9, Qt::Horizontal, tr("Total price"));
        que_model->setHeaderData(10, Qt::Horizontal, tr("Master price"));
        que_model->setHeaderData(11, Qt::Horizontal, tr("Component price"));
        que_model->setHeaderData(12, Qt::Horizontal, tr("Master"));
//        ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    } else{
        ui->statusbar->showMessage("Error" + db->lastError().databaseText());
    }
}

MainWindow::~MainWindow()
{
    delete que_model;
    db->close();
    delete ui;
}

MainWindow *MainWindow::getMainWinPtr()
{
    return pMainWindow;
}

void MainWindow::on_pb_view_clicked()
{
    QString result = "select * from ready_view;";
    que_model->setQuery(result);
}

void MainWindow::on_pb_diagnostic_clicked()
{
    QString result = "select * from diagnostic_view;";
    que_model->setQuery(result);
}

void MainWindow::on_pb_component_clicked()
{
    QString result = "select * from component_view;";
    que_model->setQuery(result);
}

void MainWindow::on_pb_all_clicked()
{
    QString result = "select * from main_view;";
    que_model->setQuery(result);
}



void MainWindow::on_order_action_triggered()
{
    order = new OrderDialog(this);
    order->show();
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString val = ui->tableView->model()->data(index).toString();
    currentRow = std::stod(ui->tableView->model()->data(index).toString().toStdString());
    qDebug() << val;

    if (ui->del_action->isChecked()){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting order",
                                                                 "Do you want to delete the order with id = "+val,
                              QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes){
            if( !query->prepare(QString("select * from delete_order(?)")) )
            {
                qDebug() << "Error = " << db->lastError().text();
            }
            else
            {
                query->addBindValue(std::stod(val.toStdString()));
            }

            bool result = qSQLDbHelper->executeDelete(query);
            if( result )
                qDebug() << "Successful delete";
            else
                qDebug() << "Delete failed";
            que_model->setQuery("select * from main_view;");
        }

    }else if (ui->edit_action->isChecked()){
        qDebug() << currentRow;
        order_edit = new OrderEditDialog(this);
        order_edit->show();
        qDebug() << "Dialog showed from mainwindow";
    }
}


void MainWindow::on_cash_action_triggered()
{
    cash = new CashDialog(this);
    cash->show();
    qDebug() << "CashDialog showed from mainwindow";

}


void MainWindow::on_search_action_triggered()
{
    search = new SearchDialog(this);
    search->show();
    qDebug() << "SearchDialog showed from mainwindow";
}

void MainWindow::on_about_action_triggered()
{
    about = new AboutDialog(this);
    about->show();
    qDebug() << "AboutDialog showed from mainwindow";

}
