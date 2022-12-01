#include <windows.h>
#include <string>

int main(int argc, char *argv[]) {

    HANDLE hPipe;
    DWORD dwWritten;

    std::string out;
    for (int i = 1; i < argc; ++i) {
        out += std::to_string(std::atoi(argv[i]) * 7) + " ";
    }
    out += '\n';


    hPipe = CreateFile(TEXT("\\\\.\\pipe\\PipeM"),
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if (hPipe != INVALID_HANDLE_VALUE) {
        WriteFile(hPipe,
                  out.c_str(),
                  out.length() + 1,
                  &dwWritten,
                  NULL);

        CloseHandle(hPipe);
    }

    return 0;
}