#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace huffman
{
    constexpr const char *DEFAULT_INPUT_PATH = ".testdata/src/TXTtest.txt";

    constexpr const char *DEFAULT_FREQ_TABLE_PATH = "output/freqTable.txt";
    constexpr const char *DEFAULT_COMPRESSED_PATH = "output/compressed.huff";
    constexpr const char *DEFAULT_ZEROPADDING_PATH = "output/zeroPadding.log";
    constexpr const char *DEFAULT_RATIO_PATH = "output/ratio.log";
    constexpr const char *DEFAULT_EXTEN_PATH = "output/exten.log";
    constexpr const char *DEFAULT_DECOMPRESSED_PATH = "output/decompressed.txt";

    extern std::string YOUR_INPUT_PATH;
    extern std::string YOUR_INPUT_COMPRESSED_PATH;
    extern std::string YOUR_INPUT_DECOMPRESSED_PATH;
}
#endif