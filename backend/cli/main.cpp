#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#include "filesystem/file_scanner.h"
#include "archive/archive_writer.h"
#include "archive/archive_reader.h"
#include "compression/huffman.h"
#include "common/errors.h"

namespace fs = std::filesystem;

// ---------------- HUFFMAN TEST (PHASE 4.1) ----------------
std::vector<uint8_t> toBytes(const std::string& s)
{
    return std::vector<uint8_t>(s.begin(), s.end());
}

void testHuffman()
{
    std::string text = "aaabbcdddddddd";
    auto data = toBytes(text);

    auto tree  = Huffman::buildTree(data);
    auto table = Huffman::buildCodeTable(tree);

    auto encoded = Huffman::encode(data, table);
    auto decoded = Huffman::decode(encoded, tree);

    std::string result(decoded.begin(), decoded.end());

    std::cout << "[HUFFMAN TEST]\n";
    std::cout << "Original: " << text << "\n";
    std::cout << "Decoded : " << result << "\n\n";
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << errors::INVALID_ARGUMENTS << std::endl;
        return 1;
    }

    std::string command = argv[1];
    fs::path inputPath  = argv[2];
    fs::path outputPath = argv[3];

    if (!fs::exists(inputPath)) {
        std::cerr << errors::PATH_NOT_FOUND << std::endl;
        return 1;
    }

    // ---------------- COMPRESS ----------------
    if (command == "compress") {

        auto files = FileScanner::scan(inputPath);

        if (files.empty()) {
            std::cerr << "No files found to compress.\n";
            return 1;
        }

        std::cout << "Compressing " << files.size() << " file(s)...\n";

        if (!ArchiveWriter::write(outputPath, files)) {
            std::cerr << "Failed to create archive.\n";
            return 1;
        }

        std::cout << "Archive created successfully: "
                  << outputPath << "\n";
    }

    // ---------------- EXTRACT ----------------
    else if (command == "extract") {

        if (!ArchiveReader::extract(inputPath, outputPath)) {
            std::cerr << "Extraction failed.\n";
            return 1;
        }

        std::cout << "Archive extracted successfully to: "
                  << outputPath << "\n";
    }

    // ---------------- INVALID ----------------
    else {
        std::cerr << errors::INVALID_ARGUMENTS << std::endl;
        return 1;
    }

    return 0;
}
