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

namespace pt = boost::property_tree;

struct SettingsStructure
{
    std::string m_videoFolder;
    int16_t m_record1Width;
    int16_t m_record1Height;

    int16_t m_preview1Width;
    int16_t m_preview1Height;
    void load(const std::string &filename = "/opt/Rekkon/config.xml");
    void save(const std::string &filename = "/opt/Rekkon/config.xml");
};

#endif // SETTINGSSTRUCTURE_H
