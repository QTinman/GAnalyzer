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

private:
    Ui::headDialog *ui;
    QStringListModel *model;
};

#endif // HEADDIALOG_H
