#ifndef RECORDWORKER_H
#define RECORDWORKER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;

class RecordWorker : public QObject
{
    Q_OBJECT
public:
    explicit RecordWorker(QObject *parent = nullptr);
    ~RecordWorker();

public slots:
    void startRecord(std::string* filepath);
    void stopRecord();
    void recordFrame(cv::Mat* frame);
    void pauseRecord();
    void resumeRecord();
    void setImageSize(const unsigned int width,const  unsigned int height);
    void setCameraFPS(const unsigned int fps);

signals:

private:

    VideoWriter* m_videoWriter;
    bool m_recording;
    bool m_pausing;
    unsigned int m_fps;
    int m_codec;
    bool m_is_rgb; // TODO add function to change RBG to grayscale
    unsigned int m_width;
    unsigned int m_height;

};

#endif // RECORDWORKER_H
