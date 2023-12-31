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
    Main_Window.cpp \
    actions/buttons/src/Button_All_Attri.cpp \
    actions/buttons/src/Button_Merge_File.cpp \
    actions/buttons/src/Button_Select_Test_Plan_File.cpp \
    actions/buttons/src/Button_Temperature.cpp \
    actions/buttons/src/Button_Time_Compare.cpp \
    main.cpp \
    tools/src/data_profile/Attri_Unit_UL.cpp \
    tools/src/data_profile/Axis_Y_Step.cpp \
    tools/src/data_profile/Compare_Plan_UUL.cpp \
    tools/src/data_profile/File_To_Targetfile.cpp \
    tools/src/data_profile/Make_Merge_File.cpp \
    tools/src/data_profile/Make_Normal_File.cpp \
    tools/src/data_profile/Make_Ration_File.cpp \
    tools/src/data_profile/Make_Temperature_File.cpp \
    tools/src/data_profile/Make_Timc_File.cpp \
    tools/src/data_profile/Scatter_Site_Scatter_Part.cpp \
    tools/src/data_profile/Site_Part.cpp \
    tools/src/data_profile/Targetfile_Ration_Valid_Data.cpp \
    tools/src/data_profile/Targetfile_Temperature_Valid_Data.cpp \
    tools/src/data_profile/Targetfile_Time_Valid_Data.cpp \
    tools/src/data_profile/Targetfile_Valid_Data.cpp \
    tools/src/data_profile/Unit_UL_Block.cpp \
    tools/src/data_profile/Unit_UL_Str.cpp \
    tools/src/data_profile/Unit_Ul.cpp \
    tools/src/draw_picture/Chart.cpp \
    tools/src/draw_picture/Chart_Category_Value.cpp \
    tools/src/draw_picture/My_Chart_View.cpp \
    widgets/widget_all_attri_show/src/Widget_All_Attri_Show.cpp \
    widgets/widget_temperature_show/src/Widget_Temperature_Show.cpp \
    widgets/widget_time_show/src/Widget_Time_Show.cpp

HEADERS += \
    Main_Window.h \
    actions/buttons/include/Button_All_Attri.h \
    actions/buttons/include/Button_Merge_File.h \
    actions/buttons/include/Button_Select_Test_Plan_File.h \
    actions/buttons/include/Button_Temperature.h \
    actions/buttons/include/Button_Time_Compare.h \
    tools/include/Tools_Header_Proxy_01.h \
    tools/include/data_profile/Attri_Unit_Ul.h \
    tools/include/data_profile/Axis_Y_Step.h \
    tools/include/data_profile/Compare_Plan_UUL.h \
    tools/include/data_profile/File_To_Targetfile.h \
    tools/include/data_profile/Make_Merge_File.h \
    tools/include/data_profile/Make_Normal_File.h \
    tools/include/data_profile/Make_Ration_File.h \
    tools/include/data_profile/Make_Temperature_File.h \
    tools/include/data_profile/Make_Timc_File.h \
    tools/include/data_profile/Scatter_Site_Scatter_Part.h \
    tools/include/data_profile/Site_Part.h \
    tools/include/data_profile/Targetfile_Ration_Valid_Data.h \
    tools/include/data_profile/Targetfile_Temperature_Valid_Data.h \
    tools/include/data_profile/Targetfile_Time_Valid_Data.h \
    tools/include/data_profile/Targetfile_Valid_Data.h \
    tools/include/data_profile/Unit_UL_Block.h \
    tools/include/data_profile/Unit_UL_Str.h \
    tools/include/data_profile/Unit_Ul.h \
    tools/include/draw_picture/Chart.h \
    tools/include/draw_picture/Chart_Category_Value.h \
    tools/include/draw_picture/My_Chart_View.h \
    widgets/Widgets_Header_Proxy_01.h \
    widgets/widget_all_attri_show/include/Widget_All_Attri_Show.h \
    widgets/widget_temperature_show/include/Widget_Temperature_Show.h \
    widgets/widget_time_show/include/Widget_Time_Show.h

FORMS += \
    Main_Window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = logo.ico

DISTFILES +=

RESOURCES +=

