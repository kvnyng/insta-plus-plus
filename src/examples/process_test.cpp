//
// Created by kevinhy on 7/25/23.
//
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
    // Initiate program
    std::cout << "program connected" << std::endl;

    int input = 1;

    for (int i = 0; i < 1000; i++){
        std::cin >> input;
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        std::cout << "sending.." << input << std::endl;
}
}