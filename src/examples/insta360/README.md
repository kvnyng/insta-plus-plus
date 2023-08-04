# Examples
Welcome to insta360 examples! Here are some code snippets that get at core aspects of the OneRS functionality.

## `cameraSDKTest.cc`
This is the test file that ships out of the box with the SDK. It's a CLI that allows you to perform numerous test (set exposure, camera parameters) as well as stream, download photos, etc.

This is a good place to see the API in action.

## `recording.cc`

## `settings.cc`
Set exposure, shutter speed, brightness, and other various settings on the camera. Updates are made by updating a pointer (very similar to Python).

```c++
    // Setting capture settings
    auto settings = std::make_shared<ins_camera::CaptureSettings>();
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Saturation, 60);
    settings->SetWhiteBalance(ins_camera::PhotographyOptions_WhiteBalance_WB_AUTO);
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Brightness, 30);
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Sharpness, 4);
    settings->SetValue(ins_camera::CaptureSettings::CaptureSettings_Contrast, 100);

    auto ret = cam.SetCaptureSettings(ins_camera::CameraFunctionMode::FUNCTION_MODE_NORMAL_VIDEO, settings)
    ```
