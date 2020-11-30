#include "rekkoncamworker.h"
#include <QDebug>

RekkonCamWorker::RekkonCamWorker(QObject *parent) : QObject(parent),
    m_pause(false)
{
    m_frame = new Mat();
    m_frame_processed = new Mat();
    startVideoPreview();


}



RekkonCamWorker::~RekkonCamWorker()
{
    delete m_frame;
    delete m_frame_processed;
}

void RekkonCamWorker::startVideoPreview()
{
    m_cam_control.startVideoPreview();
}
void RekkonCamWorker::stopVideoPreview()
{
    m_cam_control.stopVideoPreview();
}


void RekkonCamWorker::signalProcessImage()
{

    if (!m_pause)
    {
        m_frame->create(m_cam_control.getVideoPreviewHeight(), m_cam_control.getVideoPreviewWidth(), CV_8UC3);
        m_cam_control.grab();
        m_cam_control.retrieve(m_frame->ptr<uchar> ( 0 ));
        if(m_frame->empty()) return;
        emit sendFrame(m_frame);
    }

}


/**
 * @brief RekkonCamWorker::pause
 * pause the image processing and
 */
void RekkonCamWorker::pause()
{
    m_pause = true;
}
/**
 * @brief RekkonCamWorker::resume
 * Resume the image processing.
 */
void RekkonCamWorker::resume()
{
    m_pause = false;
}



void RekkonCamWorker::setVideoPreviewSize(unsigned int width, unsigned int height)
{
    m_cam_control.setVideoPreviewResolution(width, height);
}

void RekkonCamWorker::setVideoRecordSize(unsigned int width, unsigned int height)
{
    m_cam_control.setVideoRecordResolution(width, height);
}

void RekkonCamWorker::setRotation(unsigned int rotation)
{
    m_cam_control.setRotation(rotation);
}
void RekkonCamWorker::setCameraFPS(unsigned int fps)
{
   m_cam_control.setFrameRate(fps);
}

void RekkonCamWorker::startVideoRecord(std::string* filepath)
{

    qDebug() << "video starting ";
    m_cam_control.startVideoRecord(*filepath);
    qDebug() << "video started ";
    /*if (m_cam_record.startRecord(*filepath) != 0 )
        emit errorRecord();*/
}

void RekkonCamWorker::stopVideoRecord()
{
    m_cam_control.stopVideoRecord();
}
