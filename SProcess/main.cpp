#include <windows.h>
#include <iostream>
#include <cstring>

int main()
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;
    DWORD dwWritten;

    std::string outBuffer;
    int sum = 0;

    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PipeP"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1,
                            1024 * 16,
                            1024 * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);

    if(hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL))
        {
            if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL))
            {

                buffer[dwRead] = '\0';

                char* next = nullptr;
                char* ptr = strtok_s(buffer, " ", &next);
                outBuffer = "";

                while(ptr != NULL)
                {
                    sum += std::atoi(ptr);
                    ptr = strtok_s(NULL, " ", &next);
                }
                outBuffer = std::to_string(sum);
                outBuffer += '\n';
            }
        }
    }


    hPipe = CreateFile(TEXT("\\\\.\\pipe\\PipeS"),
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
                  outBuffer.c_str(),
                  outBuffer.length() + 1,
                  &dwWritten,
                  NULL);

        CloseHandle(hPipe);
    }


    return 0;
}