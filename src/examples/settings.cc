// Compile command from inside OneRS folder
// g++ src/examples/settings.cc -o ./bin/settings -I src/include/ -Lsrc/lib -lCameraSDK -ludev

// Run command
// sudo ./bin/settings

#include <camera/camera.h>
#include <camera/device_discovery.h>
#include <iostream>

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

    // Setting the exposure
    auto exposure = std::make_shared<ins_camera::ExposureSettings>();
    exposure->SetExposureMode(ins_camera::PhotographyOptions_ExposureMode::PhotographyOptions_ExposureOptions_Program_MANUAL);
    exposure->SetIso(400);
    exposure->SetShutterSpeed(1.0 / 120.0); // Set shutter speed to 60hz
    bool success = cam.SetExposureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, exposure);
    if (!success)
    {
        std::cout << "Failed to set exposure settings" << std::endl;
    }
    std::cout << "Exposure settings set" << std::endl;

    // Setting capture settings
    auto settings = std::make_shared<ins_camera::CaptureSettings>();
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Saturation, 60);
    settings->SetWhiteBalance(ins_camera::PhotographyOptions_WhiteBalance_WB_AUTO);
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Brightness, 30);
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Sharpness, 4);
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Contrast, 100);

    auto ret = cam.SetCaptureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, settings);
    if (!success)
    {
        std::cout << "Failed to set camera settings" << std::endl;
        cam.Close();
        return -1;
    }
    std::cout << "Camera settings set" << std::endl;

    cam.Close();
    return 0;
}