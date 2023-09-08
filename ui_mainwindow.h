/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDate_Search;
    QAction *action_Quit;
    QAction *actionNew_Phrase;
    QAction *actionNew_Date;
    QAction *actionSearch_History_txt;
    QAction *actionSet_Year;
    QAction *action_Analyze;
    QAction *action_Eu_date;
    QAction *actionDate_Details;
    QAction *action_Word_details;
    QAction *actionList_Ciphers;
    QAction *actionList_Primenumbers;
    QAction *actionList_Triangular_numbers;
    QAction *actionCompare_phrase_to_history;
    QAction *action_Second_date;
    QAction *actionC_ompare_date_to_History_txt;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionSolar_Eclipses;
    QAction *actionCompare_SolarE_to_history;
    QAction *action_Add_remove_ciphers;
    QAction *actionList_Solar_Eclipses;
    QAction *actionPhrase_ranking;
    QAction *action_Font;
    QAction *action_Print;
    QAction *actionList_History;
    QAction *actionCalendar;
    QAction *actionSentence_split;
    QAction *action_News_headlines_exp;
    QAction *actionNightmode;
    QAction *actionSwap_dates;
    QAction *actionSave_as;
    QAction *actionLine_numbers_in_view;
    QAction *actionRemove_view_history_on_exit;
    QAction *actionDisplay_Welcome;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *textBrowser;
    QSplitter *splitter;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QCheckBox *checkBox;
    QCheckBox *SaveHistory;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Edit;
    QMenu *menu_View;
    QMenu *menu_Tables;
    QMenu *menu_Help;
    QMenu *menuCiphers;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setLocale(QLocale(QLocale::NorwegianBokmal, QLocale::Norway));
        actionDate_Search = new QAction(MainWindow);
        actionDate_Search->setObjectName(QString::fromUtf8("actionDate_Search"));
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        actionNew_Phrase = new QAction(MainWindow);
        actionNew_Phrase->setObjectName(QString::fromUtf8("actionNew_Phrase"));
        actionNew_Date = new QAction(MainWindow);
        actionNew_Date->setObjectName(QString::fromUtf8("actionNew_Date"));
        actionSearch_History_txt = new QAction(MainWindow);
        actionSearch_History_txt->setObjectName(QString::fromUtf8("actionSearch_History_txt"));
        actionSet_Year = new QAction(MainWindow);
        actionSet_Year->setObjectName(QString::fromUtf8("actionSet_Year"));
        action_Analyze = new QAction(MainWindow);
        action_Analyze->setObjectName(QString::fromUtf8("action_Analyze"));
        action_Eu_date = new QAction(MainWindow);
        action_Eu_date->setObjectName(QString::fromUtf8("action_Eu_date"));
        action_Eu_date->setCheckable(true);
        action_Eu_date->setChecked(true);
        actionDate_Details = new QAction(MainWindow);
        actionDate_Details->setObjectName(QString::fromUtf8("actionDate_Details"));
        action_Word_details = new QAction(MainWindow);
        action_Word_details->setObjectName(QString::fromUtf8("action_Word_details"));
        actionList_Ciphers = new QAction(MainWindow);
        actionList_Ciphers->setObjectName(QString::fromUtf8("actionList_Ciphers"));
        actionList_Primenumbers = new QAction(MainWindow);
        actionList_Primenumbers->setObjectName(QString::fromUtf8("actionList_Primenumbers"));
        actionList_Triangular_numbers = new QAction(MainWindow);
        actionList_Triangular_numbers->setObjectName(QString::fromUtf8("actionList_Triangular_numbers"));
        actionCompare_phrase_to_history = new QAction(MainWindow);
        actionCompare_phrase_to_history->setObjectName(QString::fromUtf8("actionCompare_phrase_to_history"));
        action_Second_date = new QAction(MainWindow);
        action_Second_date->setObjectName(QString::fromUtf8("action_Second_date"));
        actionC_ompare_date_to_History_txt = new QAction(MainWindow);
        actionC_ompare_date_to_History_txt->setObjectName(QString::fromUtf8("actionC_ompare_date_to_History_txt"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionSolar_Eclipses = new QAction(MainWindow);
        actionSolar_Eclipses->setObjectName(QString::fromUtf8("actionSolar_Eclipses"));
        actionCompare_SolarE_to_history = new QAction(MainWindow);
        actionCompare_SolarE_to_history->setObjectName(QString::fromUtf8("actionCompare_SolarE_to_history"));
        action_Add_remove_ciphers = new QAction(MainWindow);
        action_Add_remove_ciphers->setObjectName(QString::fromUtf8("action_Add_remove_ciphers"));
        actionList_Solar_Eclipses = new QAction(MainWindow);
        actionList_Solar_Eclipses->setObjectName(QString::fromUtf8("actionList_Solar_Eclipses"));
        actionPhrase_ranking = new QAction(MainWindow);
        actionPhrase_ranking->setObjectName(QString::fromUtf8("actionPhrase_ranking"));
        action_Font = new QAction(MainWindow);
        action_Font->setObjectName(QString::fromUtf8("action_Font"));
        action_Print = new QAction(MainWindow);
        action_Print->setObjectName(QString::fromUtf8("action_Print"));
        actionList_History = new QAction(MainWindow);
        actionList_History->setObjectName(QString::fromUtf8("actionList_History"));
        actionCalendar = new QAction(MainWindow);
        actionCalendar->setObjectName(QString::fromUtf8("actionCalendar"));
        actionSentence_split = new QAction(MainWindow);
        actionSentence_split->setObjectName(QString::fromUtf8("actionSentence_split"));
        actionSentence_split->setCheckable(true);
        actionSentence_split->setChecked(true);
        action_News_headlines_exp = new QAction(MainWindow);
        action_News_headlines_exp->setObjectName(QString::fromUtf8("action_News_headlines_exp"));
        actionNightmode = new QAction(MainWindow);
        actionNightmode->setObjectName(QString::fromUtf8("actionNightmode"));
        actionNightmode->setCheckable(true);
        actionSwap_dates = new QAction(MainWindow);
        actionSwap_dates->setObjectName(QString::fromUtf8("actionSwap_dates"));
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionLine_numbers_in_view = new QAction(MainWindow);
        actionLine_numbers_in_view->setObjectName(QString::fromUtf8("actionLine_numbers_in_view"));
        actionLine_numbers_in_view->setCheckable(true);
        actionRemove_view_history_on_exit = new QAction(MainWindow);
        actionRemove_view_history_on_exit->setObjectName(QString::fromUtf8("actionRemove_view_history_on_exit"));
        actionRemove_view_history_on_exit->setCheckable(true);
        actionRemove_view_history_on_exit->setChecked(true);
        actionDisplay_Welcome = new QAction(MainWindow);
        actionDisplay_Welcome->setObjectName(QString::fromUtf8("actionDisplay_Welcome"));
        actionDisplay_Welcome->setCheckable(true);
        actionDisplay_Welcome->setChecked(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMaximumSize(QSize(800, 556));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textBrowser = new QTextBrowser(groupBox);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(textBrowser);

        splitter = new QSplitter(groupBox);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(splitter);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMaximumSize(QSize(200, 25));
        splitter->addWidget(label_2);
        lineEdit = new QLineEdit(splitter);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);
        lineEdit->setMaximumSize(QSize(200, 25));
        lineEdit->setFocusPolicy(Qt::WheelFocus);
        splitter->addWidget(lineEdit);
        label = new QLabel(splitter);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(60, 16777215));
        splitter->addWidget(label);
        pushButton = new QPushButton(splitter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(50, 16777215));
        splitter->addWidget(pushButton);
        pushButton_2 = new QPushButton(splitter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(50, 16777215));
        splitter->addWidget(pushButton_2);
        checkBox = new QCheckBox(splitter);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setMaximumSize(QSize(130, 16777215));
        splitter->addWidget(checkBox);
        SaveHistory = new QCheckBox(splitter);
        SaveHistory->setObjectName(QString::fromUtf8("SaveHistory"));
        splitter->addWidget(SaveHistory);

        verticalLayout_2->addWidget(splitter);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Edit = new QMenu(menubar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        menu_View = new QMenu(menu_Edit);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        menu_Tables = new QMenu(menubar);
        menu_Tables->setObjectName(QString::fromUtf8("menu_Tables"));
        menu_Help = new QMenu(menubar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        menuCiphers = new QMenu(menubar);
        menuCiphers->setObjectName(QString::fromUtf8("menuCiphers"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
#if QT_CONFIG(shortcut)
        label_2->setBuddy(lineEdit);
#endif // QT_CONFIG(shortcut)

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Edit->menuAction());
        menubar->addAction(menu_Tables->menuAction());
        menubar->addAction(menuCiphers->menuAction());
        menubar->addAction(menu_Help->menuAction());
        menu_File->addAction(actionDate_Search);
        menu_File->addAction(actionSearch_History_txt);
        menu_File->addAction(action_Analyze);
        menu_File->addAction(actionDate_Details);
        menu_File->addAction(actionPhrase_ranking);
        menu_File->addAction(action_Word_details);
        menu_File->addAction(actionCompare_phrase_to_history);
        menu_File->addAction(actionC_ompare_date_to_History_txt);
        menu_File->addAction(actionCompare_SolarE_to_history);
        menu_File->addAction(actionSolar_Eclipses);
        menu_File->addSeparator();
        menu_File->addAction(actionSave_as);
        menu_File->addAction(action_Print);
        menu_File->addAction(action_Quit);
        menu_Edit->addAction(actionNew_Phrase);
        menu_Edit->addAction(actionNew_Date);
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Second_date);
        menu_Edit->addAction(actionSet_Year);
        menu_Edit->addAction(action_Eu_date);
        menu_Edit->addAction(actionSwap_dates);
        menu_Edit->addSeparator();
        menu_Edit->addAction(menu_View->menuAction());
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Font);
        menu_View->addAction(actionNightmode);
        menu_View->addAction(actionSentence_split);
        menu_View->addAction(actionLine_numbers_in_view);
        menu_View->addAction(actionRemove_view_history_on_exit);
        menu_View->addAction(actionDisplay_Welcome);
        menu_Tables->addAction(actionList_Ciphers);
        menu_Tables->addAction(actionList_Primenumbers);
        menu_Tables->addAction(actionList_Triangular_numbers);
        menu_Tables->addAction(actionList_Solar_Eclipses);
        menu_Tables->addAction(actionList_History);
        menu_Tables->addAction(actionCalendar);
        menu_Tables->addAction(action_News_headlines_exp);
        menu_Help->addAction(actionHelp);
        menu_Help->addAction(actionAbout);
        menuCiphers->addAction(action_Add_remove_ciphers);

        retranslateUi(MainWindow);
        QObject::connect(action_Quit, &QAction::triggered, MainWindow, &QMainWindow::close);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gematria Analyzer", nullptr));
        actionDate_Search->setText(QCoreApplication::translate("MainWindow", "Date &Search", nullptr));
#if QT_CONFIG(shortcut)
        actionDate_Search->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Quit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
        actionNew_Phrase->setText(QCoreApplication::translate("MainWindow", "New &Phrase", nullptr));
#if QT_CONFIG(shortcut)
        actionNew_Phrase->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNew_Date->setText(QCoreApplication::translate("MainWindow", "&New Date", nullptr));
#if QT_CONFIG(shortcut)
        actionNew_Date->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSearch_History_txt->setText(QCoreApplication::translate("MainWindow", "Search &History.txt", nullptr));
#if QT_CONFIG(shortcut)
        actionSearch_History_txt->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSet_Year->setText(QCoreApplication::translate("MainWindow", "Set &Year", nullptr));
        action_Analyze->setText(QCoreApplication::translate("MainWindow", "&Analyze", nullptr));
#if QT_CONFIG(shortcut)
        action_Analyze->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Eu_date->setText(QCoreApplication::translate("MainWindow", "&Date format", nullptr));
        actionDate_Details->setText(QCoreApplication::translate("MainWindow", "&Date Details", nullptr));
#if QT_CONFIG(shortcut)
        actionDate_Details->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Word_details->setText(QCoreApplication::translate("MainWindow", "&Word details", nullptr));
#if QT_CONFIG(shortcut)
        action_Word_details->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+W", nullptr));
#endif // QT_CONFIG(shortcut)
        actionList_Ciphers->setText(QCoreApplication::translate("MainWindow", "List Ciphers", nullptr));
        actionList_Primenumbers->setText(QCoreApplication::translate("MainWindow", "List Prime numbers", nullptr));
        actionList_Triangular_numbers->setText(QCoreApplication::translate("MainWindow", "List Triangular numbers", nullptr));
        actionCompare_phrase_to_history->setText(QCoreApplication::translate("MainWindow", "&Compare phrase to history", nullptr));
#if QT_CONFIG(shortcut)
        actionCompare_phrase_to_history->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+T", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Second_date->setText(QCoreApplication::translate("MainWindow", "&Second date", nullptr));
        actionC_ompare_date_to_History_txt->setText(QCoreApplication::translate("MainWindow", "C&ompare date to History.txt", nullptr));
#if QT_CONFIG(shortcut)
        actionC_ompare_date_to_History_txt->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionHelp->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionSolar_Eclipses->setText(QCoreApplication::translate("MainWindow", "Solar Eclipses", nullptr));
        actionCompare_SolarE_to_history->setText(QCoreApplication::translate("MainWindow", "Compare Solar&Ec to history", nullptr));
#if QT_CONFIG(shortcut)
        actionCompare_SolarE_to_history->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Add_remove_ciphers->setText(QCoreApplication::translate("MainWindow", "&Add remove ciphers", nullptr));
        actionList_Solar_Eclipses->setText(QCoreApplication::translate("MainWindow", "List Solar Eclipses", nullptr));
        actionPhrase_ranking->setText(QCoreApplication::translate("MainWindow", "Phrase &ranking", nullptr));
#if QT_CONFIG(shortcut)
        actionPhrase_ranking->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Font->setText(QCoreApplication::translate("MainWindow", "&Font", nullptr));
        action_Print->setText(QCoreApplication::translate("MainWindow", "&Print", nullptr));
        actionList_History->setText(QCoreApplication::translate("MainWindow", "List History", nullptr));
        actionCalendar->setText(QCoreApplication::translate("MainWindow", "Calendar", nullptr));
        actionSentence_split->setText(QCoreApplication::translate("MainWindow", "Sentence spli&t", nullptr));
        action_News_headlines_exp->setText(QCoreApplication::translate("MainWindow", "&News headlines (exp)", nullptr));
        actionNightmode->setText(QCoreApplication::translate("MainWindow", "Nightmode", nullptr));
        actionSwap_dates->setText(QCoreApplication::translate("MainWindow", "Swap dates", nullptr));
        actionSave_as->setText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
        actionLine_numbers_in_view->setText(QCoreApplication::translate("MainWindow", "Line numbers in view", nullptr));
        actionRemove_view_history_on_exit->setText(QCoreApplication::translate("MainWindow", "Remove view-history on exit", nullptr));
        actionDisplay_Welcome->setText(QCoreApplication::translate("MainWindow", "Display Welcome", nullptr));
        groupBox->setTitle(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "Enter Phrase or a Comman&d :", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Date +/-", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
#if QT_CONFIG(shortcut)
        pushButton->setShortcut(QCoreApplication::translate("MainWindow", "+", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_2->setShortcut(QCoreApplication::translate("MainWindow", "-", nullptr));
#endif // QT_CONFIG(shortcut)
        checkBox->setText(QCoreApplication::translate("MainWindow", "To&ggle Analyze", nullptr));
#if QT_CONFIG(shortcut)
        checkBox->setShortcut(QCoreApplication::translate("MainWindow", "Alt+G", nullptr));
#endif // QT_CONFIG(shortcut)
        SaveHistory->setText(QCoreApplication::translate("MainWindow", "&Save", nullptr));
        menu_File->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menu_Edit->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
        menu_View->setTitle(QCoreApplication::translate("MainWindow", "&View", nullptr));
        menu_Tables->setTitle(QCoreApplication::translate("MainWindow", "&Tables", nullptr));
        menu_Help->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
        menuCiphers->setTitle(QCoreApplication::translate("MainWindow", "&Ciphers", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
