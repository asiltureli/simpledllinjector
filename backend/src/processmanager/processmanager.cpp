#include "processmanager.h"
#include <map>
#include <iostream>
int32_t ProcessManager::inject(unsigned long pid, std::filesystem::path dll_path)
{
    auto process_handle = OpenProcess(PROCESS_ALL_ACCESS, true, (DWORD) pid);

    if (!process_handle) {
        // Couldnt obtain handle - INVALID HANDLE
        std::cout << "Error: " << GetLastError() << std::endl;
        return GetLastError();
    }

    std::string dll_path_str = dll_path.string();
    std::replace(dll_path_str.begin(), dll_path_str.end(), '/', '\\');
    // Allocate memory for DLL path in the remote process
    void *remote_address = VirtualAllocEx(process_handle,
                                          NULL,
                                          strlen(dll_path_str.c_str()) + 1,
                                          MEM_COMMIT | MEM_RESERVE,
                                          PAGE_READWRITE);
    if (!remote_address) {
        // Couldnt allocate remote address
        std::cout << "Error: " << GetLastError() << std::endl;
        return GetLastError();
    }

    // Write DLL path to the remote process
    if (!WriteProcessMemory(process_handle,
                            remote_address,
                            dll_path_str.c_str(),
                            strlen(dll_path_str.c_str()) + 1,
                            NULL)) {
        std::cout << "Could not write process memory" << std::endl;
    }

    // Load DLL into remote process
    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    FARPROC library_add = GetProcAddress(kernel32, "LoadLibraryA");
    if (!library_add) {
        std::cerr << "GetProcAddress failed: " << GetLastError() << std::endl;
        VirtualFreeEx(process_handle, remote_address, 0, MEM_RELEASE);
        CloseHandle(process_handle);
        return GetLastError();
    }
    HANDLE remote_thread = CreateRemoteThread(process_handle,
                                              NULL,
                                              0,
                                              (LPTHREAD_START_ROUTINE) library_add,
                                              remote_address,
                                              0,
                                              NULL);

    if (!remote_thread) {
        std::cout << "Error: " << GetLastError() << std::endl;
        VirtualFreeEx(process_handle, remote_address, 0, MEM_RELEASE);
        CloseHandle(process_handle);
        return GetLastError();
    }
    // Wait for DLL main to exit
    unsigned long exit_code;
    WaitForSingleObject(remote_thread, INFINITE);
    GetExitCodeThread(remote_thread, &exit_code);
    std::cout << exit_code << std::endl;

    // Cleanup the stuff
    VirtualFreeEx(process_handle, remote_address, 0, MEM_RELEASE);
    CloseHandle(remote_thread);
    CloseHandle(process_handle);
    return 0;
}

// https://learn.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
int32_t ProcessManager::get_process_name_list(std::vector<ProcessInfo> &p_list) const
{
    p_list.clear();

    PROCESSENTRY32 pe32;
    HANDLE hProcessSnap = nullptr;

    auto ips_result = init_process_snap(hProcessSnap, pe32);
    if (ips_result < 0) {
        return ips_result;
    }

    do {
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
        p_list.emplace_back(std::string(pe32.szExeFile), pe32.th32ProcessID, is_process_x64(hProc));
        CloseHandle(hProc);
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return 0;
}

int32_t ProcessManager::init_process_snap(HANDLE &hProcessSnap, PROCESSENTRY32 &pe32) const
{
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return GetLastError();
    }

    return 0;
}

bool ProcessManager::is_process_x64(const HANDLE h_process_snap) const
{
    static std::map<USHORT, std::string> archMap = {{IMAGE_FILE_MACHINE_AMD64, "AMD64 (x64)"},
                                                    {IMAGE_FILE_MACHINE_ARM64, "ARM64"},
                                                    {IMAGE_FILE_MACHINE_IA64, "Itanium (IA64)"},
                                                    {IMAGE_FILE_MACHINE_I386, "x86 (32-bit)"},
                                                    {IMAGE_FILE_MACHINE_ARM, "ARM (32-bit)"},
                                                    {IMAGE_FILE_MACHINE_ARMNT,
                                                     "ARM (32-bit Thumb-2)"},
                                                    {IMAGE_FILE_MACHINE_THUMB,
                                                     "ARM Thumb (embedded)"},
                                                    {IMAGE_FILE_MACHINE_POWERPC, "PowerPC"}};

    USHORT p_process_machine, p_native_machine;
    if (IsWow64Process2(h_process_snap, &p_process_machine, &p_native_machine) != 0) {
        return p_process_machine == IMAGE_FILE_MACHINE_UNKNOWN && // Not running on emulation
               (p_native_machine == IMAGE_FILE_MACHINE_AMD64
                || p_native_machine == IMAGE_FILE_MACHINE_ARM64);
    } else {
        std::cerr << "Error while reading process space architecture" << std::endl;
        return false;
    }
}
