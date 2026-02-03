#include "compression/huffman.h"
#include <queue>
#include <functional>

using NodePtr = std::shared_ptr<HuffmanNode>;

namespace {

// Min-heap comparator
struct Compare {
    bool operator()(const NodePtr& a, const NodePtr& b) const {
        return a->freq > b->freq;
    }
};

// DFS to generate codes
void buildCodes(const NodePtr& node,
                const std::string& prefix,
                Huffman::CodeTable& table)
{
    if (!node) return;

    if (node->isLeaf()) {
        table[node->symbol] = prefix.empty() ? "0" : prefix;
        return;
    }

    buildCodes(node->left,  prefix + "0", table);
    buildCodes(node->right, prefix + "1", table);
}

} // anonymous namespace

// ---------------- TREE ----------------

NodePtr Huffman::buildTree(const std::vector<uint8_t>& data)
{
    std::unordered_map<uint8_t, uint64_t> freq;

    for (uint8_t b : data)
        freq[b]++;

    std::priority_queue<NodePtr,
        std::vector<NodePtr>, Compare> pq;

    for (auto& [sym, f] : freq)
        pq.push(std::make_shared<HuffmanNode>(sym, f));

    if (pq.empty())
        return nullptr;

    while (pq.size() > 1) {
        NodePtr left  = pq.top(); pq.pop();
        NodePtr right = pq.top(); pq.pop();
        pq.push(std::make_shared<HuffmanNode>(left, right));
    }

    return pq.top();
}

// ---------------- CODES ----------------

Huffman::CodeTable
Huffman::buildCodeTable(const NodePtr& root)
{
    CodeTable table;
    buildCodes(root, "", table);
    return table;
}

// ---------------- ENCODE ----------------

std::string
Huffman::encode(const std::vector<uint8_t>& data,
                const CodeTable& table)
{
    std::string bits;
    bits.reserve(data.size() * 8);

    for (uint8_t b : data)
        bits += table.at(b);

    return bits;
}

// ---------------- DECODE ----------------

std::vector<uint8_t>
Huffman::decode(const std::string& bits,
                const NodePtr& root)
{
    std::vector<uint8_t> output;
    NodePtr current = root;

    for (char bit : bits) {
        current = (bit == '0') ? current->left : current->right;

        if (current->isLeaf()) {
            output.push_back(current->symbol);
            current = root;
        }
    }

    return output;
}
