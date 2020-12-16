#include "mediafile.h"

MediaFile::MediaFile(MediaType type,
                     string filepath,
                     string filename,
                     long duration,
                     Mat * thumbnail)
{
    m_type = type;
    m_filepath = filepath;
    m_filename = filename;
    m_duration = duration;
    m_thumbnail = thumbnail;
}


MediaFile::~MediaFile()
{
    delete m_thumbnail;
}

long MediaFile::duration() const
{
    return m_duration;
}

string MediaFile::filename() const
{
    return m_filename;
}

string MediaFile::filepath() const
{
    return m_filepath;
}

MediaType MediaFile::type() const
{
    return m_type;
}

Mat *MediaFile::thumbnail() const
{
    return m_thumbnail;
}
