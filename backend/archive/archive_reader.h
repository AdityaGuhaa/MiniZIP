#pragma once
#include <filesystem>

class ArchiveReader {
public:
    static bool extract(
        const std::filesystem::path& archivePath,
        const std::filesystem::path& outputDir
    );
};
