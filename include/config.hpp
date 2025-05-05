#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace huffman
{
    constexpr const char *DEFAULT_FREQ_TABLE_PATH = "output/tmp/freqTable.txt";
    constexpr const char *DEFAULT_ZEROPADDING_PATH = "output/tmp/zeroPadding.log";
    constexpr const char *DEFAULT_RATIO_PATH = "output/tmp/ratio.log";
    constexpr const char *DEFAULT_EXTEN_PATH = "output/tmp/exten.log";
    constexpr const char *DEFAULT_BASENAME_PATH = "output/tmp/basename.log";

    extern std::string YOUR_INPUT_PATH;
    extern std::string YOUR_INPUT_COMPRESSED_PATH;
    extern std::string YOUR_INPUT_DECOMPRESSED_PATH;
}
#endif