#ifndef VIDEORECORD_H
#define VIDEORECORD_H

#include <QWidget>
#include <QDateTime>
#include <QTimeZone>
#include <cameraworker.h>
#include <audioworker.h>
#include <rekkoncamworker.h>
#include <recordworker.h>
#include <QThread>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <opencv2/imgproc.hpp>
#include "settingsstructure.h"
#include <iomanip>
#include <ctime>
#include <sstream>
#include <thread>

namespace Ui {
class VideoRecord;
}
class VideoRecord : public QWidget
{
    Q_OBJECT

public:
    explicit VideoRecord(QWidget *parent = nullptr);
    ~VideoRecord();
    void startRecordVideo();
    void pauseRecordVideo();
    void stopRecordVideo();
    void screenshot();
    void startCameraProcessing();
    void stopCameraProcessing();
    bool isRecording() const;

private slots:
    void on_pauseBtn_clicked();
    void on_stopBtn_clicked();
    void on_recordBtn_clicked();
    void on_screenshotBtn_clicked();
    void on_mediaBtn_clicked();
    void on_closeAppBtn_clicked();
    void on_settingsBtn_clicked();
    void receiveFrame(cv::Mat* frame);
    void receiveFrameCamera2(cv::Mat* frame);
    void errorRecord();

signals:
    void showVideoList();
    void showSettings();
    //void openRaspiCamera();
    //void releaseCamera();
    void sendSetupCam2(int);
    void pauseCamera();
    void resumeCamera();
    void setVideoPreviewSize(const unsigned int width,const  unsigned int height);
    void setVideoRecordSize(const unsigned int width,const  unsigned int height);
    void setRotation(const unsigned int rotation);
    void setCameraFPS(const unsigned int fps);
    void startVideoRecord(std::string* filepath);
    void stopVideoRecord();
    void pauseRecord();
    void resumeRecord();

private:
    Ui::VideoRecord *ui;
    QThread *m_camThread;
    QThread *m_cam2Thread;
    QThread *m_recThread;
    AudioWorker * m_audioWorker;
    bool m_screenshoting;
    SettingsStructure* m_settings;
    bool m_isRecording;
    bool m_isPaused;
    unsigned int m_preview_width;
    unsigned int m_preview_height;
    unsigned int m_record_width;
    unsigned int m_record_height;
    unsigned int m_fps;
    unsigned int m_rotation;
    std::string* m_filepath_video;
    std::string* m_filepath_audio;
    std::string* m_ext_video;
    std::string* m_ext_audio;

    std::thread *m_mux_clean_thread;

    void setup();

    static void muxVideoAudioAndClean(string *filepath_video, string *filepath_audio, string *ext_video,unsigned int fps, bool audio_ready);

    void removeMuxThread();
};

#endif // VIDEORECORD_H
