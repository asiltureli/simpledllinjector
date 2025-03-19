#pragma once

#include <vector>
#include <filesystem>

#include "processinfo.h"

class ProcessManagerx64
{
public:
    virtual int32_t get_process_name_list(std::vector<ProcessInfo> &process_list) const = 0;
    virtual int32_t __stdcall inject(unsigned long pid, std::filesystem::path dll_path) = 0;
};
