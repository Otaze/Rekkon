#include "mediafile.h"

MediaFile::MediaFile(MediaType type,
                     string filepath,
                     string filename,
                     long duration,
                     unsigned char* thumbnail_image,
                     unsigned int thumbnail_width,
                     unsigned int thumbnail_height)
{
    m_type = type;
    m_filepath = filepath;
    m_filename = filename;
    m_duration = duration;
    m_thumbnail_image = thumbnail_image;
    m_thumbnail_width = thumbnail_width;
    m_thumbnail_height = thumbnail_height;
}


MediaFile::~MediaFile()
{
    delete m_thumbnail_image;
}

unsigned int MediaFile::thumbnailHeight() const
{
    return m_thumbnail_height;
}

unsigned int MediaFile::thumbnailWidth() const
{
    return m_thumbnail_width;
}

unsigned char *MediaFile::thumbnailImage() const
{
    return m_thumbnail_image;
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
