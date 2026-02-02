#pragma once
#include <string>

namespace errors {
    constexpr const char* INVALID_ARGUMENTS =
        "Invalid arguments.\nUsage:\n"
        "  minizip compress <input_path> <output.mzip>\n"
        "  minizip extract  <archive.mzip> <output_folder>\n";

    constexpr const char* PATH_NOT_FOUND =
        "Error: input path does not exist.";
}
