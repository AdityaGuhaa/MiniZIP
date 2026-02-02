#include <iostream>
#include <filesystem>
#include "filesystem/file_scanner.h"
#include "common/errors.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << errors::INVALID_ARGUMENTS << std::endl;
        return 1;
    }

    std::string command = argv[1];
    fs::path inputPath = argv[2];
    fs::path outputPath = argv[3];

    if (!fs::exists(inputPath)) {
        std::cerr << errors::PATH_NOT_FOUND << std::endl;
        return 1;
    }

    if (command == "compress") {
        auto files = FileScanner::scan(inputPath);

        std::cout << "Files to compress:\n";
        for (const auto& file : files) {
            std::cout << "  " << file.relativePath << "\n";
        }

        std::cout << "\n(Compression not implemented yet)\n";
    }
    else if (command == "extract") {
        std::cout << "(Extraction not implemented yet)\n";
    }
    else {
        std::cerr << errors::INVALID_ARGUMENTS << std::endl;
        return 1;
    }

    return 0;
}
