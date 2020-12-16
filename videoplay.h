#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include "utils/utils.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoPlay;
}
QT_END_NAMESPACE

class VideoPlay : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoPlay(QWidget *parent = nullptr);
    ~VideoPlay();
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void setDuration(int duration);
    void setPosition(int position);
    void updateTimeLabel(int position, int duration);
    void setMediaSource(const QString mediaPath);

signals:
    void showMediaList();

private slots:
    void on_pauseBtn_clicked();

    void on_stopBtn_clicked();

    void on_playBtn_clicked();

    void on_backBtn_clicked();

private:
    Ui::VideoPlay *ui;
    QMediaPlayer* player;
    long m_duration;
    long m_position;
};

#endif // VIDEOPLAY_H
