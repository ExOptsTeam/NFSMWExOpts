#pragma once

#define DialogBoxReturn 0x1337DBFF
#define DialogBoxButtonOK 0x417B2601
#define ESRBOBJHASH 0xBDE7FA72
#define EVENTIDOBJHASH 0xBBF970CD
#define TAKEOVERSTRING "NFSMW Extra Options - © 2022 ExOpts Team. No Rights Reserved."

DWORD DialogBoxReturnValueExit = 0x7C7250;

void __cdecl GetGameVersionNumberStringHook(char* buffer, int bufsize)
{
	const char* PackageName;
	_asm mov eax, [esi + 0x10]
		_asm mov PackageName, eax

	sprintf(buffer, "1.3"); // print the game version string
	unsigned int* TheESRBObject = (unsigned int*)FEObject_FindObject(PackageName, ESRBOBJHASH);
	TheESRBObject[0x1A] = strlen(TAKEOVERSTRING) + 1; // FEObject@0x68 = string length

	FEColor TakeoverColor = { 255, 255, 255, 255 };
	FEObject_SetColor((void*)TheESRBObject, TakeoverColor, 0);

	FEPrintf_Obj(PackageName, (void*)TheESRBObject, TAKEOVERSTRING); // use the FEObject immediately instead of searching for it again
}

void __declspec(naked) DialogBoxReturnValue(void* Pointer)
{
	_asm
	{
		mov eax, [esi + 0x2C]
		mov ButtonResult, eax
		jmp DialogBoxReturnValueExit
	}
}

int FEngFindObjectHook(const char* pkg_name, unsigned int obj_hash)
{
	void* GRaceParameters;
	_asm mov eax, [esi + 0x108]
		_asm mov GRaceParameters, eax

	FEPrintf(pkg_name, obj_hash, GRaceParameters_GetEventID(GRaceParameters));
	return 0;
}

int FEngFindObjectHook2(const char* pkg_name, unsigned int obj_hash)
{
	void* GRaceParameters;
	_asm mov GRaceParameters, ebx

	FEPrintf(pkg_name, obj_hash, GRaceParameters_GetEventID(GRaceParameters));
	return 0;
}

int __stdcall FindPackageHook(const char* pkg_name)
{
	void* GRaceParameters;
	_asm mov GRaceParameters, edi

	FEPrintf(pkg_name, EVENTIDOBJHASH, GRaceParameters_GetEventID(GRaceParameters));
	return 0;
}

int Return0Hook()
{
	return 0;
}

float Return0fHook()
{
	return 0.0f;
}

DWORD* GetPlayerPVehicle()
{
	DWORD* ISimableFromIPlayer, * ISimableFromIVehicle, * PlayerPVehicle;

	DWORD* PlayerIPlayer = *(DWORD**)_PlayerIPlayer; // Player 1
	int NumCurrentIVehicleElements = *(int*)0x92CD24;

	for (int i = 0; i < NumCurrentIVehicleElements; i++)
	{
		PlayerPVehicle = *(DWORD**)(_PlayerPVehicle + i * 4);

		if (PlayerIPlayer) ISimableFromIPlayer = (*(DWORD * (__thiscall**)(DWORD*))(PlayerIPlayer[0] + 0x4))(PlayerIPlayer); // RecordablePlayer::GetSimable
		else ISimableFromIPlayer = 0;
		if (PlayerPVehicle) ISimableFromIVehicle = (*(DWORD * (__thiscall**)(DWORD*))(PlayerPVehicle[0] + 0x4))(PlayerPVehicle); // PVehicle::GetSimable
		else ISimableFromIVehicle = 0;

		if (ISimableFromIPlayer && ISimableFromIVehicle && (ISimableFromIPlayer == ISimableFromIVehicle)) return PlayerPVehicle;
	}

	return 0;
}