#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QString>
#include <QMessageBox>

#include "qsqldbhelper.h"
#include "orderdialog.h"
#include "ordereditdialog.h"
#include "cashdialog.h"
#include "searchdialog.h"
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow * getMainWinPtr();
    QSqlDatabase *db;
    QSqlQueryModel *que_model;
    QSQLDbHelper* qSQLDbHelper;
    int currentRow = 0;
    Ui::MainWindow *ui;

private slots:

    void on_pb_view_clicked();

    void on_order_action_triggered();

    void on_tableView_activated(const QModelIndex &index);

    void on_pb_diagnostic_clicked();

    void on_pb_component_clicked();

    void on_pb_all_clicked();


    void on_cash_action_triggered();

    void on_search_action_triggered();

    void on_about_action_triggered();

private:
    QSqlTableModel *model;

    QSqlQuery* query;
    OrderDialog *order;
    OrderEditDialog *order_edit;
    CashDialog *cash;
    SearchDialog *search;
    AboutDialog *about;
    static MainWindow * pMainWindow;

protected:

};
#endif // MAINWINDOW_H
