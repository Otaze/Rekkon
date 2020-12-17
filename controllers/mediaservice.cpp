#include "mediaservice.h"

/**
 * Initialize static attributes.
 */
MediaService* MediaService::m_instance{nullptr};
std::mutex MediaService::m_mutex;


MediaService * MediaService::instance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_instance == nullptr)
    {
        m_instance = new MediaService();
    }
    return m_instance;
}

MediaService::MediaService(){
    m_settings = SettingsStructure::instance();
    m_mediaFile_list = new vector<MediaFile*>();
}

MediaService::~MediaService(){
    delete m_frame;
}

/**
 * @brief MediaService::mediaFileList
 * Returns the list of media files in the folder defined by the config file
 * @return (vector<MediaFile>)
 */
vector<MediaFile*>* MediaService::mediaFileList() const
{
    lock_guard<std::mutex> lock(m_mutex);
    return m_mediaFile_list;
}

/**
 * @brief MediaService::refreshList
 * Refresh or create the list of media files in the folder defined by the config file
 */
void MediaService::refreshList()
{
    lock_guard<std::mutex> lock(m_mutex);

    MediaFile * fileToBeAdded;
    m_settings->load();
    cerr << "delete MediaFiles" << endl;
    for (MediaFile * mFile : *m_mediaFile_list)
    {
        delete mFile;
    }
    m_mediaFile_list->clear();

    for (const auto & file : std::filesystem::recursive_directory_iterator(m_settings->m_videoFolder))
    {
        fileToBeAdded = NULL;
        if (file.path().extension() == ".mkv"
                || file.path().extension() == ".mp4"
                || file.path().extension() == ".avi"
                || file.path().extension() == ".webm")
        {
            fileToBeAdded = createMediaFile(file.path().string(),MediaType::Video);
        }

        if (file.path().extension() == ".jpg"
                || file.path().extension() == ".png" )
        {
            fileToBeAdded = createMediaFile(file.path().string(),MediaType::Image);
        }

        if (fileToBeAdded != NULL)
            m_mediaFile_list->push_back(fileToBeAdded);
    }
}

/**
 * @brief MediaService::createMediaFile
 * @param filepath
 * @param fileType
 * Create a MediaFile from a filepath and its type.
 * It use opencv functions to retrieve an image for thumbnail
 * @return (MediaFile*)A pointer to a new MediaFile object, Null if the creation failed
 */
MediaFile* MediaService::createMediaFile(string filepath, MediaType fileType)
{
    MediaFile * file;
    string filename;
    long duration;

    filename = std::filesystem::path(filepath).filename().string();

    if (fileType == MediaType::Video)
    {
        m_frame = new Mat;
        m_cameraCap.open(filepath);
        if(!m_cameraCap.isOpened())
        {
           return NULL;
        }
        double frnb = m_cameraCap.get(CAP_PROP_FRAME_COUNT) ;
        double fps = m_cameraCap.get(CAP_PROP_FPS);
        duration = (long) 1000*(frnb/fps);
        int thumbnailPosition = rand() % (int)(0.2 * frnb);
        m_cameraCap.set(CAP_PROP_POS_FRAMES,thumbnailPosition);
        m_cameraCap >> *m_frame;
        m_cameraCap.release();

    } else if (fileType == MediaType::Image) {
        duration = -1;
        m_frame = new Mat(imread(filepath, IMREAD_COLOR));
    } else {
        return NULL;
    }

    if( !m_frame->data || m_frame->empty()) return NULL;



    file = new MediaFile(fileType, filepath, filename, duration, m_frame);

    return file;
}
