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
    void setRotation(const unsigned int rotation);
    void setCameraFPS(const unsigned int fps);

    void releaseCamera();
    void openCamera();

    void startStillPreview();
    void stopStillPreview();
    bool isPreviewStillOpened(){ return m_cam_control.isStillPreviewOpened();}
    void setStillPreviewSize(const unsigned int width,const  unsigned int height);

    void startVideoPreview();
    void stopVideoPreview();
    bool isPreviewVideoOpened(){ return m_cam_control.isVideoPreviewOpened();}
    void setVideoPreviewSize(const unsigned int width,const  unsigned int height);

    void startVideoRecord(std::string* filepath);
    void stopVideoRecord();
    void setVideoRecordSize(const unsigned int width,const  unsigned int height);

    void startStillRecord(std::string* filepath);
    void setStillRecordSize(const unsigned int width,const  unsigned int height);

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
