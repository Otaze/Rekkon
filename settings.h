#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <models/settingsstructure.h>
#include <QIntValidator>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

    struct Resolution
    {

       int16_t width;
       int16_t height;
       QString res_description;
    };

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

protected:
    Resolution getResolutionFromHeight(int16_t height);

signals:
     void selectVideoRecord();

private slots:

    void on_backBtn_clicked();

private:
    Ui::Settings *ui;
    SettingsStructure* m_settings;
    Resolution m_240p;
    Resolution m_360p;
    Resolution m_480p;
    Resolution m_540p;
    Resolution m_720p;
    Resolution m_1080p;

    Resolution m_012MPx;
    Resolution m_024MPx;
    Resolution m_1MPx;
    Resolution m_6MPx;
    Resolution m_8MPx;
    Resolution m_12MPx;

};

#endif // SETTINGS_H
