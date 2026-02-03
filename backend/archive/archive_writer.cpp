#include "archive/archive_writer.h"
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool ArchiveWriter::write(
    const fs::path& outputArchive,
    const std::vector<FileEntry>& files)
{
    std::ofstream out(outputArchive, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open archive for writing\n";
        return false;
    }

    // Header
    out.write("MZIP", 4);

    uint32_t version = 1;
    uint32_t fileCount = static_cast<uint32_t>(files.size());

    out.write(reinterpret_cast<char*>(&version), sizeof(version));
    out.write(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));

    // Files
    for (const auto& file : files) {
        std::string relPath = file.relativePath.generic_string();
        uint32_t pathLen = static_cast<uint32_t>(relPath.size());

        out.write(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        out.write(relPath.data(), pathLen);

        std::ifstream in(file.absolutePath, std::ios::binary);
        if (!in) {
            std::cerr << "Failed to open file: " << file.absolutePath << "\n";
            return false;
        }

        in.seekg(0, std::ios::end);
        uint64_t size = in.tellg();
        in.seekg(0);

        out.write(reinterpret_cast<char*>(&size), sizeof(size));

        std::vector<char> buffer(size);
        in.read(buffer.data(), size);
        out.write(buffer.data(), size);
    }

    return true;
}
