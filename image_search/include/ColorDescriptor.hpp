#ifndef __COLORDESCRIPTOR_HPP__
#define __COLORDESCRIPTOR_HPP__

#include "common.hpp"

class ColorDescriptor
{
    private:
        int bins[3];

    public:
        ColorDescriptor(int bins[3]);
        std::vector<float> describe(cv::Mat image);
        std::vector<float> histogram(cv::Mat hsv_image, cv::Mat cornerMask);
};
#endif