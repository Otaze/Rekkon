#ifndef VIDEOPREVIEWWIDGET_H
#define VIDEOPREVIEWWIDGET_H

#include <QWidget>
#include "utils/utils.h"
#include <filesystem>
#include <QFontMetrics>
#include "cameraworker.h"
#include <string>

namespace Ui {
class VideoThumbnailWidget;
}


class VideoThumbnailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoThumbnailWidget(QWidget *parent = nullptr);
    ~VideoThumbnailWidget();
    void setMediaSource(const QString mediaPath);
    const QString* getMediaSource();

public slots:
    void processImage(cv::Mat* image);

signals:
    void sendSetupCam(std::string);
    void getImageFromPositionInMillisec(long);
    void releaseCamera();


private:
    void updateTitle();
    void updateDuration();
    Ui::VideoThumbnailWidget *ui;
    QString* m_mediaSrcPath;
    CameraWorker* m_vidWorker;
};

#endif // VIDEOPREVIEWWIDGET_H
