#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>

using namespace std;

enum MediaType {
    Video,
    Image
};


class MediaFile
{
public:
    MediaFile(MediaType type,
    string filepath,
    string filename,
    long duration,
    unsigned char* thumbnail_image,
    unsigned int thumbnail_width,
    unsigned int thumbnail_height);


    ~MediaFile();

    unsigned int thumbnailHeight() const;

    unsigned int thumbnailWidth() const;

    unsigned char *thumbnailImage() const;;

    long duration() const;

    string filename() const;

    string filepath() const;

    MediaType type() const;

private:
    MediaType m_type;
    string m_filepath;
    string m_filename;
    long m_duration;
    unsigned char* m_thumbnail_image;
    unsigned int m_thumbnail_width;
    unsigned int m_thumbnail_height;



};

#endif // MEDIAFILE_H
