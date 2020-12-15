#include "rekkoncamworker.h"
#include <QDebug>

RekkonCamWorker::RekkonCamWorker(QObject *parent) : QObject(parent),
    m_pause(false)
{
    m_frame = new Mat();
    m_frame_processed = new Mat();
}

RekkonCamWorker::~RekkonCamWorker()
{
    delete m_frame;
    delete m_frame_processed;
}

void RekkonCamWorker::releaseCamera()
{
    m_cam_control.stopVideoRecord();
    m_cam_control.stopVideoPreview();
    m_cam_control.release();
}

void RekkonCamWorker::openCamera()
{
    m_cam_control.open();
}

void RekkonCamWorker::startVideoPreview()
{
    m_cam_control.startVideoPreview();
}
void RekkonCamWorker::stopVideoPreview()
{
    m_cam_control.stopVideoPreview();
}

void RekkonCamWorker::startStillPreview()
{
    m_cam_control.startStillPreview();
}
void RekkonCamWorker::stopStillPreview()
{
    m_cam_control.stopStillPreview();
}


void RekkonCamWorker::signalProcessImage()
{

    if (!m_pause)
    {
            if (m_cam_control.grab())
            {
                if (m_cam_control.isVideoPreviewOpened())
                    m_frame->create(m_cam_control.getVideoPreviewHeight(), m_cam_control.getVideoPreviewWidth(), CV_8UC3);
                else m_frame->create(m_cam_control.getStillPreviewHeight(), m_cam_control.getStillPreviewWidth(), CV_8UC3);

                m_cam_control.retrieve(m_frame->ptr<uchar> ( 0 ));
                if(m_frame->empty()) return;
                emit sendFrame(m_frame);
            }
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
    m_cam_control.setVideoPreviewSize(width, height);
}

void RekkonCamWorker::setStillPreviewSize(unsigned int width, unsigned int height)
{
    m_cam_control.setStillPreviewSize(width, height);
}

void RekkonCamWorker::setVideoRecordSize(unsigned int width, unsigned int height)
{
    m_cam_control.setVideoRecordSize(width, height);
}

void RekkonCamWorker::setStillRecordSize(unsigned int width, unsigned int height)
{
    m_cam_control.setStillRecordSize(width, height);
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
    m_cam_control.startVideoRecord(*filepath);

    /*if (m_cam_record.startRecord(*filepath) != 0 )
        emit errorRecord();*/
}

void RekkonCamWorker::stopVideoRecord()
{
    m_cam_control.stopVideoRecord();
}

void RekkonCamWorker::startStillRecord(std::string* filepath)
{
    m_cam_control.startStillRecord(*filepath);
}

