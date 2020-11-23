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
    videoRecordWidget = new VideoRecord(this);
    videoListWidget = new VideoList(this);
    settingsWidget = new Settings(this);
    ui->stackedWidget->addWidget(videoRecordWidget);
    ui->stackedWidget->addWidget(videoListWidget);
    ui->stackedWidget->addWidget(videoPlayWidget);
    ui->stackedWidget->addWidget(settingsWidget);


    connect( videoRecordWidget, SIGNAL(showVideoList()),this, SLOT(setToVideoListView()));
    connect( videoRecordWidget, SIGNAL(showSettings()),this, SLOT(setToSettingsView()));

    connect( videoListWidget, SIGNAL(selectVideoPlay(QString)), this, SLOT(setToVideoPlayView(QString)) );
    connect( videoListWidget, SIGNAL(selectVideoRecord()), this, SLOT(setToVideoRecordView()));

    connect( videoPlayWidget, SIGNAL(showVideoList()),this, SLOT(setToVideoListView()));

    connect(this, SIGNAL(changeView(int)),ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect(settingsWidget, SIGNAL(selectVideoRecord()), this, SLOT(setToVideoRecordView()));

}

MainWindow::~MainWindow()
{
    delete videoPlayWidget;
    delete videoRecordWidget;
    delete ui;
}


void MainWindow::setToVideoPlayView(QString vidPath){
    videoPlayWidget->setMediaSource(vidPath);
    changeVideoView(MainWindow::VIDEOPLAY_WIDGET);
}

void MainWindow::setToVideoListView(){
    changeVideoView(MainWindow::VIDEOLIST_WIDGET);
}

void MainWindow::setToVideoRecordView(){
    videoRecordWidget->startCameraProcessing();
    changeVideoView(MainWindow::VIDEORECORD_WIDGET);
}

void MainWindow::setToSettingsView(){
    changeVideoView(MainWindow::SETTINGS_WIDGET);
}



void MainWindow::changeVideoView(uint16_t view){
    emit changeView(view);
}



