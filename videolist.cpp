#include "videolist.h"
#include "ui_videolist.h"
#include <QDebug>
#include <QThread>

VideoList::VideoList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoList)
{
    ui->setupUi(this);
    m_thumbnailWidVector = new std::vector<VideoThumbnailWidget*>;
    m_mediaService = MediaService::instance();
}

VideoList::~VideoList()
{
    delete m_thumbnailWidVector;
    delete ui;
}

void VideoList::refreshList()
{

    for (VideoThumbnailWidget* vidThumbWid : *m_thumbnailWidVector)
    {
        ui->vidListGridLayout->removeWidget(vidThumbWid);
        //delete vidThumbWid;
    }
    m_thumbnailWidVector->clear();

    m_mediaService->refreshList();
    for(MediaFile * file : m_mediaService->mediaFileList())
    {
        addVideoThumbnail(file);
    }
    for (VideoThumbnailWidget* vidThumbWid : *m_thumbnailWidVector)
    {
        vidThumbWid->updateUI();
    }
}


VideoThumbnailWidget* VideoList::addVideoThumbnail(MediaFile * mFile)
{
    int16_t file_counter = ui->vidListGridLayout->count();
    int16_t columns_count = 3;
    int16_t file_column_counter = file_counter % columns_count;

    VideoThumbnailWidget* vidThumbWid = new VideoThumbnailWidget(this);
    vidThumbWid->installEventFilter(this);
    m_thumbnailWidVector->push_back(vidThumbWid);
    ui->vidListGridLayout->setColumnStretch(file_column_counter, 1);
    ui->vidListGridLayout->addWidget(vidThumbWid, file_counter/ columns_count ,file_column_counter,1,1);

    for (int i =0 ;i <= file_counter/ columns_count;i++) {
        ui->vidListGridLayout->setRowStretch(i, 1);
    }
    for (int i =0 ;i < columns_count;i++) {
        ui->vidListGridLayout->setColumnStretch(i, 1);
    }
    vidThumbWid->setMediaFile(*mFile);

    return vidThumbWid;

}

bool VideoList::isEmpty()
{
    return this->m_thumbnailWidVector->empty();
}

bool VideoList::eventFilter(QObject *obj, QEvent *event) {

    // normal event handling

    bool ret = QObject::eventFilter(obj, event);



    if (event->type() == QEvent::MouseButtonRelease) {
        emit selectVideoPlay(QString::fromStdString(((VideoThumbnailWidget*)obj)->mediaFile().filepath()));
    }

    return ret;

}

void VideoList::on_refreshBtn_clicked()
{
    refreshList();
}

void VideoList::on_backBtn_clicked()
{
    emit selectVideoRecord();
}
