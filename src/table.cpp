#include "../include/table.hpp"
#include "../include/tree.hpp"
#include "../include/config.hpp"
#include "../include/utils.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <string>

namespace huffman
{
    std::map<unsigned char, std::string> freqTable;
    std::map<std::string, unsigned char> reverseFreqTable;

    void buildEncodingTable(huffmanNode *node, const std::string &currentCode)
    {
        if (!node)
        {
            if (!currentCode.empty())
                std::cerr << "Warning: buildEncodingTable called with non-empty code on null node." << std::endl;
            return;
        }
        if (node->isLeaf)
        {
            // Special handling for single-character file to avoid empty code and general leaf assignment.
            freqTable[node->data] = currentCode.empty() ? "0" : currentCode;
            return;
        }
        buildEncodingTable(node->left, currentCode + "0");
        buildEncodingTable(node->right, currentCode + "1");
    }

    void writeFrequencyTable()
    {
        std::ofstream file(DEFAULT_FREQ_TABLE_PATH, std::ios::out);
        if (!file)
        {
            std::cerr << "Unable to open file: " << DEFAULT_FREQ_TABLE_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if (freqTable.empty())
            std::cerr << "Warning: freqTable is empty. Writing empty frequency file." << std::endl;

        for (auto &code : freqTable)
            // avoiding non-displayable text in ASCII turn the data type from char into int
            file << static_cast<int>(code.first) << ": " << code.second << "\n";
        file.close();
    }
    void readFrequencyTable()
    {
        std::ifstream file(DEFAULT_FREQ_TABLE_PATH, std::ios::in);
        if (!file)
        {
            std::cerr << "Unable to open frequency table: " << DEFAULT_FREQ_TABLE_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }

        reverseFreqTable.clear();

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
                continue;

            size_t delimPos = line.find(": ");
            if (delimPos == std::string::npos)
                continue;

            // Added basic error check for stoi
            try
            {
                int char_int = std::stoi(line.substr(0, delimPos));
                if (char_int < 0 || char_int > 255)
                {
                    std::cerr << "Warning: Invalid character integer value in frequency table: " << char_int << std::endl;
                    continue;
                }
                unsigned char ch = static_cast<unsigned char>(char_int);
                std::string code = line.substr(delimPos + 2);

                reverseFreqTable[code] = ch;
            }
            catch (const std::invalid_argument &ia)
            {
                std::cerr << "Warning: Invalid number format in frequency table: " << line.substr(0, delimPos) << std::endl;
            }
            catch (const std::out_of_range &oor)
            {
                std::cerr << "Warning: Number out of range in frequency table: " << line.substr(0, delimPos) << std::endl;
            }
        }
        file.close();

        // Check if reverseFreqTable is empty after reading
        if (reverseFreqTable.empty())
            std::cerr << "Warning: reverseFreqTable is empty after reading. This might indicate an empty frequency file or a problem." << std::endl;
    }
}