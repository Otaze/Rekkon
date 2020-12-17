#include "medialist.h"
#include "ui_medialist.h"
#include <QDebug>
#include <QThread>

MediaList::MediaList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaList)
{
    ui->setupUi(this);
    m_thumbnailWidVector = new std::vector<VideoThumbnailWidget*>;
    m_mediaService = MediaService::instance();
}

MediaList::~MediaList()
{
    delete m_thumbnailWidVector;
    delete ui;
}

void MediaList::refreshList()
{

    m_mediaService->refreshList();
    updateUI();
}

void MediaList::clearThumbnails()
{
    for (VideoThumbnailWidget* vidThumbWid : *m_thumbnailWidVector)
    {
        ui->vidListGridLayout->removeWidget(vidThumbWid);
        //delete vidThumbWid;
    }
    m_thumbnailWidVector->clear();
}


void MediaList::updateUI()
{
    clearThumbnails();
    populateThumbnails();
}


void MediaList::populateThumbnails()
{
    for(MediaFile * file : *(m_mediaService->mediaFileList()))
    {
        addVideoThumbnail(file);
        QCoreApplication::processEvents();
    }
}


VideoThumbnailWidget* MediaList::addVideoThumbnail(MediaFile * mFile)
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

bool MediaList::isEmpty()
{
    return this->m_thumbnailWidVector->empty();
}

bool MediaList::eventFilter(QObject *obj, QEvent *event) {

    // normal event handling

    bool ret = QObject::eventFilter(obj, event);


    if (event->type() == QEvent::MouseButtonRelease) {
        emit selectMediaDisplay(QString::fromStdString(((VideoThumbnailWidget*)obj)->mediaFile().filepath()));
    }

    return ret;
}

void MediaList::on_refreshBtn_clicked()
{
    refreshList();
}

void MediaList::on_backBtn_clicked()
{
    emit selectMediaRecord();
}
