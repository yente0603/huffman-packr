#include "../include/tree.hpp"
#include "../include/utils.hpp"
#include "../include/table.hpp"

#include <queue>
#include <map>
#include <algorithm>
#include <iostream>

namespace huffman
{
    huffmanNode *buildTree(std::vector<unsigned char> &data, std::vector<unsigned> &freq)
    {
        // Handle empty input data
        if (data.empty() || freq.empty() || data.size() != freq.size())
            return nullptr;

        std::priority_queue<huffmanNode *, std::vector<huffmanNode *>, compare> pq;
        for (size_t i = 0; i < data.size(); i++)
            pq.push(new huffmanNode(data[i], freq[i]));

        while (pq.size() > 1)
        {
            huffmanNode *left = pq.top();
            pq.pop();
            huffmanNode *right = pq.top();
            pq.pop();

            if (left->minChar > right->minChar)
                std::swap(left, right);

            huffmanNode *top = new huffmanNode(left, right); // not a leaf node
            pq.push(top);
        }
        return pq.top();
    }

    void initializeTree()
    {
        auto result = calFreq(); // return std::make_pair(data, freq);

        // Handle empty input file case before building tree
        if (result.first.empty())
        {
            // Input file was empty, no tree or table needed for compression.
            // buildEncodingTable will be called with a nullptr or handled appropriately.
            // freqTable will remain empty.
            std::cerr << "Warning: Input file is empty. No compression will be performed." << std::endl;
            // No tree to build, no tree to delete.
            // The buildEncodingTable call below will need to handle the nullptr case.
            // Or we can skip it entirely. Let's refine buildEncodingTable to handle nullptr.
            huffmanNode *empty_node = nullptr;  // Explicitly represent empty tree
            buildEncodingTable(empty_node, ""); // Pass nullptr
            // No deleteTree needed for nullptr
            return; // Skip tree-dependent steps
        }

        huffmanNode *node = buildTree(result.first, result.second);

        buildEncodingTable(node, "");
        deleteTree(node);
    }

    void deleteTree(huffmanNode *node)
    {
        if (!node)
            return;

        deleteTree(node->left);
        deleteTree(node->right);

        delete node;
    }
}
