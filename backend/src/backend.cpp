#include "backend.h"
#include <iostream>
int32_t Backend::set_dll_path(std::string &&path)
{
    m_dllPath = path;
    std::cout << "Setting DLL Path to: " << m_dllPath.string() << std::endl;
    return 0;
}
int32_t Backend::set_process(ProcessInfo process)
{
    std::cout << "Selected process: " << process.m_name << std::endl;
    m_currentProcess = process;
    return 0;
}
int32_t Backend::inject()
{
    if (m_currentProcess.m_x64) {
        return m_pManager_x64.inject(m_currentProcess.m_pid, m_dllPath);
    } else {
        std::filesystem::path command = std::filesystem::current_path()
                                        / "Injectorx86_headless.exe";
        auto command_str = command.string();
        command_str += " " + std::to_string(m_currentProcess.m_pid);
        command_str += " " + m_dllPath.string();
        return std::system(command_str.c_str());
    }
}
int32_t Backend::get_active_processes(std::vector<ProcessInfo> &p_list)
{
    return m_pManager_x64.get_process_name_list(p_list);
}
