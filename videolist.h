#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QWidget>
#include <vector>
#include "videothumbnailwidget.h"
#include <filesystem>
#include "controllers/mediaservice.h"

namespace Ui {
class VideoList;
}

class VideoList : public QWidget
{
    Q_OBJECT

public:
    explicit VideoList(QWidget *parent = nullptr);
    ~VideoList();
    void refreshList();
    bool isEmpty();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    VideoThumbnailWidget* addVideoThumbnail(MediaFile * mFile);

signals:
    void selectVideoPlay(QString);
    void selectVideoRecord();

private slots:
    void on_refreshBtn_clicked();

    void on_backBtn_clicked();

private:
    Ui::VideoList *ui;
    std::vector<VideoThumbnailWidget*>* m_thumbnailWidVector;
    MediaService* m_mediaService;
};

#endif // VIDEOLIST_H
