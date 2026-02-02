#include "filesystem/file_scanner.h"
#include <filesystem>

namespace fs = std::filesystem;

std::vector<FileEntry>
FileScanner::scan(const fs::path& inputPath)
{
    std::vector<FileEntry> files;

    if (fs::is_regular_file(inputPath)) {
        files.push_back({
            fs::absolute(inputPath),
            inputPath.filename()
        });
        return files;
    }

    const fs::path basePath = fs::absolute(inputPath);

    for (auto& entry : fs::recursive_directory_iterator(basePath)) {
        if (!entry.is_regular_file())
            continue;

        fs::path absPath = entry.path();
        fs::path relPath = fs::relative(absPath, basePath);

        files.push_back({
            absPath,
            relPath
        });
    }

    return files;
}
