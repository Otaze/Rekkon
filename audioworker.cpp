#include "audioworker.h"



AudioWorker::AudioWorker(QObject *parent) : QObject(parent),
    m_initialized(false)
{

    setupAudio();
}

AudioWorker::~AudioWorker()
{
    delete m_format_ctx;
    delete m_audio_stream;
    delete m_codec_ctx;
    delete m_codec;
    delete m_portaudio_stream;
    delete m_portaudio_autosys;
}

void AudioWorker::setupAudio()
{

    try
        {

            // Set up the System:
            m_portaudio_autosys = new portaudio::AutoSystem();
            portaudio::System &sys = portaudio::System::instance();

            // Set up the parameters required to open a (Callback)Stream:

            portaudio::Device &inputDevice = sys.defaultInputDevice();
            portaudio::Device &outputDevice = sys.defaultOutputDevice();
            unsigned int nbChannels = 1;
            portaudio::DirectionSpecificStreamParameters inParams(inputDevice, nbChannels, portaudio::FLOAT32, false, inputDevice.defaultHighInputLatency(), NULL);
            nbChannels = 2;


            //setup file format

            AVStream *audioStream = NULL;


            m_format_ctx =  avformat_alloc_context();
            if (m_format_ctx == NULL)
            {
                cerr << "Error creating format context" << endl;
                return;
            }


            if (!(m_format_ctx->url =  av_strdup(m_audio_filename.c_str())))
            {
                cerr << "Could not allocate url" << endl;
                return;
            }


            if (!(audioStream = avformat_new_stream(m_format_ctx, NULL))) {
                cerr << "Could not create audioStream" << endl;
                return;
            }

            //Set up audio encoder

            m_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
            if (m_codec == NULL)
            {
                cerr << "Error creating codec" << endl;
                return;
            }

            cerr << "alloc ctx" << endl;
            m_codec_ctx = avcodec_alloc_context3(m_codec);
            if (m_codec == NULL)
            {
                cerr << "Error creating codec context" << endl;
                return;
            }

            m_codec_ctx->bit_rate = 128000;
            m_codec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
            m_codec_ctx->sample_rate = inputDevice.defaultSampleRate();
            m_codec_ctx->channel_layout = av_get_default_channel_layout(nbChannels);
            m_codec_ctx->channels = nbChannels;
            m_codec_ctx->profile = FF_PROFILE_AAC_MAIN;
            m_codec_ctx->time_base.num= 1;
            m_codec_ctx->time_base.den= inputDevice.defaultSampleRate();

            av_opt_set(m_codec_ctx->priv_data, "tune", "zerolatency", 0);

            audioStream->time_base.den = inputDevice.defaultSampleRate();
            audioStream->time_base.num = 1;

           /* if (m_format_ctx->oformat->flags & AVFMT_GLOBALHEADER)
                 m_codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;*/

            m_codec_ctx->codec_type = AVMEDIA_TYPE_AUDIO;
            if (avcodec_open2(m_codec_ctx, m_codec, NULL))
            {
                cerr << "Error opening codec" << endl;
                return;
            }

            if (avcodec_parameters_from_context(audioStream->codecpar, m_codec_ctx) < 0)
            {
                cerr << "Could not initialize stream parameters" << endl;
                return;
            }

            int FRAMES_PER_BUFFER = m_codec_ctx->frame_size;

            portaudio::StreamParameters params(inParams,portaudio::DirectionSpecificStreamParameters::null(), inputDevice.defaultSampleRate(), FRAMES_PER_BUFFER, paClipOff);

            // Create (and open) a new Stream, using the SineGenerator::generate function as a callback:
            m_portaudio_stream =  new portaudio::MemFunCallbackStream<AudioWorker>(params, *this, &AudioWorker::audioCallback);


        }
        catch (const portaudio::PaException &e)
        {
            std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
            return;
        }


        m_initialized = true;
}

int AudioWorker::audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    assert(inputBuffer != NULL);

    const float **in = (const float **)(inputBuffer);
    float *samples, *samples2;

    if (isRecording() && !isPaused())
     {
         AVFrame *frame;


         // create frame containing raw audio input
         frame = av_frame_alloc();
         if (!frame) {
             cerr << "Could not allocate audio frame" << endl;
             return -1;
         }

         frame->nb_samples = m_codec_ctx->frame_size;
         frame->format = m_codec_ctx->sample_fmt;
         frame->channel_layout = m_codec_ctx->channel_layout;


         // allocate the data buffers
         if (av_frame_get_buffer(frame, 0) < 0) {
            cerr << "Could not allocate audio data buffers" << endl;
            return -1;
         }
         // make sure the frame is writable -- makes a copy if the encoder
         // kept a reference internally
         if (av_frame_make_writable(frame) < 0)
             return -1;
         samples = (float*)frame->data[0];
         samples2 = (float*)frame->data[1];

         for (unsigned int i = 0; i < framesPerBuffer; i++) {

             samples[i] = in[0][i];
             samples2[i] = in[0][i];
         }
         // encode raw data & write in file
         audioEncode(frame);


         // free the memory
         av_frame_free(&frame);
    }

    return 0;
}



void AudioWorker::audioEncode(AVFrame *frame)
{
    int ret;
    AVPacket *pkt;

    // create packet holding encoded ouput
    pkt = av_packet_alloc();
    if (!pkt) {
        cerr << "could not allocate the packet" << endl;
        return;
    }


    /* send the frame for encoding */
    ret = avcodec_send_frame(m_codec_ctx, frame);
    if (ret < 0) {
        cerr << "Error sending the frame to the encoder" << endl;
        if (ret == AVERROR(EAGAIN)) cerr << "input is not accepted in the current state - user must read output with avcodec_receive_packet" << endl;
        if (ret == AVERROR(EINVAL)) cerr << "codec not opened, refcounted_frames not set" << endl;
        if (ret == AVERROR(ENOMEM)) cerr << "failed to add packet to internal queue " << endl;
        return;
    }

    /* read all the available output packets (in general there may be any
     * number of them) */
    while (ret >= 0) {
        ret = avcodec_receive_packet(m_codec_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return;
        }
        else if (ret < 0)
        {
            cerr  << "Error encoding audio frame" << endl;
            return;
        }
        if (av_write_frame(m_format_ctx, pkt) < 0)
        {
            cerr << "Error writing packet data to file" << endl;
        }
        av_packet_unref(pkt);

    }
    av_packet_free(&pkt);


}
 


void AudioWorker::releaseAudio()
{

}

void AudioWorker::startRecord(std::string filepath)
{
    m_is_recording = true;
    resumeRecord();
    setAudioFile(filepath);


    if (avformat_write_header(m_format_ctx, NULL) < 0)
    {
        cerr << "Could not write file header" << endl;
        return;
    }

     // Start the Stream (audio playback starts):

    try
    {
        m_portaudio_stream->start();
    }
    catch (const portaudio::PaException &e)
    {
        std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
    }


}

void AudioWorker::stopRecord()
{
    pauseRecord();
    m_is_recording = false;
    try
    {
        m_portaudio_stream->stop();
        delete m_portaudio_stream;

    }
    catch (const portaudio::PaException &e)
    {
        std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
    }



    flushEncoder();
    if (av_write_trailer(m_format_ctx) < 0)
    {
        cerr << "Could not write file trailer" << endl;
        return;
    }


}

void AudioWorker::pauseRecord()
{
    m_is_paused = true;
}

void AudioWorker::resumeRecord()
{
    m_is_paused = false;
}

bool AudioWorker::isPaused()
{
    return m_is_paused;
}
bool AudioWorker::isRecording()
{
    return  m_is_recording;
}

bool AudioWorker::isInitialized()
{
    return m_initialized;
}

void AudioWorker::setAudioFile(string audio_filename)
{
    m_audio_filename = audio_filename;

    delete m_format_ctx->pb;
    AVIOContext *ioCtx;

    if (avio_open(&ioCtx,m_audio_filename.c_str(),AVIO_FLAG_WRITE))
    {
        cerr << "Could not open file" << endl;
        return;
    }

    m_format_ctx->pb = ioCtx;

    if (!(m_format_ctx->oformat = av_guess_format(NULL,m_audio_filename.c_str(),NULL)))
    {
        cerr << "Could not find file format" << endl;
        return;
    }



}

void AudioWorker::flushEncoder()
{
    if (m_codec_ctx != NULL && !m_audio_filename.empty())
    {
        audioEncode(NULL);
    }

}

