# image_search

Image search engine based on CBIR (Content Based Image Retrieval)

# build

Project supports CMake Build system

    $ mkdir build
    $ cd build
    $ cmake ../


# usage

To populate Indexing file.

    $ ./index --dataset ../dataset --index ../index.csv

To query image to search

    $ ./search --query ../query/12310.png --result-path ../dataset --index ../index.csv
