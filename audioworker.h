#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H

#include <QObject>
#include "portaudiocpp/PortAudioCpp.hxx"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include <libavutil/common.h>
#include "libavformat/avio.h"
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
#include <libavutil/opt.h>
#include <libavformat/avio.h>
}

using namespace std;

class AudioWorker : public QObject
{
    Q_OBJECT


public:
    explicit AudioWorker(QObject *parent = nullptr);
    ~AudioWorker();
    void releaseAudio();
    void startRecord(std::string filepath);
    void stopRecord();
    void pauseRecord();
    void resumeRecord();
    bool isPaused();
    bool isRecording();
    bool isInitialized();

protected:
//    static void audioEncode(AVFrame *frame);
    int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);


    void setupAudio();
    void audioEncode( AVFrame *frame);
    void flushEncoder();

    void setAudioFile(string audio_filename);




private:
    string m_audio_filename;
    AVFormatContext* m_format_ctx;
    AVStream* m_audio_stream;
    AVCodecContext* m_codec_ctx;
    AVCodec* m_codec;
    bool m_is_recording;
    bool m_is_paused;
    portaudio::MemFunCallbackStream<AudioWorker> *m_portaudio_stream;
    portaudio::AutoSystem * m_portaudio_autosys;

    bool m_initialized;

    
};

#endif // AUDIOWORKER_H

