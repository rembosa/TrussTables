#-------------------------------------------------
#
# Project created by QtCreator 2018-02-27T20:02:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trusstables
TEMPLATE = app

INCLUDEPATH += src src/dialogs

SOURCES += src/bar.cpp \
           src/combobox.cpp \
           src/fabricationerror.cpp \
           src/htmlreportexporter.cpp \
           src/influenceload.cpp \
           src/influenceloadresult.cpp \
           src/joint.cpp \
           src/jointload.cpp \
           src/main.cpp \
           src/modelchecker.cpp \
           src/modelsolver.cpp \
           src/modelviewer.cpp \
           src/point.cpp \
           src/scrollarea.cpp \
           src/solver.cpp \
           src/support.cpp \
           src/supportsettlement.cpp \
           src/thermaleffect.cpp \
           src/unitsandlimits.cpp \
           src/dialogs/abouttrusstablesdialog.cpp \
           src/dialogs/addbarsdialog.cpp \
           src/dialogs/addfabricationerrorsdialog.cpp \
           src/dialogs/addinfluenceloadsdialog.cpp \
           src/dialogs/addjointloadsdialog.cpp \
           src/dialogs/addjointsdialog.cpp \
           src/dialogs/addsupportsdialog.cpp \
           src/dialogs/addsupportsettlementsdialog.cpp \
           src/dialogs/addthermaleffectsdialog.cpp \
           src/dialogs/deletebarsdialog.cpp \
           src/dialogs/deletefabricationerrorsdialog.cpp \
           src/dialogs/deleteinfluenceloadsdialog.cpp \
           src/dialogs/deletejointloadsdialog.cpp \
           src/dialogs/deletejointsdialog.cpp \
           src/dialogs/deletesupportsdialog.cpp \
           src/dialogs/deletesupportsettlementsdialog.cpp \
           src/dialogs/deletethermaleffectsdialog.cpp \
           src/dialogs/editbarsdialog.cpp \
           src/dialogs/editfabricationerrorsdialog.cpp \
           src/dialogs/editinfluenceloadsdialog.cpp \
           src/dialogs/editjointloadsdialog.cpp \
           src/dialogs/editjointsdialog.cpp \
           src/dialogs/editsupportsdialog.cpp \
           src/dialogs/editsupportsettlementsdialog.cpp \
           src/dialogs/editthermaleffectsdialog.cpp \
           src/dialogs/exportmodelimagedialog.cpp \
           src/dialogs/modelareadialog.cpp \
           src/dialogs/notesdialog.cpp \
           src/dialogs/scaledeflectionsdialog.cpp \
           src/dialogs/scaleforcesdialog.cpp \
           src/dialogs/unitsandsetupdialog.cpp

HEADERS  += src/bar.h \
            src/combobox.h \
            src/config.h \
            src/fabricationerror.h \
            src/htmlreportexporter.h \
            src/influenceload.h \
            src/influenceloadresult.h \
            src/joint.h \
            src/jointload.h \
            src/modelchecker.h \
            src/modelsolver.h \
            src/modelviewer.h \
            src/point.h \
            src/scrollarea.h \
            src/solver.h \
            src/support.h \
            src/supportsettlement.h \
            src/thermaleffect.h \
            src/unitsandlimits.h \
            src/dialogs/abouttrusstablesdialog.h \
            src/dialogs/addbarsdialog.h \
            src/dialogs/addfabricationerrorsdialog.h \
            src/dialogs/addinfluenceloadsdialog.h \
            src/dialogs/addjointloadsdialog.h \
            src/dialogs/addjointsdialog.h \
            src/dialogs/addsupportsdialog.h \
            src/dialogs/addsupportsettlementsdialog.h \
            src/dialogs/addthermaleffectsdialog.h \
            src/dialogs/deletebarsdialog.h \
            src/dialogs/deletefabricationerrorsdialog.h \
            src/dialogs/deleteinfluenceloadsdialog.h \
            src/dialogs/deletejointloadsdialog.h \
            src/dialogs/deletejointsdialog.h \
            src/dialogs/deletesupportsdialog.h \
            src/dialogs/deletesupportsettlementsdialog.h \
            src/dialogs/deletethermaleffectsdialog.h \
            src/dialogs/editbarsdialog.h \
            src/dialogs/editfabricationerrorsdialog.h \
            src/dialogs/editinfluenceloadsdialog.h \
            src/dialogs/editjointloadsdialog.h \
            src/dialogs/editjointsdialog.h \
            src/dialogs/editsupportsdialog.h \
            src/dialogs/editsupportsettlementsdialog.h \
            src/dialogs/editthermaleffectsdialog.h \
            src/dialogs/exportmodelimagedialog.h \
            src/dialogs/modelareadialog.h \
            src/dialogs/notesdialog.h \
            src/dialogs/scaledeflectionsdialog.h \
            src/dialogs/scaleforcesdialog.h \
            src/dialogs/unitsandsetupdialog.h

FORMS    += src/ui/abouttrusstablesdialog.ui \
            src/ui/addbarsdialog.ui \
            src/ui/addfabricationerrorsdialog.ui \
            src/ui/addinfluenceloadsdialog.ui \
            src/ui/addjointloadsdialog.ui \
            src/ui/addjointsdialog.ui \
            src/ui/addsupportsdialog.ui \
            src/ui/addsupportsettlementsdialog.ui \
            src/ui/addthermaleffectsdialog.ui \
            src/ui/deletebarsdialog.ui \
            src/ui/deletefabricationerrorsdialog.ui \
            src/ui/deleteinfluenceloadsdialog.ui \
            src/ui/deletejointloadsdialog.ui \
            src/ui/deletejointsdialog.ui \
            src/ui/deletesupportsdialog.ui \
            src/ui/deletesupportsettlementsdialog.ui \
            src/ui/deletethermaleffectsdialog.ui \
            src/ui/editbarsdialog.ui \
            src/ui/editfabricationerrorsdialog.ui \
            src/ui/editinfluenceloadsdialog.ui \
            src/ui/editjointloadsdialog.ui \
            src/ui/editjointsdialog.ui \
            src/ui/editsupportsdialog.ui \
            src/ui/editsupportsettlementsdialog.ui \
            src/ui/editthermaleffectsdialog.ui \
            src/ui/exportmodelimagedialog.ui \
            src/ui/modelareadialog.ui \
            src/ui/notesdialog.ui \
            src/ui/scaledeflectionsdialog.ui \
            src/ui/scaleforcesdialog.ui \
            src/ui/unitsandsetupdialog.ui

RESOURCES += \
    src/rsc.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/gsl/lib/ -llibgsl -llibgslcblas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/gsl/lib/ -llibgsl -llibgslcblas

win32:INCLUDEPATH += $$PWD/gsl/include
win32:DEPENDPATH += $$PWD/gsl/include

win64:CONFIG(release, debug|release): LIBS += -L$$PWD/gsl/lib/ -llibgsl -llibgslcblas
else:win64:CONFIG(debug, debug|release): LIBS += -L$$PWD/gsl/lib/ -llibgsl -llibgslcblas

win64:INCLUDEPATH += $$PWD/gsl/include
win64:DEPENDPATH += $$PWD/gsl/include

unix:LIBS += -L/usr/local/lib -lgsl -L/usr/local/lib -lgslcblas -L/usr/local/lib -lm

win32:RC_ICONS += src/icons/tt.ico
win64:RC_ICONS += src/icons/tt.ico
