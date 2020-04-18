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

private:
    Ui::CalWindow *ui;

};

#endif // CALWINDOW_H
