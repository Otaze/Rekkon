#include "raspicamworker.h"
#include <QDebug>

RaspiCamWorker::RaspiCamWorker(QObject *parent) : QObject(parent),
    m_pause(false)
{
    m_cameraCap = new raspicam::RaspiCam_Cv;
    m_frame = new Mat();
    m_frame_processed = new Mat();

    //m_cameraCap->setVideoStabilization(true);
}



RaspiCamWorker::~RaspiCamWorker()
{
    if (m_cameraCap->isOpened()) m_cameraCap->release();
    delete m_cameraCap;
    delete m_frame;
    delete m_frame_processed;
}

/**
 * @brief RaspiCamWorker::openCamera
 *  Open the Camera component and start the capture.
 *  If already opened, release the camera and open again.
 */
void RaspiCamWorker::openCamera()
{
    releaseCamera();
    m_cameraCap->open();
    if(!m_cameraCap->isOpened())
    {
       qDebug() << "Error opening camera stream";
    }
}



void RaspiCamWorker::signalProcessImage()
{
    if (!m_pause)
    {
        m_cameraCap->grab();
        m_cameraCap->retrieve(*m_frame);
        if(m_frame->empty()) return;

        emit sendFrame(m_frame);
    }

}

/**
 * @brief CameraWorker::releaseCamera
 * Release the VideoCapture Buffer if it is opened;
 */

void RaspiCamWorker::releaseCamera()
{
    if (m_cameraCap->isOpened()) m_cameraCap->release();
}

/**
 * @brief RaspiCamWorker::pause
 * pause the image processing and
 */
void RaspiCamWorker::pause()
{
    m_pause = true;
}
/**
 * @brief RaspiCamWorker::resume
 * Resume the image processing.
 */
void RaspiCamWorker::resume()
{
    m_pause = false;
}



void RaspiCamWorker::setImageSize(unsigned int width, unsigned int height)
{
    m_cameraCap->set( CAP_PROP_FRAME_WIDTH , width );
    m_cameraCap->set( CAP_PROP_FRAME_HEIGHT , height);
}

void RaspiCamWorker::setRotation(unsigned int rotation)
{
    m_cameraCap->setRotation(rotation);
}
void RaspiCamWorker::setCameraFPS(unsigned int fps)
{
    m_cameraCap->set( CAP_PROP_FPS, fps);
}

void RaspiCamWorker::startRecord(std::string* filepath)
{
    if (m_cameraCap->startRecord(*filepath) != 0 )
        emit errorRecord();
}

void RaspiCamWorker::stopRecord()
{
    m_cameraCap->stopRecord();
}
