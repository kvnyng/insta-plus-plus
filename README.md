# Getting Started

### Camera setup:
Put One RS into Android boot mode under settings and restart camera.

For other camera models, view [Insta360 documentation](https://github.com/Insta360Develop/CameraSDK-Cpp/tree/a2411ab551f252ed32ec34b711a293cfa45f2c57#setup-your-camera-connection-mode)

### Drivers
**Linux**

Install `libusb-dev` and `libudev`.

```bash
sudo apt-get install libusb-dev
sudo apt-get install libudev-dev
```

**Mac OS (Untested)**

Nothing needs to be done

**Windows (Untested)**
Install [libusbK](https://sourceforge.net/projects/libusbk/files/libusbK-release/3.0.7.0/)

### Validating Connection
Use the command `lsusb` and check for a device with the manufacturing ID `0x2e1a`. If a coorsponding device shows up, then the computer recognizes the camera. Otherwise, revisit driver installation or camera setup.
![lsusb command output](docs/images/lsusb_example.png)

### Dynamic Linking
Add the `CameraSDK.so` library to system's dynamic linker.
```bash
sudo ldconfig ${FULL PATH TO /lib FOLDER}
```

### Compiling Example
```bash
g++ example/main.cc -o exampleTest -I include/ -Llib/ -lCameraSDK -ludev
```

### Running Examples
All programs need to be run in `sudo` mode (because of IO?). Therefore, prepend `sudo` before running executables.
```bash
sudo ./exampleTest
sudo ./example/CameraSDKTest
```