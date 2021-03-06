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
    controllers/audioworker.cpp \
    controllers/mediaservice.cpp \
    controllers/rekkoncamworker.cpp \
    cameraworker.cpp \
    main.cpp \
    mainwindow.cpp \
    mediadisplay.cpp \
    models/mediafile.cpp \
    models/settingsstructure.cpp \
    settings.cpp \
    utils/utils.cpp \
    medialist.cpp \
    videoplay.cpp \
    mediarecord.cpp \
    videothumbnailwidget.cpp


HEADERS += \
    controllers/audioworker.h \
    controllers/mediaservice.h \
    controllers/rekkoncamworker.h \
    cameraworker.h \
    mainwindow.h \
    mediadisplay.h \
    models/mediafile.h \
    models/settingsstructure.h \
    settings.h \
    utils/utils.h \
    medialist.h \
    videoplay.h \
    mediarecord.h \
    videothumbnailwidget.h

FORMS += \
    mainwindow.ui \
    mediadisplay.ui \
    settings.ui \
    medialist.ui \
    videoplay.ui \
    mediarecord.ui \
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

unix: LIBS += -lrekkon_mmal_camera -lportaudio -lportaudiocpp -lavcodec -lavutil -lavformat  -lopencv_imgproc  -lopencv_videoio -lopencv_core  -lopencv_imgproc -lopencv_imgcodecs






