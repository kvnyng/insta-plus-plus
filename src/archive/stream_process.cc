// Compile command from inside OneRS folder
// g++ src/examples/stream.cc -o ./bin/stream -I src/include/ -Lsrc/lib -lCameraSDK -ludev

// Execution command
// sudo ./bin/stream
#include <camera/camera.h>
#include <camera/device_discovery.h>
#include <iostream>

#include <chrono>
#include <thread>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstddef>
#include <bitset>

// [X] create a stream class
// [] set that stream instance on the camera
// [] begin a live stream, video will start pouring in.

class Streamer : public ins_camera::StreamDelegate
{
public:
    Streamer()
    {
        mkfifo(pipe_path, 0666);

        std::cout << "Opening pipe path @ " << pipe_path << std::endl;
        pipe = fopen(pipe_path, "wb");
        // stream_file = fopen("./data/video.h264", "wb");

    }
    ~Streamer()
    {
        // fclose(stream_file);
    }

    void OnAudioData(const uint8_t *data, size_t size, int64_t timestamp) override
    {
        // Currently does not save audio data
        // std::cout << "Audio data received!" << timestamp << std::endl;
    }

    void OnVideoData(const uint8_t *data, size_t size, int64_t timestamp, uint8_t streamType, int stream_index = 0) override
    {
         std::cout << "Writing Frame: " << size << " | " << timestamp << std::endl;
         fwrite(&sync, sizeof(uint8_t), sizeof(sync), pipe);
         fwrite(&sync, sizeof(uint8_t), sizeof(sync), pipe);
         fwrite(data, sizeof(uint8_t), sizeof(size), pipe);
        //  fwrite(&sync, sizeof(uint8_t), sizeof(sync), pipe);
        //  fwrite(&sync, sizeof(uint8_t), sizeof(sync), pipe);
        //  fwrite(data, sizeof(uint8_t), size, pipe);
        
        //  write(pipe, data, size); // Write data next
         std::cout << data << std::endl;
         // write(pipe, &sync_byte, sync_byte_length); // Write a sync char, in case just an /n
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
    const char* pipe_path = "/home/kevinhy/mit/pipe";
    const uint8_t sync_byte = 0x7E;
    const size_t sync_byte_length = sizeof(sync_byte);
    FILE* pipe;
    uint8_t sync = 0x7E;
    // FILE *file = stdout;
    int64_t last_timestamp = 0;
};

int main()
{
    std::cout << "Starting process" << std::endl;

    // Connecting
    ins_camera::DeviceDiscovery discovery;
    auto list = discovery.GetAvailableDevices();
    if (list.empty())
    {
        std::cerr << "No cameras found!" << std::endl;
        return -1;
    }

    std::shared_ptr<ins_camera::Camera> cam = std::make_shared<ins_camera::Camera>(list[0].info);
    if (!cam->Open())
    {
        std::cerr << "Failed to open up camera" << std::endl;
        cam->Close();
        return -1;
    }

    // Camera setup
    auto start = time(NULL);
    cam->SyncLocalTimeToCamera(start);

    // Set stream delegate
    std::shared_ptr<ins_camera::StreamDelegate> delegate = std::make_shared<Streamer>();
    cam->SetStreamDelegate(delegate);

    ins_camera::LiveStreamParam param;
    param.video_resolution = ins_camera::VideoResolution::RES_720_360P30;
    param.lrv_video_resulution = ins_camera::VideoResolution::RES_720_360P30;
    param.video_bitrate = 1024 * 1024 / 2;
    param.enable_audio = false;
    param.using_lrv = false;

    // Begin pipe
    std::cout << "Camera is ready" << std::endl;

    if (!cam->StartLiveStreaming(param))
    {
        std::cerr << "Failed to start live streaming" << std::endl;
    }

    // Start stream
    std::cout << "Started stream!" << std::endl;

    std::this_thread::sleep_for(std::chrono::nanoseconds(1));

    // Kill the stream
    if (!cam->StopLiveStreaming())
    {
        std::cerr << "Failed to stop stream" << std::endl;
    }
    std::cout << "Stopped stream!" << std::endl;

    cam->Close();
    return 0;
}