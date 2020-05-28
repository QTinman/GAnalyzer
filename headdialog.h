#ifndef HEADDIALOG_H
#define HEADDIALOG_H

#include <QDialog>
#include <QStringListModel>
//#include "mainwindow.h"


namespace Ui {
class headDialog;
}

class headDialog : public QDialog
{
    Q_OBJECT

public:
    explicit headDialog(QWidget *parent = nullptr);
    ~headDialog();

private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::headDialog *ui;
    QStringListModel *model;
    QStringList List;
};

#endif // HEADDIALOG_H
