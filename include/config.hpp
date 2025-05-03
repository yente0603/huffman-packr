#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace huffman
{
    constexpr const char *DEFAULT_FREQ_TABLE_PATH = "output/freqTable.txt";
    constexpr const char *DEFAULT_ZEROPADDING_PATH = "output/zeroPadding.log";
    constexpr const char *DEFAULT_RATIO_PATH = "output/ratio.log";
    constexpr const char *DEFAULT_EXTEN_PATH = "output/exten.log";
    constexpr const char *DEFAULT_BASENAME_PATH = "output/basename.log";

    extern std::string YOUR_INPUT_PATH;
    extern std::string YOUR_INPUT_COMPRESSED_PATH;
    extern std::string YOUR_INPUT_DECOMPRESSED_PATH;
}
#endif