// Quick test of the device discovery API. Pulled from Insta360 SDK README
// https://github.com/Insta360Develop/CameraSDK-Cpp#discover-your-camera

#include "camera/device_discovery.h"
#include "camera/camera.h"
#include <iostream>

int main()
{
    // Device discovery
    ins_camera::DeviceDiscovery discovery;

    auto list = discovery.GetAvailableDevices();
    for (int i = 0; i < list.size(); ++i)
    {
        std::cout << "Device " << i << ": " << list[i].serial_number << std::endl;
    }

    // Open camera
    /*
    ins_camera::Camera cam(list[0].info);
    if (!cam.Open())
    {
        std::cout << "Failed to open camera" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Camera opened" << std::endl;
    }
    cam.Close();
    */

   // Take photo
   ins_camera::Camera cam(list[0].info);

    const auto url = cam.TakePhoto();
    
    std::cout << "Camera status: " << cam.Open() << std::endl;
    std::cout << "URL is: " << std::endl;

    if (url.Empty()) {
        std::cout << "Failed to take picture" << std::endl;
        return -1;
    }

    std::cout << "Picture taken: " <<url.GetSingleOrigin() << std::endl;
    cam.Close();
}