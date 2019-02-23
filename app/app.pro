QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++14

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    lexer.cpp \
    infixToRPN.cpp \
    rpnEvaluator.cpp \
    worker.cpp \
    helpDialog.cpp \
    appExceptions.cpp

HEADERS += \
        MainWindow.h \
    lexer.h \
    token.h \
    infixToRPN.h \
    rpnEvaluator.h \
    rpn.h \
    worker.h \
    workerData.h \
    helpDialog.h \
    appExceptions.h \
    iLexer.h

FORMS += \
        MainWindow.ui \
    helpDialog.ui

CONFIG(debug, debug|release) {
win32:LIBS += -L../qcustomplot/debug -lqcustomplot2
} else {
win32:LIBS += -L../qcustomplot/release -lqcustomplot2
}
unix:LIBS += -lqcustomplot

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
