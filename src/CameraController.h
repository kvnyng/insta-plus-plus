//
// Created by kevinhy on 8/3/23.
//

#ifndef INSTA_PLUS_PLUS_CAMERA_CONTROLLER_H
#define INSTA_PLUS_PLUS_CAMERA_CONTROLLER_H

#include <camera/camera.h>
#include <camera/device_discovery.h>
#include <iostream>
#include <boost/signals2.hpp>
using namespace boost;

class StreamDelegate : public ins_camera::StreamDelegate
{
public:
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
        std::cout << "Audio data received!" << timestamp << std::endl;
    }

    void OnVideoData(const uint8_t *data, size_t size, int64_t timestamp, uint8_t streamType, int stream_index) override
    {
        std::cout << "Frame: " << size << " | " << timestamp << std::endl;
//        fwrite(data, sizeof(uint8_t), size, stream_file);
    }

    void OnGyroData(const std::vector<ins_camera::GyroData> &data) override
    {
        // Populate later
    }

    void OnExposureData(const ins_camera::ExposureData &data) override
    {
        // Populate later
    }

private:
//    FILE *stream_file;
    int64_t last_timestamp = 0;
};

class CameraController {
    std::shared_ptr<ins_camera::Camera> camera;
    std::shared_ptr<ins_camera::StreamDelegate> streamDelegate;
public:
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
        camera->SetStreamDelegate(streamDelegate);
    }

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


};


#endif //INSTA_PLUS_PLUS_INSTA_H
