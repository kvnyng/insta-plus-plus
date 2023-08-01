// Compile command from inside OneRS folder
// ++ src/examples/stream.cc -o ./bin/stream -I src/include/ -Lsrc/lib -lCameraSDK -ludev

// Execution command
// sudo ./bin/stream
#include <camera/camera.h>
#include <camera/device_discovery.h>
#include <iostream>

#include <chrono>
#include <thread>

// create a stream class
// set that stream instance on the camera
// begin a live stream, video will start pouring in.

class Streamer : public ins_camera::StreamDelegate
{
public:
    Streamer()
    {
        stream_file = fopen("./data/video.h264", "wb");

    }
    ~Streamer()
    {
        fclose(stream_file);
    }

    void OnAudioData(const uint8_t *data, size_t size, int64_t timestamp) override
    {
        // Currently does not save audio data
        std::cout << "Audio data received!" << timestamp << std::endl;
    }

    void OnVideoData(const uint8_t *data, size_t size, int64_t timestamp, uint8_t streamType, int stream_index = 0) override
    {
        std::cout << "Frame: " << size << " | " << timestamp << std::endl;
        fwrite(data, sizeof(uint8_t), size, stream_file);
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
    FILE *stream_file;
    int64_t last_timestamp = 0;
};

int main()
{
    // Connecting
    ins_camera::DeviceDiscovery discovery;
    auto list = discovery.GetAvailableDevices();
    if (list.empty())
    {
        std::cout << "No cameras found!" << std::endl;
        return -1;
    }

    std::shared_ptr<ins_camera::Camera> cam = std::make_shared<ins_camera::Camera>(list[0].info);
    if (!cam->Open())
    {
        std::cout << "Failed to open up camera" << std::endl;
        cam->Close();
        return -1;
    }

    // Camera setup
    auto start = time(NULL);
    cam->SyncLocalTimeToCamera(start);

    // Camera recording settings
    auto exposure = std::make_shared<ins_camera::ExposureSettings>();
    exposure->SetExposureMode(ins_camera::PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_AUTO);
    bool success = cam->SetExposureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, exposure);
    if (!success)
    {
        std::cout << "Failed to set exposure settings" << std::endl;
    }
    std::cout << "Exposure settings set" << std::endl;

    
    // Set stream delegate
    std::shared_ptr<ins_camera::StreamDelegate> delegate = std::make_shared<Streamer>();
    cam->SetStreamDelegate(delegate);

    ins_camera::LiveStreamParam param;
    param.video_resolution = ins_camera::VideoResolution::RES_368_368P30;
    param.lrv_video_resulution = ins_camera::VideoResolution::RES_368_368P30;
    param.video_bitrate = 1024 * 1024 / 2;
    param.enable_audio = false;
    param.using_lrv = false;

    if (!cam->StartLiveStreaming(param))
    {
        std::cout << "Failed to start live streaming" << std::endl;
    }

    // Start stream
    std::cout << "Started stream!" << std::endl;

    // Stream for 2 secs
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Kill the stream
    if (!cam->StopLiveStreaming())
    {
        std::cout << "Failed to stop stream" << std::endl;
    }
    std::cout << "Stopped stream!" << std::endl;

    cam->Close();
    return 0;
}