#include "Searcher.hpp"
#include <fstream>

Searcher::Searcher(std::string indexPath_)
{
    indexPath = indexPath_;
}
std::vector<std::pair<double, int> >
Searcher::search(std::vector<float> queryFeatures, int limit)
{
    std::ifstream csv(indexPath);
    std::vector<std::pair<double, int> > results_;

    std::string line;
    while( std::getline(csv, line) )
    {
        std::vector<float> features;
        float value = 0.0f;
        std::istringstream ss(line);
        std::string item;
        std::getline(ss, item, ',');
        int imageId = atoi(item.c_str());
        while (std::getline(ss, item, ','))
        {
            features.push_back(atof(item.c_str()));
        }
        // With copying data
        cv::Mat M1 = cv::Mat(1, features.size(), CV_32FC1);
        memcpy(M1.data,features.data(),features.size()*sizeof(float));
        cv::Mat M2 = cv::Mat(1, queryFeatures.size(), CV_32FC1);
        memcpy(M2.data,queryFeatures.data(),queryFeatures.size()*sizeof(float));

        double d = chi2_distance(M1, M2);
        std::pair<double, int> res;
        res.first = d;
        res.second = imageId;
        results_.push_back(res);
    }

    std::sort(results_.begin(), results_.end());

    std::vector<std::pair<double, int> > result;

    for (int i = 0; i < results_.size() && i < limit; i++)
    {
        std::pair<double, int> res = results_[i];
        result.push_back(res);
    }
    return result;

}


double Searcher::chi2_distance(cv::Mat& histA, cv::Mat& histB)
{
    return cv::compareHist(histA, histB, CV_COMP_CHISQR);
}