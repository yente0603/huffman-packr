#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <map>
#include "../include/tree.hpp"
namespace huffman
{
    extern std::map<unsigned char, std::string> freqTable;
    extern std::map<std::string, unsigned char> reverseFreqTable;

    void buildEncodingTable(huffmanNode *node, const std::string &currentCode);
    void writeFrequencyTable();
    void readFrequencyTable();
}
#endif