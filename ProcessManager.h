#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>

class ProcessManager
{
public:
    ProcessManager();
    ~ProcessManager();

    pid_t getPidByName(const std::string& procName);

    void killProcessById(pid_t pid);
    void killProcessByName(const std::string& procName);
    void suspendProcessById(pid_t pid);
    void resumeProcessById(pid_t pid);

private:
#ifdef _WIN32
    bool init();
    std::wstring getNextProcessName();
    void setDebugPrivileges();

    HANDLE hSnapShot;
    PROCESSENTRY32 pe32;
#endif
};

#endif // PROCESS_MANAGER_H


#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>

class ProcessManager
{
public:
    ProcessManager();
    ~ProcessManager();
    pid_t getPidByName(const std::string& procName);
    void killProcessById(pid_t pid);
    void killProcessByName(const std::string& procName);
    void suspendProcessById(pid_t pid);
    void resumeProcessById(pid_t pid);

private:
#ifdef _WIN32
    bool init();
    std::wstring getNextProcessName();
    void setDebugPrivileges();

    HANDLE hSnapShot;
    PROCESSENTRY32 pe32;
#endif
};

#endif // PROCESS_MANAGER_H
