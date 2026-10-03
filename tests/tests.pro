QT       += testlib gui widgets network xml printsupport

CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app
TARGET   = ganatests

INCLUDEPATH += ..

# The engine under test, and the one file it needs. tools.cpp comes along
# because the legacy ciphers delegate into it - that delegation is the point,
# so testing against a copy of it would test nothing.
SOURCES += \
    main.cpp \
    tst_ciphers.cpp \
    tst_analyzer.cpp \
    tst_ai.cpp \
    tst_selection.cpp \
    ../ciphers.cpp \
    ../ciphervalue.cpp \
    ../analyzer.cpp \
    ../decodegraph.cpp \
    ../historyindex.cpp \
    ../aiprovider.cpp \
    ../cipherselection.cpp \
    ../tools.cpp

# tools.cpp includes mainwindow.h, which reaches httpdownload.h, which includes
# ui_httpdownload.h. Naming the form here makes uic generate that header for
# this project too.
#
# It used to be satisfied by a copy of the generated header committed in the
# source tree - which is what made the whole repository build against stale uic
# output, and is the thing being removed. This is the honest version of the same
# dependency: declared, and regenerated from the form every build.
FORMS += \
    ../httpdownload.ui

HEADERS += \
    ../ciphers.h \
    ../ciphervalue.h \
    ../analyzer.h \
    ../decodegraph.h \
    ../historyindex.h \
    ../aiprovider.h \
    ../cipherselection.h \
    ../tools.h
