#include <iostream>
#include <string>
#include <stdexcept>
#include <ios>

#include "../include/file.hpp"
#include "../include/config.hpp"
#include "../include/utils.hpp"
#include "../include/table.hpp"

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
    try
    {
        if (argc < 2)
        {
            printHelp(argv[0]);
            return 1;
        }

        std::string cmd = argv[1];

        std::string inputPath = "";
        std::string outputPath = "";
        bool outputAssigned = false; // to flag if output path is assigned

        if (cmd == "-h")
        {
            if (argc > 2)
            {
                std::cerr << "Error: Unexpected arguments after -h.\n";
                printHelp(argv[0]);
                return 1;
            }
            printHelp(argv[0]);
        }
        else if (cmd == "-r")
        {
            if (argc > 2)
            {
                std::cerr << "Error: Unexpected arguments after -r.\n";
                printHelp(argv[0]);
                return 1;
            }
            std::cout << huffman::getRatio() << std::endl;
        }
        else if (cmd == "-c" || cmd == "-d")
        {
            for (int i = 2; i < argc; ++i)
            {
                std::string arg = argv[i];
                if (arg == "-o")
                {
                    if (i + 1 < argc)
                    {
                        outputPath = argv[i + 1];
                        outputAssigned = true;
                        i++;
                    }
                    else
                    {
                        // -o 是最後一個參數，沒有指定輸出文件路徑
                        std::cerr << "Error: -o option requires an output file path.\n";
                        printHelp(argv[0]);
                        return 1;
                    }
                }
                else
                {
                    if (inputPath.empty())
                    {
                        inputPath = arg;
                    }
                    else
                    {
                        std::cerr << "Error: Too many arguments or unexpected input file path: " << arg << "\n";
                        printHelp(argv[0]);
                        return 1;
                    }
                }
            }
            if (inputPath.empty())
            {
                std::cerr << "Error: Input file path is required for " << cmd << ".\n";
                printHelp(argv[0]);
                return 1;
            }

            huffman::YOUR_INPUT_PATH = inputPath;
            if (outputAssigned)
            {
                if (cmd == "-c")
                    huffman::YOUR_INPUT_COMPRESSED_PATH = outputPath;
                else
                    huffman::YOUR_INPUT_DECOMPRESSED_PATH = outputPath;
            }

            if (cmd == "-c")
            {
                huffman::writeBaseName();
                huffman::writeExten();
                huffman::compressFile();
                std::cout << "Compress Done." << std::endl
                          << "Default compress path: " << huffman::YOUR_INPUT_COMPRESSED_PATH << std::endl;
                std::cout << huffman::getRatio() << std::endl;
            }
            else
            {
                huffman::decompressFile();
                std::cout << "Decompress Done." << std::endl
                          << "Decompress Path: " << huffman::YOUR_INPUT_DECOMPRESSED_PATH << std::endl;
            }
        }
        else
        {
            std::cerr << "Invalid command: " << cmd << "\n";
            printHelp(argv[0]);
            return 1;
        }

        return 0;
    }
    catch (const std::ios_base::failure &e)
    {
        std::cerr << "File I/O Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }
}
