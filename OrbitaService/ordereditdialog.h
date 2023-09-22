#ifndef ORDEREDITDIALOG_H
#define ORDEREDITDIALOG_H

#include <QDialog>
#include "qsqldbhelper.h"

namespace Ui {
class OrderEditDialog;
}

class OrderEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderEditDialog(QWidget *parent = nullptr);
    ~OrderEditDialog();

private slots:
    void on_line_tel_editingFinished();

    void on_line_model_editingFinished();

    void on_line_imei_editingFinished();

    void on_line_issue_editingFinished();

    void on_line_component_editingFinished();

    void on_line_component_price_editingFinished();

    void on_line_master_price_editingFinished();

    void on_line_condition_editingFinished();

    void on_pb_accept_clicked();

    void on_rb_diagnostic_clicked();

    void on_rb_component_clicked();

    void on_rb_ready_clicked();

    void on_line_condition_textChanged(const QString &arg1);

    void on_line_brand_currentTextChanged(const QString &arg1);

    void on_line_type_currentTextChanged(const QString &arg1);

    void on_line_client_fio_currentTextChanged(const QString &arg1);

    void on_line_master_currentTextChanged(const QString &arg1);

private:
    Ui::OrderEditDialog *ui;
    QSQLDbHelper* qSQLDbHelper_order;
    int idorder;
};

#endif // ORDEREDITDIALOG_H
