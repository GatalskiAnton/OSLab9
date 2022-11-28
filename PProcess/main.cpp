#include <windows.h>
#include <iostream>
#include <cstring>
#include <cmath>

int main(void)
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;
    DWORD dwWritten;

    std::string outBuffer;

    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PipeA"),
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

                char* next = nullptr;
                char* ptr = strtok_s(buffer, " ", &next);
                outBuffer = "";

                while(ptr != NULL)
                {
                    outBuffer += std::to_string(std::pow(std::atoi(ptr),3)) + " ";
                    ptr = strtok_s(NULL, " ", &next);
                }
                outBuffer += '\n';


                hPipe = CreateFile(TEXT("\\\\.\\pipe\\PipeP"),
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
                              outBuffer.length() + 1,   // = length of string + terminating '\0' !!!
                              &dwWritten,
                              NULL);

                    CloseHandle(hPipe);
                }

                printf("%s", outBuffer.c_str());
            }
        }

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            WriteFile(hPipe,
                      outBuffer.c_str(),
                      12,   // = length of string + terminating '\0' !!!
                      &dwWritten,
                      NULL);

            CloseHandle(hPipe);
        }
        DisconnectNamedPipe(hPipe);
    }

    return 0;
}