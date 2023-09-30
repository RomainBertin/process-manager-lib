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
        manager.killProcessByName(argv[1]);
        std::cout << "Successfully killed process with name: " << argv[1] << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


