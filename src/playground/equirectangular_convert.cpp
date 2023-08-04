//
// Created by kevinhy on 8/4/23.
//
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <iostream>
#include <../deps/OmniCV-Lib/omnicv/utils.hpp>

// Create display window
int height = 500;
int width = 500;

std::string WINDOW_NAME{"Viewer"};

int main() {
    cv::namedWindow(WINDOW_NAME, CV_WINDOW_NORMAL);

}