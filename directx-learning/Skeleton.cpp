#include "Skeleton.h"
#include <fstream>
#include <assert.h>
#include "Logger.h"
#include <sstream>
Skeleton::Skeleton(std::string skeletonName) {
    std::vector<unsigned char> rawBytes;
    std::ifstream infile;

    infile.open(skeletonName, std::ios::in | std::ios::binary);
    assert(infile.is_open());

    // Convert the file stream to a byte vector
    infile.seekg(0, std::ios::end);
    int bytesSize = infile.tellg();
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

        std::stringstream childStream;
        for (int c = 0; c < MAX_JOINT_CHILDREN; c++) {
            if (joint.childIndices_[c] != -1)
                childStream << "Child: " << std::to_string(joint.childIndices_[c]) << '\n';
        }
        DEBUGLOG("Joint " + std::to_string(i) + ": \n" + joint.localTransform_.ToString() + childStream.str());
    }
}