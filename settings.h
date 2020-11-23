#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <settingsstructure.h>
#include <QIntValidator>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

    struct VideoResolution
    {

       int16_t width;
       int16_t height;
       QString res_description;
    };

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

protected:
    VideoResolution getResolutionFromHeight(int16_t height);

signals:
     void selectVideoRecord();

private slots:

    void on_backBtn_clicked();

private:
    Ui::Settings *ui;
    SettingsStructure* m_settings;
    VideoResolution m_240p;
    VideoResolution m_360p;
    VideoResolution m_480p;
    VideoResolution m_720p;
    VideoResolution m_1080p;

};

#endif // SETTINGS_H
