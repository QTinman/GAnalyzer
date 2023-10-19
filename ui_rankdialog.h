/********************************************************************************
** Form generated from reading UI file 'rankdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RANKDIALOG_H
#define UI_RANKDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>

QT_BEGIN_NAMESPACE

class Ui_rankDialog
{
public:
    QLabel *label;
    QSplitter *splitter_2;
    QLabel *label_2;
    QLineEdit *phrases;
    QSplitter *splitter;
    QLabel *label_3;
    QSpinBox *min;
    QDialogButtonBox *buttonBox;
    QSplitter *splitter_3;
    QSplitter *splitter_4;
    QCheckBox *Chipers;
    QCheckBox *Dates;
    QCheckBox *Solar;
    QCheckBox *Prime;
    QCheckBox *Triangular;

    void setupUi(QDialog *rankDialog)
    {
        if (rankDialog->objectName().isEmpty())
            rankDialog->setObjectName(QString::fromUtf8("rankDialog"));
        rankDialog->resize(352, 215);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        rankDialog->setWindowIcon(icon);
        label = new QLabel(rankDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 285, 18));
        splitter_2 = new QSplitter(rankDialog);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(9, 33, 174, 26));
        splitter_2->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(splitter_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        splitter_2->addWidget(label_2);
        phrases = new QLineEdit(splitter_2);
        phrases->setObjectName(QString::fromUtf8("phrases"));
        splitter_2->addWidget(phrases);
        splitter = new QSplitter(rankDialog);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(9, 65, 172, 27));
        splitter->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(splitter);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        splitter->addWidget(label_3);
        min = new QSpinBox(splitter);
        min->setObjectName(QString::fromUtf8("min"));
        splitter->addWidget(min);
        buttonBox = new QDialogButtonBox(rankDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(180, 180, 166, 26));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        splitter_3 = new QSplitter(rankDialog);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setGeometry(QRect(10, 90, 179, 120));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_4 = new QSplitter(splitter_3);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        Chipers = new QCheckBox(splitter_4);
        Chipers->setObjectName(QString::fromUtf8("Chipers"));
        splitter_4->addWidget(Chipers);
        Dates = new QCheckBox(splitter_4);
        Dates->setObjectName(QString::fromUtf8("Dates"));
        splitter_4->addWidget(Dates);
        Solar = new QCheckBox(splitter_4);
        Solar->setObjectName(QString::fromUtf8("Solar"));
        splitter_4->addWidget(Solar);
        Prime = new QCheckBox(splitter_4);
        Prime->setObjectName(QString::fromUtf8("Prime"));
        splitter_4->addWidget(Prime);
        Triangular = new QCheckBox(splitter_4);
        Triangular->setObjectName(QString::fromUtf8("Triangular"));
        splitter_4->addWidget(Triangular);
        splitter_3->addWidget(splitter_4);
#if QT_CONFIG(shortcut)
        label_2->setBuddy(phrases);
        label_3->setBuddy(min);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(phrases, min);
        QWidget::setTabOrder(min, Chipers);
        QWidget::setTabOrder(Chipers, Dates);
        QWidget::setTabOrder(Dates, Solar);
        QWidget::setTabOrder(Solar, Prime);
        QWidget::setTabOrder(Prime, Triangular);

        retranslateUi(rankDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, rankDialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, rankDialog, &QDialog::reject);

        QMetaObject::connectSlotsByName(rankDialog);
    } // setupUi

    void retranslateUi(QDialog *rankDialog)
    {
        rankDialog->setWindowTitle(QCoreApplication::translate("rankDialog", "Phrase ranking over a year.", nullptr));
        label->setText(QCoreApplication::translate("rankDialog", "Enter up to two phrases sepparated by comma", nullptr));
        label_2->setText(QCoreApplication::translate("rankDialog", "Phrases", nullptr));
        label_3->setText(QCoreApplication::translate("rankDialog", "Minimum ocurrances", nullptr));
        Chipers->setText(QCoreApplication::translate("rankDialog", "Chipers", nullptr));
        Dates->setText(QCoreApplication::translate("rankDialog", "Limit to Day related", nullptr));
        Solar->setText(QCoreApplication::translate("rankDialog", "Total Solar eclipses (slow)", nullptr));
        Prime->setText(QCoreApplication::translate("rankDialog", "Prime", nullptr));
        Triangular->setText(QCoreApplication::translate("rankDialog", "Triangular", nullptr));
    } // retranslateUi

};

namespace Ui {
    class rankDialog: public Ui_rankDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RANKDIALOG_H
