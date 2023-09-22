#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include "qsqldbhelper.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_rb_tel_clicked();

    void on_rb_client_clicked();

    void on_rb_brand_clicked();

    void on_rb_imei_clicked();

    void on_rb_master_clicked();

    void on_pb_accept_clicked();

private:
    Ui::SearchDialog *ui;
    QSQLDbHelper* qSQLDbHelper_order;
    int idorder;
};

#endif // SEARCHDIALOG_H
