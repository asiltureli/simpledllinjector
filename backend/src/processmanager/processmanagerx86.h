#pragma once

#include <filesystem>

class ProcessManagerx86
{
public:
    virtual int32_t __stdcall inject(unsigned long pid, std::filesystem::path dll_path) = 0;
};
