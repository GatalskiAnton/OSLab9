#include <windows.h>
#include <string>
#include <iostream>

int main(void)
{

    HANDLE hPipe;
    DWORD dwWritten;

    char inBuffer[1024] = "12 3 7 19\n";

    char* next = nullptr;
    char* ptr = strtok_s(inBuffer, " ", &next);
    std::string outBuffer = "";

    while(ptr != NULL)
    {
        outBuffer += std::to_string(std::atoi(ptr) * 7) + " ";
        ptr = strtok_s(NULL, " ", &next);
    }
    outBuffer += '\n';

    hPipe = CreateFile(TEXT("\\\\.\\pipe\\PipeM"),
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

    return (0);
}