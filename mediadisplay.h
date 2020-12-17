#ifndef MEDIADISPLAY_H
#define MEDIADISPLAY_H

#include <QWidget>
#include "controllers/mediaservice.h"
#include "models/mediafile.h"
#include <vector>

namespace Ui {
class MediaDisplay;
}

class MediaDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit MediaDisplay(QWidget *parent = nullptr);
    void setMediaSource(QString vidPath);

    ~MediaDisplay();

signals:
    void backToMediaList();
    void playVideo(QString);

private slots:
    void on_backBtn_clicked();
    void on_previousMediaBtn_clicked();
    void on_nextMediaBtn_clicked();
    void on_playBtn_clicked();

private:
    Ui::MediaDisplay *ui;
    void setMedia(MediaFile *file);
    void setMediaByPos(unsigned int pos);
    unsigned int m_media_pos_in_list;
    bool m_is_first;
    bool m_is_last;
    MediaService * m_media_service;
};

#endif // MEDIADISPLAY_H
