QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets

CONFIG += c++17

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
    audioworker.cpp \
    cameraworker.cpp \
    main.cpp \
    mainwindow.cpp \
    rekkoncamworker.cpp \
    recordworker.cpp \
    settings.cpp \
    settingsstructure.cpp \
    utils.cpp \
    videolist.cpp \
    videoplay.cpp \
    videorecord.cpp \
    videothumbnailwidget.cpp


HEADERS += \
    audioworker.h \
    cameraworker.h \
    mainwindow.h \
    rekkoncamworker.h \
    recordworker.h \
    settings.h \
    settingsstructure.h \
    utils.h \
    videolist.h \
    videoplay.h \
    videorecord.h \
    videothumbnailwidget.h

FORMS += \
    mainwindow.ui \
    settings.ui \
    videolist.ui \
    videoplay.ui \
    videorecord.ui \
    videothumbnailwidget.ui


TRANSLATIONS += \
    Rekkon_en_150.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

unix:INCLUDEPATH += \
        /usr/include/opencv4/ \
        /usr/include/opencv4/opencv2/ \
        /usr/include/raspicam \
        /usr/include/portaudiocpp \
        /usr/include/arm-linux-gnuabihf \
        /usr/include/rekkon_mmal_camera \
        /usr/include/interface \
        /opt/vc/include/interface
        
#unix: LIBS += -lraspicam_cv -lraspicam -lportaudio -lportaudiocpp -lavcodec -lavutil -lavformat -lopencv_core -lopencv_videoio -lopencv_imgproc

unix: LIBS += -lrekkon_mmal_camera -lportaudio -lportaudiocpp -lavcodec -lavutil -lavformat -lopencv_core -lopencv_videoio -lopencv_imgproc






