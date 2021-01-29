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

    void on_englisordinal_toggled(bool checked);

    void on_fullreduction_toggled(bool checked);

    void on_reverseordinal_toggled(bool checked);

    void on_reversefull_toggled(bool checked);

    void on_Adjust_clicked();

    void on_SaveNewsFilter_clicked();

private:
    Ui::headDialog *ui;
    QStringListModel *model;
    QStringList List;
};

#endif // HEADDIALOG_H
