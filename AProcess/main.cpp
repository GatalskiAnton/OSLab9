#include <windows.h>
#include <iostream>
#include <cstring>

int main()
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;
    DWORD dwWritten;

    int N = 8;
    std::string outBuffer;

    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PipeM"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1,
                            1024 * 16,
                            1024 * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {

                buffer[dwRead] = '\0';

                char* next = nullptr;
                char* ptr = strtok_s(buffer, " ", &next);
                outBuffer = "";

                while(ptr != NULL)
                {
                    outBuffer += std::to_string(std::atoi(ptr) + N) + " ";
                    ptr = strtok_s(NULL, " ", &next);
                }
                outBuffer += '\n';


                hPipe = CreateFile(TEXT("\\\\.\\pipe\\PipeA"),
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
            }
        }

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            WriteFile(hPipe,
                      outBuffer.c_str(),
                      12,
                      &dwWritten,
                      NULL);

            CloseHandle(hPipe);
        }
        DisconnectNamedPipe(hPipe);
    }




    return 0;
}