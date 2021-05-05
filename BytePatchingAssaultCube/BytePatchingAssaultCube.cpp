#include <iostream>
#include <Windows.h>
#include <vector>
#include "proc.h"

int main()
{
    HANDLE hProcess = 0;

    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false;

    const int newValue = 1999;

    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

        localPlayerPtr = moduleBase + 0x10f4f4;

        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
    }
    else
    {
        std::cout << "Process not found, press enter to exit \n";
        getchar();
        return 0;
    }

    DWORD dwExit = 0;

}
