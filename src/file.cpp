#include "../include/file.hpp"
#include "../include/config.hpp"
#include "../include/table.hpp"
#include "../include/utils.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <stdexcept>

#if __cplusplus >= 201703L
#include <filesystem>
#endif

// Original bit handling using `std::bitset` in compressFile()
// #include <bitset>

namespace huffman
{
    void compressFile()
    {
        initializeTree();
        writeFrequencyTable();

        if (YOUR_INPUT_COMPRESSED_PATH == "")
        {
#if __cplusplus >= 201703L
            try
            {
                std::filesystem::path p = std::filesystem::path("output") / (std::filesystem::path(YOUR_INPUT_PATH).stem().string() + ".huff");
                YOUR_INPUT_COMPRESSED_PATH = p.string();
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                // std::cerr << "Filesystem error during default path generation: " << e.what() << std::endl;
                throw std::runtime_error(std::string("Filesystem error during default path generation: ") + e.what());
            }
            catch (const std::exception &e)
            {
                // std::cerr << "An error occurred during default path generation: " << e.what() << std::endl;
                throw std::runtime_error(std::string("An error occurred during default path generation: ") + e.what());
            }
        }
#else
            // C++11 Fallback: Manual string manipulation for path construction
            size_t lastSlash = YOUR_INPUT_PATH.find_last_of("/\\");
            std::string fileName = (lastSlash == std::string::npos) ? YOUR_INPUT_PATH : YOUR_INPUT_PATH.substr(lastSlash + 1);
            size_t pos = fileName.find_last_of('.');
            std::string stem = (pos == std::string::npos) ? fileName : fileName.substr(0, pos);
            YOUR_INPUT_COMPRESSED_PATH = std::string("output/") + stem + ".huff";
#endif
        else
        {
            // YOUR_INPUT_COMPRESSED_PATH has been assigned
        }

        std::ifstream inputFile(YOUR_INPUT_PATH, std::ios::binary);
        std::ofstream outputFile(YOUR_INPUT_COMPRESSED_PATH, std::ios::binary);

        if (!inputFile)
            throw std::ios_base::failure(std::string("Unable to open input file for compression: ") + YOUR_INPUT_PATH);

        if (!outputFile)
            throw std::ios_base::failure(std::string("Unable to create output file for compression: ") + YOUR_INPUT_COMPRESSED_PATH);

        writeExten();
        writeBaseName();

        if (freqTable.empty())
        {
            std::cerr << "Input file was empty. Creating empty compressed file and zero padding log." << std::endl;
            // Create empty compressed file (already done by opening)
            // Write zero padding log as 0
            std::ofstream file(DEFAULT_ZEROPADDING_PATH, std::ios::out);
            if (!file)
                throw std::ios_base::failure(std::string("Unable to open zero padding log file: ") + DEFAULT_ZEROPADDING_PATH);

            file << 0; // 0 padding for empty file
            file.close();

            inputFile.close();
            outputFile.close();

            return;
        }

        /* --- Original Bit Handling using std::string and std::bitset (Commented Out) ---
         * This section of code handles reading the input file byte by byte,
         * getting the Huffman code for each byte from freqTable,
         * accumulating the bits in a std::string buffer 'buf',
         * and writing out complete bytes (8 bits) to the output file
         * using std::bitset for conversion.
         *
         * This approach is generally easier to understand conceptually but
         * can be less performant for large files due to frequent string
         * concatenations, subtractions, and bitset conversions, involving
         * dynamic memory allocation and copying.
         *
         * The code below is replaced by a more direct byte and bit manipulation
         * approach for potentially better performance.
         */

        /*
        std::bitset<8> bits;
        std::string buf;
        int byte;
        while ((byte = inputFile.get()) != EOF)
        {
            unsigned char ch = static_cast<unsigned char>(byte);
            // Ensure the character exists in the frequency table before accessing it
            if (freqTable.count(ch))
                buf += freqTable[ch];
            else
                // This case *shouldn't* happen if initializeTree and calFreq work correctly
                // for non-empty files, but as a safeguard:
                throw std::runtime_error(std::string("Character with no Huffman code encountered during compression: ") + std::to_string(static_cast<int>(ch)));

            while (buf.size() >= 8)
            {
                if (!outputFile.is_open())
                    throw std::ios_base::failure(std::string("Output file stream is no longer open for writing during compression: ") + YOUR_INPUT_COMPRESSED_PATH);
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
        */

        /*
         * --- New, More Efficient Bit Handling ---
         * This approach directly manipulates bytes and bits using bitwise operations.
         * It avoids the overhead of string operations and bitset conversions for each chunk of bits.
         * It maintains a current output byte and a count of bits already placed in it.
         */
        unsigned char currentByte = 0;
        int bitsInBuffer = 0;

        int byte;
        while ((byte = inputFile.get()) != EOF)
        {
            unsigned char ch = static_cast<unsigned char>(byte);
            std::string code = freqTable[ch];

            for (char bitChar : code)
            {
                currentByte <<= 1;

                if (bitChar == '1')
                    currentByte |= 1;

                bitsInBuffer++;

                if (bitsInBuffer == 8)
                {
                    outputFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
                    currentByte = 0, bitsInBuffer = 0;
                }
            }
        }

        int zeroPadding = 0;
        if (bitsInBuffer > 0)
        {
            zeroPadding = 8 - bitsInBuffer;
            currentByte <<= zeroPadding;
            outputFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
        }
        inputFile.close();
        outputFile.close();

        std::ofstream file(DEFAULT_ZEROPADDING_PATH, std::ios::out);
        if (!file)
            throw std::ios_base::failure(std::string("Unable to open zero padding log file for writing: ") + DEFAULT_ZEROPADDING_PATH);

        file << zeroPadding;
        file.close();

        writeRatio();
    }

    void decompressFile()
    {
        readFrequencyTable();

        std::ifstream zeroPaddingFile(DEFAULT_ZEROPADDING_PATH);
        int zeroPadding = 0;
        if (!zeroPaddingFile)
            throw std::ios_base::failure(std::string("Unable to open zero padding log file for reading: ") + DEFAULT_ZEROPADDING_PATH);

        zeroPaddingFile >> zeroPadding;
        zeroPaddingFile.close();

        if (YOUR_INPUT_DECOMPRESSED_PATH == "")
        {
            std::string baseName = getBaseName();
            std::string exten = getExten();
            if (baseName.empty())
            {
                if (baseName.empty() && std::filesystem::file_size(DEFAULT_BASENAME_PATH) == 0)
                    throw std::runtime_error(std::string("Failed to retrieve original basename (basename file is empty or corrupt): ") + DEFAULT_BASENAME_PATH);
                if (baseName.empty())
                    throw std::runtime_error(std::string("Failed to retrieve original basename (unexpected empty result from getBaseName)."));
            }
            try
            {
#if __cplusplus >= 201703L
                std::filesystem::path p = (std::filesystem::path("output") / baseName);
                YOUR_INPUT_DECOMPRESSED_PATH = p.string() + exten;
#else
                    YOUR_INPUT_DECOMPRESSED_PATH = "output/" + baseName + exten;
#endif
            }
#if __cplusplus >= 201703L
            catch (const std::filesystem::filesystem_error &e)
            {
                throw std::runtime_error(std::string("Filesystem error during default path generation: ") + e.what());
            }
#endif
            catch (const std::exception &e)
            {
                throw std::runtime_error(std::string("An error occurred during default path generation: ") + e.what());
            }
        }
        else
        {
            // YOUR_INPUT_DECOMPRESSED_PATH has been assigned
        }

        std::ifstream inputFile(YOUR_INPUT_PATH, std::ios::binary);
        std::ofstream outputFile(YOUR_INPUT_DECOMPRESSED_PATH, std::ios::out | std::ios::binary);

        if (!inputFile)
            throw std::ios_base::failure(std::string("Unable to open input compressed file for decompression: ") + YOUR_INPUT_PATH);

        if (!outputFile)
            throw std::ios_base::failure(std::string("Unable to create output file for decompression: ") + YOUR_INPUT_DECOMPRESSED_PATH);

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
                if (bitsToRead <= 0) // after this pad are padding
                    break;
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
    }
}