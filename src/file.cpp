#include "../include/file.hpp"
#include "../include/config.hpp"
#include "../include/table.hpp"
#include "../include/utils.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

#if __cplusplus >= 201703L
#include <filesystem> // C++17 filesystem
#else
#include <algorithm> // For std::max in C++11 path helpers
#endif

namespace huffman
{
    void compressFile()
    {
        // 檢查 YOUR_INPUT_COMPRESSED_PATH 是否仍然是其預設初始值 DEFAULT_COMPRESSED_PATH
        if (YOUR_INPUT_COMPRESSED_PATH == "")
        {
            try
            {
                std::filesystem::path p = std::filesystem::path("output") / (std::filesystem::path(YOUR_INPUT_PATH).stem().string() + ".huff");
                YOUR_INPUT_COMPRESSED_PATH = p.string();
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                std::cerr << "Filesystem error during default path generation: " << e.what() << std::endl;
                std::exit(EXIT_FAILURE); // 文件系統錯誤是致命的
            }
            catch (const std::exception &e)
            {
                std::cerr << "An error occurred during default path generation: " << e.what() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
        else
        {
            // YOUR_INPUT_COMPRESSED_PATH has been assigned
        }

        std::ifstream inputFile(YOUR_INPUT_PATH, std::ios::binary);
        std::ofstream outputFile(YOUR_INPUT_COMPRESSED_PATH, std::ios::binary);

        if (!inputFile)
        {
            std::cerr << "Unable to open file: " << YOUR_INPUT_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if (!outputFile)
        {
            std::cerr << "Unable to create file: " << YOUR_INPUT_COMPRESSED_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (freqTable.empty())
        {
            std::cerr << "Input file was empty. Creating empty compressed file and zero padding log." << std::endl;
            // Create empty compressed file (already done by opening)
            // Write zero padding log as 0
            std::ofstream file(DEFAULT_ZEROPADDING_PATH, std::ios::out);
            if (!file)
            {
                std::cerr << "Unable to open file: " << DEFAULT_ZEROPADDING_PATH << std::endl;
                std::exit(EXIT_FAILURE);
            }
            file << 0; // 0 padding for empty file
            file.close();

            // Write extension file (might be empty ext)
            writeExten();

            inputFile.close();
            outputFile.close();
            std::cout << "Compress Done (Empty file)." << std::endl
                      << "Default compress path: " << YOUR_INPUT_COMPRESSED_PATH << std::endl;
            return;
        }

        std::bitset<8> bits;
        std::string buf;
        int byte;
        while ((byte = inputFile.get()) != EOF)
        {
            unsigned char ch = static_cast<unsigned char>(byte);
            buf += freqTable[ch];
            while (buf.size() >= 8)
            {
                bits = std::bitset<8>(buf.substr(0, 8)); // Get the first 8 bits
                unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
                outputFile.write(reinterpret_cast<char *>(&byte), sizeof(byte));
                buf.erase(0, 8);
            }
        }

        int zeroPadding = static_cast<int>((8 - buf.size()) % 8);
        if (zeroPadding > 0)
            buf.append(zeroPadding, '0');
        if (!buf.empty())
        {
            bits = std::bitset<8>(buf);
            unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
            outputFile.write(reinterpret_cast<char *>(&byte), sizeof(byte));
        }

        inputFile.close();
        outputFile.close();

        std::ofstream file(DEFAULT_ZEROPADDING_PATH, std::ios::out);
        if (!file)
        {
            std::cerr << "Unable to open file: " << DEFAULT_ZEROPADDING_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }
        file << zeroPadding;
        file.close();

        writeExten();
        std::cout << "Compress Done." << std::endl
                  << "Default compress path: " << YOUR_INPUT_COMPRESSED_PATH << std::endl;
    }

    void decompressFile()
    {
        readFrequencyTable();

        std::ifstream inputFile(YOUR_INPUT_COMPRESSED_PATH, std::ios::binary);
        std::ifstream zeroPaddingFile(DEFAULT_ZEROPADDING_PATH);
        std::ofstream outputFile(YOUR_INPUT_DECOMPRESSED_PATH, std::ios::out | std::ios::binary);

        if (!inputFile)
        {
            std::cerr << "Unable to open file: " << YOUR_INPUT_COMPRESSED_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if (!zeroPaddingFile)
        {
            std::cerr << "Unable to open file: " << DEFAULT_ZEROPADDING_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if (!outputFile)
        {
            std::cerr << "Unable to create file: " << YOUR_INPUT_DECOMPRESSED_PATH << std::endl;
            std::exit(EXIT_FAILURE);
        }

        int zeroPadding = 0;
        zeroPaddingFile >> zeroPadding;
        zeroPaddingFile.close();

        // Get total size of file
        inputFile.seekg(0, std::ios::end);
        std::streampos fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        std::string code;
        std::size_t bytesRead = 0;
        char byte;

        while (inputFile.read(&byte, sizeof(byte)))
        {
            ++bytesRead;
            int bitsToRead = 8;
            if (bytesRead == fileSize)
            {
                bitsToRead = 8 - zeroPadding;
            }

            for (int i = 7; i >= 8 - bitsToRead; --i)
            {
                code += ((byte >> i) & 1) ? '1' : '0';

                auto it = reverseFreqTable.find(code);
                if (it != reverseFreqTable.end())
                {
                    outputFile.put(static_cast<char>(it->second));
                    code.clear();
                }
            }
        }

        inputFile.close();
        outputFile.close();
        std::cout << "Decompress Done." << std::endl;
    }
}