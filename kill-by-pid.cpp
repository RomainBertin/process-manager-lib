#include <iostream>
#include "ProcessManager.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <pid>" << std::endl;
        return 1;
    }

    pid_t pid = std::stoi(argv[1]);

    try
    {
        ProcessManager manager;
        manager.killProcessById(pid);
        std::cout << "Successfully killed process with PID: " << pid << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


