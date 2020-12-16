#ifndef VIDEOPREVIEWWIDGET_H
#define VIDEOPREVIEWWIDGET_H

#include <QWidget>
#include "utils/utils.h"
#include <QFontMetrics>
#include <string>
#include "models/mediafile.h"

namespace Ui {
class VideoThumbnailWidget;
}


class VideoThumbnailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoThumbnailWidget(QWidget *parent = nullptr);
    ~VideoThumbnailWidget();


    void setMediaFile(const MediaFile &mediafile);
    void updateUI();

    MediaFile mediaFile() const;

private:
    Ui::VideoThumbnailWidget *ui;
    MediaFile m_mediafile;
};

#endif // VIDEOPREVIEWWIDGET_H
