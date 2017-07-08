#include "ColorDescriptor.hpp"
#include "Searcher.hpp"
#include "common.hpp"



int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return -1;
    }
    char *index = NULL;
    char *query = NULL;
    char *result_path = NULL;
    for (int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "--index"))
            index = argv[i + 1];
        if (!strcmp(argv[i], "--query"))
            query = argv[i + 1];
        if (!strcmp(argv[i], "--result-path"))
            result_path = argv[i + 1];
    }



    // initialize the image descriptor
    int bins[3] = {8,12,3};
    ColorDescriptor cd(bins);
    
    //load the query image and describe it
    cv::Mat query_image = cv::imread(query);
    std::vector<float> features = cd.describe(query_image);

    // perform the search
    Searcher searcher(index);
    std::vector<std::pair<double, int> > results = searcher.search(features);

    // display the query
    cv::imshow("Query", query_image);

    // loop over the results
    // show content:
    for (int i = 0; i < results.size(); i++)
    {
        int imageID = (int) results[i].second;
        double score = (double) results[i].first;
        std::string resultPath = std::string(result_path) + "/" + std::to_string(imageID) + ".png";
        std::cout << resultPath << std::endl;
        cv::Mat result = cv::imread(resultPath);
        cv::imshow("Result", result);
        cv::waitKey(0);
    }

    return 0;
}