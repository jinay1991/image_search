#include "ColorDescriptor.hpp"

ColorDescriptor::ColorDescriptor(int in_bins[3])
{
    memcpy((void*)bins, in_bins, sizeof(bins));
}
std::vector<float>
ColorDescriptor::describe(cv::Mat image)
{
    cv::Mat hsv;
    // convert the image to the HSV color space and initialize
    // the features used to quantify the image
    cv::cvtColor(image, hsv, CV_BGR2HSV);

    std::vector<float> features;

    // grab the dimensions and compute the center of the image
    int h = hsv.rows;
    int w = hsv.cols;
    int cX = w * 0.5;
    int cY = h * 0.5;

    // divide the image into four rectangles/segments (top-left,
    // top-right, bottom-right, bottom-left)
    std::vector<cv::Rect> segments;
    segments.push_back(cv::Rect_<int>(0, 0, cX, cY)); // TL
    segments.push_back(cv::Rect_<int>(0, cY, cX, cY)); // BL
    segments.push_back(cv::Rect_<int>(cX, 0, cX, cY)); // TR
    segments.push_back(cv::Rect_<int>(cX, cY, cX, cY));  // BR


    // construct an elliptical mask representing the center of the
    // image
    int axesX = (int)(w * 0.75) / 2;
    int axesY = (int)(h * 0.75) / 2;
    cv::Mat ellipMask = cv::Mat::zeros(h, w, CV_8UC1);
    cv::ellipse(ellipMask, cv::Point(cX, cY), cv::Point(axesX, axesY), 0, 0, 360, 255, -1);

    for (int i = 0; i < segments.size(); i++)
    {
        // construct a mask for each corner of the image, subtracting
        // the elliptical center from it
        cv::Point startPt = cv::Point(segments[i].x, segments[i].y);
        cv::Point endPt = cv::Point(segments[i].x + segments[i].width, segments[i].y + segments[i].height);
        cv::Mat cornerMask = cv::Mat::zeros(h, w, CV_8UC1);
        cv::rectangle(cornerMask, startPt, endPt, 255, -1);
        cv::subtract(cornerMask, ellipMask, cornerMask);

        // extract a color histogram from the image, then update the
        // feature vector
        std::vector<float> hist = histogram(hsv, cornerMask);
        for (int i = 0; i < hist.size(); i++)
            features.push_back(hist[i]);
    }
    return features;
}

std::vector<float>
ColorDescriptor::histogram(cv::Mat hsv_image, cv::Mat cornerMask)
{
    // extract a 3D color histogram from the masked region of the
    // image, using the supplied number of bins per channel; then
    // normalize the histogram
    cv::MatND hist;
    // const int histSize[] = {30, 32, 10};
    float hranges[] = {0, 180};
    float sranges[] = {0, 256};
    float vranges[] = {0, 256};
    const float *histRange[] = { hranges, sranges, vranges };
    int channels[] = {0, 1, 2};

    cv::calcHist(&hsv_image, 1, channels, cv::Mat(), hist, 3, bins, histRange, true, false);

    cv::normalize(hist, hist);

    // cv::Mat nhist = hist.reshape(0,1);
    // Create vector from matrix data (data with data copying)
    std::vector<float> histV;
    histV.assign((float*)hist.datastart, (float*)hist.dataend);
    return histV;
}
