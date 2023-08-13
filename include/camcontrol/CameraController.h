//
// Created by kevinhy on 8/3/23.
//

#ifndef INSTA_PLUS_PLUS_CAMERA_CONTROLLER_H
#define INSTA_PLUS_PLUS_CAMERA_CONTROLLER_H

#include "camera/camera.h"
#include "camera/device_discovery.h"
#include <boost/signals2.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
extern "C"
{
#include <libavcodec/avcodec.h>
}
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <ostream>
using namespace boost;
using namespace std;

struct Audio
{
    const uint8_t* data;
    size_t size;
    int64_t timestamp;
};

struct Video
{
    const uint8_t* data;
    size_t size;
    int64_t timestamp;
    uint8_t streamType;
    int streamIndex;
};

struct Gyro
{
    std::vector<ins_camera::GyroData> data;
};

struct Exposure
{
    ins_camera::ExposureData data;
};

struct Frame
{
    Audio audio;
    Video video;
    Gyro gyro;
    Exposure exposure;
};

// ostream& operator<<(ostream& os, const Frame& frame) {
////    return os << "Audio Data " << frame.audio.data << endl;
//}

class StreamDelegate : public ins_camera::StreamDelegate
{
  public:
    //    Frame frame;
    StreamDelegate()
    {
        //        boost::signals2::signal<void (uint8_t, size_t, int64_t, uint8_t)> audio_sig;

        //        stream_file = fopen("./data/video.h264", "wb");
    }
    ~StreamDelegate()
    {
        //        fclose(stream_file);
    }

    static void pgm_save(unsigned char* buf, int wrap, int xsize, int ysize, char* filename)
    {
        FILE* f;
        int i;

        f = fopen(filename, "wb");
        fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
        for (i = 0; i < ysize; i++)
            fwrite(buf + i * wrap, 1, xsize, f);
        fclose(f);
    }

    void decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename)
    {
        char buf[1024];
        int ret;

        ret = avcodec_send_packet(dec_ctx, pkt);
        if (ret < 0) {
            fprintf(stderr, "Error sending a packet for decoding\n");
            /* exit(1); */
        }

        while (ret >= 0) {
            ret = avcodec_receive_frame(dec_ctx, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                fprintf(stderr, "could not receive frame");
                if (ret == AVERROR_EOF)
                    std::cout << "End of file error\n";
                return;
            } else if (ret < 0) {
                fprintf(stderr, "Error during decoding\n");
                exit(1);
            }

            printf("saving frame %3" PRId64 "\n", dec_ctx->frame_number);
            fflush(stdout);

            /* the picture is allocated by the decoder. no need to
               free it */
            snprintf(buf, sizeof(buf), "%s-%" PRId64, filename, dec_ctx->frame_number);
            std::cout << buf << std::endl;
            pgm_save(frame->data[0], frame->linesize[0], frame->width, frame->height, buf);
            counter++;
        }
    }

    void OnAudioData(const uint8_t* data, size_t data_size, int64_t timestamp) override
    {
        // Currently does not save audio data
        //        audio_sig()
        //        std::cout << "Audio data received!" << timestamp << std::endl;
        //        std::cout << "Status is:" << status << std::endl;
        //            frame.audio.data = data;
        //            frame.audio.size = size;
        //            frame.audio.timestamp = timestamp;
    }

    void OnVideoData(const uint8_t* data,
                     size_t data_size,
                     int64_t timestamp,
                     uint8_t streamType,
                     int stream_index) override
    {
        std::cout << "Frame: " << data_size << " | " << timestamp << std::endl;
        const AVCodec* codec;
        AVCodecParserContext* parser;
        AVCodecContext* c = NULL;
        AVPacket* pkt;

        pkt = av_packet_alloc();
        if (!pkt)
            exit(1);

        codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        if (!codec) {
            fprintf(stderr, "Codec not found\n");
            exit(1);
        }
        parser = av_parser_init(codec->id);
        if (!parser) {
            fprintf(stderr, "parser not found\n");
            exit(1);
        }

        c = avcodec_alloc_context3(codec);
        if (!c) {
            fprintf(stderr, "Could not allocate video codec context\n");
            exit(1);
        }

        /* For some codecs, such as msmpeg4 and mpeg4, width and height
           MUST be initialized there because this information is not
           available in the bitstream. */

        /* open it */
        if (avcodec_open2(c, codec, NULL) < 0) {
            fprintf(stderr, "Could not open codec\n");
            exit(1);
        }

        auto ffFrame = av_frame_alloc();
        if (!ffFrame) {
            fprintf(stderr, "Could not allocate video frame\n");
            exit(1);
        }

        int eof, ret;

        do {
            eof = !data_size;
            while (data_size > 0 || eof) {
                ret = av_parser_parse2(parser,
                                       c,
                                       &pkt->data,
                                       &pkt->size,
                                       data,
                                       data_size,
                                       AV_NOPTS_VALUE,
                                       AV_NOPTS_VALUE,
                                       0);
                if (ret < 0) {
                    fprintf(stderr, "Error while parsing\n");
                    exit(1);
                }
                data += ret;
                data_size -= ret;
                std::string filename = "test" + std::to_string(counter);
                std::cout << "Filename - " << filename << std::endl;
                if (pkt->size) {
                    decode(c, ffFrame, pkt, filename.c_str());
                    cout << "Decoded\n";
                } else if (eof) {
                    fprintf(stderr, "Did not decode\n");
                    break;
                }
            }
        } while (!eof);

        /* flush the decoder */
        std::string filename = "test" + std::to_string(counter);

        decode(c, ffFrame, NULL, filename.c_str());
        av_parser_close(parser);
        avcodec_free_context(&c);
        av_frame_free(&ffFrame);
        av_packet_free(&pkt);
        /* cv::Mat cvFrame; */
        /* cv::VideoCapture cap(-1); */
        /* if (!cap.isOpened()) { */
        /*     std::cout << "Error cannot open the capture\n"; */
        /* } */
        /* if (!cap.read(cvFrame)) { */
        /*     cv::imshow("", cvFrame); */
        /* } */
        /* cv::waitKey(1); */
        /* cv::Mat encodedImage(1, size, CV_8UC1, const_cast<uint8_t*>(data)); */

        /* auto imgDecoded = imdecode(encodedImage, cv::IMREAD_ANYCOLOR); */
        /* cv::imshow("Decoded Image", imgDecoded); */
        /* cv::waitKey(1); */
        //        fwrite(data, sizeof(uint8_t), size, stream_file);
        //            frame.video.data = data;
        //            frame.video.size = size;
        //            frame.video.timestamp = timestamp;
        //            frame.video.streamType = streamType;
        //            frame.video.streamIndex = stream_index;
    }

    void OnGyroData(const std::vector<ins_camera::GyroData>& data) override
    {
        //        frame.gyro.data = data;
        // Populate later
    }

    void OnExposureData(const ins_camera::ExposureData& data) override
    {
        //        frame.exposure.data = data;
        // Populate later
    }

  private:
    //    FILE *stream_file;
    int64_t last_timestamp = 0;
    int64_t counter = 0;
};

class CameraController
{
    std::shared_ptr<ins_camera::Camera> camera;

  public:
    std::shared_ptr<StreamDelegate> streamDelegate;
    bool streaming; // Whether camera is streaming or not. false is no stream. true is streaming
    bool connected; // Whether camera is connected to or not. false is no stream. true is streaming.

    CameraController()
    {
        open();
        streamDelegate = std::make_shared<StreamDelegate>();
        streaming = false;
        connected = false;
        setup();
    }

    // Force it to not cast
    // Create some documentation on this controller

    ~CameraController()
    {
        // Always close camera connection after handler is destroyed
        close();
    }

    void open()
    {
        if (connected) {
            return;
        }

        ins_camera::DeviceDiscovery discovery;
        auto list = discovery.GetAvailableDevices();
        if (list.empty()) {
            std::cout << "No cameras found!" << std::endl;
            throw "No cameras to be found";
        }

        std::shared_ptr<ins_camera::Camera> cam =
          std::make_shared<ins_camera::Camera>(list[0].info);
        if (!cam->Open()) {
            cam->Close();
            connected = false;
            throw "Could not open camera";
        }
        connected = true;
        camera = cam;
    }

    void close()
    {
        if (!connected) {
            return;
        }
        camera->Close();
        connected = false;
    }

  private:
    void setup()
    {
        // In the future, add a setup file reader

        // Sync time
        if (camera) {
        }
        auto start = time(NULL);
        camera->SyncLocalTimeToCamera(start);

        // Set camera recording settings
        auto exposure = std::make_shared<ins_camera::ExposureSettings>();
        exposure->SetExposureMode(ins_camera::PhotographyOptions_ExposureMode::
                                    PhotographyOptions_ExposureOptions_Program_AUTO);
        bool success = camera->SetExposureSettings(
          ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, exposure);
        if (!success) {
            throw "Failed to set exposure settings";
            //            std::cout << "Failed to set exposure settings" << std::endl;
        }
        //        std::cout << "Exposure settings set" << std::endl;

        // Set stream delegate
        std::shared_ptr<ins_camera::StreamDelegate> stream =
          std::dynamic_pointer_cast<ins_camera::StreamDelegate>(streamDelegate);
        camera->SetStreamDelegate(stream);
        //        camera->SetStreamDelegate(::make_shared<ins_camera::StreamDelegate>(internalStreamDelegate));
        //        camera->SetStreamDelegate(std::make_shared<ins_camera::StreamDelegate>(streamDelegate));
    }

  public:
    void startStream()
    {
        if (streaming) {
            return;
        }
        ins_camera::LiveStreamParam param;
        param.video_resolution = ins_camera::VideoResolution::RES_368_368P30;
        param.lrv_video_resulution = ins_camera::VideoResolution::RES_368_368P30;
        param.video_bitrate = 1024 * 1024 / 2;
        param.enable_audio = false;
        param.using_lrv = false;

        if (!camera->StartLiveStreaming(param)) {
            //            std::cout << "Failed to start live streaming" << std::endl;
            streaming = false;
            throw "Failed to start live stream";
        }
        streaming = false;
    }

    void stopStream()
    {
        if (!streaming) {
            return;
        }
        if (!camera->StopLiveStreaming()) {
            throw "Failed to stop stream";
        }
        streaming = false;
    }

    void printFrame()
    {
        //        cout << "hello" << endl;
        //        cout << "Video size: " << streamDelegate.frame.video.size << endl;
        //        cout << "Audio size: " <<  streamDelegate.frame.audio.size << endl;
        //        cout << "Gyro data: " << streamDelegate.frame.gyro << endl;
    }

    //    Frame returnFrame() {
    //        return streamDelegate.frame;
    //    }
};

#endif // INSTA_PLUS_PLUS_INSTA_H
