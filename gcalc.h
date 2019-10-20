#ifndef GCALC_H
#define GCALC_H
#include "tools.h"
#include <QDate>

using namespace std;


QString loopYear(int ns, int year, int printcal, bool eutime);
QString searchwords(int pattern, bool simpleprint);
QString printword(string line, char save, bool header, bool simpleprint);
QString add_dots(QString str,int length);
QString analyze(int dd, int mm, int year, QString phrase,bool hlist, int filter, bool eutime);
bool phrasetodate(int ns, int dd, int mm, int year, int i);
QString runanalyze(int dd, int mm, int year, string phrase,bool hlist, int filter, bool eutime);
QString print_p_to_d(int ns, int dd, int mm, int year, int i, string detail, bool eutime);
QString gcalc(int dd, int mm, int year, int dd2, int mm2, int yy2,bool eutime);
QString listciphers(int reduced, int reversed, int type);
QString searchhistory(int i, string phrase);
std::string charnumeric(int reduced, int reversed, std::string phrase,int type);
QString date2history(int dd, int mm, int year,bool hlist, bool eutime,int filter);
QString solareclipe(int dd,int mm,int year, int output, int type,bool eudate);
int weeks_months(QDate startDate, QDate endDate, bool months, bool days);
int searchzerodays(int ns,int type,int dd,int mm, int year);
QString printzerodays(int dd, int mm, int year, int ns, int type, string detail, bool eudate, bool read);
void readsolarfile(int dd, int mm, int year);
QString solar2history(int dd, int mm, int year,int type);
QString printallwords(string line, char save, bool header, bool simpleprint);
string firstletterupper(string line);
#endif // GCALC_H
