#include "tools.h"
#include "mainwindow.h"
#include <string.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <QString>
#include <QDate>
#include <QDebug>
#include <QDir>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::floor;
using std::isdigit;

constexpr int BUFFERSIZE = 256;

//#include <algorithm>



int dayNumber(int day, int month, int year)
{

    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
                       4, 6, 2, 4 };
    year -= month < 3;
    return ( year + year/4 - year/100 +
             year/400 + t[month-1] + day) % 7;
}

int valid_date(int dd, int mm, int yy) {
    if (mm < 1 || mm > 12) {
        return 0;
    }
    if (dd < 1) {
        return 0;
    }

    int days = 31;
    if (mm == 2) {
        days = 28;
        if (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)) {
            days = 29;
        }
    } else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
        days = 30;
    }

    if (dd > days) {
        return 0;
    }
    return 1;
}

QString Qformattext(QString line, int color, int style)
{
    QString colorCode;
    QString styledText;
    QString fcolor = nightmode ? "lightblue" : "#0033cc";
    QString red = "#d40000";
    QString green = "#007a00";

    switch (color) {
    case 1: colorCode = "<font color=\"" + red + "\">" + line + "</font>"; break;
    case 2: colorCode = "<font color=\"" + fcolor + "\">" + line + "</font>"; break;
    case 3: colorCode = "<font color=\"" + green + "\">" + line + "</font>"; break;
    default: colorCode = line; break;
    }

    switch (style) {
    case 1: styledText = "<b>" + colorCode + "</b>"; break;
    case 2: styledText = "<i>" + colorCode + "</i>"; break;
    case 3: styledText = "<i><b>" + colorCode + "</b></i>"; break;
    case 4: styledText = "<u>" + colorCode + "</u>"; break;
    case 5: styledText = "<s>" + colorCode + "</s>"; break;  // optional: strikethrough
    default: styledText = colorCode; break;
    }

    // Special tab spacing
    if (color == 10) {
        styledText += QString("&emsp;").repeated(style > 1 ? style - 1 : 0);
    } else if (color == 20) {
        styledText = QString("&emsp;").repeated(style > 1 ? style - 1 : 0) + styledText;
    }

    return styledText;
}


std::string formattext(std::string line, int color, int style)
{
    std::string result = line;
    std::string fcolor = nightmode ? "lightblue" : "blue";
    bool isNum = is_number(line);
    std::string styled;

    // Apply color
    switch (color) {
    case 1: styled = "<font color=\"red\">" + line + "</font>"; break;
    case 2: styled = "<font color=\"" + fcolor + "\">" + line + "</font>"; break;
    case 3: styled = "<font color=\"green\">" + line + "</font>"; break;
    default: styled = line; break;
    }

    // Apply style
    switch (style) {
    case 1: result = "<b>" + styled + "</b>"; break;
    case 2: result = "<i>" + styled + "</i>"; break;
    case 3: result = "<i><b>" + styled + "</b></i>"; break;
    case 4: result = "<u>" + styled + "</u>"; break;
    case 5: result = "<s>" + styled + "</s>"; break;
    default: result = styled; break;
    }

    // Tabs
    if (color == 10) {
        for (int i = 1; i < style; i++) result += "&emsp;";
    } else if (color == 20) {
        for (int i = 1; i < style; i++) result = "&emsp;" + result;
    }

    // Tooltip for number properties
    if (isNum) {
        std::string tooltip = numberproperties(QString::fromStdString(line)).toUtf8().constData();
        result = "<span title=\"" + tooltip + "\">" + result + "</span>";
    }

    return result;
}


QString listnumbers(char type) {
    int i = 1;
    double t1;
    stringstream ss;
    do {
        if (type == 'T'){
            if (getntriangular(i) != 0) {
                ss << i << " - " << getntriangular(i) << "    \t";
                t1 = getntriangular(i);
                if (floor(getntriangular(i)/5) == t1/5) ss << "\n";
            }

        }
        if (type == 'P') {
            if (i == 1) i=2;
            if (getnprime(i) != 0) {
                ss << i << " - " << getnprime(i) << "    \t";
                t1 = getnprime(i);
                if (floor((getnprime(i))/5) == t1/5) ss << "\n";
            }
        }
        i++;
    } while ((type == 'T' && getntriangular(i) <= 100) || (type == 'P' && getnprime(i) <= 100));
    return QString::fromStdString(ss.str());
}

std::string dayname(int day) {
       switch(day) {
          case 0 :
              return "Sunday";
              break;
          case 1 :
              return "Monday";
              break;
          case 2 :
              return "Tuesday";
              break;
          case 3 :
              return "Wednesday";
              break;
          case 4 :
              return "Thursday";
              break;
          case 5 :
              return "Friday";
              break;
          case 6 :
              return "Saturday";
              break;
       }
       return "day";
}

/*
  A Function that returns the name of the month
  with a given month number

  Month Number     Name
  0                January
  1                February
  2                March
  3                April
  4                May
  5                June
  6                July
  7                August
  8                September
  9                October
  10               November
  11               December */
std::string getMonthName(int monthNumber)
{
    std::string months[] = {"January&emsp;", "February", "March&emsp;",
                       "April&emsp;", "May&emsp;", "June&emsp;",
                       "July&emsp;", "August&emsp;", "September",
                       "October&emsp;", "November&emsp;", "December&emsp;"
                      };

    return (months[monthNumber]);
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

QString numberproperties(QString number)
{
    int str2num = 100000+number.toInt();
    QString retstr = "";
    if (str2num > 100000) {
    retstr = "Prime:"+QString::fromStdString(isprime(str2num,primes))+"<br>";
    retstr += "Triangular:"+QString::fromStdString(istriangular(str2num))+"<br>";
    retstr += "Prime related:"+numberseat('P',str2num-100000)+"<br>";
    retstr += "Triangular related:"+numberseat('T',str2num-100000);
    }
    return retstr;

    /*QString pwd = QDir::currentPath() + "/" + nrfile + ".htm";
    QFile *file = new QFile(pwd);
    if ( !file->open(QIODevice::ReadWrite) )
    {
        qDebug() << "tmp.htm not open";
    } else file->close();
    if (file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream( file );
        //stream << html;
        file->close();
    }*/
}



QString numberseat(char type, int number) {
    QString result="";
        if (type == 'T' && number > 0){
            // Use triangular number formula: n*(n+1)/2 instead of O(n) loop
            int sum = (number * (number + 1)) / 2;
            result = QString::number(sum);
        }
        if (type == 'P' && number > 0) result = QString::number(primes[number-1]);

    return result;
}

/* A Function to return the number of days in
   a month

  Month Number     Name        Number of Days
  0                January     31
  1                February    28 (non-leap) / 29 (leap)
  2                March       31
  3                April       30
  4                May         31
  5                June        30
  6                July        31
  7                August      31
  8                September   30
  9                October     31
  10               November    30
  11               December    31

*/
int numberOfDays (int monthNumber, int year)
{
    // January
    if (monthNumber == 0)
        return (31);

    // February
    if (monthNumber == 1)
    {
        // If the year is leap then February has
        // 29 days
        if (year % 400 == 0 ||
                (year % 4 == 0 && year % 100 != 0))
            return (29);
        else
            return (28);
    }

    // March
    if (monthNumber == 2)
        return (31);

    // April
    if (monthNumber == 3)
        return (30);

    // May
    if (monthNumber == 4)
        return (31);

    // June
    if (monthNumber == 5)
        return (30);

    // July
    if (monthNumber == 6)
        return (31);

    // August
    if (monthNumber == 7)
        return (31);

    // September
    if (monthNumber == 8)
        return (30);

    // October
    if (monthNumber == 9)
        return (31);

    // November
    if (monthNumber == 10)
        return (30);

    // December
    if (monthNumber == 11)
        return (31);
    return 0;
}

int spanofdate(int d2, int m2, int y2, int dd, int mm, int year) {
    int i, numberofdays;
    tm date = {};
    date.tm_year = y2 - 1900;
    date.tm_mon = m2 - 1;
    date.tm_mday = d2;
    mktime( &date );
    numberofdays = date.tm_yday +1;
    date.tm_year = y2 - 1900;
    date.tm_mon = 12 - 1;
    date.tm_mday = 31;
    mktime( &date );
    numberofdays = date.tm_yday - numberofdays+1;


    if (y2 < year)
        for(i=y2+1; i<=year; ++i){
            date.tm_year = i - 1900;
            date.tm_mon = 12 - 1;
            date.tm_mday = 31;
            mktime( &date );
            numberofdays += date.tm_yday +1;
        }
    date.tm_year = year - 1900;
    date.tm_mon = mm - 1;
    date.tm_mday = dd;
    mktime( &date );
    i = date.tm_yday +1;
    date.tm_year = year - 1900;
    date.tm_mon = 12 - 1;
    date.tm_mday = 31;
    mktime( &date );
    numberofdays -= date.tm_yday - i+1;
    return(numberofdays);
}

void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;

    // Search for the substd::string in std::string in a loop untill nothing is found
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        // If found then erase it from std::string
        mainStr.erase(pos, toErase.length());
    }
}

int reduce(int sum){
            int s1, s2;
           if (sum > 18)
              {
                s2 = sum-20;
                s1 = 2;
           } else if (sum > 9)
              {
                s2 = sum-10;
                s1 = 1;
           } else
              {
                s2 = sum;
                s1 = 0;
              }
    return (s1 + s2);

}
int reverse(int sum){
    sum = 27-sum;
    return(sum);
}


int getnprime(int prime) {
    if (prime < 2) return 0;

    int count = 0;
    for (int num = 2; num <= prime; ++num) {
        bool isPrime = true;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            ++count;
            if (num == prime) return count;  // Found the prime, return its position
        }
    }

    return 0;  // Not a prime
}


int getntriangular(int tri)
{
    int i, ntri = 0;
    bool isTri = true;
    if (floor(sqrt(8*tri+1)) != sqrt(8*tri+1)) isTri = false;
    if (isTri) {
        for(i = 1; i <= tri; ++i)
            if (floor(sqrt(8*i+1)) == sqrt(8*i+1)) ntri++;
        return ntri;
    }
    else
        return 0;

}

int getwordnumericvalue(std::string word, int reduced, int reversed, int type) // type 0 English ordenal, 1 Single Reduction, 2 Francis Bacon, 3 Satanic, 4 Jewish, 5 Sumerian
{
        int s1 = 0, sum = 0;// for summing the letter values.
        if (type != 2) tolowerCase(word);
        for(size_t i=0; i<word.size(); ++i){	    	// loop through the string 1 char at a time
            //qDebug() << int(word[i]) << "\n";
            if (int(word[i]) <= 122 && int(word[i]) >= 97) {
               sum = (int(word[i]-96));// using the ascii value for each letter
               if (type == 3) sum += 35; //Satanic
               if (type == 5 && reversed == 0) sum = sum*6; // Sumerian
            }
            if (int(word[i]) <= 90 && int(word[i]) >= 65 && type == 2) sum = (int(tolower(word[i])-96)) + 26; //Francis
            if ((int((word[i]) <= 122 && int(word[i]) >= 97) || int(word[i]) == 38) && type == 4) { // Jewish
                if (int(word[i]) == 116) sum = 100; //t
                if (int(word[i]) == 117) sum = 200; //u
                if (int(word[i]) == 120) sum = 300; //x
                if (int(word[i]) == 121) sum = 400; //y
                if (int(word[i]) == 122) sum = 500; //z
                if (int(word[i]) == 106) sum = 600; //j
                if (int(word[i]) == 118) sum = 700; //v
                if (int(word[i]) == 38) sum = 800; //&
                if (int(word[i]) == 119) sum = 900; //w
                if (int(word[i]) <= 115 && int(word[i]) >= 107) { // 10-90
                    sum = ((int(word[i])-96) - 10)*10;
                }

            }
            if (reversed == 1 && int(word[i]) <= 122 && int(word[i]) >= 97){
                sum = reverse(sum);
                if (type == 5) sum = sum*6;
            }
            if (reduced == 1 && int(word[i]) <= 122 && int(word[i]) >= 97) sum = reduce(sum);
            if (type == 1 && int(word[i]) <= 122 && int(word[i]) >= 97 && tolower(word[i]) != 's') { //Single reduction
                sum = reduce(sum);
               } else if (tolower(word[i]) == 's' && type == 1) sum = 10;
            if (int(word[i]) == 38 && type == 4) s1 += sum;
            if (int(word[i]) <= 122 && int(word[i]) >= 97) s1 += sum;
            if (int(word[i]) <= 90 && int(word[i]) >= 65) s1 += sum;
         if (int(word[i]) <= 57 && int(word[i]) >= 49) s1 += int(word[i]-48);
        }
        //qDebug() << word[i] << " = " << s1;
        return(s1);
}

bool findword(int c1, std::string line)
{
    int ns1 = 0,ns2 = 0,ns3 = 0,ns4 = 0,ns5 = 0,ns6 = 0,ns7 = 0,ns8 = 0,ns9=0,ns10=0;
    ns1 = getwordnumericvalue(line,0,0,0);
    ns2 = getwordnumericvalue(line,1,0,0);
    ns3 = getwordnumericvalue(line,0,1,0);
    ns4 = getwordnumericvalue(line,1,1,0);
    if (single_r_on) ns5 = getwordnumericvalue(line,0,0,1);
    if (francis_on) ns6 = getwordnumericvalue(line,0,0,2);
    if (satanic_on) ns7 = getwordnumericvalue(line,0,0,3);
    if (jewish_on) ns8 = getwordnumericvalue(line,0,0,4);
    if (sumerian_on) ns9 = getwordnumericvalue(line,0,0,5);
    if (rev_sumerian_on) ns10 = getwordnumericvalue(line,0,1,5);
     if (c1 == ns1 || c1 == ns2 || c1 == ns3 || c1 ==ns4 || c1 ==ns5 || c1 ==ns6 || c1 ==ns7 || c1 ==ns8 || c1 ==ns9 || c1 ==ns10)
        {
            return true;
        }


return false;
}


void savetotemp(std::string line)
{
    //std::string stext;
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    std::ofstream fout;  // Create Object of Ofstream
    std::ifstream fin;
    fin.open("temp.txt");
   fout.open ("temp.txt",ios::app); // Append mode
    if(fin.is_open())
        fout<< line; // Writing data to file
    fin.close();
    fout.close(); // Closing the file
    line = "";
}


void savelog(std::string line)
{
    //std::string stext;
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    std::ofstream fout;  // Create Object of Ofstream
    std::ifstream fin;
    fin.open("log.html");
   fout.open ("log.html",ios::app); // Append mode
    if(fin.is_open())
        fout<< line; // Writing data to file
    fin.close();
    fout.close(); // Closing the file
    line = "";
}

QString savetext(string line)
{
    string stext;
    QString buffer;
    int add = 1;
    ofstream fout;  // Create Object of Ofstream
    ifstream fin;
    eraseAllSubStr(line,"\n");
    QString historyfile = loadsettings("historyfile").toString();
    fin.open(historyfile.toUtf8().constData());
    while ( getline (fin,stext) )
    //	cout << line <<"&emsp;" << stext;
   // pause();
    {
        if (line == stext) add = 0;
    }
    fout.open (historyfile.toUtf8().constData(),ios::app); // Append mode
    if(fin.is_open() && add == 1)
        fout<< line<<"\n"; // Writing data to file
    if (add == 1) buffer = "<br> Saved to history<br>";
    if (add == 0) buffer = "<br> Already in history<br>";
    fin.close();
    fout.close(); // Closing the file
    return buffer;
}

//string replacesubstr(string str,string pattern, string replacement)
void replaceAll( string &s, const string &search, const string &replace ) { // replace in "s" the "replace" for "search"
    //string rep;
    for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
        pos = s.find( search, pos );
        if( pos == string::npos ) break;
        // Replace by erasing and inserting
        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
}

void replaceAllQ( QString &s, const QString &search, const QString &replace ) { // replace in "s" the "replace" for "search"
    QString rep;
    //for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
    if (replace == "linenumbers") {
        rep = search+QString::number(linenumbers);
        linenumbers++;
    }
        s.replace(search,rep);
        //pos = s.find( search, pos );
        //if( pos == string::npos ) break;
        // Replace by erasing and inserting
        //s.erase( pos, search.length() );
        //s.insert( pos, replace );
    //}
}

void logtime() {
    // current date/time based on current system
    std::stringstream logline;
       //time_t now = time(0);

       // convert now to std::string form
       // char* dt = ctime(&now);

       char s[1000];

       time_t t = time(NULL);
       struct tm * p = localtime(&t);

       strftime(s, 1000, "%a, %b %d %Y %H:%M:%S", p);

       //printf("%s<br>", s);


       logline << "<br>------------------------ " << s << " ------------------------<br>";
       savelog(logline.str());
       logline.str(std::string());
}

std::string getOrdinalSuffix(int number)
{
    int last_two = number % 100;
    if (last_two >= 11 && last_two <= 13)
        return "th";

    int last_digit = number % 10;
    switch (last_digit) {
    case 1: return "st";
    case 2: return "nd";
    case 3: return "rd";
    default: return "th";
    }
}


std::string isprime(int prime, const std::vector<int>& primes)
{
    bool noformat = false;

    if (prime >= 100000) {
        noformat = true;
        prime -= 100000;
    }

    auto it = std::lower_bound(primes.begin(), primes.end(), prime);
    if (it != primes.end() && *it == prime) {
        int index = std::distance(primes.begin(), it) + 1; // 1-based index

        std::stringstream ss;
        ss << "Yes (" << index << getOrdinalSuffix(index) << " Prime)";

        return noformat ? ss.str() : formattext(ss.str(), 2, 2);
    }

    return noformat ? "No" : formattext("No", 2, 2);
}




QString deletelastline() {
    std::string line;
    std::vector<std::string> lines;
    QString historyfile = loadsettings("historyfile").toString();
    std::ifstream inputStream(historyfile.toUtf8().constData());

    while (getline(inputStream,line)) {
        lines.push_back(line);
    }
    inputStream.close();

    std::fstream outputStream(historyfile.toUtf8().constData(), ios::out | ios::trunc);
    if (outputStream.is_open())
    {
        for (ulong i=0; i < lines.size()-1; i++)
        {
            //cout << i << " " << lines.size() << " " << lines[i] << " " << line << "<br>";
           outputStream << lines[i] << "\n";
           //QVector c=lines[lines.size()-1];
           if (i==lines.size()-2) return  "<br>Phrase " + QString::fromStdString(lines[lines.size()-1]) + " Deleted from history.txt<br>";
           //if (i==lines.size()-2) return  "<br>Phrase " + QVector(lines[lines.size()-1]) + " Deleted from history.txt<br>";
        }
        outputStream.close();

    }
    return "not found";
}

std::string istriangular(int tri)
{
    bool noformat = false;
    if (tri >= 100000) {
        noformat = true;
        tri -= 100000;
    }

    // Check if 8 * n + 1 is a perfect square
    bool isTri = floor(sqrt(8 * tri + 1)) == sqrt(8 * tri + 1);
    if (!isTri)
        return noformat ? "No" : formattext("No", 2, 2);

    // Count which triangular number it is
    int ntri = 0;
    for (int i = 1; i <= tri; ++i) {
        if (floor(sqrt(8 * i + 1)) == sqrt(8 * i + 1))
            ntri++;
    }

    std::stringstream ss;
    ss << "Yes (" << ntri << getOrdinalSuffix(ntri) << " Triangular)";
    return noformat ? ss.str() : formattext(ss.str(), 2, 2);
}


std::string tolowerCase(std::string &str)
{
    const int length = str.length();
    for(int i=0; i < length; ++i)
    {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

int eu_amdate(int eu_am, int d1, int d2, int m1, int m2,int ns) {

    int eudate, amdate;
    eudate = d1*1000+d2*100+m1*10+m2;
    amdate = m1*1000+m2*100+d1*10+d2;
    if (d1 == 0) amdate = m1*100+m2*10+d2;
    if (m1 == 0) eudate = d1*100+d2*10+m2;
    if (d2 == 0) {
        amdate = m1*100+m2*10+d1;
        eudate = d1*100+m1*10+m2;
    }
    if (m2 == 0) {
        amdate = m1*100+d1*10+d2;
        eudate = d1*100+d2*10+m1;
    }

    if (d1==0 && m1==0) {
        eudate = d2*10+m2;
        amdate = m2*10+d2;
    }
    if (d1==0 && m2==0) {
            eudate = d2*10+m1;
            amdate = m1*10+d2;
        }
    if (d2==0 && m2==0) {
            eudate = d1*10+m1;
            amdate = m1*10+d1;
        }
    if (d2==0 && m1==0) {
            eudate = d1*10+m2;
            amdate = m2*10+d1;
        }
    if (ns != eudate && eu_am == 2) {
    eudate = d1*1000+d2*100+m1*10+m2;
    if (m1 == 0) eudate = d1*100+d2*10+m2;
    }
    if (ns != amdate && eu_am == 1) {
    amdate = m1*1000+m2*100+d1*10+d2;
    if (d1 == 0) amdate = m1*100+m2*10+d2;
    }
   if (eu_am == 1) return amdate;
   else return eudate;
}

int daynr(int dd,int mm,int year) {
    tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = mm - 1;
    date.tm_mday = dd;
    mktime( &date );
    return date.tm_yday +1;

}

int daynrleft(int dd, int mm, int year)
{
 tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = 12 - 1;
    date.tm_mday = 31;
    mktime( &date );
    return date.tm_yday - daynr(dd,mm,year)+1;
}


void createSettings(string file, string entry)
{

       std::ofstream fout;  // Create Object of Ofstream
       fout.open (file,ios::app); // Append mode
       if(!fout)
       {
           qDebug() << "Error opening files!\n";
           //return 1;
       }
       //qDebug() << QString::fromStdString(file);

       if (file == "settings.txt") {
        fout << entry+"=\n";
       }

        fout.close(); // Closing the file
   // return false;
}


QString readSettings(string file, string entry)
{
    string strReplace = entry+"=";
    ifstream filein(file); //File to read from
    if(!filein)
    {
        createSettings(file,entry);
        cout << "Error opening files!" << endl;
        //return 1;
    }
    string strTemp;
    bool found = false;
    while(!filein.eof())
    //while(filein >> strTemp)
    {
        getline(filein,strTemp);

        if(strTemp.substr(0,strReplace.length()) == strReplace){
            found = true;
            return QString::fromStdString(strTemp.substr(strReplace.length(),strTemp.length()-strReplace.length()));
        }
        strTemp += "\n";


    }
    if(!found) createSettings(file,entry);
    return "none";
}

void writeSettings(char file[], string entry,string settings)
{
    string strReplace = entry+"=";
    string strNew = entry+"="+settings;
    //qDebug() << QString::fromStdString(strNew);
    ifstream filein(file); //File to read from
    ofstream fileout("settings.txt.tmp"); //Temporary file
    if(!filein || !fileout)
    {
        cout << "Error opening files!" << endl;
        //return 1;
    }

    string strTemp;
    //bool found = false;
    while(!filein.eof())
    //while(filein >> strTemp)
    {
        getline(filein,strTemp);
        if(strTemp.substr(0,strReplace.length()) == strReplace){

            strTemp = strNew;
            //qDebug() << QString::fromStdString(strTemp);
            //found = true;
        }
        if (strTemp != "") {
            strTemp += "\n";
            fileout << strTemp;
        }
        //if(found) break;
    }


    filein.close();
    fileout.close();
    remove(file);
    if (rename("settings.txt.tmp","settings.txt") !=0)
       // cout<<"file renamed successfully.";
     //else
        cout<<"error renaming file.";
}


QString listhistory(QString filter)
{
    QFile infile;
    QString buffer;
    QString content="";
    QStringList history;
    int i = 1;
    double t1;
    QString historyfile = loadsettings("historyfile").toString();
    infile.setFileName(historyfile);
    //outfile.setFileName("headlines.txt");
    if (infile.open(QIODevice::ReadOnly)) {
        QTextStream in(&infile);
        content = in.readAll();
        infile.close();
    }
    history = content.split("\n");
    history.sort();
    buffer = Qtotable("",1,1,0,70);
    for ( const auto& l : history  )
    {
        if (l.contains(filter, Qt::CaseInsensitive) && l.trimmed() != "")
        {
            t1 = i;
            buffer += Qtotable(Qformattext(l,2,2),0,0,1,350);
            if (floor(i/2) == t1/2) buffer += "</tr><tr>";
            i++;
        }
    }

/*
ifstream myfile;

//int i = 1;
//double t1;
string line;
myfile.open("history.txt");
buffer = Qtotable("",1,1,0,70);
if (myfile.is_open())
      {

        while ( getline (myfile,line) )
        {
            if (QString::fromStdString(line).contains(filter, Qt::CaseInsensitive))
            {
                t1 = i;
                buffer += Qtotable(Qformattext(line,2,2),0,0,1,350);
                if (floor(i/2) == t1/2) buffer += "</tr><tr>";
                i++;
            }

        }

myfile.close();
}*/
return buffer;
}

int monthbeetween(int m_from, int m_to, int daysbeetween,QChar type)
{
    int returnnum, nm2, nm1=0,mm1,i,year1,year2;
    nm2=daysbeetween; //monster begins
    if (m_from > m_to) mm1 = m_to+12;
    else mm1 = m_to;
    QDate c_date(year, mm, dd);
    int days_year = c_date.daysInYear();
    if (daysbeetween < days_year) {
      for (i=m_from;i<mm1;i++){
        //nm1++;
        if (i > 12) {
            if (nm2 >= numberOfDays(i-13,year)) {
                nm2 -= numberOfDays(i-13,year);
                nm1++;
            }
        } else {
            if (nm2 >= numberOfDays(i-1,year)) {
                nm2 -= numberOfDays(i-1,year);
                nm1++;
            }
        }
        //if (i > 12) qDebug() << numberOfDays(i-13,year) << " " << nm1 << " " << nm2 << " " << daysbeetween << " " << i << " " << mm1;
        //       else qDebug() << numberOfDays(i-1,year) << " " << nm1 << " " << nm2 << " " << daysbeetween << " " << i << " " << mm1;
      }
    } else {
        if (year > y2) {
            year1 = y2;
            year2 = year;
        } else {
            year1 = year;
            year2 = y2;
        }
        i=m_from;
        if (m_to > 12) m_to -=12;
        do {
            nm2 -= numberOfDays(i-1,year1);
            nm1++;
            i++;
            if (i > 12){
                year1++;
                i=1;
            }
        } while (year1 < year2 || i != m_to-1);
    }
    if (type == 'M') returnnum = nm1;
    else returnnum = nm2;
    return returnnum;
}


int a_seconddate(QString output_type)
{
    int wd1, wd2, nm1=0,nm2=0,returnnum=0;
    double w1;

    if (output_type.indexOf("d_s") != -1) {
        if (QDate(year,mm,dd) > QDate(year,m2,d2))
        {
            QDate c_date(year-1, mm, dd);
            QDate s_date(year, m2, d2);
            returnnum = c_date.daysTo(s_date);
            nm1 = monthbeetween(mm,m2,returnnum,'M');
            nm2 = monthbeetween(mm,m2,returnnum,'D');
            //qDebug() << "1:d_s " << dd << "." << mm << " -> " << d2 << "." << m2 << " - " << returnnum << " days " << nm1 << " months +" << nm2 << " days";
        } else {
            QDate c_date(year, mm, dd);
            QDate s_date(year, m2, d2);
            returnnum = c_date.daysTo(s_date);
            nm1 = monthbeetween(mm,m2,returnnum,'M');
            nm2 = monthbeetween(mm,m2,returnnum,'D');
            //qDebug() << "2:d_s " << dd << "." << mm << " -> " << d2 << "." << m2 << " - " << returnnum << " days " << nm1 << " months +" << nm2 << " days";
        }

    }
    if (output_type.indexOf("s_d") != -1) {
        if (QDate(year,mm,dd) < QDate(year,m2,d2))
        {
            QDate c_date(year, mm, dd);
            QDate s_date(year-1, m2, d2);
            returnnum = s_date.daysTo(c_date);
            nm1 = monthbeetween(m2,mm,returnnum,'M');
            nm2 = monthbeetween(m2,mm,returnnum,'D');
            //qDebug() << "3:s_d " << d2 << "." << m2 << " -> " << dd << "." << mm << " - " << returnnum << " days " << nm1 << " months +" << nm2 << " days";
        } else {
            QDate c_date(year, mm, dd);
            QDate s_date(year, m2, d2);
            returnnum = s_date.daysTo(c_date);
            nm1 = monthbeetween(m2,mm,returnnum,'M');
            nm2 = monthbeetween(m2,mm,returnnum,'D');
            //qDebug() << "4:s_d " << d2 << "." << m2 << " -> " << dd << "." << mm << " - " << returnnum << " days " << nm1 << " months +" << nm2 << " days";
        }
    }

    if (output_type.indexOf("full") != -1) {
        if (QDate(year,mm,dd) > QDate(y2,m2,d2))
        {
            QDate c_date(year, mm, dd);
            QDate s_date(y2, m2, d2);
            returnnum = s_date.daysTo(c_date);
            nm1 = monthbeetween(m2,mm,returnnum,'M');
            nm2 = monthbeetween(m2,mm,returnnum,'D');
            //qDebug() << "1:d_s " << dd << "." << mm << " -> " << d2 << "." << m2 << " - " << returnnum << " days " << nm1 << " months +" << nm2 << " days";
        } else {
            QDate c_date(year, mm, dd);
            QDate s_date(y2, m2, d2);
            returnnum = c_date.daysTo(s_date);
            nm1 = monthbeetween(mm,m2,returnnum,'M');
            nm2 = monthbeetween(mm,m2,returnnum,'D');
            //qDebug() << "2:d_s " << dd << "." << mm << " -> " << d2 << "." << m2 << " - " << returnnum << " days " << nm1 << " months +" << nm2 << " days";
        }

    }
    QDate c_date(year, mm, dd);
    int days_year = c_date.daysInYear();

    if (output_type.indexOf("full") != -1 && returnnum < days_year) returnnum = 0;
    wd1 = returnnum/7;
    w1 = returnnum;
    w1 = w1/7-wd1;
    wd2 = round(w1*7);
    if (wd1 == 0) wd2 = 0;
    if (nm1 == 0) nm2 = 0;
    if (output_type == "week_d_s" || output_type == "week_s_d" || output_type == "week_full") {
        if (wd2 == 0)
            returnnum = wd1;
        else returnnum = wd1*10+wd2;
    }
    if (output_type == "month_d_s" || output_type == "month_s_d" || output_type == "month_full") {
        if (nm2 == 0)
            returnnum = nm1;
        else {
            //qDebug() << nm1 << " " << nm2;
        QString numret = QString::number(nm1) + QString::number(nm2);
        returnnum = numret.toInt();
        }
    }

    return returnnum;
}
