#include "mediarecord.h"
#include "ui_mediarecord.h"

MediaRecord::MediaRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaRecord),
    m_screenshoting(false),
    m_isRecording(false),
    m_isPaused(false),
    m_isStillModeEnabled(false),
    m_preview_width(960),
    m_preview_height(540),
    m_record_width(1920),
    m_record_height(1080),
    m_fps(30),
    m_rotation(0)
{
     ui->setupUi(this);
     m_settings = SettingsStructure::instance();
     m_settings->load();


     setFixedSize(800,480);
     setWindowFlags(Qt::Window | Qt::FramelessWindowHint);


     m_ext_video = new string(".h264");
     m_ext_audio = new string(".aac");
     m_ext_still = new string(".jpg");

     ui->btnFrame->layout()->removeWidget(ui->stopBtn);
     ui->stopBtn->hide();
     ui->btnFrame->layout()->removeWidget(ui->captureStillBtn);
     ui->captureStillBtn->hide();
     ui->btnFrame->layout()->removeWidget(ui->pauseBtn);
     ui->pauseBtn->hide();


     setup();
}
MediaRecord::~MediaRecord()
{
   if (m_camThread->isRunning()) { m_camThread->quit(); m_camThread->wait(); }
   if (m_cam2Thread->isRunning()) { m_cam2Thread->quit(); m_cam2Thread->wait(); }
   if (m_recThread->isRunning()) { m_recThread->quit(); m_recThread->wait(); }

   delete m_camThread;
   delete m_cam2Thread;
   delete m_recThread;
   delete m_filepath_audio;
   delete m_filepath_video;
   delete m_ext_audio;
   delete m_ext_video;
   delete ui;
   removeMuxThread();
}


void MediaRecord::removeMuxThread()
{
    if (m_mux_clean_thread)
    {
        m_mux_clean_thread->join();
        delete m_mux_clean_thread;
    }
}

void MediaRecord::setup()
{
    m_audioWorker = new AudioWorker;
    m_rekkon_cam_worker = new RekkonCamWorker();
    setRekkonCamSizeSettings();
    reloadCamera();
    reloadCameraMode();
    m_rekkon_cam_worker->setRotation(m_rotation);
    m_rekkon_cam_worker->setCameraFPS(m_fps);

    m_camThread = new QThread();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(0);

    connect(workerTrigger, SIGNAL(timeout()), m_rekkon_cam_worker, SLOT(signalProcessImage()));
    connect(m_camThread, SIGNAL(finished()), m_rekkon_cam_worker, SLOT(deleteLater()));
    connect(m_camThread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));
    connect(m_rekkon_cam_worker, SIGNAL(sendFrame(cv::Mat*)), this, SLOT(receiveFrame(cv::Mat*)));
    connect(m_rekkon_cam_worker, SIGNAL(errorRecord()), this, SLOT(errorRecord()));

    workerTrigger->start();
    m_rekkon_cam_worker->moveToThread(m_camThread);
    workerTrigger->moveToThread(m_camThread);

    m_camThread->start();

}

void MediaRecord::restartMediaRecordProcessing()
{
    // reload cam settings
    setRekkonCamSizeSettings();
    startCameraProcessing();
}

void MediaRecord::stopMediaRecordProcessing()
{
    if (m_isRecording) stopRecordVideo();
    stopCameraProcessing();
}

void MediaRecord::setRekkonCamSizeSettings()
{
    m_rekkon_cam_worker->setVideoRecordSize(m_settings->m_recordVideoWidth, m_settings->m_recordVideoHeight);
    m_rekkon_cam_worker->setStillRecordSize(m_settings->m_recordStillWidth, m_settings->m_recordStillHeight);
    m_rekkon_cam_worker->setVideoPreviewSize(m_settings->m_previewVideoWidth, m_settings->m_previewVideoHeight);
    m_rekkon_cam_worker->setStillPreviewSize(m_settings->m_previewStillWidth, m_settings->m_previewStillHeight);
}

void MediaRecord::reloadCamera()
{
    m_rekkon_cam_worker->releaseCamera();
    m_rekkon_cam_worker->openCamera();
}


void MediaRecord::reloadCameraMode()
{
    // TO DO: change to right function when photo functions are ready in RekkonMMALCamera
    if (m_isStillModeEnabled)
    {
        cerr << " mode Still enabled" << endl;
        m_rekkon_cam_worker->stopVideoRecord();
        m_rekkon_cam_worker->stopVideoPreview();
        m_rekkon_cam_worker->startStillPreview();
    } else
    {

        cerr << " mode video enabled" << endl;
        m_rekkon_cam_worker->stopVideoRecord();
        m_rekkon_cam_worker->stopStillPreview();
        m_rekkon_cam_worker->startVideoPreview();
    }

}

void MediaRecord::startCameraProcessing()
{
    reloadCameraMode();
    m_rekkon_cam_worker->resume();
}

void MediaRecord::stopCameraProcessing()
{
    m_rekkon_cam_worker->pause();
    m_rekkon_cam_worker->stopVideoPreview();
    m_rekkon_cam_worker->stopVideoRecord();
    m_rekkon_cam_worker->stopStillPreview();

}


bool MediaRecord::isRecording() const
{
    return m_isRecording;
}

void MediaRecord::receiveFrame(cv::Mat* frame)
{
    QImage output((const unsigned char *)frame->data, frame->cols, frame->rows, QImage::Format_RGB888);
    //QImage output((const unsigned char *)frame->data, frame->cols, frame->rows, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(output);
    //pixmap = pixmap.scaledToWidth(ui->vidWidCam1Label->width(),Qt::FastTransformation);
    pixmap = pixmap.scaledToHeight(ui->vidWidCam1Label->height(),Qt::FastTransformation);
    ui->vidWidCam1Label->setPixmap(pixmap);
    ui->vidWidCam1Label->setAlignment(Qt::AlignHCenter);
}


void MediaRecord::startRecordVideo()
{
    if (m_isPaused)
    {
        m_isPaused = false;

        ui->btnFrame->layout()->addWidget(ui->pauseBtn);
        ui->pauseBtn->show();
        //emit resumeRecord();
        if (m_audioWorker->isInitialized())
            m_audioWorker->resumeRecord();
    } else {
        // Enable record mode
        // hide all useless buttons in record mode
        ui->mediaBtn->hide();
        ui->settingsBtn->hide();
        ui->closeAppBtn->hide();
        ui->stillVideoModeBtn->hide();
        ui->btnFrame->layout()->addWidget(ui->pauseBtn);
        ui->pauseBtn->show();
        ui->btnFrame->layout()->addWidget(ui->stopBtn);
        ui->stopBtn->show();


        // create filename and path
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
        std::string date(ss.str());
        m_filepath_video = new std::string(m_settings->m_videoFolder +"/"+date + *m_ext_video);
        m_filepath_audio = new std::string(m_settings->m_videoFolder +"/"+date + *m_ext_audio);

        m_rekkon_cam_worker->startVideoRecord(m_filepath_video);
        if (m_audioWorker->isInitialized())
            m_audioWorker->startRecord(*m_filepath_audio);
    }

    ui->recordBtn->hide();
}

void MediaRecord::pauseRecordVideo()
{
    m_isPaused = true;

    ui->btnFrame->layout()->removeWidget(ui->pauseBtn);
    ui->pauseBtn->hide();
    ui->btnFrame->layout()->removeWidget(ui->stopBtn);
    ui->stopBtn->hide();

    ui->btnFrame->layout()->addWidget(ui->recordBtn);
    ui->recordBtn->show();
    ui->btnFrame->layout()->addWidget(ui->stopBtn);
    ui->stopBtn->show();

    //emit pauseRecord();
    if (m_audioWorker->isInitialized())
        m_audioWorker->pauseRecord();
}

void MediaRecord::stopRecordVideo()
{
    // Disable record mode
    // show hidden buttons by record mode
    ui->btnFrame->layout()->removeWidget(ui->stopBtn);
    ui->stopBtn->hide();
    ui->btnFrame->layout()->removeWidget(ui->pauseBtn);
    ui->pauseBtn->hide();

    ui->btnFrame->layout()->addWidget(ui->recordBtn);
    ui->recordBtn->show();

    ui->settingsBtn->show();
    ui->closeAppBtn->show();
    ui->stillVideoModeBtn->show();
    ui->mediaBtn->show();

    m_rekkon_cam_worker->stopVideoRecord();
    if (m_audioWorker->isInitialized())
        m_audioWorker->stopRecord();

    m_mux_clean_thread = new std::thread(muxVideoAudioAndClean,m_filepath_video, m_filepath_audio, m_ext_video,m_fps, m_audioWorker->isInitialized());

}

void MediaRecord::muxVideoAudioAndClean( string *filepath_video, string *filepath_audio, string *ext_video,unsigned int fps,bool audio_ready)
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


void MediaRecord::errorRecord()
{
    stopRecordVideo();
}


void MediaRecord::captureStill()
{
    // create filename and path
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    std::string date(ss.str());
    m_filepath_still = new std::string(m_settings->m_videoFolder +"/"+date + *m_ext_still);
    m_rekkon_cam_worker->startStillRecord(m_filepath_still);

}


void MediaRecord::on_recordBtn_clicked()
{
    startRecordVideo();
}

void MediaRecord::on_pauseBtn_clicked()
{
    pauseRecordVideo();
}

void MediaRecord::on_stopBtn_clicked()
{
    stopRecordVideo();
}


void MediaRecord::on_mediaBtn_clicked()
{
    stopCameraProcessing();
    emit showMediaList();
}

void MediaRecord::on_closeAppBtn_clicked()
{

    m_rekkon_cam_worker->pause();
    /*if (isRecording()){
        stopRecordVideo();
        m_mux_clean_thread->join();
    }*/
    stopMediaRecordProcessing();
    m_camThread->quit();
    QApplication::quit();
}

void MediaRecord::on_settingsBtn_clicked()
{
    stopMediaRecordProcessing();
    emit showSettings();
}

void MediaRecord::on_stillVideoModeBtn_clicked()
{
    stopMediaRecordProcessing();
    m_isStillModeEnabled = !m_isStillModeEnabled;
    if (m_isStillModeEnabled)
    {
        ui->stillVideoModeBtn->setText("Video");
        ui->btnFrame->layout()->removeWidget(ui->recordBtn);
        ui->recordBtn->hide();
        ui->btnFrame->layout()->addWidget(ui->captureStillBtn);
        ui->captureStillBtn->show();
    }
    else
    {
        ui->btnFrame->layout()->addWidget(ui->recordBtn);
        ui->recordBtn->show();
        ui->btnFrame->layout()->removeWidget(ui->captureStillBtn);
        ui->captureStillBtn->hide();
        ui->stillVideoModeBtn->setText("Photo");
    }


    startCameraProcessing();
}

void MediaRecord::on_captureStillBtn_clicked()
{
    captureStill();
}
