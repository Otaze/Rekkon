#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

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
    Mat * thumbnail);


    ~MediaFile();


    long duration() const;

    string filename() const;

    string filepath() const;

    MediaType type() const;

    Mat *thumbnail() const;

private:
    MediaType m_type;
    string m_filepath;
    string m_filename;
    long m_duration;
    Mat * m_thumbnail;


};

#endif // MEDIAFILE_H
