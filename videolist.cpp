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
    m_settings = SettingsStructure::instance();
}

VideoList::~VideoList()
{
    delete m_thumbnailWidVector;
    delete ui;
}

void VideoList::refreshList()
{
    m_settings->load();
    std::string path = m_settings->m_videoFolder;
    for (VideoThumbnailWidget* vidThumbWid : *m_thumbnailWidVector)
    {
        ui->vidListGridLayout->removeWidget(vidThumbWid);
        delete vidThumbWid;
    }
    m_thumbnailWidVector->clear();
    for (const auto & file : std::filesystem::recursive_directory_iterator(path))
    {
        if (file.path().extension() == ".mkv"
                || file.path().extension() == ".mp4"
                || file.path().extension() == ".avi"
                || file.path().extension() == ".webm" )
        {
            //QThread::msleep(500);
            VideoThumbnailWidget* thumbnail = addVideoThumbnail( QString::fromStdString(file.path().string()));
            //QCoreApplication::processEvents();

            thumbnail->getImageFromPositionInMillisec(1000);
        }

    }

}


VideoThumbnailWidget* VideoList::addVideoThumbnail(const QString filename)
{
    int16_t file_counter = ui->vidListGridLayout->count();
    int16_t columns_count = 3;
    int16_t file_column_counter = file_counter % columns_count;

    VideoThumbnailWidget* vidThumbWid = new VideoThumbnailWidget(this);
    vidThumbWid->installEventFilter(this);
    m_thumbnailWidVector->push_back(vidThumbWid);
    ui->vidListGridLayout->setColumnStretch(file_column_counter, 1);
    ui->vidListGridLayout->addWidget(vidThumbWid, file_counter/ columns_count ,file_column_counter,1,1);

    vidThumbWid->setMediaSource(filename);
    for (int i =0 ;i <= file_counter/ columns_count;i++) {
        ui->vidListGridLayout->setRowStretch(i, 1);
    }
    for (int i =0 ;i < columns_count;i++) {
        ui->vidListGridLayout->setColumnStretch(i, 1);
    }

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

        emit selectVideoPlay(*(((VideoThumbnailWidget*)obj)->getMediaSource()));

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
