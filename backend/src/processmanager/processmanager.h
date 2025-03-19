#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <vector>

#include "processmanagerx64.h"
#include "processmanagerx86.h"

class ProcessManager : public ProcessManagerx86, public ProcessManagerx64
{
public:
    ProcessManager() = default;
    int32_t get_process_name_list(std::vector<ProcessInfo> &process_list) const override final;
    int32_t __stdcall inject(unsigned long pid, std::filesystem::path dll_path) override final;

private:
    int32_t init_process_snap(HANDLE &hProcessSnap, PROCESSENTRY32 &pe32) const;
    bool is_process_x64(const HANDLE hProcessSnap) const;
};
