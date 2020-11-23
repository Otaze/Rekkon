#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include <QObject>
#include <QImage>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <string>

using namespace cv;

class CameraWorker : public QObject
{
    Q_OBJECT
public:
    explicit CameraWorker(QObject *parent = nullptr);
    ~CameraWorker();
    long duration();


public slots:
    void setupCamera(const int device);
    void setupCamera(const std::string filepath);
    void getImageFromPositionInMillisec(const long position_milliseconds);
    void signalProcessImage();
    void releaseCamera();

signals:
    void sendFrame(cv::Mat* frame);
private:
    VideoCapture * m_cameraCap;
    Mat* m_frame;
    Mat* m_frame_processed;
};

#endif // CAMERAWORKER_H
