#pragma once

#include "processmanager.h"
#include <filesystem>
class Backend
{
public:
    Backend() = default;
    int32_t set_dll_path(std::string &&path);
    int32_t set_process(ProcessInfo process);
    int32_t inject();
    int32_t get_active_processes(std::vector<ProcessInfo> &p_list);

private:
    ProcessInfo m_currentProcess;
    ProcessManager m_pManager_x64;
    static constexpr std::string_view x32_path{""};
    std::filesystem::path m_dllPath;
};
