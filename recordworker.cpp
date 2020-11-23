#include "recordworker.h"

RecordWorker::RecordWorker(QObject *parent) : QObject(parent),
        m_recording(false),
        m_pausing(false),
        m_fps(30),
        m_codec(VideoWriter::fourcc('H', '2', '6', '4')),
        m_is_rgb(true),
        m_width(1920),
        m_height(1080)
{
    m_videoWriter = new VideoWriter;

}

RecordWorker::~RecordWorker()
{
    delete m_videoWriter;
}

void RecordWorker::startRecord(std::string* filepath)
{
    //open the mp4 file and start recording
    m_videoWriter->open(*filepath, m_codec, m_fps, cv::Size(m_width,m_height), m_is_rgb);
    m_recording = true;
}

void RecordWorker::stopRecord()
{
    m_recording = false;
    m_videoWriter->release();
}

void RecordWorker::recordFrame(cv::Mat* frame)
{
    // TODO : Process record here instead of raspicam worker (use thread ? how?)
    if (m_recording && !m_pausing)
    {
        *m_videoWriter << *frame;
    }
}

void RecordWorker::pauseRecord()
{
    m_pausing = true;
}

void RecordWorker::resumeRecord()
{
    m_pausing = false;
}

void RecordWorker::setImageSize(const unsigned int width,const  unsigned int height)
{
    m_width = width;
    m_height = height;
}

void RecordWorker::setCameraFPS(const unsigned int fps)
{
    m_fps = fps;
}

