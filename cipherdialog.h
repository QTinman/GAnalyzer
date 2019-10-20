#ifndef CIPHERDIALOG_H
#define CIPHERDIALOG_H

#include <QDialog>

namespace Ui {
class cipherDialog;
}

class cipherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cipherDialog(QWidget *parent = nullptr);
    ~cipherDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::cipherDialog *ui;
};

#endif // CIPHERDIALOG_H
