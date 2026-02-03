#pragma once
#include <filesystem>
#include <vector>
#include "filesystem/file_scanner.h"

class ArchiveWriter {
public:
    static bool write(
        const std::filesystem::path& outputArchive,
        const std::vector<FileEntry>& files
    );
};
