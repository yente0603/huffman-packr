#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

namespace huffman
{
    std::pair<std::vector<unsigned char>, std::vector<unsigned>> calFreq();
    void writeExten();
    std::string getExten();
    long long getFileSize(const std::string &fileName);
    void writeRatio();
    std::string getRatio();
}
#endif
