#pragma once

#include <string>
#include <filesystem>
#ifndef _WIN32
#include <unistd.h>
#endif

inline std::string getExePath() {
#ifdef _WIN32 
    /*
    // Will have to test this, probably need windows.h include, but probably everyone will just double click the exe anyway...
    wchar_t path[FILENAME_MAX] = { 0 };
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    return std::filesystem::path(path).parent_path().string()+"/";
    */
    return "";
#else
    char path[FILENAME_MAX];
    ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    return std::filesystem::path(std::string(path, (count > 0) ? count: 0)).parent_path().string()+"/";
#endif
}