//
// Created by kevinhy on 8/3/23.
//

#include "camcontrol/CameraController.h"
#include <chrono>
#include <thread>

int main() {
    CameraController controller = CameraController();
    controller.startStream();
    for (int i; i < 100; i++ ){
        controller.returnFrame();
//        cout << "Video size: " << controller.streamDelegate.frame.video.size << endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }
//    for (int i; i < 1000; i++) {
//        controller.returnFrame();
//        cout << "--Internal--" << endl;
//        controller.printFrame();
//        cout << "--External--" << endl;
////        Frame frame = controller.returnFrame();
////        cout << "Video size: " << frame.video.size << endl;
////        cout << "Audio size: " << frame.audio.size << endl;
//        std::this_thread::sleep_for(std::chrono::nanoseconds (10));
//
//    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
//
    controller.stopStream();
    controller.close();
}