cmake_minimum_required(VERSION 3.16)
project(MiniZIPPlusPlus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(minizip
    backend/cli/main.cpp
    backend/filesystem/file_scanner.cpp
    backend/archive/archive_writer.cpp
    backend/archive/archive_reader.cpp
)

target_include_directories(minizip PRIVATE backend)
