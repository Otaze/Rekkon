#ifndef RASPICAMWORKER_H
#define RASPICAMWORKER_H

#include <QObject>
#include <QImage>
#include <raspicam/raspicam_cv.h>
#include <raspicam/raspicamtypes.h>
#include <string>

using namespace cv;

class RaspiCamWorker : public QObject
{
    Q_OBJECT
public:
    explicit RaspiCamWorker(QObject *parent = nullptr);
    ~RaspiCamWorker();



public slots:
    void openCamera();
    void signalProcessImage();
    void releaseCamera();
    void pause();
    void resume();
    void setImageSize(const unsigned int width,const  unsigned int height);
    void setRotation(const unsigned int rotation);
    void setCameraFPS(const unsigned int fps);
    void startRecord(std::string* filepath);
    void stopRecord();

signals:
    void sendFrame(cv::Mat* frame);
    void errorRecord();


private:
    raspicam::RaspiCam_Cv * m_cameraCap;
    Mat* m_frame;
    Mat* m_frame_processed;
    bool m_pause;

};

#endif // RASPICAMWORKER_H
