#include "gcalc.h"
#include "tools.h"
#include "mainwindow.h"
#include <QDebug>
#include <QDate>
#include <QFile>
#include <QMessageBox>

#define BUFFERSIZE 256
using namespace std;

/*
  QString::fromStdString(string)  <- from string to Qstring
  QString::number(int) <- from int to QString
  string = QString.toUtf8().constData() from QString to string
  QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,1))
  std::to_string(42) int to string
 */

QString loopYear(int ns, int year, int printcal, bool eudate)
{
      int days, mm2 = 12, dd2 = 31;
      int y1 = year/1000;
      int y2 = (year/100)-(y1*10);
      int y3 = (year-(y1*1000)-(y2*100))/10;
      int y4 = year-(y1*1000)-(y2*100)-(y3*10);
      QString Qdate,Qns,dd_mm,d_d_m_m,YY_yy,Y_Y_y_y,yy,y_y;
      QString buffer;

    // i --> Iterate through all the months
    // j --> Iterate through all the days of the
    //       month - i
    readsolarfile(dd,mm,year);
    //buffer = Qtotable("",1,0,0,0);
    for (int i = 0; i < mm2; i++)
    {
        //days = numberOfDays (i, year);
        QDate cd(year,i+1,1);
        days = cd.daysInMonth();
        int d1, d2, m1 ,m2;
        for (int j = 1; j <= days; j++)
        {
            if (j == dd2 && i == mm2) break;
              if (i > 8)
              {
                  m1 = 1;
                  m2 = i - 9;
              } else {
                  m1 = 0;
                  m2 = i + 1;
              }
            if (j > 29)
            {
                d2 = j-30;
                d1 = 3;
            } else if (j > 19)
            {
                d2 = j-20;
                d1 = 2;
            } else if (j > 9)
            {
                d2 = j-10;
                d1 = 1;
            } else
            {
                d2 = j;
                d1 = 0;
            }
            if (printcal == 1) {
          //      MainWindow::setui("hello");
            if (eudate) {
             Qdate = QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str() + " - ";
             d_d_m_m = QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0));
             dd_mm = "("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")" ;
            } else {
             Qdate = QString::fromStdString(getMonthName (i).c_str())+" "+QString::number(d1)+QString::number(d2) + " - ";
             d_d_m_m = QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0));
             dd_mm = "("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")";
            }
             Qns = QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1));

             YY_yy = "("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
             Y_Y_y_y = QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
             yy = "("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
             y_y = QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));

             if (ns == (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4) buffer += Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150) + Qtotable(dd_mm + "+" + YY_yy + "= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4) buffer += Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150) + Qtotable(dd_mm+ "+" +Y_Y_y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+y1+y2+y3+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(d_d_m_m+" + "+Y_Y_y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2+(y3*10)+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150) + Qtotable(dd_mm + "+" + yy+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+y3+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(d_d_m_m+" + "+y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150) + Qtotable(dd_mm +"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+(y1*10)+y2+(y3*10)+y4) buffer += Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(d_d_m_m+ " + " +YY_yy+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2+y3+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150) +Qtotable(dd_mm+"+"+y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+(y3*10)+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(d_d_m_m+" + "+yy+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == daynr((d1*10)+d2,(m1*10)+m2,year)) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(" It is the "+Qns+"th day of the year",0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == daynrleft((d1*10)+d2,(m1*10)+m2,year)) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable("There are "+Qns+" days left of the year",0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == eu_amdate(2, d1, d2, m1, m2, ns)) buffer+=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(Qns+" fits Europian style of date of dd/mm",0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == eu_amdate(1, d1, d2, m1, m2, ns)) buffer+=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,150)+Qtotable(Qns+" fits American style of date of mm/dd",0,0,1,300)+Qtotable("",0,2,0,0);

             if (searchzerodays(ns,1,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,1,"",eudate,false);
             }
             if (searchzerodays(ns,2,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,2,"",eudate,false);
             }
             if (searchzerodays(ns,3,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,3,"",eudate,false);
             }
             if (searchzerodays(ns,4,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,4,"",eudate,false);
             }
            }
        }
    }
    //qDebug() << buffer;
    //buffer += Qtotable("",2,0,0,0);
    return buffer;
}



QString gcalc(int dd, int mm, int year, int dd2, int mm2, int yy2,bool eudate) {
    int d1, d2, m1 ,m2, ns, daynumb, dayleft, wd1, wd2, nm1,nm2,days,daysbeetween;
    double w1;
    QString buffer;
    stringstream logline;
      int y1 = year/1000;
      int y2 = (year/100)-(y1*10);
      int y3 = (year-(y1*1000)-(y2*100))/10;
      int y4 = year-(y1*1000)-(y2*100)-(y3*10);
      readsolarfile(dd,mm,year);

    if (mm > 9)
          {
            m1 = 1;
            m2 = mm - 10;
          } else {
            m1 = 0;
            m2 = mm;
          }
    if (dd > 29)
          {
            d2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            d2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            d2 = dd-10;
            d1 = 1;
       } else
          {
            d2 = dd;
            d1 = 0;
          }
    daynumb = daynr(dd,mm,year);
    dayleft = daynrleft(dd,mm,year);
    days = spanofdate(dd2,mm2,yy2,dd,mm,year);
    wd1 = spanofdate(dd2,mm2,yy2,dd,mm,year)/7;
    w1 = spanofdate(dd2,mm2,yy2,dd,mm,year);
    w1 = w1/7-wd1;
    wd2 = round(w1*7);
    if (dd2<=dd) {
        nm1 = mm - mm2 + (year - yy2) *12;
        nm2 = spanofdate(dd2,mm,year,dd,mm,year);
    }
    else {
        nm1 = mm - mm2-1 + (year - yy2) *12;
        nm2 = spanofdate(dd2,mm-1,year,dd,mm,year);
    }

    logtime();
    logline.clear();
    if (eudate) logline << "<br>Date of " << dd << "/" << mm << "/" << year << " is " << formattext(dayname(dayNumber(dd,mm,year)),1,2) << "<br>";
    else logline << "<br>Date of " << mm << "/" << dd << "/" << year << " is " << formattext(dayname(dayNumber(dd,mm,year)),1,2) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
        ns = (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);

        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        //buffer += formattext(QString::fromStdString(logline.str()),0,1);
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+y1+y2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")&emsp;&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")&emsp;&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+20+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        logline.str("");


        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year" << " Prime? " << isprime(daynumb) << " Triangular? " << istriangular(daynumb) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Is the " << formattext(std::to_string(daynumb),1,1) << "th day of the year - " << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        if (searchzerodays(daynumb,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,1,"",eudate,true);
        if (searchzerodays(daynumb,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,2,"",eudate,true);
        if (searchzerodays(daynumb,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,3,"",eudate,true);
        if (searchzerodays(daynumb,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,4,"",eudate,true);
        savelog(logline.str());

        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year" << " Prime? " << isprime(dayleft) << " Triangular? " << istriangular(dayleft) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;There is " << formattext(std::to_string(dayleft),1,1) << " days left in the year - " << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        if (searchzerodays(dayleft,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,1,"",eudate,true);
        if (searchzerodays(dayleft,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,2,"",eudate,true);
        if (searchzerodays(dayleft,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,3,"",eudate,true);
        if (searchzerodays(dayleft,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,4,"",eudate,true);
        savelog(logline.str());
        logline.str("");
        if (dd2 != 0){
          if (eudate) {
              logline << "<br>" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;From " << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(days),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;From " << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks +" << formattext(std::to_string(wd2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;From " << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months +" << formattext(std::to_string(nm2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              } else {
              logline << "<br>" << getMonthName (mm-1).c_str() << " " << d1 << d2 << " - &emsp;From " << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(days),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << " - &emsp;From " << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks +" << formattext(std::to_string(wd2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << " - &emsp;From " << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months +" << formattext(std::to_string(nm2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");}
                daynumb = daynr(dd2,mm2,year);
                dayleft = daynrleft(dd2,mm2,year);
                if (eudate) {
                    logline << dd2 << " " << getMonthName (mm2-1).c_str() << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    logline << dd2 << " " << getMonthName (mm2-1).c_str() << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    } else {
                    logline << getMonthName (mm2-1).c_str() << " " << dd2 << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    logline << getMonthName (mm2-1).c_str() << " " << dd2 << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");}
                dayleft = daynr(dd,mm,year);
                if (dayleft > daynumb) daysbeetween = dayleft - daynumb;
                else daysbeetween = daynumb-dayleft;
                wd1 = daysbeetween/7;
                w1 = daysbeetween;
                w1 = w1/7-wd1;
                wd2 = round(w1*7);
                if (dayleft > daynumb) {
                    if (dd2 <= dd) {
                     nm1 = mm - mm2;
                     nm2 = spanofdate(dd2,mm2+nm1,year,dd,mm,year);
                    }
                    else {
                     nm1 = mm - mm2-1;
                     nm2 = spanofdate(dd2,mm2+nm1,year,dd,mm,year);
                    }
                }
                else {
                    if (dd2 < dd) {
                     nm1 = mm2 - mm-1;
                     nm2 = spanofdate(dd,mm+nm1,year,dd2,mm2,year);
                    }
                    else {
                     nm1 = mm2 - mm;
                     nm2 = spanofdate(dd,mm+nm1,year,dd2,mm2,year);
                    }
                }
                if (eudate) {
                    if (dayleft > daynumb) logline << "From " << dd << " " << getMonthName (mm2-1).c_str() << " to " << dd2 << " " << getMonthName (mm-1).c_str() << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    else logline << "From " << dd << " " << getMonthName (mm-1).c_str() << " to " << dd2 << " " << getMonthName (mm2-1).c_str() << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    } else {
                    if (dayleft > daynumb) logline << "From " << getMonthName (mm2-1).c_str() << " " << dd << " to " << getMonthName (mm-1).c_str() << " " << dd2 << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days or " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    else logline << "From " << getMonthName (mm-1).c_str() << " " << dd << " to " << getMonthName (mm2-1).c_str() << " " << dd2 << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                }
        }
        //qDebug() << buffer;
    return buffer;
}

QString add_dots(QString str,int length)
{
    int i;

    QByteArray bytes = str.toUtf8();
    int lengt = bytes.size(); //Number of bytes
    for (i=0;i<length-lengt-1;i++) str += "-";
    str += ">";
    return str;
}

int weeks_months(QDate startDate, QDate endDate, bool months, bool days)
{
    int wd1,wd2,nm1,nm2=0,y,m,startmonth=startDate.month(),endmonth=endDate.month();
    double w1;
    QDateTime qstartDate(startDate);
    QDateTime qendDate(endDate);
    if (!qstartDate.isValid()) qDebug() << " invalid start date ";
    if (!qendDate.isValid()) qDebug() << " invalid end date ";
    nm1 = startDate.daysTo(endDate);
    for (y=startDate.year();y<=endDate.year();y++){
        if (y>startDate.year()) {
            startmonth = 1;
            endmonth = 12;
        }
        if (y<endDate.year()) endmonth = 12;
        if (y==endDate.year()) endmonth=endDate.month()-1;

        for (m=startmonth;m<=endmonth;m++)
        {
            if (nm1 > 30){
              nm1 -=numberOfDays(m-1,y);
              nm2++;
            }

        }
    }
    wd1 = startDate.daysTo(endDate)/7;
    w1 = startDate.daysTo(endDate);
    w1 = w1/7-wd1;
    wd2 = round(w1*7);
   // qDebug() << count2 << " " << counter;
    //qDebug() << " weeks " << wd1 << " weeks " << wd2 << " days, " << nm2 << " months " << nm1 << " days";
    if (months && !days) return nm2;  // number of months
    if (months && days) return nm1;   // number of days left from months
    if (!months && !days) return wd1; // number of weeks
    if (!months && days) return wd2;  // number of days left from weeks
    return 0;
}


void readsolarfile(int dd, int mm, int year)
{
    int counter=0;
    string line;
    ifstream myfile;
    stringstream ss;

    myfile.open("solareclipse.txt");
    if (myfile.is_open())
          {
          while ( getline (myfile,line) )
            {

        if (QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))) > QDate(year, mm, dd)) {
            //if (weeks_months(QDate(year, mm, dd),QDate(fields[0].toInt(), fields[1].toInt(), fields[2].toInt()),false,true) == 0) {*/
                zerodays[0][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),false,false);
                zerodays[1][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),false,true);
                zerodays[2][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),true,false);
                zerodays[3][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),true,true);
                zerodays[4][counter] =  stoi(line.substr(8,2)) ;
                zerodays[5][counter] = stoi(line.substr(5,2)) ;
                zerodays[6][counter] = stoi(line.substr(0,4)) ;
                if (line.substr(11,1) == "T") zerodays[7][counter] = 1;
                if (line.substr(11,1) == "A") zerodays[7][counter] = 2;
                if (line.substr(11,1) == "P") zerodays[7][counter] = 3;
                if (line.substr(11,1) == "H") zerodays[7][counter] = 4;
                counter ++;
           // qDebug() << zerodays[0][counter] << " " << zerodays[1][counter] << " " << zerodays[2][counter] << " " << zerodays[3][counter] << " " << zerodays[4][counter] << " " << zerodays[5][counter] << " " << counter2;
            //qDebug() << fields[2].toInt() << " " << fields[1].toInt() << " " << fields[0].toInt() << " " << fields[3] << " " << line;

           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 && startoffile) break;
           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 ) startoffile=true;



            //if (counter == 50) break;

        }
        if (QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))) < QDate(year, mm, dd)) {
            //if (weeks_months(QDate(year, mm, dd),QDate(fields[0].toInt(), fields[1].toInt(), fields[2].toInt()),false,true) == 0) {*/
                zerodays[0][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),false,false);
                zerodays[1][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),false,true);
                zerodays[2][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),true,false);
                zerodays[3][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),true,true);
                zerodays[4][counter] = stoi(line.substr(8,2)) ;
                zerodays[5][counter] = stoi(line.substr(5,2)) ;
                zerodays[6][counter] = stoi(line.substr(0,4)) ;
                if (line.substr(11,1) == "T") zerodays[7][counter] = 1;
                if (line.substr(11,1) == "A") zerodays[7][counter] = 2;
                if (line.substr(11,1) == "P") zerodays[7][counter] = 3;
                if (line.substr(11,1) == "H") zerodays[7][counter] = 4;
                counter ++;
           // qDebug() << zerodays[0][counter] << " " << zerodays[1][counter] << " " << zerodays[2][counter] << " " << zerodays[3][counter] << " " << zerodays[4][counter] << " " << zerodays[5][counter] << " " << counter2;
            //qDebug() << fields[2].toInt() << " " << fields[1].toInt() << " " << fields[0].toInt() << " " << fields[3] << " " << line;

           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 && startoffile) break;
           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 ) startoffile=true;



            //if (counter == 50) break;

        }

    }
   }
    myfile.close();
    for (counter=0;counter<250;counter++) {
    ss << "Counter " << counter << " - Weeks " << zerodays[0][counter] << " and days " << zerodays[1][counter] << " - months " << zerodays[2][counter] << " and days " << zerodays[3][counter]
          << " - date " << zerodays[4][counter] << "/" << zerodays[5][counter] << "/" << zerodays[6][counter] << " - type " << zerodays[7][counter] << "\n";
   savetotemp(ss.str());
   ss.str("");
}
}


QString solareclipe(int dd,int mm,int year, int output, int type,bool eudate)
{
    QString buffer,line,solartype,solartype2,solartypeweeksbefore,solartypemonthsbefore,solartypeweeksafter,solartypemonthsafter,typetmp;

    int sdd2=0,smm2=0,syear2=0,sdd1=0,smm1=0,syear1=0,counter=0,solartype2int,solartypeint=0,solartmp;
    int ddmonthsbeforezero=0,mmmonthsbeforezero=0,yearmonthsbeforezero=0,ddweeksbeforezero=0,mmweeksbeforezero=0,yearweeksbeforezero=0;
    int ddmonthsafterzero=0,mmmonthsafterzero=0,yearmonthsafterzero=0,ddweeksafterzero=0,mmweeksafterzero=0,yearweeksafterzero=0;
    int beforeweeks=0,beforeweekdays=0,afterweeks=0,afterweeksdays=0,beforemonths=0,beforemonthsdays=0,aftermonths=0,aftermonthsdays=0,solartypeweeksbeforeint=0,solartypemonthsbeforeint=0,solartypeweeksafterint=0,solartypemonthsafterint=0;
    int beforeweekszero=0,beforeweekdayszero=0,afterweekszero=0,afterweeksdayszero=0,beforemonthszero=0,beforemonthszerodays=0,aftermonthszero=0,aftermonthsdayszero=0;
    int eudd,eumm,eusdd1,eusmm1,eusdd2,eusmm2,euddweeksbeforezero,eummweeksbeforezero,euddweeksafterzero,eummweeksafterzero,euddmonthsbeforezero,eummmonthsbeforezero,euddmonthsafterzero,eummmonthsafterzero;
    bool foundafter=false,foundafterweek=false,foundaftermonths=false;
    solartype2int=type;
    readsolarfile(dd,mm,year);

    while (counter < 250) {

        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && !foundafter && (type == zerodays[7][counter] || type == 5)) {

                sdd1=zerodays[4][counter];smm1=zerodays[5][counter];syear1=zerodays[6][counter];solartypeint=zerodays[7][counter];
                 beforeweeks=zerodays[0][counter];beforeweekdays=zerodays[1][counter];
                 beforemonths=zerodays[2][counter];beforemonthsdays=zerodays[3][counter];

        }
        if (zerodays[1][counter] == 0 && !foundafterweek && QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && (type == zerodays[7][counter] || type == 5)) {
            ddweeksbeforezero=zerodays[4][counter];mmweeksbeforezero=zerodays[5][counter];yearweeksbeforezero=zerodays[6][counter];solartypeweeksbeforeint=zerodays[7][counter];
            beforeweekszero=zerodays[0][counter];
            beforeweekdayszero=zerodays[1][counter];
             //qDebug() << "Weeks before " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,false) << " days " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,true) << " " << ddweeksbeforezero << " " << mmweeksbeforezero << " " << yearweeksbeforezero;
        } //found weeks before with zero days
        if (zerodays[3][counter] == 0 && !foundaftermonths && QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && (type == zerodays[7][counter] || type == 5)) {
            ddmonthsbeforezero=zerodays[4][counter];mmmonthsbeforezero=zerodays[5][counter];yearmonthsbeforezero=zerodays[6][counter];solartypemonthsbeforeint=zerodays[7][counter];
            beforemonthszero=zerodays[2][counter];
            beforemonthszerodays=zerodays[3][counter];

            //qDebug() << "Months before " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,false) << " days " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,true) << " " << ddmonthsbeforezero << " " << mmmonthsbeforezero << " " << yearmonthsbeforezero;
        } //found months before with zero days
     counter ++;
     if (zerodays[6][counter] != 0) {
     solartmp = zerodays[7][counter];


    if (type == solartmp || type == 5) {
     if (!foundafter) {
         afterweeks=zerodays[0][counter];
         afterweeksdays =zerodays[1][counter];
         aftermonths=zerodays[2][counter];
         aftermonthsdays=zerodays[3][counter];
         sdd2 = zerodays[4][counter];
         smm2 = zerodays[5][counter];
         syear2 = zerodays[6][counter];
         solartype2int = zerodays[7][counter];
     }
     if (!foundafterweek) {
         afterweekszero=zerodays[0][counter];
         afterweeksdayszero =zerodays[1][counter];
         ddweeksafterzero = zerodays[4][counter];
         mmweeksafterzero = zerodays[5][counter];
         yearweeksafterzero = zerodays[6][counter];
         solartypeweeksafterint = zerodays[7][counter];
     }
     if (!foundaftermonths) {
         aftermonthszero=zerodays[2][counter];
         aftermonthsdayszero=zerodays[3][counter];
         ddmonthsafterzero = zerodays[4][counter];
         mmmonthsafterzero = zerodays[5][counter];
         yearmonthsafterzero = zerodays[6][counter];
         solartypemonthsafterint = zerodays[7][counter];
     }


    }
    if (QDate(syear2, smm2, sdd2) > QDate(year, mm, dd)) foundafter=true; // found date after

    if (zerodays[1][counter] == 0 && !foundafterweek && QDate(zerodays[6][counter],zerodays[5][counter],zerodays[4][counter]) > QDate(year, mm, dd) && (type == solartmp || type == 5))  //found weeks after with zero days
           foundafterweek=true;
    else if (!foundafterweek) ddweeksafterzero =0;

        //qDebug() << "Weeks after " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,false) << " days " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,true) << " " << ddweeksafterzero << " " << mmweeksafterzero << " " << yearweeksafterzero;
        //break;
    if (zerodays[3][counter] == 0 && !foundaftermonths && QDate(zerodays[6][counter],zerodays[5][counter],zerodays[4][counter]) > QDate(year, mm, dd) && (type == solartmp || type == 5))  //found months after with zero days
            foundaftermonths=true;
    else if (!foundaftermonths) ddmonthsafterzero = 0;

    }
}

   // qDebug() << "Weeks before " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,false) << " days " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,true) << " " << ddweeksbeforezero << " " << mmweeksbeforezero << " " << yearweeksbeforezero;
   // qDebug() << "Months before " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,false) << " days " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,true) << " " << ddmonthsbeforezero << " " << mmmonthsbeforezero << " " << yearmonthsbeforezero;
   // qDebug() << "Weeks after " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,false) << " days " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,true) << " " << ddweeksafterzero << " " << mmweeksafterzero << " " << yearweeksafterzero;
   // qDebug() << "Months after" << weeks_months(QDate(year, mm, dd),QDate(yearmonthsafterzero, mmmonthsafterzero, ddmonthsafterzero),true,false) << " days " << weeks_months(QDate(year, mm, dd),QDate(yearmonthsafterzero, ddmonthsafterzero, ddmonthsafterzero),true,true) << " " << ddmonthsafterzero << " " << ddmonthsafterzero << " " << yearmonthsafterzero;
    if (solartypeint == 1) solartype = "Total";
    if (solartypeint == 2) solartype = "Annular";
    if (solartypeint == 3) solartype = "Partial";
    if (solartypeint == 4) solartype = "Hybrid";
    if (solartype2int == 1) solartype2 = "Total";
    if (solartype2int == 2) solartype2 = "Annular";
    if (solartype2int == 3) solartype2 = "Partial";
    if (solartype2int == 4) solartype2 = "Hybrid";
    if (solartypeweeksbeforeint == 1) solartypeweeksbefore = "Total";
    if (solartypeweeksbeforeint == 2) solartypeweeksbefore = "Annular";
    if (solartypeweeksbeforeint == 3) solartypeweeksbefore = "Partial";
    if (solartypeweeksbeforeint == 4) solartypeweeksbefore = "Hybrid";
    if (solartypemonthsbeforeint == 1) solartypemonthsbefore = "Total";
    if (solartypemonthsbeforeint == 2) solartypemonthsbefore = "Annular";
    if (solartypemonthsbeforeint == 3) solartypemonthsbefore = "Partial";
    if (solartypemonthsbeforeint == 4) solartypemonthsbefore = "Hybrid";
    if (solartypeweeksafterint == 1) solartypeweeksafter = "Total";
    if (solartypeweeksafterint == 2) solartypeweeksafter = "Annular";
    if (solartypeweeksafterint == 3) solartypeweeksafter = "Partial";
    if (solartypeweeksafterint == 4) solartypeweeksafter = "Hybrid";
    if (solartypemonthsafterint == 1) solartypemonthsafter = "Total";
    if (solartypemonthsafterint == 2) solartypemonthsafter = "Annular";
    if (solartypemonthsafterint == 3) solartypemonthsafter = "Partial";
    if (solartypemonthsafterint == 4) solartypemonthsafter = "Hybrid";
    if (type == 1) typetmp = "Total";
    if (type == 2) typetmp = "Partial";
    if (type == 3) typetmp = "Annular";
    if (type == 4) typetmp = "Hybrid";
    if (type == 5) typetmp = "Any";
    QDateTime startDate(QDate(syear1, smm1, sdd1));
    QDateTime currentDate(QDate(year, mm, dd));
    QDateTime endDate(QDate(syear2, smm2, sdd2));
    QDateTime startDateweeksbefore(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero));
    QDateTime startDateweeksafter(QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero));
    QDateTime startDatemonthsbefore(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero));
    QDateTime startDatemonthsafter(QDate(yearmonthsafterzero, mmmonthsafterzero, ddmonthsafterzero));

    if (eudate) {
        eudd = dd;
        eumm = mm;
        eusdd1 = sdd1;
        eusmm1 = smm1;
        eusdd2 = sdd2;
        eusmm2 = smm2;
        euddweeksbeforezero = ddweeksbeforezero;
        eummweeksbeforezero = mmweeksbeforezero;
        euddweeksafterzero = ddweeksafterzero;
        eummweeksafterzero = mmweeksafterzero;
        euddmonthsbeforezero = ddmonthsbeforezero;
        eummmonthsbeforezero = mmmonthsbeforezero;
        euddmonthsafterzero = ddmonthsafterzero;
        eummmonthsafterzero = mmmonthsafterzero;
    } else {
        eudd = mm;
        eumm = dd;
        eusdd1 = smm1;
        eusmm1 = sdd1;
        eusdd2 = smm2;
        eusmm2 = sdd2;
        euddweeksbeforezero = mmweeksbeforezero;
        eummweeksbeforezero = ddweeksbeforezero;
        euddweeksafterzero = mmweeksafterzero;
        eummweeksafterzero = ddweeksafterzero;
        euddmonthsbeforezero = mmmonthsbeforezero;
        eummmonthsbeforezero = ddmonthsbeforezero;
        euddmonthsafterzero = mmmonthsafterzero;
        eummmonthsafterzero = ddmonthsafterzero;
    }

    switch (output) {
    case 1: {// print out to textbrowser

        buffer += "<br>Searching after " + typetmp +" Solar Eclipse<br>";
        buffer += "<br>Current working date : " + QString::fromStdString(formattext(QString::number(eudd).toUtf8().constData(),1,1)) + "/" + QString::fromStdString(formattext(QString::number(eumm).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(year).toUtf8().constData(),1,1)) + "<br>"; // display entered currrent date

        buffer += "<br>Last " + solartype + " Solar Eclipse : " + QString::fromStdString(formattext(QString::number(eusdd1).toUtf8().constData(),1,1)) + "/" + QString::fromStdString(formattext(QString::number(eusmm1).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(syear1).toUtf8().constData(),1,1)) + "<br>Next " + solartype2 + " Solar Eclipse : " + QString::fromStdString(formattext(QString::number(eusdd2).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eusmm2).toUtf8().constData(),1,1)) + "/" + QString::fromStdString(formattext(QString::number(syear2).toUtf8().constData(),1,1));
        buffer += "<br><br>From last " + solartype + " Solar eclipse to current date are: " + QString::fromStdString(formattext(QString::number(startDate.daysTo(currentDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(beforeweeks).toUtf8().constData(),1,1)) + " Weeks and "
                + QString::fromStdString(formattext(QString::number(beforeweekdays).toUtf8().constData(),1,1)) + " days or<br>" //weeks ends
                + QString::fromStdString(formattext(QString::number(beforemonths).toUtf8().constData(),1,1))  + " Months and "
                + QString::fromStdString(formattext(QString::number(beforemonthsdays).toUtf8().constData(),1,1)) + " days<br><br>"; //before ends
        buffer += "From current date to next " + solartype2 + " Solar eclipse are: " + QString::fromStdString(formattext(QString::number(currentDate.daysTo(endDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(afterweeks).toUtf8().constData(),1,1)) + " weeks and "
                + QString::fromStdString(formattext(QString::number(afterweeksdays).toUtf8().constData(),1,1)) + " days or<br>" //weeks ends
                + QString::fromStdString(formattext(QString::number(aftermonths).toUtf8().constData(),1,1)) + " months and "
                + QString::fromStdString(formattext(QString::number(aftermonthsdays).toUtf8().constData(),1,1)) + " days.<br><br>"; //after ends

        if (ddweeksbeforezero > 0)
        buffer += "<br>Last " + solartypeweeksbefore + " Solar Eclipse with zero days after week: " + QString::fromStdString(formattext(QString::number(euddweeksbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummweeksbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearweeksbeforezero).toUtf8().constData(),1,1));
        if (ddweeksafterzero > 0)
        buffer += "<br>Next " + solartypeweeksafter + " Solar Eclipse with zero days after week: " + QString::fromStdString(formattext(QString::number(euddweeksafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummweeksafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearweeksafterzero).toUtf8().constData(),1,1)) + "<br><br>";
        if (ddweeksbeforezero > 0)
        buffer += "From last " + solartypeweeksbefore + " Solar eclipse to current date with zero days after week are: " + QString::fromStdString(formattext(QString::number(startDateweeksbefore.daysTo(currentDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(beforeweekszero).toUtf8().constData(),1,1)) + " Weeks and "
                + QString::fromStdString(formattext(QString::number(beforeweekdayszero).toUtf8().constData(),1,1)) + " days<br>"; //weeks before with zero days
        if (ddweeksafterzero > 0)
        buffer += "From current date to next " + solartypeweeksafter + " Solar eclipse are: " + QString::fromStdString(formattext(QString::number(currentDate.daysTo(startDateweeksafter)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(afterweekszero).toUtf8().constData(),1,1)) + " weeks and "
                + QString::fromStdString(formattext(QString::number(afterweeksdayszero).toUtf8().constData(),1,1)) + " days<br><br>"; //weeks after with zero days

        if (ddmonthsbeforezero > 0)
        buffer += "<br>Last " + solartypemonthsbefore + " Solar Eclipse with zero days after month: " + QString::fromStdString(formattext(QString::number(euddmonthsbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummmonthsbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearmonthsbeforezero).toUtf8().constData(),1,1));
        if (ddmonthsafterzero > 0)
        buffer += "<br>Next " + solartypemonthsafter + " Solar Eclipse with zero days after month: " + QString::fromStdString(formattext(QString::number(euddmonthsafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummmonthsafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearmonthsafterzero).toUtf8().constData(),1,1)) + "<br><br>";
        if (ddmonthsbeforezero > 0)
        buffer += "From last " + solartypemonthsbefore + " Solar eclipse to current date with zero days after month are: " + QString::fromStdString(formattext(QString::number(startDatemonthsbefore.daysTo(currentDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(beforemonthszero).toUtf8().constData(),1,1)) + " months and "
                + QString::fromStdString(formattext(QString::number(beforemonthszerodays).toUtf8().constData(),1,1)) + " days<br>"; //months before with zero days
        if (ddmonthsafterzero > 0)
        buffer += "From current date to next " + solartypemonthsafter + " Solar eclipse are: " + QString::fromStdString(formattext(QString::number(currentDate.daysTo(startDatemonthsafter)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(aftermonthszero).toUtf8().constData(),1,1)) + " months and "
                + QString::fromStdString(formattext(QString::number(aftermonthsdayszero).toUtf8().constData(),1,1)) + " days<br><br>"; //months after with zero days
        break;
    }
  }
    return buffer;
}

int searchzerodays(int ns,int type,int dd,int mm, int year)
{
    int counter;

    for (counter=0;counter<250;counter++) {
        if (dd==0) {
        if (zerodays[0][counter] == ns && (zerodays[7][counter] == type || type == 5) && zerodays[1][counter] == 0) return 1;
        if (zerodays[2][counter] == ns && (zerodays[7][counter] == type || type == 5) && zerodays[3][counter] == 0) return 2;
        //if (ns == 303) qDebug() << ns;
        //if (zerodays[0][counter] == 303) qDebug() << " zerodays " << zerodays[0][counter];
        } else {

        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) > QDate(year, mm, dd) && zerodays[7][counter] == type) {

                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,true) == 0) return ns;
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,true) == 0) return ns;


        }
        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && zerodays[7][counter] == type) {
            if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,false) == ns &&
                             weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,true) == 0) return ns;
            if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,false) == ns &&
                             weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,true) == 0) return ns;
        }
        }
       }
    return 0;
}


QString solar2history(int dd, int mm, int year, int type, bool eudate)
{
    string line;
    //type =2;
    QString buffer;
    stringstream logline;
    int lines=0;
    ifstream myfile;
    readsolarfile(dd,mm,year);
    buffer += "Searching History.txt for match where Phrase is connected to Solar Eclipse and current date of " + QString::number(dd) + "/" +QString::number(mm) + "/" + QString::number(year) + "<br>";
    myfile.open("history.txt");
    if (myfile.is_open())
          {

          while ( getline (myfile,line) )
            {
                ns = getwordnumericvalue(line,0,0,0);
                if (searchzerodays(ns,type,0,0,0) > 0){
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - English Ordinal",eudate,true);
                }
                ns = getwordnumericvalue(line,1,0,0);
                if (searchzerodays(ns,type,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Full Reduction",eudate,true);
                }
                ns = getwordnumericvalue(line,0,1,0);
                if (searchzerodays(ns,type,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Reverse Ordinal",eudate,true);
                }
                ns = getwordnumericvalue(line,1,1,0);
                if (searchzerodays(ns,type,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Reverse full Reduction",eudate,true);
                }
                if (single_r_on) {
                    ns = getwordnumericvalue(line,0,0,1);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Single Reduction",eudate,true);
                    }
                }
                if (francis_on) {
                    ns = getwordnumericvalue(line,0,0,2);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Francis Bacon",eudate,true);
                    }
                }
                if (satanic_on) {
                    ns = getwordnumericvalue(line,0,0,3);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Satanic",eudate,true);
                    }
                }
                if (jewish_on) {
                    ns = getwordnumericvalue(line,0,0,4);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,1,1) + " - Jewish",eudate,true);
                    }
                }

            }

     buffer += "<br>" + QString::fromStdString(formattext(std::to_string(lines),1,1)) + " words found matching Solar Eclipses <br><br>";
     // buffer += QString::fromStdString(logline.str());
      logline << buffer.toStdString();
      logtime();
      savelog(logline.str());

    myfile.close();
   }

  else buffer += "Unable to open file";
  return buffer;
}

QString printzerodays(int dd, int mm, int year, int ns, int type, string detail, bool eudate,bool read)
{
    int counter;
    bool printbuffer=false;
    stringstream logline;
    QString buffer,solartype, se1,se2,se3,se4,se5,se6,se7;
    eraseAllSubStr(detail,"th ");
    //if (eudate) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << " There was a Annular Solar Eclipse exacly " << formattext(std::to_string(ns),1,1) << " weeks ago - " << detail << "<br>";
    if (eudate) se7 = QString::number(dd) + " " +  getMonthName (mm-1).c_str() + " - ";
    else se7 = QString::fromStdString(getMonthName (mm-1).c_str()) + " " +  QString::number(dd) + " - ";
    for (counter=0;counter<250;counter++) {
        if (detail == "listsolareclipses" && read) {
            se1 = "";
            se2 = "";
            se3 = " on ";
            if (eudate) {
                se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
            } else {
                se5 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                se4 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
            }
            se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
            if (zerodays[7][counter] == 1) solartype = "Total";
            if (zerodays[7][counter] == 2) solartype = "Annular";
            if (zerodays[7][counter] == 3) solartype = "Partial";
            if (zerodays[7][counter] == 4) solartype = "Hybrid";
            if ((zerodays[7][counter] == type || type == 5) && zerodays[4][counter] != 0) buffer += solartype + " Solar Eclipse " + se3 + se2 + se4 + "/" + se5 + "/" + se6 +"<br>";
        }
        else if (((zerodays[0][counter] == ns && (zerodays[7][counter] == type || type == 5)) || (zerodays[2][counter] == ns && (zerodays[7][counter] == type || type == 5))) && read) {
            se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
            se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
            se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
            if (zerodays[7][counter] == 1) solartype = "Total";
            if (zerodays[7][counter] == 2) solartype = "Annular";
            if (zerodays[7][counter] == 3) solartype = "Partial";
            if (zerodays[7][counter] == 4) solartype = "Hybrid";
            if (QDate(zerodays[6][counter],zerodays[5][counter],zerodays[4][counter]) < QDate(year, mm, dd)) {
            if (zerodays[0][counter] == ns) {
                se1 = " - There was a ";
                se2 = " weeks ago on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[0][counter]),1,1));
                printbuffer = true;
           } else {
                se1 = " - There was a ";
                se2 = " months ago on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
                printbuffer = true;
            }
          } else {
            if (zerodays[0][counter] == ns) {
                se1 = " - There is a ";
                se2 = " weeks from now on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[0][counter]),1,1));
                printbuffer = true;
           } else {
                se1 = " - There is a ";
                se2 = " months from now on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
                printbuffer = true;
                }
            }
        } else {

            if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) > QDate(year, mm, dd) && zerodays[7][counter] == type) {
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,true) == 0) {
                    se1 = " - There is a ";
                   se2 = " weeks from now on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,true) == 0) {
                    se1 = " - There is a ";
                    se2 = " months from now on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
        }
            if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && zerodays[7][counter] == type) {
                if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,false) == ns &&
                                 weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,true) == 0) {
                    se1 = " - There was a ";
                    se2 = " weeks ago on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
                if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,false) == ns &&
                                 weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,true) == 0) {
                    se1 = " - There was a ";
                    se2 = " months ago on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
          }
        }
    }
    if (printbuffer) {
        buffer += Qtotable("",1,0,0,0);
        if (eudate) buffer += Qtotable("",0,1,0,0)+Qtotable(se7,0,0,1,150) + Qtotable(solartype + " Solar Eclipse " + se3 + se2 + se4 + "/" + se5 + "/" + se6 + QString::fromStdString(detail),0,0,1,1000)+Qtotable("",0,2,0,0);
        else buffer += Qtotable("",0,1,0,0)+Qtotable(se7,0,0,1,150) + Qtotable(solartype + " Solar Eclipse " + se3 + se2 + se5 + "/" + se4 + "/" + se6 + QString::fromStdString(detail),0,0,1,1000)+Qtotable("",0,2,0,0);
        buffer += Qtotable("",2,0,0,0);
    }
    return buffer;
}

bool phrasetodate(int ns, int dd, int mm, int year, int i) {
    int d1, d2, m1 ,m2,daynumb,dayleft,eudate,amdate;
    int y1 = year/1000;
    int y2 = (year/100)-(y1*10);
    int y3 = (year-(y1*1000)-(y2*100))/10;
    int y4 = year-(y1*1000)-(y2*100)-(y3*10);


    QString qns;

    if (mm > 9)
          {
            m1 = 1;
            m2 = mm - 10;
          } else {
            m1 = 0;
            m2 = mm;
          }
    if (dd > 29)
          {
            d2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            d2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            d2 = dd-10;
            d1 = 1;
       } else
          {
            d2 = dd;
            d1 = 0;
          }
    daynumb = daynr(dd,mm,year);
    dayleft = daynrleft(dd,mm,year);
    eudate = eu_amdate(2, d1, d2, m1, m2,ns);
    amdate = eu_amdate(1, d1, d2, m1, m2,ns);

    //cout << eudate << "-" << amdate << "<br>";
   switch(i) {
      case 1 :
        if (ns == (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4) return true;
        break;
      case 2 :
        if (ns == (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4) return true;
        break;
      case 3 :
        if (ns == d1+d2+m1+m2+y1+y2+y3+y4) return true;
        break;
      case 4 :
        if (ns == (d1*10)+d2+(m1*10)+m2+(y3*10)+y4) return true;
        break;
      case 5 :
        if (ns == d1+d2+m1+m2+y3+y4) return true;
        break;
      case 6 :
        if (ns == (d1*10)+d2+(m1*10)+m2) return true;
        break;
      case 7 :
        if (ns == d1+d2+m1+m2+(y1*10)+y2+(y3*10)+y4) return true;
        break;
      case 8 :
        if (ns == (d1*10)+d2+(m1*10)+m2+y3+y4) return true;
        break;
      case 9 :
        if (ns == d1+d2+m1+m2+(y3*10)+y4) return true;
        break;
      case 10 :
        if (ns == daynumb) return true;
        break;
      case 11 :
        if (ns == dayleft) return true;
        break;
      case 12 :
        if (ns == eudate) return true;
        break;
      case 13 :
        if (ns == amdate) return true;
        break;
      case 14 :
        if (ns == getnprime(daynumb)) return true;
        break;
      case 15 :
        if (ns == getnprime(dayleft)) return true;
        break;
      case 16 :
        if (ns == getnprime(eudate)) return true;
        break;
      case 17 :
        if (ns == getnprime(amdate)) return true;
        break;
      case 18 :
        if (ns == getntriangular(daynumb)) return true;
        break;
      case 19 :
        if (ns == getntriangular(dayleft)) return true;
        break;
      case 20 :
        if (ns == getntriangular(eudate)) return true;
        break;
      case 21 :
        if (ns == getntriangular(amdate)) return true;
        break;
   case 22 :
     if (searchzerodays(ns,1,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 23 :
     if (searchzerodays(ns,2,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 24 :
     if (searchzerodays(ns,3,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 25 :
     if (searchzerodays(ns,4,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;

   }
 return false;
}

QString print_p_to_d(int ns, int dd, int mm, int year, int i, string detail, bool eudate) {
    int d1, d2, m1 ,m2;
    QString buffer;
    QString Qdate,Qns,dd_mm,d_d_m_m,YY_yy,Y_Y_y_y,yy,y_y;
    int y1 = year/1000;
    int y2 = (year/100)-(y1*10);
    int y3 = (year-(y1*1000)-(y2*100))/10;
    int y4 = year-(y1*1000)-(y2*100)-(y3*10);
    stringstream logline;
    logline.str("");

    if (mm > 9)
          {
            m1 = 1;
            m2 = mm - 10;
          } else {
            m1 = 0;
            m2 = mm;
          }
    if (dd > 29)
          {
            d2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            d2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            d2 = dd-10;
            d1 = 1;
       } else
          {
            d2 = dd;
            d1 = 0;
          }

        if (eudate) {
         Qdate = QString::number(d1)+QString::number(d2)+" "+getMonthName (mm-1).c_str() + " - ";
         d_d_m_m = QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0));
         dd_mm = "("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")" ;
        } else {
         Qdate = QString::fromStdString(getMonthName (mm-1).c_str())+" "+QString::number(d1)+QString::number(d2) + " - ";
         d_d_m_m = QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0));
         dd_mm = "("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")";
        }
         Qns = QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1));

         YY_yy = "("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
         Y_Y_y_y = QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
         yy = "("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
         y_y = QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));









       switch(i) {
          case 1 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << YY_yy.toUtf8().constData() << ")&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 2 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << Y_Y_y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 3 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << Y_Y_y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 4 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << yy.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 5 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 6 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 7 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << YY_yy.toUtf8().constData() << ")&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 8 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 9 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << yy.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 10 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Is the " << Qns.toUtf8().constData() << "th day of the year - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 11 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;There is " << Qns.toUtf8().constData() << " days left of the year - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 12 :
              logline << Qdate.toUtf8().constData() << " &emsp;" <<  "Matches European style date as " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 13 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << "Matches American style date as " << Qns.toUtf8().constData() << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 14 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Day of the year is " << Qns.toUtf8().constData() << "th prime number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 15 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Days left in the year is " << Qns.toUtf8().constData() << "th prime number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 16 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(2, d1, d2, m1, m2,ns)),1,1) << " is " << Qns.toUtf8().constData() << "th prime number from European style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 17 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(1, d1, d2, m1, m2,ns)),1,1) << " is " << Qns.toUtf8().constData() << "th prime number from American style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 18 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Day of the year is " << Qns.toUtf8().constData() << "th triangular number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 19 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Days left in the year is " << Qns.toUtf8().constData() << "th triangular number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 20 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(2, d1, d2, m1, m2,ns)),1,1) << " is " << Qns.toUtf8().constData() << "th triangular number from European style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 21 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(1, d1, d2, m1, m2, ns)),1,1) << " is " << Qns.toUtf8().constData() << "th triangular number from American style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
       case 22 :
           //logline << printzerodays(dd,mm,year,ns,1);
           buffer += printzerodays(dd,mm,year,ns,1," - " + detail,eudate,true); // printing of phrase compare to solar eclipse
           savelog(logline.str());
           break;
       case 23 :
           //logline << searchzerodays(ns,2);
           buffer += printzerodays(dd,mm,year,ns,2," - " + detail,eudate,true);
           savelog(logline.str());
           break;
       case 24 :
           //logline << searchzerodays(ns,3);
           buffer += printzerodays(dd,mm,year,ns,3," - " + detail,eudate,true);
           savelog(logline.str());
           break;
       case 25 :
           //logline << searchzerodays(ns,4);
           buffer += printzerodays(dd,mm,year,ns,4," - " + detail,eudate,true);
           savelog(logline.str());
           break;

       }
       return buffer;
}


QString runanalyze(int dd, int mm, int year, string phrase,bool hlist, int filter, bool eudate) {
    stringstream ex1, logline;
    QString buffer,qt1;
    bool found=false,used=true;
    int i,ns=0;
    ex1.str("");
    found = false;
    if (!hlist) {
        logtime();
        logline.str("");
        logline << "<br>Statistic for " << formattext(phrase,2,3) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());
    }
    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare cifers to date
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==1||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," English Ordinal",eudate);
        found = true;
    }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==2||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Full Reduction",eudate);
        found = true;
    }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==3||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Ordinal",eudate);
        found = true;
    }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==4||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Full Reduction",eudate);
        found = true;
    }
    if (filter==5||!hlist) ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==5||!hlist) && single_r_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Single Reduction",eudate);
        found = true;
    }
    if (filter==6||!hlist) ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==6||!hlist) && francis_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Francis Bacon",eudate);
        found = true;
    }
    if (filter==7||!hlist) ns = getwordnumericvalue(phrase,0,0,3); //Satanic
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==7||!hlist) && satanic_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Satanic",eudate);
        found = true;
    }
    if (filter==8||!hlist) ns = getwordnumericvalue(phrase,0,0,4); //Jewish
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==8||!hlist) && jewish_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Jewish",eudate);
        found = true;
    }
    if (found) {
        used = false;
        //ex1 << ns << " " << phrase.size() ;
        ex1 << formattext(std::to_string(ns),1,1) ;
    }

    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare prime numbers to date
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==1||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from English Ordinal",eudate);
            found = true;
        }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==2||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Full Reduction",eudate);
            found = true;
        }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==3||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Ordinal",eudate);
            found = true;
        }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==4||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Full Reduction",eudate);
            found = true;
        }
    if (found && used) {
        used = false;
        ex1 << formattext(std::to_string(getnprime(ns)),1,1) << formattext("th Prime",2,2);
    }
    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare triangular numbers to date
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==1||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from English Ordinal",eudate);
            found = true;
        }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==2||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Full Reduction",eudate);
            found = true;
        }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==3||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Ordinal",eudate);
            found = true;
        }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==4||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Full Reduction",eudate);
            found = true;
        }
    if (found && used) {
        ex1 << formattext(std::to_string(getntriangular(ns)),1,1) << formattext("th Triangular",2,2) ;
        used = false;
    }


    if (found && hlist){
        buffer += Qtotable("",1,0,0,0);
        logline << totable("",0,1,0,0) << totable(QString::fromStdString(formattext(phrase,2,2)).toUtf8().constData(),0,0,1,150) << totable(ex1.str(),0,0,1,300) << totable("",0,2,0,0);

      buffer += QString::fromStdString(logline.str());
      buffer += Qtotable("",2,0,0,0);
      savelog(logline.str());
    }

    if (!found){
        if (!hlist) {
            logline.str("");
            logline << "<br>No pattern found between date and phrase!<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());

        }
    }
    return buffer;
}

QString analyze(int dd, int mm, int year, QString Qphrase,bool hlist, int filter, bool eudate) {


        bool dogoanalize = true;
        std::string phrase = Qphrase.toUtf8().constData();

        readsolarfile(dd,mm,year);


        QString buffer;


        std::vector<std::string> v;

        std::istringstream is( phrase );

        std::string word;
        if (dogoanalize) buffer += runanalyze( dd, mm, year, phrase, hlist, filter,eudate);
        while ( is >> word && dogoanalize) {
        v.push_back( word );
        eraseAllSubStr(word,"<br>");
        eraseAllSubStr(phrase,"<br>");
        if (getwordnumericvalue(phrase,0,0,0) == getwordnumericvalue(word,0,0,0)) break;
        else buffer += runanalyze( dd, mm, year, word, hlist, filter,eudate);
        }
        dogoanalize = false;
        return buffer;
    }

QString date2history(int dd, int mm, int year,bool hlist, bool eudate,int filter)
{
    string line;
    stringstream logline;
    QString buffer;
    int d1,d2;
    if (eudate) {d1=dd;d2=mm;}
    else {d1=mm;d2=dd;}
    switch(filter) {
          case 1 :
              logline << "<br>English Ordinal - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
          case 2 :
              logline << "<br>Full Reduction - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
          case 3 :
              logline << "<br>Reverse Ordinal - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
          case 4 :
              logline << "<br>Reverse Full Reduction - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
        case 5 :
            //single_r_on = true;
            logline << "<br>Single Reduction - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
            buffer += QString::fromStdString(logline.str());
            savelog(logline.str());
          break;
        case 6 :
            //francis_on = true;
            logline << "<br>Francis Bacon - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
            buffer += QString::fromStdString(logline.str());
            savelog(logline.str());
          break;
        case 7 :
            //satanic_on = true;
            logline << "<br>Satanic - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
            buffer += QString::fromStdString(logline.str());
            savelog(logline.str());
          break;
    case 8 :
        //jewish_on = true;
        logline << "<br>Jewis - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());
      break;
    }


    ifstream myfile;
    myfile.open("history.txt");
    if (myfile.is_open())
          {

            while ( getline (myfile,line) )
            {
                buffer += runanalyze(dd, mm, year, line, hlist,filter,eudate);

            }

    myfile.close();
    }
    return buffer;
}

string char2string(char c)
{

    std::string str(1,c);

    return str;
}

string totable(string str, int start, int endrow, int colum, int width)
{
    //QString str;
    if (start == 1) str = "<table>";
    if (endrow == 1) str = "<tr>";
    if (colum == 1) str = "<td width=\"" + to_string(width) + "\">" + str + "</td>";
    if (endrow == 2) str = "</tr>";
    if (start == 2) str = "</table>";
    return str;
}

QString Qtotable(QString str, int start, int endrow, int colum, int width)
{
    //QString str;
    if (start == 1) str = "<table>";
    if (endrow == 1) str = "<tr>";
    if (colum == 1) str = "<td width=\"" + QString::number(width) + "\">" + str + "</td>";
    if (endrow == 2) str = "</tr>";
    if (start == 2) str = "</table>";
    return str;
}

QString listciphers(int reduced, int reversed, int type) {
    int i = 1;
    double t1;
    char l=0;
    int jcount=0;
    stringstream ss;
    //QString buffer;
    //ss << "<tbody><tr>";
    ss << totable("",1,1,0,70);

    for (char c = 'A'; c <= 'Z'; c++)
    {
        std::string s(1,c);
        t1 = i;
        if (type == 2) l= c+32;
        //if (type != 2 && type != 4) ss << "<td>" << c << " = " << getwordnumericvalue(tolowerCase(s),reduced,reversed,type) << "</td>";
        if (type != 2 && type != 4) ss << totable(formattext(char2string(c),1,1) + " = " + formattext(to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0),0,0,1,70);
        //if (type != 2 && type != 4) ss << totable(char2string(c) + " = " + to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0,1);
        if (type == 4 && (tolowerCase(s) != "j" ) && (tolowerCase(s) != "&" ) &&
                (tolowerCase(s) != "v" ) && (tolowerCase(s) != "w" )) {
            if (tolowerCase(s) == "z") jcount = 1;
            if (tolowerCase(s) == "i") i--;
            ss << totable(formattext(char2string(c),1,1) + " = " + formattext(to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0),0,0,1,70);
        }
        if (type == 2) ss << totable(formattext(char2string(l),1,1) + " = " + formattext(to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0),0,0,1,70);
        if (floor(i/9) == t1/9) ss << "</tr><tr>";

        if (type == 4 && jcount == 1) {
            ss << totable(formattext("J",1,1) + " = " + formattext(to_string(getwordnumericvalue("j",reduced,reversed,type)),0,0),0,0,1,70);
            jcount = 2;
        }
        if (type == 4 && jcount == 2) {
            ss << totable(formattext("V",1,1) + " = " + formattext(to_string(getwordnumericvalue("v",reduced,reversed,type)),0,0),0,0,1,70);
            jcount = 3;
        }
        if (type == 4 && jcount == 3) {
            ss << totable(formattext("&",1,1) + " = " + formattext(to_string(getwordnumericvalue("&",reduced,reversed,type)),0,0),0,0,1,70);
            jcount = 4;
        }
        if (type == 4 && jcount == 4) ss << totable(formattext("W",1,1) + " = " + formattext(to_string(getwordnumericvalue("w",reduced,reversed,type)),0,0),0,0,1,70);
        i++;
    }

    if (type == 2) {
        i = 1;
        ss << "</tr><br><tr>";
        for (char c = 'A'; c <= 'Z'; c++)
        {
            std::string s(1,c);
            t1 = i;
            ss << totable(formattext(char2string(c),1,1) + " = " + formattext(to_string(getwordnumericvalue(s,reduced,reversed,type)),0,0),0,0,1,70);
            if (floor(i/9) == t1/9) ss << "</tr><tr>";
            i++;
        }


    }
    //ss << "</tr></tbody>";
    ss << totable("",2,2,0,70);
    //qDebug() << QString::fromStdString(ss.str());
    return QString::fromStdString(ss.str());
    //return QString::fromStdString(formattext(ss.str(),1,1));
    //return "<html><head><style>thead {color:green;}tbody {color:blue;}tfoot {color:red;}table, th, td {border: 1px solid black;}</style></head><body><table>  <thead>    <tr>      <th width=\"300\">Month</th>      <th width=\"200\">Savings</th>    </tr>  </thead>  <tbody>    <tr>      <td>January</td>      <td>$100</td>    </tr>    <tr>      <td>February</td>      <td>$80</td>    </tr>  </tbody>  <tfoot>    <tr>      <td>Sum</td>      <td>$180</td>    </tr>  </tfoot></table><p><b>Tip:</b> The thead, tbody, and tfoot elements will not affect the layout of the table by default. However, you can use CSS to style these elements.</p></body></html>";
}

string charnumeric(int reduced, int reversed, string phrase, int type) {
    int i = 1;
    stringstream ss,s1;
    for (i=0;i<phrase.length();i++)
    {
        s1 << phrase[i];
        if (i < phrase.length()-1) ss << getwordnumericvalue(s1.str(),reduced,reversed,type) << "+";
        else
        ss << getwordnumericvalue(s1.str(),reduced,reversed,type) << "=";
        s1.str("");
    }
    return ss.str();
}

QString printallwords(string line, char save, bool header, bool simpleprint)
{
    bool dogoanalize = true;
    //std::string phrase = Qphrase.toUtf8().constData();

    QString buffer;


        std::vector<std::string> v;

        std::istringstream is( line );

        std::string word;
        if (dogoanalize) buffer += printword(line,save,header,simpleprint) + "<br>";
        while ( is >> word && dogoanalize) {
        v.push_back( word );
        eraseAllSubStr(word,"<br>");
        eraseAllSubStr(line,"<br>");
        if (getwordnumericvalue(line,0,0,0) == getwordnumericvalue(word,0,0,0)) break;
        else buffer += printword(word,save,header,simpleprint) + "<br>";
        }
        dogoanalize = false;
        return buffer;

}

QString printword(string line, char save, bool header, bool simpleprint)
{
    int ns1,ns2,ns3,ns4,ns5,ns6,ns7,ns8;
    string tabs;
    QString buffer,qs1,qs2,qs3,qs4,qs5,qs6="",qs7="",qs8="",qs9="";
    ns1 = getwordnumericvalue(line,0,0,0); //English Ordinal
    ns2 = getwordnumericvalue(line,1,0,0); //Full Reduction
    ns3 = getwordnumericvalue(line,0,1,0); //Reverse Ordinal
    ns4 = getwordnumericvalue(line,1,1,0); //Reverse Full Reduction
    ns5 = getwordnumericvalue(line,0,0,1); //Single Reduction
    ns6 = getwordnumericvalue(line,0,0,2); //Francis Bacon
    ns7 = getwordnumericvalue(line,0,0,3); //Satanic
    ns8 = getwordnumericvalue(line,0,0,4); //Jewish
    eraseAllSubStr(line,"<br>");
    stringstream logline;
    if (header) logtime();
    logline.str("");
    if (line.size() <= 7) tabs = "&emsp;&emsp;&emsp;";
    if (line.size() < 16 && line.size() >= 8) tabs = "&emsp;&emsp;";
    if (line.size() < 24 && line.size() >= 16) tabs = "&emsp;";
    if (line.size() > 24) tabs = " ";
    if (save != 'D' && !simpleprint) {
    logline << "Statistic for phrase : " << formattext(line,1,1) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
    logline.str("");
    logline << "English Orinal : &emsp;" << charnumeric(0,0,line,0) << formattext(std::to_string(ns1),1,1) << " Prime? "<< isprime(ns1) << " Triangular? " << istriangular(ns1) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
    logline.str("");
    logline << "Full Reduction : &emsp;" << charnumeric(1,0,line,0) << formattext(std::to_string(ns2),1,1) << " Prime? "<< isprime(ns2) << " Triangular? " << istriangular(ns2) << "<br>";
    //logline << "Prime number&emsp;&emsp;" << isprime(ns1) << "" << isprime(ns2) << "" << isprime(ns3) << "" << isprime(ns4) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      logline.str("");
    //logline << "Triangular number&emsp;&emsp;" << istriangular(ns1) << "" << istriangular(ns2) << "" << istriangular(ns3) << "" << istriangular(ns4) << "<br><br>";
      logline << "Reverse Ordinal :&emsp; " << charnumeric(0,1,line,0) << formattext(std::to_string(ns3),1,1) << " Prime? "<< isprime(ns3) << " Triangular? " << istriangular(ns3) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      logline.str("");
      logline << "Reverse Full Reduction : " << charnumeric(1,1,line,0) << formattext(std::to_string(ns4),1,1) << " Prime? "<< isprime(ns4) << " Triangular? " << istriangular(ns4) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      if (single_r_on) {
          logline.str("");
          logline << "Single Reduction : &emsp;" << charnumeric(0,0,line,1) << formattext(std::to_string(ns5),1,1) << " Prime? "<< isprime(ns5) << " Triangular? " << istriangular(ns5) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (francis_on) {
          logline.str("");
          logline << "Francis Bacon : &emsp;" << charnumeric(0,0,line,2) << formattext(std::to_string(ns6),1,1) << " Prime? "<< isprime(ns6) << " Triangular? " << istriangular(ns6) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (satanic_on) {
          logline.str("");
          logline << "Satanic : &emsp;" << charnumeric(0,0,line,3) << formattext(std::to_string(ns7),1,1) << " Prime? "<< isprime(ns7) << " Triangular? " << istriangular(ns7) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (jewish_on) {
          logline.str("");
          logline << "Jewish : &emsp;" << charnumeric(0,0,line,4) << formattext(std::to_string(ns8),1,1) << " Prime? "<< isprime(ns8) << " Triangular? " << istriangular(ns8) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
    }
    if (simpleprint) {
     logline << "<br>" << line << tabs << ns1 << "&emsp;" << ns2 << "&emsp;" << ns3 << "&emsp;" << ns4 << "&emsp;";
     //qs1 = add_dots(QString::fromStdString(formattext(line,2,2)),50);
     qs1 = QString::fromStdString(formattext(line,2,2));
     //qs1 = add_dots(QString::fromStdString(line),25);

     qs2 = QString::fromStdString(formattext(std::to_string(ns1),1,1));
     qs3 = QString::fromStdString(formattext(std::to_string(ns2),1,1));
     qs4 = QString::fromStdString(formattext(std::to_string(ns3),1,1));
     qs5 = QString::fromStdString(formattext(std::to_string(ns4),1,1));
     if (single_r_on) qs6 = "SR " + QString::fromStdString(formattext(std::to_string(ns5),1,1));
     if (francis_on) qs7 = "FB " + QString::fromStdString(formattext(std::to_string(ns6),1,1));
     if (satanic_on) qs8 = "Sat " + QString::fromStdString(formattext(std::to_string(ns7),1,1));
     if (jewish_on) qs9 = "Jew " + QString::fromStdString(formattext(std::to_string(ns8),1,1));
     //buffer += "<br>" + qs1 + "&emsp;EO " + qs2 + "&emsp;FR " + qs3 + "&emsp;RO " + qs4 + "&emsp;RF " + qs5 + "&emsp;" + qs6 + "&emsp;" + qs7 + "&emsp;" + qs8 + "&emsp;" + qs9;
     buffer += Qtotable("",0,1,0,0)+Qtotable(qs1,0,0,1,150) + Qtotable("EO " + qs2,0,0,1,70) + Qtotable("FR " + qs3,0,0,1,70) + Qtotable("RO " + qs4,0,0,1,70) + Qtotable("RF " + qs5,0,0,1,70) + Qtotable(qs6,0,0,1,70) + Qtotable(qs7,0,0,1,70) + Qtotable(qs8,0,0,1,70) + Qtotable(qs9,0,0,1,70)+Qtotable("",0,2,0,0);
     buffer += Qtotable("",2,0,0,0);
     savelog(logline.str());
    }
    if (int(line[0]) <= 122 && int(line[0]) >= 97) line[0] -= 32;
    if (save == 'D' && ns1 > 0) buffer += deletelastline();
    if (save == 'Y' && ns1 > 0) buffer += savetext(line);
    return buffer;
}


string firstletterupper(string line)
{


    string buffer;

    std::vector<std::string> v;

    std::istringstream is( line );

    std::string word;
    size_t found;

    while ( is >> word) {
    v.push_back( word );
    word[0] -= 32;
    found = line.find(" ");
    if (found != string::npos) buffer = buffer + word + " ";
    else buffer = word;

    }

    return buffer + "\n";
}

QString searchwords(int pattern, bool simpleprint) //Ctrl-H
{
    string line;
    QString buffer,qs5="",qs6="",qs7="",qs8="";
    bool header=true;
    stringstream logline;
    int c1, lines = 0;
    ifstream myfile;
    if (single_r_on) qs5 = " - Single Reduction";
    if (francis_on) qs6 = " - Francis Bacon";
    if (satanic_on) qs7 = " - Satanic";
    if (jewish_on) qs8 = " - Jewish";
    if (simpleprint) buffer += "&emsp;&emsp;&emsp;Englis Ordinal - Full Reduction - Reverse Ordinal - Reverse Full Reduction" + qs5 + qs6 + qs7 + qs8;
    myfile.open("history.txt");
    if (myfile.is_open())
          {
          c1 = pattern;
          while ( getline (myfile,line) )
            {
                  if (findword(c1,line)) {
                      buffer += printword(line,0,header,simpleprint);
                      lines++;
                      header = false;

                  }
                 // savetotemp(firstletterupper(line));
            }

    logline << "<br>" << formattext(std::to_string(lines),1,1) << " words found matching number " << formattext(std::to_string(c1),1,1) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());

    myfile.close();
    //buffer += "</table>";
  }

  else buffer += "Unable to open file";
  return buffer;
}


QString searchhistory(int i, string phrase) {
    int ns;
    QString buffer;
    stringstream ss;
    switch(i) {

    case 1 :
        ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 2 :
        ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 3 :
        ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 4 :
        ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 5 :
        ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 6 :
        ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 7 :
        ns = getwordnumericvalue(phrase,0,0,3); //Satanic
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    case 8 :
        ns = getwordnumericvalue(phrase,0,0,4); //Jewish
        buffer += searchwords(ns,true);
        if (getnprime(ns) != 0) {
            ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getnprime(ns),true);
        }
        if (getntriangular(ns) != 0) {
            ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
            buffer += QString::fromStdString(ss.str());
            buffer += searchwords(getntriangular(ns),true);
        }
        break;
    }
    return buffer;
}
