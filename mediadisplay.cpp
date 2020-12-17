#include "mediadisplay.h"
#include "ui_mediadisplay.h"
#include <QDebug>

MediaDisplay::MediaDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaDisplay),
    m_media_pos_in_list(0),
    m_is_first(false),
    m_is_last(false)
{
    ui->setupUi(this);

    m_media_service = MediaService::instance();
}

MediaDisplay::~MediaDisplay()
{
    delete ui;
}

void MediaDisplay::setMediaSource(QString vidPath)
{
    unsigned int counter = 0;
    qDebug() << vidPath ;
    for (MediaFile* media : *(m_media_service->mediaFileList())) {
        if (media->filepath() == vidPath.toStdString())
        {
            setMediaByPos(counter);
        }
        counter++;
    }

}

void MediaDisplay::setMedia(MediaFile *file)
{
    if (m_media_pos_in_list == 0)
    {
        ui->previousMediaBtn->hide();
        m_is_first = true;
    } else
    {
        ui->previousMediaBtn->show();
        m_is_first = false;
    }

    if (m_media_pos_in_list == m_media_service->mediaFileList()->size() -1)
    {
        ui->nextMediaBtn->hide();
        m_is_last = true;
    } else
    {
        ui->nextMediaBtn->show();
        m_is_last = false;
    }
    if (file->type() == MediaType::Video)
        ui->playFrame->show();
    else ui->playFrame->hide();
    qDebug() << QString::fromStdString(file->filename());
    QImage output((const unsigned char *) file->thumbnail()->ptr(0), file->thumbnail()->cols, file->thumbnail()->rows, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(output);
    if ( (output.width() / (double) output.height())  <= (ui->mediaLabel->width() / (double) ui->mediaLabel->height() ))
        pixmap = pixmap.scaledToHeight(ui->mediaLabel->height(),Qt::FastTransformation);
    else pixmap = pixmap.scaledToWidth(ui->mediaLabel->width(),Qt::FastTransformation);
    ui->mediaLabel->setPixmap(pixmap);
    ui->mediaLabel->setAlignment(Qt::AlignCenter);

}

void MediaDisplay::setMediaByPos(unsigned int pos)
{
    if (pos < m_media_service->mediaFileList()->size())
    {
        m_media_pos_in_list = pos;
        setMedia(m_media_service->mediaFileList()->at(pos));
    }

}


void MediaDisplay::on_backBtn_clicked()
{
    emit backToMediaList();
}

void MediaDisplay::on_previousMediaBtn_clicked()
{
    if (m_media_pos_in_list > 0)
        setMediaByPos(m_media_pos_in_list - 1);
}

void MediaDisplay::on_nextMediaBtn_clicked()
{
    setMediaByPos(m_media_pos_in_list + 1);
}

void MediaDisplay::on_playBtn_clicked()
{
    emit playVideo(QString::fromStdString(m_media_service->mediaFileList()->at(m_media_pos_in_list)->filepath()));
}
