#ifndef SETTINGSSTRUCTURE_H
#define SETTINGSSTRUCTURE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <mutex>

namespace pt = boost::property_tree;

#define CONFIG_FILE_PATH "/opt/Rekkon/config.xml"

class SettingsStructure
{

public:
    static SettingsStructure * instance();
    std::string m_videoFolder;
    int16_t m_recordVideoWidth;
    int16_t m_recordVideoHeight;
    int16_t m_previewVideoWidth;
    int16_t m_previewVideoHeight;
    int16_t m_recordStillWidth;
    int16_t m_recordStillHeight;
    int16_t m_previewStillWidth;
    int16_t m_previewStillHeight;
    void load(const std::string &filename = CONFIG_FILE_PATH);
    void save(const std::string &filename = CONFIG_FILE_PATH);

private:
    SettingsStructure();
    ~SettingsStructure();
    static SettingsStructure * m_instance;
    static std::mutex m_mutex;

};

#endif // SETTINGSSTRUCTURE_H
