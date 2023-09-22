#ifndef CASHDIALOG_H
#define CASHDIALOG_H

#include <QDialog>
#include "qsqldbhelper.h"

namespace Ui {
class CashDialog;
}

class CashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CashDialog(QWidget *parent = nullptr);
    ~CashDialog();

private slots:
    void on_pb_cash_clicked();

private:
    Ui::CashDialog *ui;
    QSQLDbHelper* qSQLDbHelper_order;
    int idorder;
};

#endif // CASHDIALOG_H
