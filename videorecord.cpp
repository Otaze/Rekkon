#include "videorecord.h"
#include "ui_videorecord.h"

VideoRecord::VideoRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoRecord),
    m_screenshoting(false),
    m_isRecording(false),
    m_isPaused(false),
    m_fps(30),
    m_width(1920),
    m_height(1080),
    m_rotation(0)
{
     ui->setupUi(this);
     m_settings = new SettingsStructure;
     m_settings->load();


     setFixedSize(800,480);
     setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    /* QGraphicsDropShadowEffect * secondVidDropShadow = new QGraphicsDropShadowEffect;
     secondVidDropShadow->setOffset(5);
     secondVidDropShadow->setBlurRadius(15);
     ui->vidWidCam2Label->setGraphicsEffect(secondVidDropShadow);*/
     ui->vidWidCam2Label->hide();

     m_ext_video = new string(".h264");
     m_ext_audio = new string(".aac");

     setup();

     emit setImageSize(m_width,m_height);
     emit setRotation(m_rotation);
     emit setCameraFPS(m_fps);
     emit openRaspiCamera();

}
VideoRecord::~VideoRecord()
{
   if (m_camThread->isRunning()) { m_camThread->quit(); m_camThread->wait(); }
   if (m_cam2Thread->isRunning()) { m_cam2Thread->quit(); m_cam2Thread->wait(); }
   if (m_recThread->isRunning()) { m_recThread->quit(); m_recThread->wait(); }

   delete m_camThread;
   delete m_cam2Thread;
   delete m_recThread;
   delete m_settings;
   delete m_filepath_audio;
   delete m_filepath_video;
   delete m_ext_audio;
   delete m_ext_video;
   delete ui;
   removeMuxThread();
}


void VideoRecord::removeMuxThread()
{
    if (m_mux_clean_thread != nullptr)
    {
        m_mux_clean_thread->join();
        delete m_mux_clean_thread;
    }
}

void VideoRecord::setup()
{
    m_audioWorker = new AudioWorker;

    m_camThread = new QThread();
    RaspiCamWorker *worker = new RaspiCamWorker();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(0);

    connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(signalProcessImage()));
    connect(m_camThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(m_camThread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));
    connect(this, SIGNAL(openRaspiCamera()), worker, SLOT(openCamera()));
    connect(this, SIGNAL(releaseCamera()), worker, SLOT(releaseCamera()));
    connect(this, SIGNAL(resumeCamera()), worker, SLOT(resume()));
    connect(this, SIGNAL(pauseCamera()), worker, SLOT(pause()));
    connect(this, SIGNAL(setCameraFPS(const unsigned int)), worker, SLOT(setCameraFPS(const unsigned int)));
    connect(this, SIGNAL(setRotation(const unsigned int)), worker, SLOT(setRotation(const unsigned int)));
    connect(this, SIGNAL(setImageSize(const unsigned int,const unsigned int)), worker, SLOT(setImageSize(const unsigned int,const unsigned int)));
    connect(worker, SIGNAL(sendFrame(cv::Mat*)), this, SLOT(receiveFrame(cv::Mat*)));
    connect(this, SIGNAL(startRecord(std::string*)), worker, SLOT(startRecord(std::string*)));
    connect(this, SIGNAL(stopRecord()), worker, SLOT(stopRecord()));
    connect(worker, SIGNAL(errorRecord()), this, SLOT(errorRecord()));

    workerTrigger->start();
    worker->moveToThread(m_camThread);
    workerTrigger->moveToThread(m_camThread);

    m_camThread->start();



/*
    m_cam2Thread = new QThread();

    CameraWorker *workerCamera2 = new CameraWorker();
    QTimer *workerTriggerCamera2 = new QTimer();
    workerTriggerCamera2->setInterval(30);

    connect(workerTriggerCamera2, SIGNAL(timeout()), workerCamera2, SLOT(signalProcessImage()));
    connect(m_cam2Thread, SIGNAL(finished()), workerCamera2, SLOT(deleteLater()));
    connect(m_cam2Thread, SIGNAL(finished()), workerTriggerCamera2, SLOT(deleteLater()));
    connect(this, SIGNAL(sendSetupCam2(int)), workerCamera2, SLOT(setupCamera(int)));
    connect(workerCamera2, SIGNAL(sendFrame(cv::Mat*)), this, SLOT(receiveFrameCamera2(cv::Mat*)));
    emit sendSetupCam2(0
                       );
    workerTriggerCamera2->start();
    workerCamera2->moveToThread(m_cam2Thread);
    workerTriggerCamera2->moveToThread(m_cam2Thread);

    m_cam2Thread->start();*/



  /*  m_recThread = new QThread();
    RecordWorker *recordWorker = new RecordWorker();

    connect(worker, SIGNAL(sendFrame(cv::Mat*)), recordWorker, SLOT(recordFrame(cv::Mat*)));
   // connect(workerCamera2, SIGNAL(sendFrame(cv::Mat*)), recordWorker, SLOT(recordFrame(cv::Mat*)));
    connect(m_recThread, SIGNAL(finished()), recordWorker, SLOT(deleteLater()));
    connect(this, SIGNAL(startRecord(std::string)), recordWorker, SLOT(startRecord(std::string)));
    connect(this, SIGNAL(stopRecord()), recordWorker, SLOT(stopRecord()));
    connect(this, SIGNAL(pauseRecord()), recordWorker, SLOT(pauseRecord()));
    connect(this, SIGNAL(resumeRecord()), recordWorker, SLOT(resumeRecord()));
    connect(this, SIGNAL(setCameraFPS(const unsigned int)), recordWorker, SLOT(setCameraFPS(const unsigned int)));
    connect(this, SIGNAL(setImageSize(const unsigned int,const unsigned int)), recordWorker, SLOT(setImageSize(const unsigned int,const unsigned int)));

    recordWorker->moveToThread(m_recThread);
    m_recThread->start();*/
}

void VideoRecord::startCameraProcessing()
{
    emit resumeCamera();
}

void VideoRecord::stopCameraProcessing()
{
    if (m_isRecording) stopRecordVideo();
    emit pauseCamera();
}


bool VideoRecord::isRecording() const
{
    return m_isRecording;
}

void VideoRecord::receiveFrame(cv::Mat* frame)
{

    QImage output((const unsigned char *)frame->data, frame->cols, frame->rows, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(output);
    pixmap = pixmap.scaledToWidth(ui->vidWidCam1Label->width(),Qt::FastTransformation);
    ui->vidWidCam1Label->setPixmap(pixmap);

}

void VideoRecord::receiveFrameCamera2(cv::Mat* frame)
{
    QImage output((const unsigned char *)frame->data, frame->cols, frame ->rows, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(output);
    pixmap = pixmap.scaledToWidth(ui->vidWidCam2Label->width(),Qt::FastTransformation);
    ui->vidWidCam2Label->setPixmap(pixmap);
}

void VideoRecord::startRecordVideo()
{
    if (m_isPaused)
    {
        m_isPaused = false;
        ui->pauseBtn->show();
        emit resumeRecord();
        if (m_audioWorker->isInitialized())
            m_audioWorker->resumeRecord();
    } else {
        // Enable record mode
        // hide all useless buttons in record mode
        ui->mediaBtn->hide();
        ui->settingsBtn->hide();
        ui->closeAppBtn->hide();

        // create filename and path
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
        std::string date(ss.str());
        m_filepath_video = new std::string(m_settings->m_videoFolder +"/"+date + *m_ext_video);
        m_filepath_audio = new std::string(m_settings->m_videoFolder +"/"+date + *m_ext_audio);

        emit startRecord(m_filepath_video);
        if (m_audioWorker->isInitialized())
            m_audioWorker->startRecord(*m_filepath_audio);
    }

    ui->recordBtn->hide();
}

void VideoRecord::pauseRecordVideo()
{
    m_isPaused = true;
    ui->pauseBtn->hide();
    ui->recordBtn->show();
    emit pauseRecord();
    if (m_audioWorker->isInitialized())
        m_audioWorker->pauseRecord();
}

void VideoRecord::stopRecordVideo()
{
    // Disable record mode
    // show hidden buttons by record mode
    ui->mediaBtn->show();
    ui->recordBtn->show();
    ui->settingsBtn->show();
    ui->closeAppBtn->show();

    emit stopRecord();
    if (m_audioWorker->isInitialized())
        m_audioWorker->stopRecord();

    removeMuxThread();
    m_mux_clean_thread = new std::thread(muxVideoAudioAndClean,m_filepath_video, m_filepath_audio, m_ext_video,m_fps, m_audioWorker->isInitialized());

}

void VideoRecord::muxVideoAudioAndClean( string *filepath_video, string *filepath_audio, string *ext_video,unsigned int fps,bool audio_ready)
{
    string ffmpegCmd = "ffmpeg ";
    ffmpegCmd.append("-r ");
    ffmpegCmd.append(to_string(fps));
    ffmpegCmd.append(" -hide_banner -loglevel panic -i ");
    ffmpegCmd.append(filepath_video->c_str());
    if (audio_ready)
    {
    ffmpegCmd.append(" -i ");
    ffmpegCmd.append(filepath_audio->c_str());
    }
    ffmpegCmd.append(" -c copy ");
    ffmpegCmd.append(filepath_video->c_str(),filepath_video->size() - ext_video->size());
    ffmpegCmd.append(".mp4");

    system(ffmpegCmd.c_str());

    remove(filepath_audio->c_str());
    remove(filepath_video->c_str());

}


void VideoRecord::errorRecord()
{
    stopRecordVideo();
}

void VideoRecord::screenshot()
{
    QDateTime currentDate = QDateTime::currentDateTime();
    currentDate.setTimeZone(QTimeZone::systemTimeZone());
    m_screenshoting = false;
}


void VideoRecord::on_recordBtn_clicked()
{
    startRecordVideo();
}

void VideoRecord::on_pauseBtn_clicked()
{
    pauseRecordVideo();
}

void VideoRecord::on_stopBtn_clicked()
{
    stopRecordVideo();
}


void VideoRecord::on_screenshotBtn_clicked()
{
    m_screenshoting = true;
}

void VideoRecord::on_mediaBtn_clicked()
{
    stopCameraProcessing();
    emit showVideoList();
}

void VideoRecord::on_closeAppBtn_clicked()
{
    emit releaseCamera();
    sleep(2);
    QApplication::quit();
}

void VideoRecord::on_settingsBtn_clicked()
{
    stopCameraProcessing();
    emit showSettings();
}
