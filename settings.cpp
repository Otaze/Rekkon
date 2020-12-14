#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    m_settings = SettingsStructure::instance();
    m_settings->load(CONFIG_FILE_PATH);
    ui->mediaFolderPathLineEdit->setText(QString::fromStdString(m_settings->m_videoFolder));

    m_240p.width=432;
    m_240p.height=243;
    m_240p.res_description = QString("240p");

    m_360p.width=640;
    m_360p.height=360;
    m_360p.res_description = QString("360p");

    m_480p.width=848;
    m_480p.height=477;
    m_480p.res_description = QString("480p");

    m_540p.width=960;
    m_540p.height=540;
    m_540p.res_description = QString("540p");


    m_720p.width=1280;
    m_720p.height=720;
    m_720p.res_description = QString("720p");


    m_1080p.width=1920;
    m_1080p.height=1080;
    m_1080p.res_description = QString("1080p");

    m_012MPx.width=400;
    m_012MPx.height=300;
    m_012MPx.res_description = QString("0,12MP");

    m_024MPx.width=800;
    m_024MPx.height=600;
    m_024MPx.res_description = QString("0.24MP");

    m_1MPx.width=1152;
    m_1MPx.height=864;
    m_1MPx.res_description = QString("1MP");

    m_6MPx.width=2832;
    m_6MPx.height=2128;
    m_6MPx.res_description = QString("6MP");

    m_8MPx.width=3264;
    m_8MPx.height=2448;
    m_8MPx.res_description = QString("8MP");

    m_12MPx.width=4056;
    m_12MPx.height=3040;
    m_12MPx.res_description = QString("12,3MP");

    ui->preview1ResolutionComboBox->addItem(m_240p.res_description,m_240p.height);
    ui->preview1ResolutionComboBox->addItem(m_360p.res_description,m_360p.height);
    ui->preview1ResolutionComboBox->addItem(m_480p.res_description,m_480p.height);
    ui->preview1ResolutionComboBox->addItem(m_540p.res_description,m_540p.height);

    ui->record1ResolutionComboBox->addItem(m_540p.res_description,m_540p.height);
    ui->record1ResolutionComboBox->addItem(m_720p.res_description,m_720p.height);
    ui->record1ResolutionComboBox->addItem(m_1080p.res_description,m_1080p.height);

    ui->previewStillResolutionComboBox->addItem(m_012MPx.res_description,m_012MPx.height);
    ui->previewStillResolutionComboBox->addItem(m_024MPx.res_description,m_024MPx.height);
    ui->previewStillResolutionComboBox->addItem(m_1MPx.res_description,m_1MPx.height);

    ui->recordStillResolutionComboBox->addItem(m_6MPx.res_description,m_6MPx.height);
    ui->recordStillResolutionComboBox->addItem(m_8MPx.res_description,m_8MPx.height);
    ui->recordStillResolutionComboBox->addItem(m_12MPx.res_description,m_12MPx.height);

    ui->record1ResolutionComboBox->setCurrentIndex(ui->record1ResolutionComboBox->findData(m_settings->m_recordVideoHeight));
    ui->preview1ResolutionComboBox->setCurrentIndex(ui->preview1ResolutionComboBox->findData(m_settings->m_previewVideoHeight));

    ui->recordStillResolutionComboBox->setCurrentIndex(ui->recordStillResolutionComboBox->findData(m_settings->m_recordStillHeight));
    ui->previewStillResolutionComboBox->setCurrentIndex(ui->previewStillResolutionComboBox->findData(m_settings->m_previewStillHeight));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_backBtn_clicked()
{
    Resolution recordvideoRes = getResolutionFromHeight(ui->record1ResolutionComboBox->currentData().value<int16_t>());
    m_settings->m_recordVideoWidth = recordvideoRes.width;
    m_settings->m_recordVideoHeight = recordvideoRes.height;
    Resolution previewVideoRes = getResolutionFromHeight(ui->preview1ResolutionComboBox->currentData().value<int16_t>());
    m_settings->m_previewVideoWidth = previewVideoRes.width;
    m_settings->m_previewVideoHeight = previewVideoRes.height;

    Resolution recordStillRes = getResolutionFromHeight(ui->recordStillResolutionComboBox->currentData().value<int16_t>());
    m_settings->m_recordStillWidth = recordStillRes.width;
    m_settings->m_recordStillHeight = recordStillRes.height;
    Resolution previewStillRes = getResolutionFromHeight(ui->previewStillResolutionComboBox->currentData().value<int16_t>());
    m_settings->m_previewStillWidth = previewStillRes.width;
    m_settings->m_previewStillHeight = previewStillRes.height;

    m_settings->save("/opt/Rekkon/config.xml");
    emit selectVideoRecord();
}

Settings::Resolution Settings::getResolutionFromHeight(int16_t height)
{
    if (height == m_240p.height) return m_240p;
    if (height == m_360p.height) return m_360p;
    if (height == m_480p.height) return m_480p;
    if (height == m_540p.height) return m_540p;
    if (height == m_720p.height) return m_720p;
    if (height == m_1080p.height) return m_1080p;
    if (height == m_012MPx.height) return m_012MPx;
    if (height == m_024MPx.height) return m_024MPx;
    if (height == m_1MPx.height) return m_1MPx;
    if (height == m_6MPx.height) return m_6MPx;
    if (height == m_8MPx.height) return m_8MPx;
    if (height == m_12MPx.height) return m_12MPx;
    else return  m_480p; //default_res
}

