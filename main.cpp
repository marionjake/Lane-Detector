// user headers
#include "utils.hpp"

// lib headers
#include <opencv2/opencv.hpp>

// system headers
#include <iostream>
#include <string>
#include <vector>
#include <cmath>


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

cv::Mat getLineSegments(cv::Mat& img)
{
    cv::Mat linedImg;
    cv::HoughLinesP(
        img,
        linedImg,
        6,
        M_PI/60,
        160,
        40.0,
        25.0
    );

    if(linedImg.empty()) {
        std::cerr << "Could not get line segments via Hough Transform\n";
    }
    return linedImg;
}

cv::Mat drawLines(cv::Mat img, cv::Mat lines, cv::Scalar color = cv::Scalar(0, 0, 255), int thickness =6) {
    cv::Mat line_img = cv::Mat::zeros(img.size(), CV_8UC3);

    if (lines.empty())
        return img;

    lines = lines.reshape(1, lines.rows / 4);

    for (int i = 0; i < lines.rows; ++i) {
        int x1 = lines.at<int>(i, 0);
        int y1 = lines.at<int>(i, 1);
        int x2 = lines.at<int>(i, 2);
        int y2 = lines.at<int>(i, 3);

        cv::line(line_img, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
    }

    cv::addWeighted(img, 0.8, line_img, 4.0, 0.0, img);

    return img;
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
    auto croppedImage = regionOfInterest(imgCanny, ROIvertices);

    //get line segments via Hough Transform 
    auto linedImg = getLineSegments(croppedImage);

    auto lineDrawn = drawLines(img, linedImg, cv::Scalar(255, 0, 0));

    cv::imshow(" Outline lanes ", lineDrawn);
    auto key = cv::waitKey(0);
}

