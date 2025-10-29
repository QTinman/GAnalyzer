#include "headdialog.h"
#include "ui_headdialog.h"
//#include "mainwindow.h"

#include "gcalc.h"
//#include "tools.h"
#include <QIntValidator>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QTextStream>


headDialog::headDialog(const QString &filename, const QString &label, QWidget *parent)
    : QDialog(parent), ui(new Ui::headDialog), _filename(filename), _label(label)
{
    ui->setupUi(this);
    this->setWindowTitle("News Headlines - " + _label);
    QString nightm = readSettings("settings.txt", "nightmode");
    bool nightmode = (nightm == "1" || nightm.toLower() == "true");

    if (nightmode) {
        this->setStyleSheet("background-color: #1a1a1a; color: lightgray;");

        ui->textBrowser->setStyleSheet("background-color: #1a1a1a; color: lightgray; border: 1px solid #333;");
        ui->lineEdit->setStyleSheet("background-color: #2a2a2a; color: white; border: 1px solid #555;");
        ui->current_url->setStyleSheet("color: lightgray;");

        ui->SaveNewsFilter->setStyleSheet("background-color: #333333; color: white; border: 1px solid #444;");
        ui->ajustinput->setStyleSheet("background-color: #333333; color: white;");
        ui->label_2->setStyleSheet("color: lightgray;");
        ui->numberofchar->setStyleSheet("color: lightgray;");

        ui->englisordinal->setStyleSheet("color: lightgray;");
        ui->fullreduction->setStyleSheet("color: lightgray;");
        ui->reverseordinal->setStyleSheet("color: lightgray;");
        ui->reversefull->setStyleSheet("color: lightgray;");
        ui->textBrowser->setStyleSheet(
            "QTextBrowser { background-color: #1a1a1a; color: lightgrey; }"
            "a { color: grey; text-decoration: none; }"
            "a:hover { text-decoration: underline; }"
            );
    }

    // 2. Also override the palette (in case system settings are enforced)
    QPalette p = ui->textBrowser->palette();
    p.setColor(QPalette::Link, QColor("grey"));
    p.setColor(QPalette::Text, QColor("lightgrey"));
    ui->textBrowser->setPalette(p);

    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setTextInteractionFlags(Qt::TextBrowserInteraction);

    ui->current_url->setText(_label);
    List = getheadlines(_filename, 20);

    ui->lineEdit->setValidator(new QIntValidator(0, 9999, this));
    ui->lineEdit->setFocus();
    ui->lineEdit->clear();

    for (const auto &i : List) {
        ui->textBrowser->append(i);
    }

    ui->SaveNewsFilter->hide();
    ui->ajustinput->setDisabled(true);
    ui->label_2->hide();
    ui->numberofchar->hide();
}

void headDialog::on_lineEdit_returnPressed()
{
    QStringList filterlist, displaylist;
    QString nstype;
    int nslist = ui->lineEdit->text().toUInt(), count, startw, endw, red = 0, rev = 0;

    if (ui->englisordinal->isChecked()) nstype = "English Ordinal";
    if (ui->fullreduction->isChecked()) { red = 1; nstype = "Full Reduction"; }
    if (ui->reverseordinal->isChecked()) { rev = 1; nstype = "Reverse Ordinal"; }
    if (ui->reversefull->isChecked()) { red = 1; rev = 1; nstype = "Reverse Full Reduction"; }

    for (const auto& i : List) {
        if (i.trimmed().isEmpty()) continue;

        filterlist = i.split(" ");
        QVector<int> sum;
        for (const auto& l : filterlist)
            sum.append(getwordnumericvalue(l.toUtf8().constData(), red, rev, 0));

        bool found = false;
        int result = 0, c = 0, loops = 0;
        startw = endw = -1;

        while (!found && loops < sum.size()) {
            result += sum[c];
            if (result == nslist) {
                found = true;
                startw = loops;
                endw = c;
            }
            if (c >= sum.size() - 1 || result > nslist) {
                result = 0;
                loops++;
                c = loops;
            } else if (!found) c++;
        }

        if (!found) {
            displaylist << i;
        } else {
            QString formatted;
            for (int j = 0; j < filterlist.size(); ++j) {
                if (j >= startw && j <= endw && sum[j] != 0)
                    formatted += Qformattext(filterlist[j].toUtf8().constData(), 2, 1) + " ";
                else
                    formatted += filterlist[j] + " ";
            }
            displaylist << formatted.trimmed() + " - " + nstype + ": " + Qformattext(QString::number(nslist).toUtf8().constData(), 1, 1);
        }
    }

    ui->textBrowser->clear();
    for (const auto& i : displaylist)
        ui->textBrowser->append(i);
}


void headDialog::on_englisordinal_toggled(bool checked) {
    if (checked) emit on_lineEdit_returnPressed();
}

void headDialog::on_fullreduction_toggled(bool checked) {
    if (checked) emit on_lineEdit_returnPressed();
}

void headDialog::on_reverseordinal_toggled(bool checked) {
    if (checked) emit on_lineEdit_returnPressed();
}

void headDialog::on_reversefull_toggled(bool checked) {
    if (checked) emit on_lineEdit_returnPressed();
}

headDialog::~headDialog() {
    delete ui;
}
