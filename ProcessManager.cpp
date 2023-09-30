#include "ProcessManager.h" 

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#else
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#endif


ProcessManager::ProcessManager()
{
#ifdef _WIN32
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!init())
    {
        throw std::runtime_error("Failed to initialize ProcessManager.");
    }
    setDebugPrivileges();
#endif
}

ProcessManager::~ProcessManager()
{
#ifdef _WIN32
    if (hSnapShot != INVALID_HANDLE_VALUE)
    {
	CloseHandle(hSnapShot);
    }
#endif
}

#ifdef _WIN32
bool ProcessManager::init()
{
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (Process32First(hSnapShot, &pe32) == false)
    {
        CloseHandle(hSnapShot);
        return false;
    }

    return true;
}

std::wstring ProcessManager::getNextProcessName()
{
    if (Process32Next(hSnapShot, &pe32) == false)
    {
        CloseHandle(hSnapShot);
        return L"";
    }

    return pe32.szExeFile;
}
#endif

pid_t ProcessManager::getPidByName(const std::string& procName)
{
#ifdef _WIN32
    std::wstring current;
    std::wstring wProcName(procName.begin(), procName.end());

    while (!(current = getNextProcessName()).empty())
    {
        if (current == wProcName)
        {
            return static_cast<pid_t>(pe32.th32ProcessID);
        }
    }
#else
    DIR* dir;
    struct dirent* ent;
    char* endptr;
    char buf[512];

    if (!(dir = opendir("/proc")))
    {
        throw std::runtime_error("Can't open /proc");
    }

    while ((ent = readdir(dir)) != nullptr)
    {
        long lpid = strtol(ent->d_name, &endptr, 10);

        if (*endptr != '\0') continue;

        snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
        FILE* fp = fopen(buf, "r");

        if (fp)
        {
            if (fgets(buf, sizeof(buf), fp) != nullptr)
            {
                if (strstr(buf, procName.c_str()))
                {
                    fclose(fp);
                    closedir(dir);
                    return static_cast<pid_t>(lpid);
                }
            }
            fclose(fp);
        }
    }

    closedir(dir);
#endif

    throw std::runtime_error("Process not found.");
}

void ProcessManager::killProcessById(pid_t pid)
{
#ifdef _WIN32
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pid);
    if (!hProcess)
    {
        throw std::runtime_error("Failed to open process.");
    }

    if (TerminateProcess(hProcess, 0) == 0)
    {
        CloseHandle(hProcess);
        throw std::runtime_error("Failed to terminate process.");
    }

    CloseHandle(hProcess);
#else
    if (kill(pid, SIGTERM) != 0)
    {
        throw std::runtime_error("Failed to kill process.");
    }
#endif
}

void ProcessManager::killProcessByName(const std::string& procName)
{
    pid_t pid = getPidByName(procName);
    killProcessById(pid);
}

void ProcessManager::suspendProcessById(pid_t pid)
{
#ifdef _WIN32
    throw std::runtime_error("Suspend process not implemented for Windows.");
#else
    if (kill(pid, SIGSTOP) != 0)
    {
        throw std::runtime_error("Failed to suspend process.");
    }
#endif
}

void ProcessManager::resumeProcessById(pid_t pid)
{
#ifdef _WIN32
    throw std::runtime_error("Resume process not implemented for Windows.");
#else
    if (kill(pid, SIGCONT) != 0)
    {
        throw std::runtime_error("Failed to resume process.");
    }
#endif
}

#ifdef _WIN32
void ProcessManager::setDebugPrivileges()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES DBGPriv;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        throw std::runtime_error("Failed to open process token.");
    }

    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &DBGPriv.Privileges[0].Luid))
    {
        CloseHandle(hToken);
        throw std::runtime_error("Failed to lookup privilege value.");
    }

    DBGPriv.PrivilegeCount = 1;
    DBGPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, false, &DBGPriv, 0, nullptr, nullptr))
    {
        CloseHandle(hToken);
        throw std::runtime_error("Failed to adjust token privileges.");
    }

    CloseHandle(hToken);
}
#endif


