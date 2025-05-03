#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <algorithm>

namespace huffman
{
    struct huffmanNode
    {
        unsigned char data;
        unsigned freq;
        unsigned char minChar;
        huffmanNode *left, *right;
        bool isLeaf;

        huffmanNode(unsigned char data, unsigned freq)
            : data(data), freq(freq), minChar(data), left(nullptr), right(nullptr), isLeaf(true) {}

        huffmanNode(huffmanNode *left, huffmanNode *right)
            : data(0), freq(left->freq + right->freq), minChar(std::min(left->minChar, right->minChar)), left(left), right(right), isLeaf(false) {}
    };
    struct compare
    {
        bool operator()(huffmanNode *left, huffmanNode *right)
        {
            if (left->freq != right->freq)
                return left->freq > right->freq;    // Elements with smaller frequencies will be placed at the fron
            return left->minChar >= right->minChar; // If frequencies are the same, compare by data
        }
    };

    huffmanNode *buildTree(std::vector<unsigned char> &data, std::vector<unsigned> &freq);
    void initializeTree();
    void deleteTree(huffmanNode *node);
}
#endif