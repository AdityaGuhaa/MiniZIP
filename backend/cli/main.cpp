#include <iostream>
#include <filesystem>

#include "filesystem/file_scanner.h"
#include "archive/archive_writer.h"
#include "common/errors.h"
#include "archive/archive_reader.h"

namespace fs = std::filesystem;

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

    // ---------------- EXTRACT (later) ----------------
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
