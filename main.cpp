// user headers
#include "utils.hpp"

// lib headers
#include <opencv2/opencv.hpp>

// system headers
#include <iostream>
#include <string>
#include <vector>


std::vector<cv::Point> getROIVertices(const cv::Mat& img)
{
    int height = img.rows;
    int width = img.cols;
    return {
        cv::Point(0, height),
        cv::Point(width / 2, height / 2),
        cv::Point(width, height)
    };
}

cv::Mat regionOfInterest(const cv::Mat& img, const std::vector<cv::Point>& vertices) {
    cv::Mat mask = cv::Mat::zeros(img.size(), img.type());
    cv::Scalar match_mask_color = cv::Scalar(255, 255, 255); // White color for the mask

    // Fill the region of interest polygon with white color
    std::vector<std::vector<cv::Point>> pts = {vertices};
    cv::fillPoly(mask, pts, match_mask_color);

    // Bitwise AND operation between the input image and the mask to get the masked image
    cv::Mat masked_image;
    cv::bitwise_and(img, mask, masked_image);

    return masked_image;
}

int main(int argc, char** argv)
{
    // Read image 
    std::string laneImgPath{getResourcePath("driving-lane.jpg", IMAGE)};
    cv::Mat img = cv::imread( laneImgPath, cv::IMREAD_COLOR );

    // Gray scale and Canny image
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::Mat imgCanny;
    cv::Canny( imgGray, imgCanny, 100, 200 );

    // Get region of interest 
    auto ROIvertices = getROIVertices(img);
    const auto croppedImage = regionOfInterest(imgCanny, ROIvertices);

    cv::imshow(" Outline lanes ", croppedImage);
    auto key = cv::waitKey(0);
}

