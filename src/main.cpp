#include <iostream>
#include <string>

#include "../include/file.hpp"
#include "../include/config.hpp"
#include "../include/utils.hpp"
#include "../include/table.hpp"
#include "../include/utils.hpp"

void printHelp(const std::string &progName)
{
    std::cout << "Usage:\n";
    std::cout << "  " << progName << " -c <input_file> [-o <output_file>] # Compress a file\n";
    std::cout << "  " << progName << " -d <input_file> [-o <output_file>] # Decompress a file\n";
    std::cout << "  " << progName << " -r                                 # Show compression ratio\n";
    std::cout << "  " << progName << " -h                                 # Show this help message\n";

    std::cout << "\nOptions:\n";
    std::cout << "  -c <input_file>     Compress the specified input file.\n";
    std::cout << "  -d <input_file>     Decompress the specified input file.\n";
    std::cout << "  -o <output_file>    Specify the output file path. If not provided, a default name and location are used.\n";
    std::cout << "  -r                  Show the compression ratio.\n";
    std::cout << "  -h                  Show this help message.\n";

    std::cout << std::endl;
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
        if (argc < 3)
        {
            std::cerr << "Error: Input file required for compression (-c).\n";
            printHelp(argv[0]);
            return 1;
        }
        huffman::YOUR_INPUT_PATH = argv[2];

        if (argc == 5 && std::string(argv[3]) == "-o")
            huffman::YOUR_INPUT_COMPRESSED_PATH = argv[4];
        if (argc > 5)
        {
            std::cerr << "Error: Too many arguments for compression.\n";
            printHelp(argv[0]);
            return 1;
        }

        huffman::compressFile();
        std::cout << huffman::getRatio() << std::endl;
    }
    else if (option == "-d")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Input compressed file required for decompression (-d).\n";
            printHelp(argv[0]);
            return 1;
        }
        huffman::YOUR_INPUT_PATH = argv[2];

        if (argc == 5 && std::string(argv[3]) == "-o")
            huffman::YOUR_INPUT_DECOMPRESSED_PATH = argv[4];
        if (argc > 5)
        {
            std::cerr << "Error: Too many arguments for decompression.\n";
            printHelp(argv[0]);
            return 1;
        }

        huffman::decompressFile();
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
