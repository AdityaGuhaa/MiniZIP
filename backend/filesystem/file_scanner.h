#pragma once
#include <filesystem>
#include <vector>

struct FileEntry {
    std::filesystem::path absolutePath;
    std::filesystem::path relativePath;
};

class FileScanner {
public:
    static std::vector<FileEntry>
    scan(const std::filesystem::path& inputPath);
};
