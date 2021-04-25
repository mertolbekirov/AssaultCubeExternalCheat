// AssaultCubeExternalCheat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    //Get ProcId of the target process
    DWORD procId = GetProcId(L"ac_client.exe");

    //GetModuleBaseAddress
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    //Get Handle to process
    HANDLE hProc = 0;
    hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    //Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10f4f4;

    std::cout << "Dynamic pointer BaseAdddress = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProc, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "Dynamic pointer AmmoAddress = " << "0x" << std::hex << ammoAddr << std::endl;

    //Read Ammo Value
    int ammoValue = 0;

    ReadProcessMemory(hProc, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current Ammo = " << std::dec << ammoValue << std::endl;

    //Write Ammo Value
    int newAmmo = 1337;

    WriteProcessMemory(hProc, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read out again

    ReadProcessMemory(hProc, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

    std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

    getchar();
    return 0;
}
