#ifndef MEDIALIST_H
#define MEDIALIST_H

#include <QWidget>
#include <vector>
#include "videothumbnailwidget.h"
#include <filesystem>
#include "controllers/mediaservice.h"

namespace Ui {
class MediaList;
}

class MediaList : public QWidget
{
    Q_OBJECT

public:
    explicit MediaList(QWidget *parent = nullptr);
    ~MediaList();
    void refreshList();
    bool isEmpty();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    VideoThumbnailWidget* addVideoThumbnail(MediaFile * mFile);

signals:
    void selectVideoPlay(QString);
    void selectMediaRecord();

private slots:
    void on_refreshBtn_clicked();

    void on_backBtn_clicked();

private:
    Ui::MediaList *ui;
    std::vector<VideoThumbnailWidget*>* m_thumbnailWidVector;
    MediaService* m_mediaService;
};

#endif // MEDIALIST_H
