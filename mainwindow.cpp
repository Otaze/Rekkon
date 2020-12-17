#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGlobalStatic>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800,480);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    showFullScreen();
    videoPlayWidget = new VideoPlay(this);
    mediaRecordWidget = new MediaRecord(this);
    mediaListWidget = new MediaList(this);
    mediaDisplayWidget = new MediaDisplay(this);
    settingsWidget = new Settings(this);

    ui->stackedWidget->addWidget(mediaRecordWidget);
    ui->stackedWidget->addWidget(mediaListWidget);
    ui->stackedWidget->addWidget(videoPlayWidget);
    ui->stackedWidget->addWidget(mediaDisplayWidget);
    ui->stackedWidget->addWidget(settingsWidget);


    connect( mediaRecordWidget, SIGNAL(showMediaList()),this, SLOT(setToMediaListView()));
    connect( mediaRecordWidget, SIGNAL(showSettings()),this, SLOT(setToSettingsView()));

    connect( mediaListWidget, SIGNAL(selectMediaDisplay(QString)), this, SLOT(setToMediaDisplayView(QString)) );
    connect( mediaListWidget, SIGNAL(selectMediaRecord()), this, SLOT(setToMediaRecordView()));

    connect( mediaDisplayWidget, SIGNAL(backToMediaList()),this, SLOT(setToMediaListView()));
    connect( mediaDisplayWidget, SIGNAL(playVideo(QString)), this, SLOT(setToVideoPlayView(QString)));

    connect( videoPlayWidget, SIGNAL(showMediaList()),this, SLOT(setToMediaListView()));

    connect(this, SIGNAL(changeView(int)),ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect(settingsWidget, SIGNAL(selectMediaRecord()), this, SLOT(setToMediaRecordView()));


}

MainWindow::~MainWindow()
{
    delete videoPlayWidget;
    delete mediaRecordWidget;
    delete mediaListWidget;
    delete mediaDisplayWidget;
    delete settingsWidget;
    delete ui;
}


void MainWindow::setToVideoPlayView(QString vidPath){
    videoPlayWidget->setMediaSource(vidPath);
    changeVideoView(MainWindow::VIDEOPLAY_WIDGET);
}

void MainWindow::setToMediaListView(){
    mediaListWidget->updateUI();
    changeVideoView(MainWindow::MEDIALIST_WIDGET);
}

void MainWindow::setToMediaRecordView(){
    mediaRecordWidget->restartMediaRecordProcessing();
    changeVideoView(MainWindow::MEDIARECORD_WIDGET);
}

void MainWindow::setToMediaDisplayView(QString vidPath){
    mediaDisplayWidget->setMediaSource(vidPath);
    changeVideoView(MainWindow::MEDIADISPLAY_WIDGET);
}


void MainWindow::setToSettingsView(){
    changeVideoView(MainWindow::SETTINGS_WIDGET);
}



void MainWindow::changeVideoView(uint16_t view){
    emit changeView(view);
}



