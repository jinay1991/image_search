#include <glob.h>
#include "ColorDescriptor.hpp"
#include <fstream>

int main(int argc, char **argv)
{
    if (argc < 2)
        return -1;

    char *dataset = NULL;
    char *index = NULL;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp("--dataset", argv[i]))
            dataset = argv[i + 1];
        if (!strcmp("--index", argv[i]))
            index = argv[i + 1];
    }

    int bins[] = { 8, 12, 3 };
    ColorDescriptor cd(bins);

    std::ofstream output(index);

    glob_t globbuf;
    std::string pattern = std::string(dataset) + "/*.png";
    int err = glob(pattern.c_str(), 0, NULL, &globbuf);
    if (err == 0)
    {
        for (int i = 0; i < globbuf.gl_pathc; i++)
        {
            std::string imagePath(globbuf.gl_pathv[i]);

            //extract the image ID (i.e. unique filename) from the image
            //path and load image itself.
            std::string imageName = imagePath.substr(imagePath.rfind("/") + 1);
            std::string imageID = imageName.substr(0, imageName.find_last_of("."));
            cv::Mat image = cv::imread(imagePath);

            // describe the image
            std::vector<float> features = cd.describe(image);

            // write the features to file
            std::string buf = imageID;
            for (int i = 0; i < features.size(); i++)
            {
                buf = buf + "," + std::to_string(features[i]);
            }
            buf = buf + "\n";
            output << buf;
        }
        globfree(&globbuf);
    }

    return 0;
}
