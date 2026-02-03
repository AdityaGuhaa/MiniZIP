#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

struct HuffmanNode {
    uint8_t symbol;
    uint64_t freq;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(uint8_t s, uint64_t f)
        : symbol(s), freq(f), left(nullptr), right(nullptr) {}

    HuffmanNode(std::shared_ptr<HuffmanNode> l,
                std::shared_ptr<HuffmanNode> r)
        : symbol(0), freq(l->freq + r->freq), left(l), right(r) {}

    bool isLeaf() const {
        return !left && !right;
    }
};

class Huffman {
public:
    using CodeTable = std::unordered_map<uint8_t, std::string>;

    static std::shared_ptr<HuffmanNode>
    buildTree(const std::vector<uint8_t>& data);

    static CodeTable
    buildCodeTable(const std::shared_ptr<HuffmanNode>& root);

    static std::string
    encode(const std::vector<uint8_t>& data,
           const CodeTable& table);

    static std::vector<uint8_t>
    decode(const std::string& bits,
           const std::shared_ptr<HuffmanNode>& root);
};
