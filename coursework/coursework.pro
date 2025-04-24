QT       += core gui
QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    adminwindow.cpp \
    assignmentreportwindow.cpp \
    assignmentwindow.cpp \
    databasemanager.cpp \
    departmentreportwindow.cpp \
    departmentwindow.cpp \
    educationreportwindow.cpp \
    educationwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    personnelreportwindow.cpp \
    personnelwindow.cpp \
    positionreportwindow.cpp \
    positionwindow.cpp \
    reportselectionwindow.cpp \
    tableselectionwindow.cpp \
    userwindow.cpp \
    workexperiencereportwindow.cpp \
    workexperiencewindow.cpp

HEADERS += \
    adminwindow.h \
    assignmentreportwindow.h \
    assignmentwindow.h \
    databasemanager.h \
    departmentreportwindow.h \
    departmentwindow.h \
    educationreportwindow.h \
    educationwindow.h \
    mainwindow.h \
    personnelreportwindow.h \
    personnelwindow.h \
    positionreportwindow.h \
    positionwindow.h \
    reportselectionwindow.h \
    tableselectionwindow.h \
    userwindow.h \
    workexperiencereportwindow.h \
    workexperiencewindow.h

FORMS += \
    adminwindow.ui \
    assignmentreportwindow.ui \
    assignmentwindow.ui \
    departmentreportwindow.ui \
    departmentwindow.ui \
    educationreportwindow.ui \
    educationwindow.ui \
    mainwindow.ui \
    personnelreportwindow.ui \
    personnelwindow.ui \
    positionreportwindow.ui \
    positionwindow.ui \
    reportselectionwindow.ui \
    tableselectionwindow.ui \
    userwindow.ui \
    workexperiencereportwindow.ui \
    workexperiencewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
