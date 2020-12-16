#include "videothumbnailwidget.h"
#include "ui_videothumbnailwidget.h"
#include <QDebug>

VideoThumbnailWidget::VideoThumbnailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoThumbnailWidget),
    m_mediafile(MediaType::Image,"","",-1,NULL)
{
    ui->setupUi(this);
}

VideoThumbnailWidget::~VideoThumbnailWidget()
{

    delete ui;
}

void VideoThumbnailWidget::setMediaFile(const MediaFile &mediafile)
{
    m_mediafile = mediafile;
    updateUI();
}

void VideoThumbnailWidget::updateUI()
{
    if (m_mediafile.type() == MediaType::Video)
    {
        ui->vidLengthLabel->setText(Utils::formatMillisecondsToDateString(m_mediafile.duration()));
        ui->vidLengthLabel->show();
    } else
    {
        ui->vidLengthLabel->hide();
    }

    QFontMetrics metrics (ui->vidTitleLabel->font());
    ui->vidTitleLabel->setText(metrics.elidedText(QString::fromStdString(m_mediafile.filename()),Qt::ElideLeft,ui->vidTitleLabel->width()));

    if (m_mediafile.thumbnail() == NULL) return;

    QImage output((const unsigned char *) m_mediafile.thumbnail()->ptr(0), m_mediafile.thumbnail()->cols, m_mediafile.thumbnail()->rows, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(output);
    pixmap = pixmap.scaledToWidth(ui->vidTitleLabel->width()+ui->vidLengthLabel->width(),Qt::FastTransformation);

    ui->thumbnailWidget->setPixmap(pixmap);
    ui->thumbnailWidget->setAlignment(Qt::AlignHCenter);
}

MediaFile VideoThumbnailWidget::mediaFile() const
{
    return m_mediafile;
}
