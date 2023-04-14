#include "SkeletonResource.h"
#include <fstream>
#include <assert.h>

SkeletonResource::SkeletonResource(std::string skeletonName) {
    std::vector<unsigned char> rawBytes;
    std::ifstream infile;

    infile.open(skeletonName + ".jsk", std::ios::in | std::ios::binary);
    assert(infile.is_open());

    // Convert the file stream to a byte vector
    infile.seekg(0, std::ios::end);
    int bytesSize = int(infile.tellg());
    rawBytes.reserve(bytesSize);
    infile.seekg(0, std::ios::beg);
    std::copy((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>(), std::back_inserter(rawBytes));
    infile.close();

    int jointCount = (uint8_t)rawBytes.data()[0];

    Joint* jointDataStart = (Joint*)&rawBytes.data()[SKELETON_HEADER_SIZE];
    for (int i = 0; i < jointCount; i++) {
        Joint joint = {};
        joint = jointDataStart[i];

        joints_.push_back(joint);
    }
}
