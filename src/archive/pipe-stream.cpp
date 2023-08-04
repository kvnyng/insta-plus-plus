//
// Created by kevinhy on 7/26/23.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    int fd;

    // FIFO file path
    char* pipe_path = "/home/kevinhy/mit/pipe";

    mkfifo(pipe_path, 0666); // Search up other modes

    std::cout << "Created pipe @ " << pipe_path << std::endl;

    uint8_t package [6] = {1,2,3,4,5,6};
    const size_t package_length = sizeof (package);

    std::cout<< "Entering while loop" << std::endl;
    const uint8_t sync_byte = 0x7E;
    const size_t sync_byte_length = sizeof(sync_byte);

    while (1){
        fd = open(pipe_path, O_WRONLY);

        std::cout << "Writing message" << std::endl;
        write(fd, package, package_length);
        std::cout << "Writing sync byte" << std::endl;
        write(fd, &sync_byte, sync_byte_length);
        std::this_thread::sleep_for(std::chrono::nanoseconds (1000));
    }
}