#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <filesystem>

namespace huffman
{
    // CLI
    constexpr const char *DEFAULT_FREQ_TABLE_PATH = "output/tmp/freqTable.txt";
    constexpr const char *DEFAULT_ZEROPADDING_PATH = "output/tmp/zeroPadding.log";
    constexpr const char *DEFAULT_RATIO_PATH = "output/tmp/ratio.log";
    constexpr const char *DEFAULT_EXTEN_PATH = "output/tmp/exten.log";
    constexpr const char *DEFAULT_BASENAME_PATH = "output/tmp/basename.log";

    extern std::string YOUR_INPUT_PATH;
    extern std::string YOUR_INPUT_COMPRESSED_PATH;
    extern std::string YOUR_INPUT_DECOMPRESSED_PATH;

    // GUI
    extern std::filesystem::path DEFAULT_FILE_PATH;

    std::filesystem::path getFreqTablePath();
    std::filesystem::path getZeropaddingPath();
    std::filesystem::path getRatioPath();
    std::filesystem::path getExtenPath();
    std::filesystem::path getBasenamePath();

}
#endif