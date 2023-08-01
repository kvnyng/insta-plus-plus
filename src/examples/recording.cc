// Example that will start a recording on the camera, and stop after 5 secs

#include <camera/device_discovery.h>
#include <camera/camera.h>
#include <iostream>

#include <chrono>
#include <thread>

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

    ins_camera::Camera cam(list[0].info); // Defines camera object
    if (!cam.Open())
    {
        std::cout << "Failed to open up camera" << std::endl;
        cam.Close();
        return -1;
    }

    // Start recording
    if (!cam.SetVideoCaptureParams({ins_camera::VideoResolution::RES_3840_2160P30, 1024 * 1024 * 60}))
    {
        std::cout << "Failed to set recording params" << std::endl;
    }
    if (!cam.StartRecording())
    {
        std::cout << "Failed to begin recording" << std::endl;
    }

    std::cout << "Recording started!" << std::endl;

    // Take a 5 second recording
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop recording
    auto url = cam.StopRecording();
    if (url.Empty()) {
        std::cout << "Stopping recording failed" << std::endl;
        cam.Close();
        return -1;
    }
    auto& origins = url.OriginUrls();
    std::cout << "Stop recording success" << std::endl;
    for (auto& origin_url: origins) {
        std::cout << "URL: " << origin_url << std::endl;
    }

    cam.Close();
    return 0;
}