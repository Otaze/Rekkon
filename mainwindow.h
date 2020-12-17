#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplay.h"
#include "mediarecord.h"
#include "medialist.h"
#include "mediadisplay.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class MainWindowClass;

public:
    ~MainWindow();
    MainWindow(QWidget *parent = nullptr);

    static const uint16_t VIDEOPLAY_WIDGET = 2;
    static const uint16_t MEDIARECORD_WIDGET = 0;
    static const uint16_t MEDIALIST_WIDGET = 1;
    static const uint16_t MEDIADISPLAY_WIDGET = 3;
    static const uint16_t SETTINGS_WIDGET = 4;

protected:
    void changeVideoView(uint16_t view);

private slots:
    void setToVideoPlayView(QString vidPath);
    void setToMediaDisplayView(QString vidPath);
    void setToMediaListView();
    void setToMediaRecordView();
    void setToSettingsView();

signals:
    void changeView(int index);


private:
    Ui::MainWindow *ui;
    VideoPlay *videoPlayWidget;
    MediaRecord *mediaRecordWidget;
    MediaList * mediaListWidget;
    MediaDisplay *mediaDisplayWidget;
    Settings *settingsWidget;

};

#endif // MAINWINDOW_H

