//
// Created by kevinhy on 8/3/23.
//

#include "CameraController.h"
#include <chrono>
#include <thread>

int main() {
    CameraController controller = CameraController();
    controller.startStream();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    controller.stopStream();
    controller.close();`
}