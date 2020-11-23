#include "settingsstructure.h"


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
    m_record1Width = tree.get("settings.record.1.width", 1920);
    m_record1Height= tree.get("settings.record.1.height", 1080);
    m_preview1Width = tree.get("settings.preview.1.width", 853);
    m_preview1Height = tree.get("settings.preview.1.height", 480);


}
void SettingsStructure::save(const std::string &filename)
{
    // Create an empty property tree object.
    pt::ptree tree;


    tree.put("settings.videoFolder", m_videoFolder);
    tree.put("settings.record.1.width", m_record1Width);
    tree.put("settings.record.1.height", m_record1Height);
    tree.put("settings.preview.1.width", m_preview1Width);
    tree.put("settings.preview.1.height", m_preview1Height);


    // Write property tree to XML file
    pt::write_xml(filename, tree);
}
