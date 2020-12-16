#include "videoplay.h"
#include "ui_videoplay.h"


VideoPlay::VideoPlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VideoPlay)
{
    ui->setupUi(this);

    setFixedSize(800,480);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->vidWid);

    connect(player,&QMediaPlayer::durationChanged,ui->videoSliderBar,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,ui->videoSliderBar,&QSlider::setValue);
    connect(ui->videoSliderBar,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);

    connect(player,&QMediaPlayer::durationChanged,this,&VideoPlay::setDuration);
    connect(player,&QMediaPlayer::positionChanged,this,&VideoPlay::setPosition);

    m_duration = 0;
    m_position = 0;
}

VideoPlay::~VideoPlay()
{
    delete ui;
}

void VideoPlay::setMediaSource(const QString mediaPath)
{
    player->setMedia(QUrl::fromLocalFile(mediaPath));
}


void VideoPlay::playVideo()
{

   player->play();

}

void VideoPlay::pauseVideo()
{

   player->pause();

}

void VideoPlay::stopVideo()
{

   player->stop();

}


void VideoPlay::on_playBtn_clicked()
{
    playVideo();
}

void VideoPlay::on_pauseBtn_clicked()
{
    pauseVideo();
}

void VideoPlay::on_stopBtn_clicked()
{
    stopVideo();
}


void VideoPlay::on_backBtn_clicked()
{
    stopVideo();
    emit showMediaList();
}

void VideoPlay::setDuration(int duration) {
    this->m_duration = duration;
    updateTimeLabel(this->m_position,this->m_duration);
}

void VideoPlay::setPosition(int position) {
    this->m_position = position;
    updateTimeLabel(this->m_position,this->m_duration);
}

void VideoPlay::updateTimeLabel(int position, int duration) {
    ui->timeLabel->setText(Utils::formatMillisecondsToDateString(position) + " / " + Utils::formatMillisecondsToDateString(duration));
}



