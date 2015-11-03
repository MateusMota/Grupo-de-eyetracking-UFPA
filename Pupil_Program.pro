#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T12:39:54
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Pupil_Program
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_nonfree

SOURCES += main.cpp \
    helpers.cpp \
    findEyeCenter.cpp \
    findEyeCorner.cpp

HEADERS += \
    helpers.h \
    constants.h \
    findEyeCenter.h \
    findEyeCorner.h
