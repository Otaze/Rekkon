#include "settingsstructure.h"

/**
 * Initialize static attributes.
 */
SettingsStructure* SettingsStructure::m_instance{nullptr};
std::mutex SettingsStructure::m_mutex;


SettingsStructure * SettingsStructure::instance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_instance == nullptr)
    {
        m_instance = new SettingsStructure();
    }
    return m_instance;
}

SettingsStructure::SettingsStructure(){}

SettingsStructure::~SettingsStructure(){}

void SettingsStructure::load(const std::string &filename)
{
    // Create empty property tree object
    bool file_exists = std::filesystem::exists(std::filesystem::directory_entry(std::filesystem::path(filename)).status());
    if (!file_exists)
    {
        std::ofstream outfile (filename);
        outfile.close();
    }
    pt::ptree tree;

    pt::read_xml(filename, tree);

    m_videoFolder = tree.get("settings.videoFolder","/opt/Rekkon/videos");
    m_recordVideoWidth = tree.get("settings.record.video.width", 1920);
    m_recordVideoHeight= tree.get("settings.record.video.height", 1080);
    m_previewVideoWidth = tree.get("settings.preview.video.width", 960);
    m_previewVideoHeight = tree.get("settings.preview.video.height", 540);
    m_recordStillWidth = tree.get("settings.record.still.width", 4056);
    m_recordStillHeight= tree.get("settings.record.still.height", 3040);
    m_previewStillWidth = tree.get("settings.preview.still.width", 800);
    m_previewStillHeight = tree.get("settings.preview.still.height", 600);


}
void SettingsStructure::save(const std::string &filename)
{
    // Create an empty property tree object.
    pt::ptree tree;


    tree.put("settings.videoFolder", m_videoFolder);
    tree.put("settings.record.video.width", m_recordVideoWidth);
    tree.put("settings.record.video.height", m_recordVideoHeight);
    tree.put("settings.preview.video.width", m_previewVideoWidth);
    tree.put("settings.preview.video.height", m_previewVideoHeight);

    tree.put("settings.record.still.width", m_recordStillWidth);
    tree.put("settings.record.still.height", m_recordStillHeight);
    tree.put("settings.preview.still.width", m_previewStillWidth);
    tree.put("settings.preview.still.height", m_previewStillHeight);



    // Write property tree to XML file
    pt::write_xml(filename, tree);
}
