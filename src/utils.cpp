#include "../include/utils.hpp"
#include "../include/config.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <string>
#include <stdexcept>

#if __cplusplus >= 201703L
#include <filesystem>
#endif

namespace huffman
{
    std::pair<std::vector<unsigned char>, std::vector<unsigned>> calFreq()
    {
        // Compute the frequency of each character, where each character is stored in a byte.
        std::vector<unsigned> freqMap(256, 0);

        std::ifstream file(YOUR_INPUT_PATH, std::ios::binary);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open input file for frequency calculation: ") + YOUR_INPUT_PATH);

        int byte;
        while ((byte = file.get()) != EOF)
            freqMap[static_cast<unsigned char>(byte)]++;
        file.close();

        std::vector<unsigned char> data;
        std::vector<unsigned> freq;

        for (int i = 0; i < 256; ++i)
        {
            if (freqMap[i] > 0)
            {
                data.push_back(i);
                freq.push_back(freqMap[i]);
            }
        }

        return std::make_pair(data, freq);
    }
    void writeExten()
    {
        std::ofstream file(DEFAULT_EXTEN_PATH, std::ios::out);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open extension log file for writing: ") + DEFAULT_EXTEN_PATH);

        // Use std::filesystem for more robust path handling (requires C++17)
        // If using older C++, you'll need manual string manipulation.
#if __cplusplus >= 201703L
        std::filesystem::path p(YOUR_INPUT_PATH);
        file << p.extension().string(); // Includes the dot (e.g., ".txt")
#else
        // Fallback for older C++ standards
        size_t pos = YOUR_INPUT_PATH.find_last_of('.');
        if (pos != std::string::npos)
            file << YOUR_INPUT_PATH.substr(pos);
        else
            file << "";
#endif
        file.close();
    }
    std::string getExten()
    {
        std::ifstream file(DEFAULT_EXTEN_PATH, std::ios::in);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open extension log file for reading: ") + DEFAULT_EXTEN_PATH);

        std::string extenName;
        getline(file, extenName); // Read the rest of the line to clear the buffer
        file.close();
        return extenName;
    }
    void writeBaseName()
    {
        std::ofstream file(DEFAULT_BASENAME_PATH, std::ios::out);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open basename log file for writing: ") + DEFAULT_BASENAME_PATH);

#if __cplusplus >= 201703L
        std::filesystem::path p(YOUR_INPUT_PATH);
        file << p.stem().string();
#else
        size_t lastSlash = YOUR_INPUT_PATH.find_last_of("/\\");
        std::string fileName = (lastSlash == std::string::npos) ? YOUR_INPUT_PATH : YOUR_INPUT_PATH.substr(lastSlash + 1);
        size_t pos = fileName.find_last_of('.');
        if (pos != std::string::npos)
            file << fileName.substr(0, pos);
        else
            file << fileName; // File has no extension
#endif
        file.close();
    }
    std::string getBaseName()
    {
        std::ifstream file(DEFAULT_BASENAME_PATH, std::ios::in);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open basename log file for reading: ") + DEFAULT_BASENAME_PATH);

        std::string baseName;
        getline(file, baseName); // Read the rest of the line to clear the buffer
        file.close();
        return baseName;
    }
    long long getFileSize(const std::string &fileName)
    {
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open file to get size: ") + fileName);

        std::streampos fileSizePos = file.tellg();
        file.close();

        // Check if tellg failed (often returns -1 on failure)
        if (fileSizePos == std::streampos(-1))
        {
            std::cerr << "Error getting file size for: " << fileName << std::endl;
            std::exit(EXIT_FAILURE); // Exit if unable to get size
        }

        // Convert std::streampos to long long.
        // This cast is safe as long as the streampos value fits within long long,
        // which is true for files sizes up to the maximum value of long long.
        // On most modern systems, long long is 64-bit, offering a huge range.
        long long fileSize = static_cast<long long>(fileSizePos);
        return fileSize;
    }

    void writeRatio()
    {
        std::ofstream file(DEFAULT_RATIO_PATH, std::ios::out);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open ratio log file for writing: ") + DEFAULT_RATIO_PATH);

        long long originalFileSize = getFileSize(YOUR_INPUT_PATH),
                  compressedFileSize = getFileSize(YOUR_INPUT_COMPRESSED_PATH);
        double compressionRatio = 0.0;

        if (originalFileSize > 0)
            compressionRatio = 100.0 * (1.0 - static_cast<double>(compressedFileSize) / originalFileSize);
        else
            std::cerr << "Warning: Original file size is 0. Compression ratio is not meaningful." << std::endl;

        file << "Orginal File Size: " << originalFileSize << " bytes." << std::endl
             << "Compressed File Size: " << compressedFileSize << " bytes." << std::endl
             << "Compression Ratio: " << std::fixed << std::setprecision(2) << compressionRatio << "%" << std::endl;
        file.close();
    }
    std::string getRatio()
    {
        std::ifstream file(DEFAULT_RATIO_PATH, std::ios::in);
        if (!file)
            throw std::runtime_error(std::string("Unable to open ratio log file. Has compression been performed yet? File: ") + DEFAULT_RATIO_PATH);

        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("Compression Ratio: ") != std::string::npos)
            {
                return line;
            }
        }
        throw std::runtime_error(std::string("Compression ratio not found in log file: ") + DEFAULT_RATIO_PATH);
    }
}