#include "cameraworker.h"
#include <QDebug>

CameraWorker::CameraWorker(QObject *parent) : QObject(parent)
{
    m_cameraCap = new VideoCapture();
    m_frame = new Mat();
    m_frame_processed = new Mat();
}

CameraWorker::~CameraWorker()
{
    if (m_cameraCap->isOpened()) m_cameraCap->release();
    delete m_cameraCap;
    delete m_frame;
    delete m_frame_processed;
}

void CameraWorker::setupCamera(const int device)
{
    releaseCamera();
    m_cameraCap->open(device);
    if(!m_cameraCap->isOpened())
    {
       qDebug() << "Error opening camera stream";
    }
}

void CameraWorker::setupCamera(const std::string filepath)
{
    releaseCamera();
    m_cameraCap->open(filepath);
    if(!m_cameraCap->isOpened())
    {
       qDebug() << "Error opening camera stream";
    }
}


void CameraWorker::signalProcessImage()
{
    (*m_cameraCap) >> *m_frame;
    if(m_frame->empty()) return;


    emit sendFrame(m_frame);

}

/**
 * @brief CameraWorker::releaseCamera
 * Release the VideoCapture Buffer if it is opened;
 */

void CameraWorker::releaseCamera()
{
    if (m_cameraCap->isOpened()) m_cameraCap->release();
}


/**
 * @brief CameraWorker::getImageFromPositionInSeconds.
 *  Get the frame at position_seconds seconds of the video File.
 *  VideoCapture has to be opened with a file and no cameradevice.
 * @param position_seconds
 */
void CameraWorker::getImageFromPositionInMillisec(const long position_milliseconds)
{

        double frnb = m_cameraCap->get(CAP_PROP_FRAME_COUNT) ;
        double fps = m_cameraCap->get(CAP_PROP_FPS);
        long frameNb = fps*position_milliseconds/1000;
        // if frame from position doesn't exist, stop the process
        if (frameNb > frnb || frameNb < 0) return;

        m_cameraCap->set(CAP_PROP_POS_FRAMES,frameNb);

       signalProcessImage();

}

/**
 * @brief CameraWorker::duration.
 *  return the duration time in milliseconds of the video file.
 *  VideoCapture has to be opened with a file and no cameradevice.
 *  Return -1 if VideoCapture has not been opened.
 */
long CameraWorker::duration()
{
    long duration = -1;
    double frnb = m_cameraCap->get(CAP_PROP_FRAME_COUNT) ;
    double fps = m_cameraCap->get(CAP_PROP_FPS);
    if (m_cameraCap->isOpened()) duration = (long) 1000*(frnb/fps);
    return duration;
}
