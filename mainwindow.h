#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include "gcalc.h"
#include "calwindow.h"
#include "httpdownload.h"
#include "headdialog.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QEventLoop>

extern QString phrase;
extern QString labeltext,tmpstring;
extern int year,dd,mm,ns,d2,m2,y2,filter,hmempos;
extern bool single_r_on,francis_on,satanic_on,jewish_on,sumerian_on,rev_sumerian_on;
extern vector<int> primes;
extern QString filesource;
extern int zerodays[8][250],linenumbers;
extern QString hmem[10];
extern bool nightmode;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//class CalWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  //  QEventLoop loop;

public slots:
  void handleAnchorClicked(const QUrl &url);
  void handleSourceChanged(const QUrl &url);

private slots:
    void on_actionDate_Search_triggered();

    void on_actionNew_Phrase_triggered();

    void on_actionNew_Date_triggered();

    void on_actionSearch_History_txt_triggered();

    void on_actionSet_Year_triggered();

    void on_action_Analyze_triggered();

    void on_action_Eu_date_toggled(bool arg1);

    void on_actionDate_Details_triggered();

    void on_lineEdit_returnPressed();

    void on_action_Word_details_triggered();

    void on_actionList_Ciphers_triggered();

    void on_actionList_Primenumbers_triggered();

    void on_actionList_Triangular_numbers_triggered();

    void on_actionCompare_phrase_to_history_triggered();

    void on_action_Second_date_triggered();

    void shorthelp();

    void welcome();

    void on_actionC_ompare_date_to_History_txt_triggered();

    void on_actionHelp_triggered();

    void on_actionSolar_Eclipses_triggered();

    void on_actionCompare_SolarE_to_history_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionAbout_triggered();

    void on_action_Add_remove_ciphers_triggered();

    void on_actionList_Solar_Eclipses_triggered();

    void keymem(QString memstr);

    void on_actionPhrase_ranking_triggered();

    void on_action_Font_triggered();

    void on_action_Print_triggered();
    void doPrint(QPrinter * printer);

    void on_actionList_History_triggered();

    void on_actionCalendar_triggered();

    void writetmpfile(QString html);
    void SieveOfEratosthenes(vector<int> &primes);
    void updatestatusbar();
    void show_news(const QString& source);

    void on_action_News_headlines_exp_triggered();


    void on_actionNightmode_triggered(bool checked);

    void on_actionSwap_dates_triggered();

    void on_actionSave_as_triggered();
     void savelog(QString line, QString filename);

     void on_actionLine_numbers_in_view_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
    bool eudate=true;
    CalWindow *Calwindow;
    HttpDownload *Httpdownload;
    headDialog *headdialog;
    void calc(QString calcstr);



protected:
    //void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
};
//#endif // MAINWINDOW_H

namespace Ui {class inputDialog;}

class inputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputDialog(QWidget *parent = nullptr);
    ~inputDialog();
    bool eudate;




private slots:
    void displaydialog();
    void on_lineEdit_returnPressed();
    void on_buttonBox_accepted();

private:
    Ui::inputDialog *ui;

};


namespace Ui {class selectDialog;}

class selectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit selectDialog(QWidget *parent = nullptr);
    ~selectDialog();



private slots:

    void displaydialog();
    void on_pushButton_clicked();

private:
    Ui::selectDialog *ui;

};

#endif // MAINWINDOW_H
