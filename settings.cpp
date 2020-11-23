#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    m_settings = new SettingsStructure;
    m_settings->load("/opt/Rekkon/config.xml");
    ui->mediaFolderPathLineEdit->setText(QString::fromStdString(m_settings->m_videoFolder));

    m_240p.width=426;
    m_240p.height=240;
    m_240p.res_description = QString("240p");

    m_360p.width=640;
    m_360p.height=360;
    m_360p.res_description = QString("360p");

    m_480p.width=853;
    m_480p.height=480;
    m_480p.res_description = QString("480p");


    m_720p.width=1280;
    m_720p.height=720;
    m_720p.res_description = QString("720p");


    m_1080p.width=1920;
    m_1080p.height=1080;
    m_1080p.res_description = QString("1080p");

    ui->preview1ResolutionComboBox->addItem(m_240p.res_description,m_240p.height);
    ui->preview1ResolutionComboBox->addItem(m_360p.res_description,m_360p.height);
    ui->preview1ResolutionComboBox->addItem(m_480p.res_description,m_480p.height);

    ui->record1ResolutionComboBox->addItem(m_240p.res_description,m_240p.height);
    ui->record1ResolutionComboBox->addItem(m_360p.res_description,m_360p.height);
    ui->record1ResolutionComboBox->addItem(m_480p.res_description,m_480p.height);
    ui->record1ResolutionComboBox->addItem(m_720p.res_description,m_720p.height);
    ui->record1ResolutionComboBox->addItem(m_1080p.res_description,m_1080p.height);

    ui->record1ResolutionComboBox->setCurrentIndex(ui->record1ResolutionComboBox->findData(m_settings->m_record1Height));

    ui->preview1ResolutionComboBox->setCurrentIndex(ui->preview1ResolutionComboBox->findData(m_settings->m_preview1Height));


}

Settings::~Settings()
{
    delete m_settings;
    delete ui;
}

void Settings::on_backBtn_clicked()
{
    VideoResolution cam1Res = getResolutionFromHeight(ui->record1ResolutionComboBox->currentData().value<int16_t>());
    m_settings->m_record1Width = cam1Res.width;
    m_settings->m_record1Height = cam1Res.height;
    VideoResolution preview1Res = getResolutionFromHeight(ui->preview1ResolutionComboBox->currentData().value<int16_t>());
    m_settings->m_preview1Width = preview1Res.width;
    m_settings->m_preview1Height = preview1Res.height;

    m_settings->save("/opt/Rekkon/config.xml");
    emit selectVideoRecord();
}

Settings::VideoResolution Settings::getResolutionFromHeight(int16_t height)
{
    if (height == m_240p.height) return m_240p;
    if (height == m_360p.height) return m_360p;
    if (height == m_480p.height) return m_480p;
    if (height == m_720p.height) return m_720p;
    if (height == m_1080p.height) return m_1080p;
    else return  m_480p; //default_res
}

