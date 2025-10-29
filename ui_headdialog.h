/********************************************************************************
** Form generated from reading UI file 'headdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEADDIALOG_H
#define UI_HEADDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_headDialog
{
public:
    QGridLayout *gridLayout_2;
    QTextBrowser *textBrowser;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QRadioButton *reverseordinal;
    QLabel *label_2;
    QRadioButton *englisordinal;
    QLabel *label;
    QRadioButton *fullreduction;
    QRadioButton *reversefull;
    QLineEdit *lineEdit;
    QLineEdit *ajustinput;
    QPushButton *SaveNewsFilter;
    QPushButton *Adjust;
    QSpinBox *numberofchar;
    QLabel *current_url;

    void setupUi(QDialog *headDialog)
    {
        if (headDialog->objectName().isEmpty())
            headDialog->setObjectName(QString::fromUtf8("headDialog"));
        headDialog->resize(711, 474);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        headDialog->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(headDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textBrowser = new QTextBrowser(headDialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout_2->addWidget(textBrowser, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(headDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setAutoDefault(false);

        gridLayout->addWidget(pushButton, 2, 5, 1, 1);

        reverseordinal = new QRadioButton(headDialog);
        reverseordinal->setObjectName(QString::fromUtf8("reverseordinal"));

        gridLayout->addWidget(reverseordinal, 2, 2, 1, 1);

        label_2 = new QLabel(headDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        englisordinal = new QRadioButton(headDialog);
        englisordinal->setObjectName(QString::fromUtf8("englisordinal"));
        englisordinal->setChecked(true);

        gridLayout->addWidget(englisordinal, 2, 0, 1, 1);

        label = new QLabel(headDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        fullreduction = new QRadioButton(headDialog);
        fullreduction->setObjectName(QString::fromUtf8("fullreduction"));

        gridLayout->addWidget(fullreduction, 2, 1, 1, 1);

        reversefull = new QRadioButton(headDialog);
        reversefull->setObjectName(QString::fromUtf8("reversefull"));

        gridLayout->addWidget(reversefull, 2, 3, 1, 1);

        lineEdit = new QLineEdit(headDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        ajustinput = new QLineEdit(headDialog);
        ajustinput->setObjectName(QString::fromUtf8("ajustinput"));

        gridLayout->addWidget(ajustinput, 0, 3, 1, 1);

        SaveNewsFilter = new QPushButton(headDialog);
        SaveNewsFilter->setObjectName(QString::fromUtf8("SaveNewsFilter"));

        gridLayout->addWidget(SaveNewsFilter, 2, 4, 1, 1);

        Adjust = new QPushButton(headDialog);
        Adjust->setObjectName(QString::fromUtf8("Adjust"));
        Adjust->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(Adjust, 0, 5, 1, 1);

        numberofchar = new QSpinBox(headDialog);
        numberofchar->setObjectName(QString::fromUtf8("numberofchar"));

        gridLayout->addWidget(numberofchar, 0, 4, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

        current_url = new QLabel(headDialog);
        current_url->setObjectName(QString::fromUtf8("current_url"));
        current_url->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(current_url, 0, 0, 1, 1);

#if QT_CONFIG(shortcut)
        label_2->setBuddy(ajustinput);
        label->setBuddy(lineEdit);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEdit, englisordinal);
        QWidget::setTabOrder(englisordinal, fullreduction);
        QWidget::setTabOrder(fullreduction, reverseordinal);
        QWidget::setTabOrder(reverseordinal, reversefull);
        QWidget::setTabOrder(reversefull, ajustinput);
        QWidget::setTabOrder(ajustinput, numberofchar);
        QWidget::setTabOrder(numberofchar, SaveNewsFilter);
        QWidget::setTabOrder(SaveNewsFilter, pushButton);
        QWidget::setTabOrder(pushButton, textBrowser);

        retranslateUi(headDialog);
        QObject::connect(pushButton, &QPushButton::clicked, headDialog, &QDialog::accept);

        QMetaObject::connectSlotsByName(headDialog);
    } // setupUi

    void retranslateUi(QDialog *headDialog)
    {
        headDialog->setWindowTitle(QCoreApplication::translate("headDialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("headDialog", "Close", nullptr));
        reverseordinal->setText(QCoreApplication::translate("headDialog", "Reverse ordinal", nullptr));
        label_2->setText(QCoreApplication::translate("headDialog", "Adjustment string", nullptr));
        englisordinal->setText(QCoreApplication::translate("headDialog", "English ordinal", nullptr));
        label->setText(QCoreApplication::translate("headDialog", "&Cipher search:", nullptr));
        fullreduction->setText(QCoreApplication::translate("headDialog", "Full Reduction", nullptr));
        reversefull->setText(QCoreApplication::translate("headDialog", "Reverse full", nullptr));
        lineEdit->setInputMask(QString());
        SaveNewsFilter->setText(QCoreApplication::translate("headDialog", "Save", nullptr));
        Adjust->setText(QCoreApplication::translate("headDialog", "Adjust news", nullptr));
        current_url->setText(QCoreApplication::translate("headDialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class headDialog: public Ui_headDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEADDIALOG_H
