#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

bool is_obs_running() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;

    if (Process32First(snapshot, &entry)) {
        do {
            std::wstring name = entry.szExeFile;
            if (name == L"obs64.exe" || name == L"obs32.exe") {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return false;
}

int main() {
    if (is_obs_running()) {
        std::cout << "OBS detected! Please close OBS before playing.\n";
        return 1;
    }
    std::cout << "No OBS detected. Starting game...\n";
}
