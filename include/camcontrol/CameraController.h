//
// Created by kevinhy on 8/3/23.
//

#ifndef INSTA_PLUS_PLUS_CAMERA_CONTROLLER_H
#define INSTA_PLUS_PLUS_CAMERA_CONTROLLER_H

#include "camera/camera.h"
#include "camera/device_discovery.h"
#include <iostream>
#include <boost/signals2.hpp>
#include <ostream>
using namespace boost;
using namespace std;

struct Audio {
    const uint8_t* data;
    size_t size;
    int64_t timestamp;

};

struct Video {
    const uint8_t* data;
    size_t size;
    int64_t timestamp;
    uint8_t streamType;
    int streamIndex;

};

struct Gyro {
    std::vector<ins_camera::GyroData> data;
};

struct Exposure {
    ins_camera::ExposureData data;

};

struct Frame {
    Audio audio;
    Video video;
    Gyro gyro;
    Exposure exposure;
};

//ostream& operator<<(ostream& os, const Frame& frame) {
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

    void OnAudioData(const uint8_t *data, size_t size, int64_t timestamp) override
    {
        // Currently does not save audio data
//        audio_sig()
//        std::cout << "Audio data received!" << timestamp << std::endl;
//        std::cout << "Status is:" << status << std::endl;
//            frame.audio.data = data;
//            frame.audio.size = size;
//            frame.audio.timestamp = timestamp;
    }

    void OnVideoData(const uint8_t *data, size_t size, int64_t timestamp, uint8_t streamType, int stream_index) override
    {
        std::cout << "Frame: " << size << " | " << timestamp << std::endl;
//        fwrite(data, sizeof(uint8_t), size, stream_file);
//            frame.video.data = data;
//            frame.video.size = size;
//            frame.video.timestamp = timestamp;
//            frame.video.streamType = streamType;
//            frame.video.streamIndex = stream_index;
    }

    void OnGyroData(const std::vector<ins_camera::GyroData> &data) override
    {
//        frame.gyro.data = data;
        // Populate later
    }

    void OnExposureData(const ins_camera::ExposureData &data) override
    {
//        frame.exposure.data = data;
        // Populate later
    }

private:
//    FILE *stream_file;
    int64_t last_timestamp = 0;
};

class CameraController {
    std::shared_ptr<ins_camera::Camera> camera;
public:
    std::shared_ptr<StreamDelegate> streamDelegate;
    bool streaming;                 // Whether camera is streaming or not. false is no stream. true is streaming
    bool connected;                 // Whether camera is connected to or not. false is no stream. true is streaming.

    CameraController() {
        open();
        streamDelegate = std::make_shared<StreamDelegate>();
        streaming = false;
        connected = false;
        setup();
    }

// Force it to not cast
// Create some documentation on this controller

    ~CameraController() {
        // Always close camera connection after handler is destroyed
        close();
    }

    void open() {
        if (connected) {
           return;
        }

        ins_camera::DeviceDiscovery discovery;
        auto list = discovery.GetAvailableDevices();
        if (list.empty())
        {
            std::cout << "No cameras found!" << std::endl;
            throw "No cameras to be found";
        }

        std::shared_ptr<ins_camera::Camera> cam = std::make_shared<ins_camera::Camera>(list[0].info);
        if (!cam->Open())
        {
            cam->Close();
            connected = false;
            throw "Could not open camera";
        }
        connected = true;
        camera = cam;
    }

    void close() {
        if (!connected) {
            return;
        }
        camera->Close();
        connected = false;
    }
private:
    void setup() {
        // In the future, add a setup file reader

        // Sync time
        if (camera) {

        }
        auto start = time(NULL);
        camera->SyncLocalTimeToCamera(start);

        // Set camera recording settings
        auto exposure = std::make_shared<ins_camera::ExposureSettings>();
        exposure->SetExposureMode(ins_camera::PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_AUTO);
        bool success = camera->SetExposureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, exposure);
        if (!success)
        {
            throw "Failed to set exposure settings";
//            std::cout << "Failed to set exposure settings" << std::endl;
        }
//        std::cout << "Exposure settings set" << std::endl;

        // Set stream delegate
        std::shared_ptr<ins_camera::StreamDelegate> stream = std::dynamic_pointer_cast<ins_camera::StreamDelegate>(streamDelegate);
        camera->SetStreamDelegate(stream);
//        camera->SetStreamDelegate(::make_shared<ins_camera::StreamDelegate>(internalStreamDelegate));
//        camera->SetStreamDelegate(std::make_shared<ins_camera::StreamDelegate>(streamDelegate));
    }
public:
    void startStream(){
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

    void stopStream() {
        if (!streaming) {
            return;
        }
        if(!camera->StopLiveStreaming()) {
            throw "Failed to stop stream";
        }
        streaming = false;
    }

    void printFrame() {
//        cout << "hello" << endl;
//        cout << "Video size: " << streamDelegate.frame.video.size << endl;
//        cout << "Audio size: " <<  streamDelegate.frame.audio.size << endl;
//        cout << "Gyro data: " << streamDelegate.frame.gyro << endl;
    }

//    Frame returnFrame() {
//        return streamDelegate.frame;
//    }
};


#endif //INSTA_PLUS_PLUS_INSTA_H
