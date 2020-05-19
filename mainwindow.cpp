#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_inputdialog.h"
#include "ui_selectdialog.h"
#include "ui_rankdialog.h"
#include "cipherdialog.h"
#include "rankdialog.h"
#include "calwindow.h"
#include "httpdownload.h"
#include "headdialog.h"
#include <QtCore>
#include <QDate>
#include "string.h"
#include <QKeyEvent>
#include <QUrl>
#include <QtNetwork>

#include <QLabel>
#include <QFont>
#include <QFontDialog>
#include <QDateTime>
#include "gcalc.h"
#include <QLocale>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStringList>
//#include "textprogressbar.h"
//#include "downloadmanager.h"

using namespace std;
#define MAX_SIZE 1000005
int zerodays[8][250];
QString hmem[10];
vector<int> primes;
QString phrase = "<none>";
QString pwd = QDir::currentPath() + "/tmp.htm";
QFile *file = new QFile(pwd);

/*QFile output;
QNetworkAccessManager manager;
QNetworkReply *currentDownload = nullptr;
int downloadedCount = 0;
TextProgressBar progressBar;
QElapsedTimer downloadTimer;*/
QString filesource;
QStringList List;
QString labeltext,tmpstring;
int year,dd,mm,ns,d2,m2,y2,filter,hmempos = -1;
bool single_r_on=false,francis_on=false,satanic_on=false,jewish_on=false,sumerian_on=false,rev_sumerian_on=false;

void MainWindow::SieveOfEratosthenes(vector<int> &primes)
{
    // Create a boolean array "IsPrime[0..MAX_SIZE]" and
    // initialize all entries it as true. A value in
    // IsPrime[i] will finally be false if i is
    // Not a IsPrime, else true.
    bool IsPrime[MAX_SIZE];
    memset(IsPrime, true, sizeof(IsPrime));

    for (int p = 2; p * p < MAX_SIZE; p++)
    {
        // If IsPrime[p] is not changed, then it is a prime
        if (IsPrime[p] == true)
        {
            // Update all multiples of p greater than or
            // equal to the square of it
            // numbers which are multiple of p and are
            // less than p^2 are already been marked.
            for (int i = p * p; i <  MAX_SIZE; i += p)
                IsPrime[i] = false;
        }
    }

    // Store all prime numbers
    for (int p = 2; p < MAX_SIZE; p++)
       if (IsPrime[p])
            primes.push_back(p);

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDate cd = QDate::currentDate();
    year = cd.year();
    dd = cd.day();
    mm = cd.month();
    QString scomstr = "Active phrase : " +phrase+ " - Current date : " + QString::number(dd) + "/" + QString::number(mm) + "/" + QString::number(year);
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->statusbar->installEventFilter(this);
    //ui->textBrowser->installEventFilter(this);
    //ui->textBrowser->setContextMenuPolicy(Qt::NoContextMenu);
    connect(ui->textBrowser, &QTextBrowser::anchorClicked,
            this, &MainWindow::handleAnchorClicked);

    //connect(&CalWindow::buttonpressed, SIGNAL(CalWindow::buttonpressed),
     //       this, updatestatusbar());

    //connect(this,&CalWindow::on_date_clicked(),this,&MainWindow::updatestatusbar());


    //Calwindow = new CalWindow;
    //connect(Calwindow,SIGNAL(buttonpressed()),this,SLOT(updatestatusbar()));


    //connect(ui->textBrowser, &QTextBrowser::sourceChanged,
    //        this, &MainWindow::handleSourceChanged);
    //ui->textBrowser->setTextInteractionFlags(Qt::TextSelectableByMouse);
    //mousePressEvent(QMouseEvent())
    if (eudate) ui->action_Eu_date->setText("Date DMY");
    else ui->action_Eu_date->setText("Date MDY");
    SieveOfEratosthenes(primes);
    QString font = readSettings("settings.txt","font");
    QString DMY = readSettings("settings.txt","DMY");
    QString ciphers = readSettings("settings.txt","ciphers");
    if (DMY == "false") {
        eudate = false;
        ui->action_Eu_date->setText("Date MDY");
        ui->action_Eu_date->setChecked(false);
    }
    if (font != "none") {
        QFont f1;
        f1.fromString(font);
        ui->textBrowser->setFont(f1);
    }
    if (ciphers.mid(0,1) == "1") single_r_on = true;
    if (ciphers.mid(1,1) == "1") francis_on = true;
    if (ciphers.mid(2,1) == "1") satanic_on = true;
    if (ciphers.mid(3,1) == "1") jewish_on = true;
    if (ciphers.mid(4,1) == "1") sumerian_on = true;
    if (ciphers.mid(5,1) == "1") rev_sumerian_on = true;

    setCentralWidget(ui->groupBox);
    //setCentralWidget(ui->centralwidget);
    ui->lineEdit->focusWidget();
    ui->statusbar->showMessage(scomstr);
    if ( !file->open(QIODevice::ReadWrite) )
    {
        qDebug() << "tmp.htm not open";
    } else file->close();


    ui->textBrowser->setSource("file:///"+pwd);
    std::setlocale(LC_ALL,"");

            //int curr_locale = QLocale().language();
            //qDebug() << " String ="<< curr_locale << endl;
    emit welcome();

    //qDebug() << MainWindow::width() << " " << MainWindow::x() << " " << ui->textBrowser->width();
}

MainWindow::~MainWindow()
{
    QString ciphers;
    if (single_r_on) ciphers = "1";
    else ciphers = "0";
    if (francis_on) ciphers += "1";
    else ciphers += "0";
    if (satanic_on) ciphers += "1";
    else ciphers += "0";
    if (jewish_on) ciphers += "1";
    else ciphers += "0";
    if (sumerian_on) ciphers += "1";
    else ciphers += "0";
    if (rev_sumerian_on) ciphers += "1";
    else ciphers += "0";
    //qDebug() << ciphers;
    char filename[13] = "settings.txt";
    writeSettings(filename,"ciphers",ciphers.toUtf8().constData());
    //qDebug()  << MainWindow::width() << " " << MainWindow::x() << " " << ui->textBrowser->width();
    //QString pwd = QDir::currentPath() + "/tmp.htm";
    //QFile *File = new QFile(pwd);
    file->remove();
    delete ui;
}


bool MainWindow::eventFilter(QObject* obj, QEvent *event)
{
    if (obj == ui->lineEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up)
            {

                 //ui->lineEdit->setText("Up Key");
                 if (hmempos > 0) {
                     hmempos --;
                     ui->lineEdit->setText(hmem[hmempos]);
                     //qDebug() << hmem[hmempos] << hmempos;

                 }
                 return true;
            }
            else if(keyEvent->key() == Qt::Key_Down)
            {

                //ui->lineEdit->setText("Down Key");
                if (hmempos > -1) {
                if (hmempos < 10 && hmem[hmempos] != "") {
                    if (hmem[hmempos] != "") hmempos ++;
                    //qDebug() << hmem[hmempos] << hmempos;
                    if(hmempos <= 9 ) ui->lineEdit->setText(hmem[hmempos]);
                    else ui->lineEdit->setText("");

                    //if (hmem[hmempos+2] == "") ui->lineEdit->setText("");
                }
            }
                //if (hmempos == 10) ui->lineEdit->setText("");
                return true;
            }

        }
        return false;
    }

    if(obj == ui->statusbar) emit updatestatusbar();
    //if(obj == ui->textBrowser) qDebug() << event;
    return QMainWindow::eventFilter(obj, event);

}


void MainWindow::handleAnchorClicked(const QUrl &url)
{
 /* QString msg = "new URL == ";
  msg.append(url.toDisplayString());
  msg.append("\nold URL == ");
  msg.append(ui->textBrowser->source().toDisplayString());
  QMessageBox::information(this, __FUNCTION__, msg);*/
  int int1=url.toString().lastIndexOf(".");
  int int2=url.toString().lastIndexOf("/");
  QString number = url.toString().mid(int2+1,int1-int2-1);
  //qDebug() << number;

}

void MainWindow::handleSourceChanged(const QUrl &url)
{
  QString msg = "new URL == ";
  msg.append(url.toDisplayString());
  msg.append("\nold URL == ");
  msg.append(ui->textBrowser->source().toDisplayString());
  QMessageBox::information(this, __FUNCTION__, msg);
}

void MainWindow::keymem(QString memstr)
{
    if (hmem[9] != "") {
        for (int i=0;i<9;i++){
            hmem[i] = hmem[i+1];
        }
    }
    hmem[9] = "";
    for (hmempos=1;hmempos<10;hmempos++){
        if (hmem[hmempos-1] == "") break;
    }
    hmem[hmempos-1] = memstr;
}
void MainWindow::on_actionDate_Search_triggered() //Ctrl-S
{
    labeltext = "Search number :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    QString html = loopYear(ns,dd,mm,year,1,eudate);
    if (ns > 0) {
    writetmpfile("<br>Searching dates for whole year starts<br>");
    writetmpfile("<html>"+html+"</html>");
    }
}

void MainWindow::updatestatusbar()
{
    QString scombstr;

    if (d2 == 0) {
    if (eudate) scombstr = "Active phrase : " +phrase+ " - Current date : " + QString::number(dd) + "/" + QString::number(mm) + "/" + QString::number(year);
    else scombstr = "Active phrase : " +phrase+ " - Current date : " + QString::number(mm) + "/" + QString::number(dd) + "/" + QString::number(year);
    } else {
    if (eudate) scombstr = "Active phrase : " +phrase+ " - Current date : " + QString::number(dd) + "/" + QString::number(mm) + "/" + QString::number(year) + " Second date : " + QString::number(d2) + "/" + QString::number(m2) + "/" + QString::number(y2);
    else scombstr = "Active phrase : " +phrase+ " - Current date : " + QString::number(mm) + "/" + QString::number(dd) + "/" + QString::number(year) + " Second date : " + QString::number(m2) + "/" + QString::number(d2) + "/" + QString::number(y2);
    }
    //ui->statusbar->setStyleSheet("color: red");
    ui->menu_File->menuAction()->setStatusTip(scombstr);
    ui->statusbar->showMessage(scombstr);
}

inputDialog::inputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDialog)
{

    ui->setupUi(this);
    if (labeltext == "New date :") {
        inputDialog::setWindowTitle("Enter new date");
        ui->lineEdit->setInputMask("00-00");
    }
    if (labeltext == "Enter second date :") {
        inputDialog::setWindowTitle("Enter second date for extended date details");
        ui->lineEdit->setInputMask("00-00-0000");
    }

    if (labeltext == "Search number :") {
        inputDialog::setWindowTitle("Search whole year for cipher");
        ui->lineEdit->setInputMask("0000");
    }
    if (labeltext == "New Phrase :") inputDialog::setWindowTitle("Phrase needed for Analyzer to run");
    if (labeltext == "New phrase :") inputDialog::setWindowTitle("Enter new phrase");
    if (labeltext == "Enter Year:") {
        inputDialog::setWindowTitle("Change year");
        ui->lineEdit->setInputMask("0000");
    }
    if (labeltext == "Enter Cipher nr. :") {
        inputDialog::setWindowTitle("Search history.txt");
        ui->lineEdit->setInputMask("0000");
    }
    if (labeltext == "Filter history :") {
        inputDialog::setWindowTitle("List History");
        tmpstring = "<none>";
    }
    ui->label->setText(labeltext);
}

inputDialog::~inputDialog()
{
    delete ui;
}

void inputDialog::displaydialog()
{
    if (labeltext == "New Phrase :") phrase = ui->lineEdit->text();
    if (labeltext == "New phrase :") phrase = ui->lineEdit->text();
    if (labeltext == "Search number :") ns = ui->lineEdit->text().mid(0,4).toInt();
    if (labeltext == "Compare to history :") ns = ui->lineEdit->text().mid(0,4).toInt();
    if (labeltext == "Enter Year:") year = ui->lineEdit->text().mid(0,4).toInt();
    if (labeltext == "Enter Cipher nr. :") ns = ui->lineEdit->text().mid(0,4).toInt();
    if (labeltext == "Filter history :") tmpstring = ui->lineEdit->text();
    if (eudate) {
    if (labeltext == "New date :") {
        dd = ui->lineEdit->text().mid(0,2).toInt();
        mm = ui->lineEdit->text().mid(3,2).toInt();
        if (valid_date(dd,mm,year) == 0) {
            QDate cd = QDate::currentDate();
            year = cd.year();
            dd = cd.day();
            mm = cd.month();
        }

    }
    }else{
       if (labeltext == "New date :") {
            dd = ui->lineEdit->text().mid(3,2).toInt();
            mm = ui->lineEdit->text().mid(0,2).toInt();
            if (valid_date(dd,mm,year) == 0) {
                QDate cd = QDate::currentDate();
                year = cd.year();
                dd = cd.day();
                mm = cd.month();
            }
        }
    }
   if (eudate) {
    if (labeltext == "Enter second date :") {
        d2 = ui->lineEdit->text().mid(0,2).toInt();
        m2 = ui->lineEdit->text().mid(3,2).toInt();
        y2 = ui->lineEdit->text().mid(6,4).toInt();
        if (valid_date(d2,m2,y2) == 0) {
            d2 = 0;
            m2 = 0;
            y2 = 0;
        }
    }
    }else {
     if (labeltext == "Enter second date :") {
        d2 = ui->lineEdit->text().mid(3,2).toInt();
        m2 = ui->lineEdit->text().mid(0,2).toInt();
        y2 = ui->lineEdit->text().mid(6,4).toInt();
        if (valid_date(d2,m2,y2) == 0) {
            d2 = 0;
            m2 = 0;
            y2 = 0;
        }

    }
   }
}


void inputDialog::on_buttonBox_accepted()
{
    emit displaydialog();
}


void inputDialog::on_lineEdit_returnPressed()
{
    emit displaydialog();
}

void MainWindow::on_actionNew_Phrase_triggered()
{
    labeltext = "New phrase :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar();
}

void MainWindow::on_actionNew_Date_triggered()
{
    labeltext = "New date :";

    inputDialog datesearch;
    datesearch.eudate = eudate;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar();
}

void MainWindow::on_actionSearch_History_txt_triggered() //Ctrl-H
{
    ns = 0;
    labeltext = "Enter Cipher nr. :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    if (ns > 0){
    writetmpfile("<br>Search history with number starts<br>");
    QString html = searchwords(ns,true);
    if (ns >0) writetmpfile("<html>"+html+"</html>");
    }
}

void MainWindow::on_actionSet_Year_triggered()
{
    labeltext = "Enter Year:";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar();
}

void MainWindow::on_action_Analyze_triggered() //Ctrl-A
{
    if (phrase == "<none>") {
     labeltext = "New Phrase :";
     inputDialog datesearch;
     datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
     datesearch.exec();
     emit updatestatusbar();
    }
   if (phrase != "<none>") {
       writetmpfile("<br>Analyzing phrase starts");
       readsolarfile(dd,mm,year);
       bool sentences = ui->actionSentence_split->isChecked();
       QString html;
       if (sentences) html = analyze(dd,mm,year,phrase,false,0,eudate);
       else html = runanalyze(dd,mm,year,phrase.toUtf8().constData(),false,0,eudate);
       writetmpfile("<html>"+html+"</html>");
   }

}

void MainWindow::on_action_Eu_date_toggled(bool arg1)
{
    if (arg1) eudate = true;
    else eudate = false;
    char filename[13] = "settings.txt";
    if (eudate) {
        ui->action_Eu_date->setText("Date DMY");
        writeSettings(filename,"DMY","true");
    } else  {
        ui->action_Eu_date->setText("Date MDY");
        writeSettings(filename,"DMY","false");
    }
    emit updatestatusbar();
}

void MainWindow::on_actionDate_Details_triggered() //Ctrl-D
{
    QString html = gcalc(dd,mm,year,d2,m2,y2,eudate);
    writetmpfile("<html>"+html+"</html>");
    //#include <QDebug>//qDebug() << "\n" << html;
}

void MainWindow::calc(QString calcstr)
{
    int result=0,ppos=0,mpos=0;
    ppos = calcstr.indexOf("+");
    mpos = calcstr.indexOf("-");
    if (ppos == -1) ppos = calcstr.length();
    if (mpos == -1) mpos = calcstr.length();
    if (ppos==0) {
        int ppos2 = calcstr.indexOf("+",ppos+1);
        if (mpos < ppos2 || ppos2 == -1) result += calcstr.mid(0,mpos).toInt();
        else result += calcstr.mid(0,ppos2).toInt();
        ppos = calcstr.indexOf("+",ppos+1);
    } else if (mpos==0) {
        int mpos2 = calcstr.indexOf("-",mpos+1);
        if (ppos < mpos2 || mpos2 == -1) result += calcstr.mid(0,ppos).toInt();
        else result += calcstr.mid(0,mpos2).toInt();
        mpos = calcstr.indexOf("-",mpos+1);
    } else if ((ppos < mpos  || mpos == -1) && ppos != -1) result += calcstr.mid(0,ppos).toInt();
    else result += calcstr.mid(0,mpos).toInt();
    if (ppos == calcstr.length()) ppos = calcstr.indexOf("+");
    if (mpos == calcstr.length()) mpos = calcstr.indexOf("-");
    do {
        if ((ppos < mpos  || mpos == -1) && ppos != -1) {
            int ppos2 = calcstr.indexOf("+",ppos+1);
            if (ppos2 < mpos && ppos2 != -1) result += calcstr.mid(ppos,ppos2-ppos).toInt();
            else if (mpos == -1 && ppos2 == -1) result += calcstr.mid(ppos,calcstr.length()-ppos).toInt();
            else if (mpos == -1 && ppos2 > ppos) result += calcstr.mid(ppos,ppos2-ppos).toInt();
            else result += calcstr.mid(ppos,mpos-ppos).toInt();
            ppos = calcstr.indexOf("+",ppos+1);
        } else {
            int mpos2 = calcstr.indexOf("-",mpos+1);
            if (mpos2 < ppos && mpos2 != -1) result += calcstr.mid(mpos,mpos2-mpos).toInt();
            else if (ppos == -1 && mpos2 == -1) result += calcstr.mid(mpos,calcstr.length()-mpos).toInt();
            else if (ppos == -1 && mpos2 > mpos) result += calcstr.mid(mpos,mpos2-mpos).toInt();
            else result += calcstr.mid(mpos,ppos-mpos).toInt();
            mpos = calcstr.indexOf("-",mpos+1);
        }
    } while (ppos != -1 || mpos != -1);
    writetmpfile(calcstr+"="+QString::number(result));
}

void MainWindow::on_lineEdit_returnPressed()
{

    QString tphrase = ui->lineEdit->text();
    std::string stdphrase = tphrase.toUtf8().constData();
    if (ui->checkBox->isChecked() && stdphrase[0] != '/') stdphrase = "/a" +stdphrase;
    if (tphrase.indexOf("+") != -1 || tphrase.indexOf("-") != -1) {
        calc(tphrase);

    }
    else if (stdphrase == "dd" || stdphrase == "DD") {
        QString html = printword(stdphrase,'D',true,false);
        writetmpfile("<html>"+html+"</html>");
    }

    else if (stdphrase [0] == '/' || stdphrase [0] == '.') {
        //qDebug() << QString::fromStdString(stdphrase) << "\n";
        //eraseAllSubStr(stdphrase," ");
        tphrase = QString::fromStdString(stdphrase);
        replaceAll(stdphrase,"/ ","/");
        replaceAll(stdphrase," /","/");
        replaceAll(stdphrase,"/a ","/a");
        replaceAll(stdphrase,"/c ","/c");
        replaceAll(stdphrase,"/n ","/n");
        replaceAll(stdphrase,"/s ","/s");
        replaceAll(stdphrase,"/h ","/h");
        replaceAll(stdphrase,"/w ","/w");
        replaceAll(stdphrase,"/e ","/e");
        replaceAll(stdphrase,"/o ","/o");
        replaceAll(stdphrase,"/r ","/r");
        //qDebug() << QString::fromStdString(stdphrase) << "\n";
        if (stdphrase[1] != 'a') keymem(QString::fromStdString(stdphrase));
        switch (stdphrase[1]) {
        case 'a':
        {
            if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) == "" && phrase == "<none>") {
             labeltext = "New Phrase :";
             inputDialog datesearch;
             datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
             datesearch.exec();
            } else if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) != ""){

             phrase = QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2));
             keymem(phrase);
             readsolarfile(dd,mm,year);
             bool sentences = ui->actionSentence_split->isChecked();
             QString html;
             if (sentences) html = analyze(dd,mm,year,phrase,false,0,eudate);
             else html = runanalyze(dd,mm,year,phrase.toUtf8().constData(),false,0,eudate);
             writetmpfile("<html>"+html+"</html>");
             emit updatestatusbar();
            }
            break;
        }
        case 'c':
        {
            ui->textBrowser->clear();
            break;
        }
        case 'n':
        {
            QStringList dags= tphrase.mid(2,tphrase.length()-2).split("/");
            if (dags.count() < 2) dags= tphrase.mid(2,tphrase.length()-2).split(".");
            if (eudate && dags.count() == 2) {
                dd = dags[0].toUInt();
                mm = dags[1].toUInt();
                //dd = ui->lineEdit->text().mid(2,2).toInt();
                //mm = ui->lineEdit->text().mid(5,2).toInt();
            } else if (dags.count() == 2){
                mm = tphrase.mid(2,2).toInt();
                dd = tphrase.mid(5,2).toInt();
            }
            //if (ui->lineEdit->text().mid(8,4).toInt() > 0) year = ui->lineEdit->text().mid(8,4).toInt();
            if (dags.count() > 2) {

                year = dags[2].toInt();
                if (year < 100) year += 2000;
            }
            if (valid_date(dd,mm,year) == 0 || dags.count() < 2) {
                //qDebug() << ui->lineEdit->text().mid(2,2) << " " << ui->lineEdit->text().mid(5,2) << " " << year;
                QDate cd = QDate::currentDate();
                dd = cd.day();
                mm = cd.month();
                year = cd.year();
            }
            emit updatestatusbar();
            break;
        }
        case 's':
        {
            QStringList dags= tphrase.mid(2,tphrase.length()-2).split("/");
            if (dags.count() < 2) dags= tphrase.mid(2,tphrase.length()-2).split(".");

            if (eudate && dags.count() == 2) {
                d2 = dags[0].toUInt();
                m2 = dags[1].toUInt();
            } else if (dags.count() == 2){
                m2 = dags[0].toUInt();
                d2 = dags[1].toUInt();
            }
            if (dags.count() == 3) {
                y2 = dags[2].toInt();
                if (y2 < 100) y2 += 2000;
            } else y2 = year;

            if (valid_date(d2,m2,y2) == 0 || dags.count() < 2) {
                //qDebug() << ui->lineEdit->text().mid(2,2) << " " << ui->lineEdit->text().mid(5,2) << " " << y2;
                d2 = 0;
                m2 = 0;
                y2 = 0;
            }
            emit updatestatusbar();
            break;
        }
        case 'h':
            emit shorthelp();
            break;
        case 'w':
        {
            if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) == "" && phrase == "<none>") {
             labeltext = "New phrase :";
             inputDialog datesearch;
             datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
             datesearch.exec();

            } else if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) != "") phrase = QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2));
             keymem(phrase);
            emit updatestatusbar();
            if (phrase != "<none>") {
                std::string stdphrase = phrase.toUtf8().constData();
                bool sentences = ui->actionSentence_split->isChecked();
                if (sentences) writetmpfile("<html>"+printallwords(stdphrase,'N',true,false)+"</html>");
                else writetmpfile("<html>"+printword(stdphrase,'N',true,false)+"</html>");
            }
            break;
        case 'd':
            {
            QString html;
            if (eudate) {if (valid_date(ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(5,2).toInt(),year) == 1) {
                    if (ui->lineEdit->text().mid(8,4).toInt() > 0) html = gcalc(ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(8,4).toInt(),d2,m2,y2,eudate);
                    else html = gcalc(ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(5,2).toInt(),year,d2,m2,y2,eudate);
                }
                else html = gcalc(dd,mm,year,d2,m2,y2,eudate);
            }else { if (valid_date(ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(2,2).toInt(),year) == 1) {
                    if (ui->lineEdit->text().mid(8,4).toInt() > 0) html = gcalc(ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(8,4).toInt(),d2,m2,y2,eudate);
                    else gcalc(ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(2,2).toInt(),year,d2,m2,y2,eudate);
                }
                else html = gcalc(dd,mm,year,d2,m2,y2,eudate);
             }
            writetmpfile("<html>"+html+"</html>");
            break;
            }
            case 'e':
                {
                QString html;
                int type=5;
                if (ui->lineEdit->text().mid(2,1) == "T") type = 1;
                if (ui->lineEdit->text().mid(2,1) == "A") type = 2;
                if (ui->lineEdit->text().mid(2,1) == "P") type = 3;
                if (ui->lineEdit->text().mid(2,1) == "H") type = 4;
                if (ui->lineEdit->text().mid(2,1) == "X") type = 5;
                //qDebug() << ui->lineEdit->text().mid(2,1) << " " << ui->lineEdit->text().mid(4,2) << " " << ui->lineEdit->text().mid(7,2) << " " << ui->lineEdit->text().mid(10,4);
                if (eudate) {if (valid_date(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year) == 1) {
                        if (ui->lineEdit->text().mid(10,4).toInt() > 0) html = solareclipe(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(10,4).toInt(),1,type,eudate);
                        else if (ui->lineEdit->text().mid(2,1) != "") html = solareclipe(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year,1,type,eudate);
                    }
                    else html = solareclipe(dd,mm,year, 1,type,eudate);
                }else { if (valid_date(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year) == 1) {
                        if (ui->lineEdit->text().mid(10,4).toInt() > 0) html = solareclipe(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(10,4).toInt(),1,type,eudate);
                        else solareclipe(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year,1,type,eudate);
                    }
                    else if (type > 0) html = solareclipe(dd,mm,year,1,type,eudate);
                 }
                writetmpfile("<html>"+html+"</html>");
                break;
                }

            case 'o':
                {
                QString html;
                if (ui->lineEdit->text().mid(2,1).toInt() > 0 && ui->lineEdit->text().mid(2,1).toInt() < 5) {
                if (eudate) {if (valid_date(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year) == 1) html = date2history(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                    else html = date2history(dd,mm,year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                }else { if (valid_date(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year) == 1) html = date2history(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                    else html = date2history(dd,mm,year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                 }
                writetmpfile("<html>"+html+"</html>");
                }
                break;
                }
            case 'r':
            {
                if (jewish_on && single_r_on && francis_on && satanic_on && sumerian_on && rev_sumerian_on) {
                    single_r_on=false;
                    francis_on=false;
                    satanic_on=false;
                    jewish_on=false;
                    sumerian_on=false;
                    rev_sumerian_on=false;
                } else if (!jewish_on && !single_r_on && !francis_on && !satanic_on && !sumerian_on && !rev_sumerian_on) {
                    single_r_on=true;
                    francis_on=true;
                    satanic_on=true;
                    jewish_on=true;
                    sumerian_on=true;
                    rev_sumerian_on=true;
                }
                break;
            }
           case 'y':
            {
                QString html;

              html = phraserank(phrase.toUtf8().constData(),eudate,3,true,true);

              writetmpfile("<html>"+html+"</html>");
              break;
            }
            case 'x':
            {
               if (stdphrase [2] == 'd') { //add/subtrackt to date
                ns = ui->lineEdit->text().length();
                ns = ui->lineEdit->text().mid(3,ns-3).toInt();
                int d1,m1,y1;
                QDate cd(year,mm,dd);
                cd = cd.addDays(ns);
                y1 = cd.year();
                d1 = cd.day();
                m1 = cd.month();
                if (eudate) writetmpfile("<html>"+QString::number(d1)+"/"+QString::number(m1)+"/"+QString::number(y1)+"</html>");
                else writetmpfile("<html>"+QString::number(m1)+"/"+QString::number(d1)+"/"+QString::number(y1)+"</html>");
               } else if (stdphrase [2] == 's') { // add/subtrackt to second date
                ns = ui->lineEdit->text().length();
                ns = ui->lineEdit->text().mid(3,ns-3).toInt();
                QDate cd(year,mm,dd);
                cd = cd.addDays(ns);
                y2 = cd.year();
                d2 = cd.day();
                m2 = cd.month();
                emit updatestatusbar();
               } else { // add/subtrackt to date and display only
                   ns = ui->lineEdit->text().length();
                   ns = ui->lineEdit->text().mid(2,ns-2).toInt();
                   QDate cd(year,mm,dd);
                   cd = cd.addDays(ns);
                   year = cd.year();
                   dd = cd.day();
                   mm = cd.month();
                   emit updatestatusbar();
               }
            break;
            }
            case 'f':
            {
                //Httpdownload = new HttpDownload(this);
                //Httpdownload->setWindowTitle("Http Download");

                //Httpdownload->show();

                HttpDownload Httpdownload;
                Httpdownload.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
                Httpdownload.exec();
                //filesource = "index.html";
                //tmpstring = "https://www.nytimes.com/";
                //qDebug() << filesource;
                if (filesource != "") {
                    List = getheadlines(filesource,20);
                    headdialog = new headDialog(this);
                    headdialog->setWindowTitle("Http Download");
                    headdialog->show();
                }

                break;
            }
            case 't': // test
             {
                //qDebug() << "days d_s " << a_seconddate("day_d_s");
                //qDebug() << "days d_s " << a_seconddate("day_s_d");
                //qDebug() << "week d_s " << a_seconddate("week_d_s");
                //qDebug() << "week s_d " << a_seconddate("week_s_d");
                qDebug() << "month d_s " << a_seconddate("month_d_s");
                qDebug() << "month s_d " << a_seconddate("month_s_d");
                qDebug() << "day_full " << a_seconddate("day_full");
                qDebug() << "week_full " << a_seconddate("week_full");
                qDebug() << "month_full " << a_seconddate("month_full");
                //qDebug() << "months_d_s " << monthbeetween(mm,m2,a_seconddate("day_d_s"),'M') << " days " << monthbeetween(mm,m2,a_seconddate("day_d_s"),'D');
                //qDebug() << "months_s_d " << monthbeetween(m2,mm,a_seconddate("day_s_d"),'M') << " days " << monthbeetween(m2,mm,a_seconddate("day_s_d"),'D');
                //int test = a_seconddate("week_d_s");
                //test = a_seconddate("week_s_d");
             }
          }
        }

    }
    else if (stdphrase != ""){
        QString html;

        keymem(QString::fromStdString(stdphrase));
        if (ui->SaveHistory->isChecked()) html = printword(stdphrase,'Y',true,false);
        else html = printword(stdphrase,'N',true,false);
        writetmpfile("<html>"+html+"</html>");
    }
    ui->lineEdit->clear();
}

void MainWindow::shorthelp()
{
        writetmpfile("<br><h1>Short help</h1><br>");
        writetmpfile("<b>All functions are available from the Menu</b>");
        writetmpfile("<b>By shortcut following functions are available</b>");
        writetmpfile("<font color=\"blue\">(Ctrl-S)</font> <b>Date search</b> connect number to dates spanning the active year.");
        writetmpfile("<font color=\"blue\">(Ctrl-H)</font> <b>Search history.txt</b> searches all words connected to entered number in history.txt.");
        writetmpfile("<font color=\"blue\">(Ctrl-A)</font> <b>Analyze</b> takes active phrase and compare it to current date displayed on the status bar.");
        writetmpfile("<font color=\"blue\">(Ctrl-D)</font> <b>Date details</b> displays calculations for current date. Second date will extend the information.");
        writetmpfile("<font color=\"blue\">(Ctrl-W)</font> <b>Word details</b> shows details about active phrase.");
        writetmpfile("<font color=\"blue\">(Ctrl-E)</font> <b>Compare Solar Eclipses to History.txt</b> for current date");
        writetmpfile("<font color=\"blue\">(Ctrl-O)</font> <b>Date compare to history</b> calculate current date and compares it to history.txt");
        writetmpfile("<font color=\"blue\">(Ctrl-T)</font> <b>Compare phrase to history.txt</b> takes one of the base ciphers from active phrase and compares it to history.txt<br>");
        writetmpfile("<b>Sentence split</b> When unchecked analyze and word print will not split sentences<br>");


        writetmpfile("The input area takes phrases wich are displayed and saved to history.txt if Save is checked<br>");
        writetmpfile("Toggle Analyze will run entered phrases through analyzer instead of word details.<br>");
        writetmpfile("<b>The input area also takes commands:</b>");

        writetmpfile("<font color=\"blue\">/a(phrase)</font> runs analyzer, (<b>Phrase</b> is optional)");
        writetmpfile("<font color=\"blue\">/w(phrase)</font> phrase details (<b>Phrase</b> is optional)");
        writetmpfile("<font color=\"blue\">/n##/##/####</font> New date, (Year is optional)");
        writetmpfile("<font color=\"blue\">/s##/##/####</font> New second date");
        writetmpfile("<font color=\"blue\">/d##/##/####</font> date details (date is optional, year is extra option)");
        writetmpfile("<font color=\"blue\">/o#/##/##</font> Date compare to history (first number is filter 1-4, date is optional)");
        writetmpfile("<font color=\"blue\">/e@/##/##/####</font> Last and next Solar eclipse relative to date. @ is type \"T A P H-X=for all\" (date is optional, year is extra option)");
        writetmpfile("<font color=\"blue\">/r</font> Toggle all extra ciphers on or off");
        writetmpfile("<font color=\"blue\">/x##</font> Add or subtract days from current date and set that date.");
        writetmpfile("<font color=\"blue\">/xs##</font> Add or subtract days from current date and set that as second date.");
        writetmpfile("<font color=\"blue\">/xd##</font> Add or subtract days from current date and display only in output.");
        writetmpfile("<font color=\"blue\">dd</font> deletes last line from history.txt");
        writetmpfile("<font color=\"blue\">/h</font> shows this help");


        writetmpfile("<font color=\"blue\">/c</font> Clears output");
        writetmpfile("Enter <b>Word Phrase</b> shows details about that phrase and saves it to history if Save is checked.");
        writetmpfile("Change <b>Current Year</b> from The Edit menu");
        writetmpfile("Change <b>Date Style</b> from The Edit menu");
        writetmpfile("<br><b>© jonssofh@hotmail.com</b><br>");

}

void MainWindow::welcome()
{
        writetmpfile("<h1>Welcome to Gematria Analyzer!</h1><br>");
        writetmpfile("<h2>This program calculates Kabbalah ciphers from phrases and compares it to date numerology.</h2><br>");
        writetmpfile("<b>For details about ciphers select <font color=\"blue\">Tables->List ciphers</font></b>");
        writetmpfile("The program takes a phrase and date for comparison, also second date can be entered<br>");
        writetmpfile("Select <b>Help</b> from menu or type <font color=\"blue\">/h</font> in input area<br>");
        //writetmpfile("<div title=\"them's hoverin' words\">hover me</div>");
}

void MainWindow::on_action_Word_details_triggered() //Ctrl-W
{
    if (phrase == "<none>") {
     labeltext = "New phrase :";
     inputDialog datesearch;
     datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
     datesearch.exec();
     emit updatestatusbar();
    }
   if (phrase != "<none>") {
       std::string stdphrase = phrase.toUtf8().constData();
       bool sentences = ui->actionSentence_split->isChecked();
       if (sentences) writetmpfile("<html>"+printallwords(stdphrase,'N',true,false)+"</html>");
       else writetmpfile("<html>"+printword(stdphrase,'N',true,false)+"</html>");

   }

}

void MainWindow::on_actionList_Ciphers_triggered()
{
    writetmpfile("English Ordinal");
    writetmpfile(listciphers(0,0,0));
    writetmpfile("Full Reduction");
    writetmpfile(listciphers(1,0,0));
    writetmpfile("Reverse Ordinal");
    writetmpfile(listciphers(0,1,0));
    writetmpfile("Reverse full Reduction");
    writetmpfile(listciphers(1,1,0));
    if (single_r_on) {
    writetmpfile("Single Reduction");
    writetmpfile(listciphers(0,0,1));
    }
    if (francis_on) {
    writetmpfile("Francis Bacon");
    writetmpfile(listciphers(0,0,2));
    }
    if (satanic_on) {
    writetmpfile("Satanic");
    writetmpfile(listciphers(0,0,3));
    }
    if (jewish_on) {
    writetmpfile("Jewish");
    writetmpfile(listciphers(0,0,4));
    }
    if (sumerian_on) {
    writetmpfile("Sumerian");
    writetmpfile(listciphers(0,0,5));
    }
    if (rev_sumerian_on) {
    writetmpfile("Reverse Sumerian");
    writetmpfile(listciphers(0,1,5));
    }
}

void MainWindow::on_actionList_Primenumbers_triggered()
{
    writetmpfile(listnumbers('P'));
}

void MainWindow::on_actionList_Triangular_numbers_triggered()
{
    writetmpfile(listnumbers('T'));
}

void MainWindow::on_actionCompare_phrase_to_history_triggered() //Ctrl-T
{
    QString str2;
    if (phrase == "<none>") {
     labeltext = "New Phrase :";
     inputDialog datesearch;
     datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
     datesearch.exec();
     emit updatestatusbar();
    }
    ns = 0;
    if (phrase != "<none>") {
    labeltext = "New Phrase :";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();
    if (ns > 0) {
    writetmpfile("<br>Search history from phrase starts<br>");
    writetmpfile(searchhistory(ns,phrase.toUtf8().constData()));

    if (ns == 1) writetmpfile( "Calculated from " +QString::fromStdString(formattext("English ordinal",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 2) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Full Reduction",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 3) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Reverse Ordinal",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 4) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Reverse full Reduction",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 5) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Single Reduction",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 6) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Francis Bacon",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 7) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Satanic",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 8) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Jewish",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 9) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Sumerian",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 10) writetmpfile( "Calculated from " +QString::fromStdString(formattext("Reverse Sumerian",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    }
    }
}

selectDialog::selectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectDialog)
{
    if (labeltext == "Date to history")selectDialog::setWindowTitle("Choose filter");
    ui->setupUi(this);
    if (labeltext == "solar") {
        ui->Jewish->show();
        ui->Francis->hide();
        ui->Satanic->hide();
        ui->SingleRed->hide();
        ui->Sumerian->hide();
        ui->rev_sumerian->hide();
        ui->radioButton1->setText("Total Solar Eclipse");
        ui->radioButton2->setText("Annular Solar Eclipse");
        ui->radioButton3->setText("Partial Solar Eclipse");
        ui->radioButton4->setText("Hybrid Solar Eclipse");
        ui->Jewish->setText("All Solar Eclipses");

    } else {
    if (!single_r_on) ui->SingleRed->hide();
    if (!francis_on) ui->Francis->hide();
    if (!satanic_on) ui->Satanic->hide();
    if (!jewish_on) ui->Jewish->hide();
    if (!sumerian_on) ui->Sumerian->hide();
    if (!rev_sumerian_on) ui->rev_sumerian->hide();
    }

}

selectDialog::~selectDialog()
{
    delete ui;
}

void selectDialog::displaydialog()
{
    if (labeltext == "New Phrase :") {
    if (ui->radioButton1->isChecked()) ns = 1;
    if (ui->radioButton2->isChecked()) ns = 2;
    if (ui->radioButton3->isChecked()) ns = 3;
    if (ui->radioButton4->isChecked()) ns = 4;
    if (ui->SingleRed->isChecked()) ns = 5;
    if (ui->Francis->isChecked()) ns = 6;
    if (ui->Satanic->isChecked()) ns = 7;
    if (ui->Jewish->isChecked()) ns = 8;
    if (ui->Sumerian->isChecked()) ns = 9;
    if (ui->rev_sumerian->isChecked()) ns = 10;
    }
    if (labeltext == "Date to history") {
    if (ui->radioButton1->isChecked()) filter = 1;
    if (ui->radioButton2->isChecked()) filter = 2;
    if (ui->radioButton3->isChecked()) filter = 3;
    if (ui->radioButton4->isChecked()) filter = 4;
    if (ui->SingleRed->isChecked()) filter = 5;
    if (ui->Francis->isChecked()) filter = 6;
    if (ui->Satanic->isChecked()) filter = 7;
    if (ui->Jewish->isChecked()) filter = 8;
    if (ui->Sumerian->isChecked()) filter = 9;
    if (ui->rev_sumerian->isChecked()) filter = 10;
    }
    if (labeltext == "solar") {
    if (ui->radioButton1->isChecked()) filter = 1;
    if (ui->radioButton2->isChecked()) filter = 2;
    if (ui->radioButton3->isChecked()) filter = 3;
    if (ui->radioButton4->isChecked()) filter = 4;
    if (ui->Jewish->isChecked()) filter = 5;
    }
}


void selectDialog::on_pushButton_clicked()
{
    emit displaydialog();
}

void MainWindow::on_action_Second_date_triggered()
{
    labeltext = "Enter second date :";
    inputDialog datesearch;
    datesearch.eudate = eudate;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar();
}

void MainWindow::on_actionC_ompare_date_to_History_txt_triggered() // Ctrl-O
{
    filter = 0;
    labeltext = "Date to history";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();
    if (filter > 0) {
    writetmpfile("<br>Search history connected to current date starts");
    writetmpfile(date2history(dd,mm,year,true,eudate,filter));
    }
}

void MainWindow::on_actionHelp_triggered()
{
    emit MainWindow::shorthelp();
}

void MainWindow::on_actionSolar_Eclipses_triggered()
{
    filter = 0;
    labeltext = "solar";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();
    if (filter > 0)
    emit writetmpfile(solareclipe(dd,mm,year,1,filter,eudate)); // 1=print
}

void MainWindow::on_actionCompare_SolarE_to_history_triggered()
{
    filter = 0;
    labeltext = "solar";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();

    if (filter > 0)
    writetmpfile(solar2history(dd,mm,year,filter,eudate));
}

void MainWindow::on_pushButton_clicked()
{
    if (numberOfDays(mm-1,year) == dd) {
        dd = 1;
        mm ++;
    } else dd ++;
    emit updatestatusbar();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (dd == 1) {
        mm --;
        dd = numberOfDays(mm-1,year);
    } else dd --;
    emit updatestatusbar();
}

void MainWindow::on_actionAbout_triggered()
{
   // QMessageBox::about(this,"About Gematria Analyzer","Version 0.2.3 <br>Gematria Analyzer is a free software created for playing with the English language. Support is most appreciated.");
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Gematria Analyzer");
    msgBox.setText(tr("Version 0.2.7 <br>Gematria Analyzer is a free software created for playing with the English language"));
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Donate!"), QMessageBox::YesRole);
    msgBox.addButton(tr("Ok"), QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton()==pButtonYes) {
        QMessageBox::about(this,"Donate","Donations can be done by Paypal to jonssofh@hotmail.com or cryptocurrency BTC 3F8BM7RLiYp5j8k47JKSFqqpcru1pBbA9s , Support is most appreciated.");
    }
}

void MainWindow::on_action_Add_remove_ciphers_triggered()
{
    cipherDialog mDialog;
    mDialog.setWindowTitle("Add/Remove ciphers");
    mDialog.setModal(true);
    mDialog.exec();
}

void MainWindow::on_actionList_Solar_Eclipses_triggered()
{
    labeltext = "solar";
    readsolarfile(dd,mm,year);
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();

    writetmpfile(printzerodays(dd,mm,year,0,filter,"listsolareclipses",eudate,true));

}

void MainWindow::on_actionPhrase_ranking_triggered()
{
    bool prime=false,triangular=false;
    rankDialog rDialog;
    rDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    rDialog.exec();
    QString html;
    if (phrase != "<none>") {
        if (ns>=10000){
            triangular = true;
            ns -= 10000;
        }
        if (ns>=1000){
            prime = true;
            ns -= 1000;
        }
    html = phraserank(phrase.toUtf8().constData(),eudate,ns,prime,triangular);
    writetmpfile("<html>"+html+"</html>");

    QStringList myStringList = phrase.split(',').first().split(':');
    phrase = myStringList.first();
    }
    emit updatestatusbar();
}

void MainWindow::on_action_Font_triggered()
{
    //const QFontDialog::FontDialogOptions options = QFlag(fontDialogOptionsWidget->value());
    bool ok;
    QFont font = QFontDialog::getFont(&ok,QFont(ui->textBrowser->font()),this,"Select Font");
    if (ok) {
        //qDebug() << font;
        char filename[13] = "settings.txt";
        writeSettings(filename,"font",font.toString().toUtf8().constData());
        ui->textBrowser->setFont(font);
    }
}

void MainWindow::doPrint(QPrinter * printer)
{
    QTime ct = QTime::currentTime();
    //ui->textBrowser->print(printer);
    printer->newPage();
    printer->setDocName("Gematria Analyzer - "+ct.currentTime().toString());
    ui->textBrowser->print(printer);
}

void MainWindow::on_action_Print_triggered()
{


    QPrintPreviewDialog * printPreview = new QPrintPreviewDialog(this);
    connect(printPreview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(doPrint(QPrinter *)));
    printPreview->exec();
}


void MainWindow::on_actionList_History_triggered()
{
    labeltext = "Filter history :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    if (tmpstring != "<none>") {
        QString buffer = listhistory(tmpstring);
        writetmpfile("List history starts<br>");
        writetmpfile(buffer);
    }
}

void MainWindow::on_actionCalendar_triggered()
{
    Calwindow = new CalWindow(this);
    connect(Calwindow,SIGNAL(buttonpressed()),this,SLOT(updatestatusbar()));
    Calwindow->setWindowTitle("Calendar");
    Calwindow->show();
}

void MainWindow::writetmpfile(QString html)
{
    ui->textBrowser->append(html);
    if (file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream( file );
        stream << html;
        file->close();
    }
}


