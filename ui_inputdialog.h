/********************************************************************************
** Form generated from reading UI file 'inputdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTDIALOG_H
#define UI_INPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_inputDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *inputDialog)
    {
        if (inputDialog->objectName().isEmpty())
            inputDialog->setObjectName(QString::fromUtf8("inputDialog"));
        inputDialog->resize(375, 100);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        inputDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(inputDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(inputDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(inputDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(inputDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(inputDialog);
        QObject::connect(label, &QLabel::linkActivated, inputDialog, &QDialog::open);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, inputDialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, inputDialog, &QDialog::reject);

        QMetaObject::connectSlotsByName(inputDialog);
    } // setupUi

    void retranslateUi(QDialog *inputDialog)
    {
        inputDialog->setWindowTitle(QCoreApplication::translate("inputDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("inputDialog", "Text", nullptr));
    } // retranslateUi

};

namespace Ui {
    class inputDialog: public Ui_inputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTDIALOG_H
