#ifndef GCALC_H
#define GCALC_H
#include "tools.h"
#include <QDate>
#include <QUrl>
using namespace std;


QString loopYear(int ns,int dd, int mm, int year, int printcal, bool eudate);
QString searchwords(int pattern, bool simpleprint);
QString printword(string line, char save, bool header, bool simpleprint);
QString add_dots(QString str,int length);
QString analyze(int dd, int mm, int year, QString phrase,bool hlist, int filter, bool eudate);
bool phrasetodate(int ns, int dd, int mm, int year, int i);
QString runanalyze(int dd, int mm, int year, string phrase,bool hlist, int filter, bool eudate);
QString print_p_to_d(int ns, int dd, int mm, int year, int i, string detail, bool eudate);
QString gcalc(int dd, int mm, int year, int dd2, int mm2, int yy2,bool eudate);
QString listciphers(int reduced, int reversed, int type);
QString searchhistory(int i, string phrase);
QVariant loadsettings(QString settings);
void savesettings(QString settings, QVariant attr);
std::string charnumeric(int reduced, int reversed, std::string phrase,int type);
QString date2history(int dd, int mm, int year,bool hlist, bool eudate,int filter);
QString solareclipe(int dd,int mm,int year, int output, int type,bool eudate);
int weeks_months(QDate startDate, QDate endDate, bool months, bool days);
int searchzerodays(int ns,int type,int dd,int mm, int year);
QString printzerodays(int dd, int mm, int year, int ns, int type, string detail, bool eudate, bool read);
void readsolarfile(int dd, int mm, int year);
QString solar2history(int dd, int mm, int year,int type, bool eudate);
QString printallwords(string line, char save, bool header, bool simpleprint);
string firstletterupper(string line);
string totable(string str, int start, int endrow, int colum, int width);
QString Qtotable(QString str, int start, int endrow, int colum, int width);
string char2string(char *c);
QString detail(int ns,int dd, int mm, int year, int i,bool eudate);
QString phraserank(string phrase, bool eudate, int minimum, bool prime, bool triangular, bool dates, bool chipers);
int getns(string phrase, int out, int pt);
int counter(string phrase, int dd, int mm, int year,int minimum,bool runsolar, bool prime, bool triangular, bool dates, bool chipers);
QStringList getheadlines(QString source, int numberof);
void eraseAllQSubStr(QString & mainStr, const QString & toErase);
QString headline(QString content, QString pattern);
bool isheadlines(QString content, QString pattern);
void erasefromToQSubStr(QString & mainStr, const QString & fromErase, const QString & toErase);
QString wordnumbericlist(QString head);
QString clean_news_content(QString content, QString pattern);
QString tobuffer(QString html);
#endif // GCALC_H
