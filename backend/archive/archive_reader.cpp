#include "archive/archive_reader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

bool ArchiveReader::extract(
    const fs::path& archivePath,
    const fs::path& outputDir)
{
    std::ifstream in(archivePath, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open archive.\n";
        return false;
    }

    // --- Read header ---
    char magic[4];
    in.read(magic, 4);
    if (std::string(magic, 4) != "MZIP") {
        std::cerr << "Invalid archive format.\n";
        return false;
    }

    uint32_t version;
    uint32_t fileCount;

    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    in.read(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));

    fs::create_directories(outputDir);

    // --- Extract files ---
    for (uint32_t i = 0; i < fileCount; ++i) {
        uint32_t pathLen;
        in.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));

        std::string relPath(pathLen, '\0');
        in.read(relPath.data(), pathLen);

        uint64_t fileSize;
        in.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

        fs::path outFile = outputDir / fs::path(relPath);
        fs::create_directories(outFile.parent_path());

        std::ofstream out(outFile, std::ios::binary);
        if (!out) {
            std::cerr << "Failed to create file: " << outFile << "\n";
            return false;
        }

        std::vector<char> buffer(static_cast<size_t>(fileSize));
        in.read(buffer.data(), fileSize);
        out.write(buffer.data(), fileSize);
    }

    return true;
}
