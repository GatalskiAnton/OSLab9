#include <windows.h>
#include <iostream>

void callProcess(std::string str)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL, &str.front(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        std::cerr << "Error " << GetLastError() << '\n';
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    if (!CloseHandle(pi.hProcess) && !CloseHandle(pi.hThread))
    {
        std::cerr << "Error  " << GetLastError() << '\n';
    }
}

int main(void)
{
//    callProcess("C:\\Users\\ag629\\CLionProjects\\OSLab9\\SProcess\\cmake-build-debug\\SProcess.exe");
//    callProcess("C:\\Users\\ag629\\CLionProjects\\OSLab9\\PProcess\\cmake-build-debug\\PProcess.exe");
//    callProcess("C:\\Users\\ag629\\CLionProjects\\OSLab9\\AProcess\\cmake-build-debug\\AProcess.exe");
//    callProcess("C:\\Users\\ag629\\CLionProjects\\OSLab9\\MProcess\\cmake-build-debug\\MProcess.exe");



    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;


    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PipeS"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                            1,
                            1024 * 16,
                            1024 * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                buffer[dwRead] = '\0';

                printf("%s", buffer);
                DisconnectNamedPipe(hPipe);
            }
        }

    }

    return 0;
}