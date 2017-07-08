#ifndef __SEARCHER_HPP__
#define __SEARCHER_HPP__

#include "common.hpp"

class Searcher
{   
    private:
        std::string indexPath;
        double chi2_distance(cv::Mat& histA, cv::Mat& histB);
    public:
        Searcher(std::string indexPath_);
        std::vector<std::pair<double, int> > search(std::vector<float> queryFeatures, int limit = 10);
};


#endif