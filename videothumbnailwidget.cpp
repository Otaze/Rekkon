#include "videothumbnailwidget.h"
#include "ui_videothumbnailwidget.h"
#include <QDebug>

VideoThumbnailWidget::VideoThumbnailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoThumbnailWidget)
{
    ui->setupUi(this);

    m_vidWorker = new CameraWorker();
    connect(m_vidWorker, SIGNAL(sendFrame(cv::Mat*)), this, SLOT(processImage(cv::Mat*)));


    m_mediaSrcPath = new QString;
}

void VideoThumbnailWidget::setMediaSource(const QString mediaPath)
{
    delete m_mediaSrcPath;
    m_mediaSrcPath = new QString(mediaPath);
    emit sendSetupCam(m_mediaSrcPath->toStdString());

}

void VideoThumbnailWidget::updateTitle()
{
    QString title = QString::fromStdString(std::filesystem::path(m_mediaSrcPath->toUtf8().constData()).filename().string());
    QFontMetrics metrics (ui->vidTitleLabel->font());


    ui->vidTitleLabel->setText(metrics.elidedText(title,Qt::ElideRight,ui->vidTitleLabel->width()));
}

void VideoThumbnailWidget::updateDuration()
{
    qDebug() << m_vidWorker->duration();
    ui->vidLengthLabel->setText(Utils::formatMillisecondsToDateString(m_vidWorker->duration()));
}

VideoThumbnailWidget::~VideoThumbnailWidget()
{
    emit releaseCamera();
    delete m_vidWorker;
    delete m_mediaSrcPath;
    delete ui;
}

const QString* VideoThumbnailWidget::getMediaSource()
{
    return this->m_mediaSrcPath;
}

void VideoThumbnailWidget::processImage(cv::Mat* image)
{
    updateDuration();
    updateTitle();
    qDebug() << "processed Image thumbnail";
    QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*) image->data, image->cols, image->rows, QImage::Format_BGR888));
    pixmap = pixmap.scaledToWidth(ui->thumbnailWidget->width(),Qt::FastTransformation);
    ui->thumbnailWidget->setPixmap(pixmap);
    emit releaseCamera();
}
