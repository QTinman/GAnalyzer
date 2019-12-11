#ifndef RANKDIALOG_H
#define RANKDIALOG_H

#include <QDialog>

namespace Ui {
class rankDialog;
}

class rankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit rankDialog(QWidget *parent = nullptr);
    ~rankDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::rankDialog *ui;
};

#endif // RANKDIALOG_H
