#include "processmanager.h"

#include <iostream>

constexpr int ARGUMENT_NUM = 3;

int main(int argc, char **argv)
{
    if (argc != ARGUMENT_NUM) {
        std::cerr << "You must provide " << ARGUMENT_NUM << " arguments as: .exe <PID> <DLL_PATH> "
                  << std::endl;
        return -1;
    }

    std::unique_ptr<ProcessManagerx86> manager = std::make_unique<ProcessManager>();

    unsigned long pid = stoi(std::string{argv[1]});
    std::string dll_path = std::string{argv[2]};
    std::cout << "Injecting: " << std::to_string(pid) << " " << dll_path << std::endl;
    auto result = manager->inject(pid, dll_path);

    result == 0 ? std::cout << "Succesfully injected!" << std::endl
                : std::cout << "An error occured during injection!" << std::endl;
}
