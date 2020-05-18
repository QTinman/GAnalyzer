#ifndef CALWINDOW_H
#define CALWINDOW_H

#include <QDialog>


namespace Ui {
class CalWindow;
}



class CalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalWindow(QWidget *parent = nullptr);
    ~CalWindow();

signals:
    void buttonpressed();

private slots:
    void on_date_clicked();

    void on_seconddate_clicked();

private:
    Ui::CalWindow *ui;

};

#endif // CALWINDOW_H
