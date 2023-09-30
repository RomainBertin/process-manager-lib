#include <iostream>
#include "ProcessManager.h" 

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <process_name>" << std::endl;
        return 1;
    }

    try
    {
        ProcessManager manager;
        pid_t pid = manager.getPidByName(argv[1]);
        std::cout << "PID of process " << argv[1] << " is: " << pid << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


