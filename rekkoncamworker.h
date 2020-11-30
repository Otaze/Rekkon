#ifndef REKKONCAMWORKER_H
#define REKKONCAMWORKER_H

#include <QObject>
#include <QImage>
#include <string>
#include <opencv4/opencv2/core.hpp>

#include <rekkon_mmal_camera/rekkoncamcontrol.h>

using namespace cv;

class RekkonCamWorker : public QObject
{
    Q_OBJECT
public:
    explicit RekkonCamWorker(QObject *parent = nullptr);
    ~RekkonCamWorker();



public slots:
    void signalProcessImage();
    void pause();
    void resume();
    void setVideoPreviewSize(const unsigned int width,const  unsigned int height);
    void setVideoRecordSize(const unsigned int width,const  unsigned int height);
    void setRotation(const unsigned int rotation);
    void setCameraFPS(const unsigned int fps);

    void startVideoPreview();
    void stopVideoPreview();
    void startVideoRecord(std::string* filepath);
    void stopVideoRecord();

signals:
    void sendFrame(cv::Mat* frame);
    void errorRecord();


private:
    RekkonCamControl m_cam_control;
    Mat* m_frame;
    Mat* m_frame_processed;
    bool m_pause;

};

#endif // REKKONCAMWORKER_H
