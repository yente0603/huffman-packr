#include <iostream>
#include <string>

#include "../include/file.hpp"
#include "../include/config.hpp"
#include "../include/utils.hpp"
#include "../include/table.hpp"

void printHelp(const std::string &progName)
{
    std::cout << "Usage:\n"
              << "  " << progName << " -c [input_file]                      # Compress \n"
              << "  " << progName << " -d [output_file]                     # Decompress\n"
              << "  " << progName << " -c [input_file] -o [output_file]     # Compress to assigned path\n"
              << "  " << progName << " -d [input_file] -o [output_file]     # Decompress to assigned path\n"
              << "  " << progName << " -r                                   # Show compression ratio\n"
              << "  " << progName << " -h                                   # Show this help\n";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printHelp(argv[0]);
        return 1;
    }

    std::string option = argv[1];

    if (option == "-h")
        printHelp(argv[0]);
    else if (option == "-c")
    {
        if (argc == 3)
            huffman::YOUR_INPUT_PATH = argv[2];
        if (argc == 5)
            huffman::YOUR_INPUT_COMPRESSED_PATH = argv[4];
        huffman::initializeTree();
        huffman::writeFrequencyTable();
        huffman::compressFile();
        huffman::writeRatio();
        std::cout << huffman::getRatio() << std::endl;
    }
    else if (option == "-d")
    {
        if (argc == 3)
            huffman::YOUR_INPUT_DECOMPRESSED_PATH = argv[2];
        if (argc == 5)
        {
            huffman::YOUR_INPUT_COMPRESSED_PATH = argv[2];
            huffman::YOUR_INPUT_DECOMPRESSED_PATH = argv[4];
        }
        huffman::decompressFile();

        if (argc == 3 || argc == 5)
            std::cout << "Your decompress path: " << huffman::YOUR_INPUT_DECOMPRESSED_PATH << std::endl;
        else
            std::cout << "Default decompress path: " << huffman::DEFAULT_DECOMPRESSED_PATH << std::endl;
    }
    else if (option == "-r")
        std::cout << huffman::getRatio() << std::endl;
    else
    {
        std::cerr << "Invalid command.\n";
        printHelp(argv[0]);
        return 1;
    }
    return 0;
}
