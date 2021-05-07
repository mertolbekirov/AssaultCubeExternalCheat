#include <iostream>
#include <Windows.h>
#include <vector>
#include "proc.h"
#include "mem.h"

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

    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;

            if (bAmmo)
            {
                // ff 06 = inc [esi];
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else
            {
                // ff 0E = dec [esi];
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }
    }
}
