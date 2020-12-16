#ifndef MEDIASERVICE_H
#define MEDIASERVICE_H

#include <mutex>
#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>
#include <filesystem>
#include "models/mediafile.h"
#include "models/settingsstructure.h"
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

class MediaService
{
public:
    static MediaService * instance();
    void refreshList();
    vector<MediaFile*> mediaFileList() const;

private:
    MediaService();
    ~MediaService();
    MediaFile* createMediaFile(string filepath, MediaType fileType);
    static MediaService * m_instance;
    static std::mutex m_mutex;
    vector<MediaFile*> m_mediaFile_list;
    SettingsStructure * m_settings;
    VideoCapture m_cameraCap;
    Mat * m_frame;
};

#endif // MEDIASERVICE_H
