#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
#include "qsqldbhelper.h"

namespace Ui {
class OrderDialog;
}

class OrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDialog(QWidget *parent = nullptr);
    ~OrderDialog();

private slots:
    void on_pb_accept_clicked();

    void on_rb_diagnostic_clicked();

    void on_rb_component_clicked();

    void on_rb_ready_clicked();

    void on_line_client_fio_currentTextChanged(const QString &arg1);

private:
    Ui::OrderDialog *ui;
    QSQLDbHelper* qSQLDbHelper_order;
    QString idorder;

};

#endif // ORDERDIALOG_H
