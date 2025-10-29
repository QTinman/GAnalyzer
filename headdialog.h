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
    explicit headDialog(const QString &filename, const QString &label, QWidget *parent = nullptr);
    ~headDialog();

private slots:
    void on_lineEdit_returnPressed();

    void on_englisordinal_toggled(bool checked);

    void on_fullreduction_toggled(bool checked);

    void on_reverseordinal_toggled(bool checked);

    void on_reversefull_toggled(bool checked);

private:
    Ui::headDialog *ui;
    QStringListModel *model;
    QStringList List;
    QString _filename;
    QString _label;
};

#endif // HEADDIALOG_H
