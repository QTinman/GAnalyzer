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
    tst_ciphers.cpp \
    ../ciphers.cpp \
    ../ciphervalue.cpp \
    ../tools.cpp

HEADERS += \
    ../ciphers.h \
    ../ciphervalue.h \
    ../tools.h
