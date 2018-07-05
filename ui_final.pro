#-------------------------------------------------
#
# Project created by QtCreator 2018-06-24T11:18:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ui_final
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    handclassifier.cpp \
    videocap.cpp

HEADERS  += mainwindow.h \
    common.h \
    handclassifier.h \
    videocap.h

FORMS    += mainwindow.ui


### opencv (change according to your own opencv)
INCLUDEPATH +=  /usr/include  \
                /usr/include/opencv \
                /usr/include/opencv2
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui

### caffe (change according to your own opencv)
INCLUDEPATH += /home/ubuntu/lichenyang/caffe-1.0/include \
               /home/ubuntu/lichenyang/caffe-1.0/build/src
LIBS += -L/home/ubuntu/lichenyang/caffe-1.0/build/lib -lcaffe

### cuda (change according to your own opencv)
INCLUDEPATH += /usr/local/cuda/include
LIBS += -L/usr/local/cuda/lib   ### Attention, not   "LIBS += -L/usr/local/cuda/lib64"
LIBS += -lcudart -lcublas -lcurand

### others
LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread -llmdb -lleveldb -lstdc++ -lcudnn -lcblas -latlas
