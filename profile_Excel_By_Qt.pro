QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    tools/src/data_profile/Attri_UL_Unit_NUMBER.cpp \
    tools/src/data_profile/SITE_PART.cpp \
    tools/src/data_profile/Valid_Data.cpp \
    tools/src/data_profile/operate_file.cpp \
    tools/src/data_profile/ul_unit_number.cpp \
    tools/src/draw_picture/chart.cpp \
    widgets/widget_all_attri_show/src/widget_all_attri_show.cpp \

HEADERS += \
    mainwindow.h \
    tools/include/HEADER_01.h \
    tools/include/data_profile/Attri_UL_Unit_NUMBER.h \
    tools/include/data_profile/SITE_PART.h \
    tools/include/data_profile/Valid_Data.h \
    tools/include/data_profile/operate_file.h \
    tools/include/data_profile/ul_unit_number.h \
    tools/include/draw_picture/chart.h \
    widgets/widget_all_attri_show/include/widget_all_attri_show.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
