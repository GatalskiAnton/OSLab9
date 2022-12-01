#include <windows.h>
#include <iostream>

int main() {

    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;

    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PipeS"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1,
                            1024 * 16,
                            1024 * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);

    if (hPipe != INVALID_HANDLE_VALUE) {
        if (ConnectNamedPipe(hPipe, NULL)) {
            if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
                buffer[dwRead] = '\0';

                printf("%s", buffer);
                DisconnectNamedPipe(hPipe);
            }
        }
    }

    return 0;
}