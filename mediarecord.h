#ifndef MEDIARECORD_H
#define MEDIARECORD_H

#include <QWidget>
#include <QDateTime>
#include <QTimeZone>
#include "cameraworker.h"
#include "controllers/audioworker.h"
#include "controllers/rekkoncamworker.h"
#include "models/settingsstructure.h"
#include <QThread>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <thread>

namespace Ui {
class MediaRecord;
}
class MediaRecord : public QWidget
{
    Q_OBJECT

public:
    explicit MediaRecord(QWidget *parent = nullptr);
    ~MediaRecord();
    void startRecordVideo();
    void pauseRecordVideo();
    void stopRecordVideo();
    void screenshot();
    void restartMediaRecordProcessing();
    void stopMediaRecordProcessing();
    void captureStill();

    bool isRecording() const;

private slots:
    void setRekkonCamSizeSettings();

    void on_pauseBtn_clicked();
    void on_stopBtn_clicked();
    void on_recordBtn_clicked();
    void on_mediaBtn_clicked();
    void on_closeAppBtn_clicked();
    void on_settingsBtn_clicked();
    void receiveFrame(cv::Mat* frame);
    void errorRecord();

    void on_stillVideoModeBtn_clicked();

    void on_captureStillBtn_clicked();

signals:
    void showMediaList();
    void showSettings();

private:
    Ui::MediaRecord *ui;
    QThread *m_camThread;
    QThread *m_cam2Thread;
    QThread *m_recThread;
    RekkonCamWorker *m_rekkon_cam_worker;
    AudioWorker * m_audioWorker;
    bool m_screenshoting;
    SettingsStructure* m_settings;
    bool m_isRecording;
    bool m_isPaused;
    bool m_isStillModeEnabled;
    unsigned int m_preview_width;
    unsigned int m_preview_height;
    unsigned int m_record_width;
    unsigned int m_record_height;
    unsigned int m_fps;
    unsigned int m_rotation;
    std::string* m_filepath_video;
    std::string* m_filepath_audio;
    std::string* m_filepath_still;
    std::string* m_ext_video;
    std::string* m_ext_audio;
    std::string* m_ext_still;

    std::thread *m_mux_clean_thread;

    void setup();

    void startCameraProcessing();
    void stopCameraProcessing();

    void reloadCamera();
    void reloadCameraMode();

    static void muxVideoAudioAndClean(string *filepath_video, string *filepath_audio, string *ext_video,unsigned int fps, bool audio_ready);

    void removeMuxThread();
};

#endif // MEDIARECORD_H
