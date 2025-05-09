#include "../include/config.hpp"

#include <string>
namespace huffman
{
    // CLI
    std::string YOUR_INPUT_PATH = "";
    std::string YOUR_INPUT_COMPRESSED_PATH = "";
    std::string YOUR_INPUT_DECOMPRESSED_PATH = "";

    // GUI
    std::filesystem::path DEFAULT_FILE_PATH = std::filesystem::current_path();

    std::filesystem::path getFreqTablePath() { return DEFAULT_FILE_PATH / DEFAULT_FREQ_TABLE_PATH; }
    std::filesystem::path getZeropaddingPath() { return DEFAULT_FILE_PATH / DEFAULT_ZEROPADDING_PATH; }
    std::filesystem::path getRatioPath() { return DEFAULT_FILE_PATH / DEFAULT_RATIO_PATH; }
    std::filesystem::path getExtenPath() { return DEFAULT_FILE_PATH / DEFAULT_EXTEN_PATH; }
    std::filesystem::path getBasenamePath() { return DEFAULT_FILE_PATH / DEFAULT_BASENAME_PATH; }
}
