#include "stdafx.h"
#include "stdio.h"
#include "DialogInterfaceHook.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <math.h>

float heatLevel, gameSpeed, FallingRainSize, RainAmount, RoadReflection, RainIntensity, RainXing, RainFallSpeed, RainGravity, SplashScreenTimeLimit, CarSelectTireSteerAngle, MaxHeatLevel, MinHeatLevel, WorldAnimationSpeed, CarScale, VTRed, VTBlue, VTGreen, VTBlackBloom, VTColorBloom, VTSaturation, DebugCameraTurboSpeed, DebugCameraSuperTurboSpeed, SBRechargeTime, SBRechargeSpeedLimit, SBMassMultiplier, SpeedingLimit, ExcessiveSpeedingLimit, RecklessDrivingLimit;
int hotkeyToggleForceHeat, hotkeyForceHeatLevel, hotkeyToggleCopLights, hotkeyToggleHeadlights, hotkeyCarHack, hotkeyUnlockAllThings, hotkeyAutoDrive, randomizeCount, hotkeyToggleCops, hotkeyFreezeCamera, NosTrailRepeatCount;
unsigned char raceType, raceMode, minLaps, maxLaps, minOpponents, maxOpponents, maxLapsRandomQR, maxOpponentsRandomQR, maxBlacklist, csBlacklist, headlightsMode, lowTraffic, medTraffic, highTraffic, ShowHiddenTracks, MaxUniqueOpponentCars, ShowAllCarsInFE, WindowedMode, SelectableMarkerCount, PurchasedCarLimit;
bool copLightsEnabled, ShowTollbooth, ShowMoreRaceOptions, HideOnline, ShowOnlineOpts, removeSceneryGroupDoor, removePlayerBarriers, unfreezeKO, EnablePresetAndDebugCars, AlwaysRain, SkipMovies, EnableSound, EnableMusic, EnableCameras, ExOptsTeamTakeOver, ShowSubs, EnableHeatLevelOverride, CarbonStyleRaceProgress, moreVinyls, eatSomeBurgers, UnlockAllThings, ShowChallenge, GarageRotate, GarageZoom, GarageShowcase, EnableSaveLoadHotPos, EnableMaxPerfOnShop, EnableVTOverride, EnableDebugWorldCamera, DebugWorldCamera, DebugWatchCarCamera, ForceBlackEdition, HelicopterFix, X10Fix, WheelFix, ExperimentalSplitScreenFix, ShowDebugCarCustomize, CarbonStyleBustedScreen, ShowMessage, ReplayBlacklistRaces, PursuitActionMode, MoreCarsForOpponents, VisualFixesAndTweaks, UncensoredBustedScreen, ShowPursuitCops, ShowNonPursuitCops, ShowDebugEventID, CarbonStyleRandomCars, ShowMoreCustomizationOptions, SkipCareerIntro, ShowTimeOfDay, BetterRandomRaces, AllowMultipleInstances, TimeBugFix, NoCatchUp, CarSkinFix, ImmobileColFix, NFSU2StyleLookBackCamera, NoRevLimiter, SkipNISs, ExpandMemoryPools, ShowPresetCarsInFE, AllowLongerProfileNames, DDayFix;
DWORD selectedCar, careerCar, raceOptions, Strings, HeatLevelAddr, VTecx, StartingCashDWORD, GameState, ThreadDelay;
HWND windowHandle;

void Thing();

bool forceHeatLevel = 0, carHackEnabled = 0, once1 = 0, once2 = 0, once3 = 0, IsOnFocus, AutoDrive, ToggleCops = 1;
DWORD ButtonResult = 0;

DWORD CameraNamesCodeCaveExit = 0x51C98C;
DWORD HeatLevelsCodeCaveExit = 0x443dc9;
DWORD VinylMenuCodeCaveExit = 0x7BC908;
DWORD VinylMenuCodeCave2Exit = 0x7BD30F;
DWORD VinylMenuCodeCave2ifAbove = 0x7BD210;
DWORD ShowHiddenTracksCodeCaveExit = 0x7BA6F1;
DWORD GRaceDatabase_GetBinNumber = 0x5E3980;
DWORD GRaceDatabase_GetRaceFromHash = 0x5FB710;
DWORD AddToRaceList = 0x7AAC70;
DWORD GRaceDatabase_GetRaceCount = 0x5DC920;
DWORD IconFixCodeCaveExit = 0x56E099;
DWORD LANRaceSelectFixCodeCaveExit = 0x7aa8ed;
DWORD LANRaceModeFixCodeCaveExit = 0x5395a3;
DWORD LANRaceModeFixCodeCave2Exit = 0x535ecb;
DWORD HeliBountyFixCodeCaveExit = 0x595BF2;
DWORD HeliBountyFixCodeCave2Exit = 0x418F8C;
DWORD X10FixCodeCaveExit = 0x418F61;
DWORD X10FixCodeCave2Exit = 0x41901F;
DWORD AnnounceBountyReward = 0x595C37;
DWORD VTCodeCaveExit = 0x73E361;
DWORD DialogBoxReturnValueExit = 0x007C7250;
DWORD CreateGameWindow = 0x6E6310;
DWORD WindowedModeCodeCaveExit = 0x6E6C13;
DWORD ReplayBlacklistFixCodeCaveExit = 0x6243F4;
DWORD CarSkinFixCodeCaveExit = 0x747F3F;
DWORD CarSkinFixCodeCaveExit2 = 0x747F2B;
DWORD GRaceCustomHeatLevelCodeCaveExit = 0x56DD63;

DWORD GlobalTimerAddress = 0x009885D8;
DWORD TimerAddress = 0x009142DC;
float PreviousRaceTime = 0.0f;

// Properly implemented functions for calling
void(*CameraAI_SetAction)(int EVIEW_ID, char* Action) = (void(*)(int, char*))0x479EB0;
int(__cdecl *UnPause)(const void* unk) = (int(__cdecl*)(const void*))0x632190;
unsigned int(__cdecl *stringhash32)(const char* k) = (unsigned int (__cdecl*)(const char*))0x005CC240;
unsigned int(__thiscall *CustomizeCategoryScreen_AddCustomOption)(void* TheThis, const char* unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4) = (unsigned int(__thiscall*)(void*, const char*, unsigned int, unsigned int, unsigned int))0x007BB560;
const char*(__thiscall* GRaceParameters_GetEventID)(void* GRaceParameters) = (const char*(__thiscall*)(void*))0x005FBA70;
int(__cdecl *bSPrintf)(char* destString, const char* format, ...) = (int(__cdecl*)(char*, const char*, ...))0x004647C0;
unsigned int(__cdecl *bStringHash)(const char *text) = (unsigned int(__cdecl*)(const char*))0x460BF0;
int(__thiscall *UIQRTrackOptions_SetupTollbooth)(void* UIQRTrackOptions) = (int(__thiscall*)(void*))0x7AD310;
void(__cdecl *FE_Object_SetCenter)(DWORD* FEObject, float _PositionX, float _PositionY) = (void(__cdecl*)(DWORD*, float, float))0x525050;
void(__cdecl *FE_Object_GetCenter)(DWORD* FEObject, float *PositionX, float *PositionY) = (void(__cdecl*)(DWORD*, float*, float*))0x524EE0;
int(__cdecl *FEngSetVisible)(DWORD* FEObject) = (int(__cdecl*)(DWORD*))0x514CC0;
int(__cdecl *FEngSetInvisible)(DWORD* FEObject) = (int(__cdecl*)(DWORD*))0x514C70;
int(__cdecl *FEngSetVisible2)(const char *FEPackage, DWORD FEObjectHash) = (int(__cdecl*)(const char*,DWORD))0x527DF0;
void(*Game_SetWorldHeat)(float DesiredHeatLevel) = (void(*)(float))0x612660;
void(*Game_ForceAIControl)(int Unk) = (void(*)(int))0x6123B0;
void(*Game_ClearAIControl)(int Unk) = (void(*)(int))0x612420;
void(*Game_SetCopsEnabled)(bool AreCopsEnabled) = (void(*)(bool))0x604F40;
bool(__thiscall *PVehicle_IsGlareOn)(DWORD* PVehicle, int VehicleFXID) = (bool(__thiscall*)(DWORD*, int))0x6881C0;
void(__thiscall *PVehicle_GlareOn)(DWORD* PVehicle, int VehicleFXID) = (void(__thiscall*)(DWORD*, int))0x669360;
void(__thiscall *PVehicle_GlareOff)(DWORD* PVehicle, int VehicleFXID) = (void(__thiscall*)(DWORD*, int))0x669380;
void(__thiscall *GRaceDatabase_GetScoreInfo)(void* GRaceParameters, int EventHash) = (void(__thiscall*)(void*, int))0x5DCA70;
const char* (__thiscall* GRaceDatabase_GetNextDDayRace)(void* GRaceParameters) = (const char* (__thiscall*)(void*))0x600C30;

DWORD GRaceDatabase_GetRaceParameters = 0x005DC930;
DWORD GRaceParameters_GetEventHash = 0x005FAB40;
DWORD j_malloc = 0x652AD0;
DWORD IconOption_Create = 0x586FA0;
DWORD IconScrollerMenu_AddOption = 0x573960;
DWORD FEngGetLastButton = 0x571E80;
DWORD SplitScreenCodeCaveExit = 0x7AA82A;

#define DialogBoxReturn 0x1337DBFF
#define DialogBoxButtonOK 0x417B2601
#define ESRBOBJHASH 0xBDE7FA72
#define EVENTIDOBJHASH 0xBBF970CD
#define TAKEOVERSTRING "NFSMW Extra Options - © 2020 ExOpts Team. No Rights Reserved."

void __declspec(naked) CameraNamesCodeCave()
{
	_asm
	{
			cmp ecx, 0
			je bumper
			cmp ecx, 1
			je hood
			cmp ecx, 2
			je close
			cmp ecx, 3
			je farcam
			cmp ecx, 4
			je superfar
			cmp ecx, 5
			je drift
			cmp ecx, 6
			je pursuit
			jmp CameraNamesCodeCaveExit

		bumper :
			mov eax, 0xC3E9AE58
			jmp CameraNamesCodeCaveExit

		hood :
			mov eax, 0x414F19D7
			jmp CameraNamesCodeCaveExit

		close :
			mov eax, 0x5AE3441F
			jmp CameraNamesCodeCaveExit

		farcam :
			mov eax, 0x1EA4CEC2
			jmp CameraNamesCodeCaveExit

		superfar :
			mov eax, 0x916039B4
			jmp CameraNamesCodeCaveExit

		drift :
			mov eax, 0xA1211428
			jmp CameraNamesCodeCaveExit

		pursuit :
			mov eax, 0x7448870B
			jmp CameraNamesCodeCaveExit
	}
}

void __declspec(naked) HeatLevelsCodeCave()
{
	if (EnableHeatLevelOverride)
	{
		_asm
		{
			mov ebx, MaxHeatLevel
			mov[esi + 0xE0], ebx
			mov ebx, MinHeatLevel
			mov[esi + 0xDC], ebx
		}
	}
	_asm
	{
		mov edx, [esi + 0xE0]
		jmp HeatLevelsCodeCaveExit
	}
}

void __declspec(naked) VinylMenuCodeCave()
{
	_asm
	{
			mov edx, dword ptr ds : [0x905EB0]
			push 0x0000040A
			push 0xB715070A
			push 0x55778E5A
			push edx
			mov ecx, esi
			call CustomizeCategoryScreen_AddCustomOption
			lea eax, [esp + 0x14]
			mov ebp, 1
			jmp VinylMenuCodeCaveExit
	}
}

void __declspec(naked) VinylMenuCodeCave2()
{
	_asm
	{
			cmp eax, 0x06
			ja ifabove

			cmp eax, 0x00
			je stripes
			cmp eax, 0x01
			je raceflags
			cmp eax, 0x02
			je nflags
			cmp eax, 0x03
			je body
			cmp eax, 0x04
			je uniques
			cmp eax, 0x05
			je contest
			cmp eax, 0x06
			je specials
			jmp VinylMenuCodeCave2Exit


		stripes :
			mov dword ptr ds : [edi + 0x15C], 0x1C619FD8
			mov ebp, 0xF7352706
			mov dword ptr ds : [esp + 0x18], 0x00000002
			jmp VinylMenuCodeCave2Exit

		raceflags :
			mov dword ptr ds : [edi + 0x15C], 0x9C1B8935
			mov ebp, 0x1223CC89
			mov dword ptr ds : [esp + 0x18], 0x00000003
			jmp VinylMenuCodeCave2Exit

		nflags :
			mov dword ptr ds : [edi + 0x15C], 0x7956F7B0
			mov ebp, 0xBC44BBCB
			mov dword ptr ds : [esp + 0x18], 0x00000004
			jmp VinylMenuCodeCave2Exit

		body :
			mov dword ptr ds : [edi + 0x15C], 0x2D5BFF0F
			mov ebp, 0x694CA0CA
			mov dword ptr ds : [esp + 0x18], 0x00000005
			jmp VinylMenuCodeCave2Exit

		uniques :
			mov dword ptr ds : [edi + 0x15C], 0x209A9158
			mov ebp, 0x1B3A8DD3
			mov dword ptr ds : [esp + 0x18], 0x00000006
			jmp VinylMenuCodeCave2Exit

		contest :
			mov dword ptr ds : [edi + 0x15C], 0xCD057D21
			mov ebp, 0x1BA508FC
			mov dword ptr ds : [esp + 0x18], 0x00000007
			jmp VinylMenuCodeCave2Exit

		specials :
			mov dword ptr ds : [edi + 0x15C], 0xB715070A
			mov ebp, 0x55778E5A
			mov dword ptr ds : [esp + 0x18], 0x0000008
			jmp VinylMenuCodeCave2Exit

		ifabove :
			jmp VinylMenuCodeCave2ifAbove
	}

}

void __declspec(naked) ShowHiddenTracksCodeCave()
{
	_asm
	{
		resetcounter :
		xor edi, edi

		letsgo :
		push edi
		mov ecx, dword ptr ds: [0x91E004] // GRaceDatabase::mObj
		call GRaceDatabase_GetRaceParameters

		test eax, eax
		jz check

		mov ecx, [esp + 0x10]
		push 15 // BinID
		push ecx // Region??
		push eax // Race
		mov ecx, ebp // this
		call AddToRaceList // AddToRaceList

		check :
		inc edi
		mov ecx, dword ptr ds : [0x91E004]
		call GRaceDatabase_GetRaceCount
		cmp edi, eax
		jb letsgo

		caveexit :
		jmp ShowHiddenTracksCodeCaveExit
	}

}

void __declspec(naked) GetRaceTypeCodeCave()
{
	_asm
	{
		fix_20_5_1:
			cmp dword ptr ds : [eax], 0x9C614397 // 20.5.1
			jne fix_20_8_1
			mov byte ptr ds : [eax + 0x2B], 5
			jmp originalcode

		fix_20_8_1:
			cmp dword ptr ds : [eax], 0x102E6410 // 20.8.1
			jne fix_20_1_1
			mov byte ptr ds : [eax + 0x2B], 8
			jmp originalcode

		fix_20_1_1:
			cmp dword ptr ds : [eax], 0xF984278B // 20.1.1
			jne fix_20_2_1
			mov byte ptr ds : [eax + 0x2B], 1
			jmp originalcode

		fix_20_2_1:
			cmp dword ptr ds : [eax], 0x2D442AD8 // 20.2.1
			jne fix_20_2_2
			mov byte ptr ds : [eax + 0x2B], 0
			jmp originalcode

		fix_20_2_2:
			cmp dword ptr ds : [eax], 0xC2AC6CB3 // 20.2.2
			jne fix_maxload
			mov byte ptr ds : [eax + 0x2B], 0
			jmp originalcode

		fix_maxload:
			cmp dword ptr ds : [eax], 0xA137ED5B // MaxLoad
			jne originalcode
			mov byte ptr ds : [eax + 0x2B], 1
			jmp originalcode

		originalcode:
			movsx eax, byte ptr ds:[eax + 0x2B]
			pop esi
			retn
	}
}

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

void __declspec(naked) HeliBountyFixCodeCave()
{
	_asm
	{
			push 0x8915DC //copheli
			call stringhash32
			add esp, 4
			cmp esi, eax
			jne originalcode
			push 0x4EE07213 // Federal Pursuit Vehicle text
			jmp AnnounceBountyReward

		originalcode :
			push 0x891238 //copsuv
			jmp HeliBountyFixCodeCaveExit
	}
}

void __declspec(naked) HeliBountyFixCodeCave2()
{
	_asm
	{
		// Simple method: Replaces any bounty <= 10 to 100,000.
			cmp eax, 0x0A // 10
			jg originalcode
			mov eax, 0x000186A0 // 100000

		originalcode :
			mov dword ptr ds: [esi + 0xF4], eax

			jmp HeliBountyFixCodeCave2Exit

	}
}

void __declspec(naked) X10FixCodeCave()
{
	_asm
	{
			mov edi, [esi + 0x98]
			cmp edi, 0x0A // If heat level is 10 or more
			jl originalcode // If less, do nothing

			mov edi, 0x09 // Read x9 Bounty Reward

		originalcode:
			jmp X10FixCodeCaveExit
	}
}

void __declspec(naked) X10FixCodeCave2()
{
	_asm
	{
		//Simple Method: Replaces any Combo Timer >=120s to 10s.
			cmp ecx, 0x42F00000 // if 120 seconds or higher
			jl originalcode
			mov ecx, 0x41200000 // 10 seconds

		originalcode:
			mov dword ptr ds: [esi + 0xEC], ecx

			jmp X10FixCodeCave2Exit
	}
}

void __declspec(naked) VTCodeCave()
{
	_asm
	{
		mov VTecx, ecx

		mov eax, [edi + 0x08]

		mov ecx, VTRed
		mov	dword ptr ds : [eax + 0xC0], ecx //red

		mov ecx, VTGreen
		mov	dword ptr ds : [eax + 0xc4], ecx //green

		mov ecx, VTBlue
		mov	dword ptr ds : [eax + 0xc8], ecx //blue

		mov ecx, VTColorBloom
		mov dword ptr ds : [eax + 0xd0], ecx //color bloom

		mov ecx, VTSaturation
		mov dword ptr ds : [eax + 0xd4], ecx //saturation

		mov ecx, VTBlackBloom
		mov	dword ptr ds : [eax + 0xdc], ecx //black bloom

		fld dword ptr [eax + 0xC4]

		mov ecx, VTecx
		jmp VTCodeCaveExit
	}
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

void __declspec(naked) ReplayBlacklistFixCodeCave()
{
	_asm
	{
		mov dword ptr ds: [0x91CA3C],edx
		push 0x0089EB30
		jmp ReplayBlacklistFixCodeCaveExit
	}
}

void __declspec(naked) CarSkinFixCodeCave()
{
	_asm
	{
		cmp edi, 1
		jl caveexit2
		cmp edi, 4
		jg fixskinid

		caveexit:
			mov esi,ecx
			mov byte ptr ds: [esi+07], 1
			jmp CarSkinFixCodeCaveExit

		fixskinid: // swap around DUMMY_SKIN/WHEEL2,3 and 4
			sub edi, 03
			cmp edi, 04
			jg fixskinid
			jmp caveexit

		caveexit2:
			jmp CarSkinFixCodeCaveExit2
	}
}

void __declspec(naked) TrackOptionsHeatLevelCodeCave()
{
	_asm
	{
		mov ecx, edi
		call UIQRTrackOptions_SetupTollbooth

		originalcode :
			pop esi
			pop ebp
			pop edi
			mov ecx, [esp + 0x08]
			mov fs : [00000000], ecx
			add esp, 0x14
			retn
	}
}

void __declspec(naked) TrackOptionsHeatLevelCodeCave2()
{
	_asm
	{
		mov ecx, esi
		call UIQRTrackOptions_SetupTollbooth

		originalcode :
			mov ecx, [esp + 0x14]
			pop edi
			pop esi
			pop ebp
			mov fs : [00000000], ecx
			add esp, 0x14
			retn
	}
}

int(__thiscall *cFrontendDatabase_GetQuickRaceSettings)(DWORD *, signed int) = (int(__thiscall*)(DWORD *, signed int))0x0056DBE0;
unsigned int(__cdecl *FEngSetLanguageHash)(DWORD FEString, unsigned int StringHash) = (unsigned int(__cdecl*)(DWORD, unsigned int))0x515C00;

int __fastcall CopDensity_Act(BYTE *_this, void *Unused, const char *unk, unsigned int a3)
{
	BYTE *v4; // esi
	__int8 v5; // ebx
	int v6; // eax
	int FEDatabase = 0x91cf90;

	v4 = _this;
	v5 = *(__int8 *)(cFrontendDatabase_GetQuickRaceSettings((DWORD *)*(DWORD *)FEDatabase, 11) + 9);

	if (*(unsigned char*)(*(DWORD*)FEDatabase + 0x18) == 8)
	{
		if (a3 == 0x9120409E && --v5 < 1)
		{
			v5 = 3;
		}
		else if (a3 == 0xB5971BF1 && ++v5 > 3)
		{
			v5 = 1;
		}
	}

	else
	{
		if (a3 == 0x9120409E && --v5 < 0)
		{
			v5 = 3;
		}
		else if (a3 == 0xB5971BF1 && ++v5 > 3)
		{
			v5 = 0;
		}
	}
	
	*(BYTE *)(cFrontendDatabase_GetQuickRaceSettings((DWORD *)*(DWORD *)FEDatabase, 11) + 9) = v5;

	// Toggle cops option according to cop density
	if (v5 != 0) *(BYTE *)(cFrontendDatabase_GetQuickRaceSettings((DWORD *)*(DWORD *)FEDatabase, 11) + 12) = 1;
	else *(BYTE *)(cFrontendDatabase_GetQuickRaceSettings((DWORD *)*(DWORD *)FEDatabase, 11) + 12) = 0;

	v6 = *(DWORD *)v4;
	v4[42] = 1;
	(*(void(__thiscall **)(BYTE *, unsigned int))(v6 + 56))(v4, a3); // FEToggleWidget_BlinkArrows
	return (*(int(__thiscall **)(BYTE *))(*(DWORD *)v4 + 12))(v4); // CopDensity_Draw
}

int __fastcall CopDensity_Draw(DWORD *_this, void *Unused)
{
	DWORD *v1; // edi
	signed int v2; // esi
	int FEDatabase = 0x91cf90;

	if (*(unsigned char*)(*(DWORD*)FEDatabase + 0x18) == 8)
	{
		*(unsigned char*)(*(DWORD*)FEDatabase + 0xE8) = 1;
	}

	v1 = _this;
	v2 = 0;
	switch (*(unsigned __int8 *)(cFrontendDatabase_GetQuickRaceSettings((DWORD *)*(DWORD *)FEDatabase, 11) + 9))
	{
	case 0u:
		v2 = 0x8CDC3937; // None
		break;
	case 1u:
		v2 = 0x73C615A3; // Minimum
		break;
	case 2u:
		v2 = 0xA2CCA838; // Moderate
		break;
	case 3u:
		v2 = 0x61D1C5A5; // Maximum
		break;
	default:
		break;
	}
	FEngSetLanguageHash(v1[12], v2);
	return FEngSetLanguageHash(v1[11], 0x251A34F8);
}

int(__thiscall *GRaceCustom_SetAttribute)(DWORD *_this, unsigned int AttribHash, signed int *ValuePtr, int Unk) = (int(__thiscall*)(DWORD*, unsigned int, signed int*, int))0x5E74C0;

int __fastcall GRaceCustom_SetCopDensity(DWORD *_this, DWORD* Unused, signed int a1)
{
	int result; // eax

	if (a1)
	{
		if (a1 == 1)
		{
			a1 = 66;
			result = GRaceCustom_SetAttribute(_this, 0xDBC08D32, &a1, 0);
		}
		else
		{
			if (a1 == 2)
				a1 = 100;
			else
				a1 = 0;
			result = GRaceCustom_SetAttribute(_this, 0xDBC08D32, &a1, 0);
		}
	}
	else
	{
		a1 = 33;
		result = GRaceCustom_SetAttribute(_this, 0xDBC08D32, &a1, 0);
	}
	return result;
}

void __declspec(naked) GRaceCustomHeatLevelCodeCave()
{
	_asm
	{
		movzx edx, byte ptr ds: [edi + 0x9]
		push edx
		mov ecx, esi
		call GRaceCustom_SetCopDensity
		movzx eax, byte ptr ds : [edi + 0x8]
		push eax
		jmp GRaceCustomHeatLevelCodeCaveExit
	}
}

void __declspec(naked) SplitScreen_React()
{
	
	_asm
	{
		cmp dword ptr ds : [esp + 0x8], 0x0C407210
		jnz funcexit
		mov dword ptr ds : [0x9B9E68], 2
		
		funcexit:
			retn 0x14
	}
}

void __declspec(naked) SplitScreenCodeCave()
{
	_asm
	{
		push 0x4C
		call j_malloc
		mov edi, eax
		add esp, 04
		mov dword ptr ds: [esp + 0x8], edi
		test edi, edi
		mov dword ptr ds: [esp + 0x14], 00000003
		je ssjump1
		push 0
		push 0x841D518A
		push 0xF365B5F5
		mov ecx, edi
		call IconOption_Create
		push ebx
		mov ebx, 0x901400
		mov dword ptr ds: [edi], ebx
		pop ebx
		jmp ssjump2

		ssjump1:
			xor edi, edi

		ssjump2:
			mov ecx, esi
			push edi
			mov[esp + 0x18], 0xFFFFFFFF
			call IconScrollerMenu_AddOption


		originalcode:
			mov eax, [esi + 0x10]
			push eax
			call FEngGetLastButton

		caveexit:
		jmp SplitScreenCodeCaveExit
	}
}

void __declspec(naked) MaxPerfButtonCodeCave()
{
	_asm
	{
		mov esi, [esi + 0x10]
		push 0x8805E665
		push esi
		call FEObject_FindObject
		push 0x43340000 // 180.0
		push 0xC3710000 // -241.0
		push eax // object
		call FE_Object_SetCenter
		push ecx // setcenter moves object to ecx??
		call FEngSetVisible
		add esp, 0x18
		pop esi
		add esp, 0x40
		retn
	}
}

void __declspec(naked) DDayFixCodeCaveResumeCareer()
{
	_asm
	{
		call GRaceDatabase_GetScoreInfo
		test al,al
		jnz StartFreeRoamOrFinalPursuit

		mov eax, dword ptr ds: [0x91CF90] // FEDatabase
		test eax,eax
		jz StartDDay
		mov eax, [eax + 0x10]
		test eax, eax
		jz StartDDay
		movzx eax, byte ptr ds : [eax + 0xB0]
		cmp eax, 16
		jl StartFreeRoamOrFinalPursuit

		StartDDay:
			push 0x57F5B2
			retn

		StartFreeRoamOrFinalPursuit:
			push 0x57F5B6
			retn
	}
}

void __declspec(naked) DDayFixCodeCaveCareerCribNotificationMessage()
{
	_asm
	{
		mov eax, dword ptr ds : [0x91CF90] // FEDatabase
		test eax, eax
		jz StartDDay
		mov eax, [eax + 0x10]
		test eax, eax
		jz StartDDay
		movzx eax, byte ptr ds: [eax + 0xB0]
		cmp eax, 16
		jl StartFreeRoamOrFinalPursuit

		StartDDay:
			call GRaceDatabase_GetNextDDayRace
			push 0x531FD6
			retn

		StartFreeRoamOrFinalPursuit :
			push 0x532015
			retn
	}
}

int Return0Hook()
{
	return 0;
}

float Return0fHook()
{
	return 0.0f;
}

void Init()
{
	CIniReader iniReader("NFSMWExtraOptionsSettings.ini");

	// Hotkeys
	hotkeyToggleForceHeat = iniReader.ReadInteger("Hotkeys", "HeatLevelHack", 118); //F7
	hotkeyForceHeatLevel = iniReader.ReadInteger("Hotkeys", "ChangeHeatLevel", 33); //PageUp
	hotkeyToggleCops = iniReader.ReadInteger("Hotkeys", "ToggleCops", 145); // Scroll Lock
	hotkeyToggleCopLights = iniReader.ReadInteger("Hotkeys", "CopLights", 79); // O
	hotkeyToggleHeadlights = iniReader.ReadInteger("Hotkeys", "Headlights", 72); // H
	hotkeyCarHack = iniReader.ReadInteger("Hotkeys", "FreezeCar", 115); //F4
	hotkeyFreezeCamera = iniReader.ReadInteger("Hotkeys", "FreezeCamera", 19); // Pause/Break
	hotkeyUnlockAllThings = iniReader.ReadInteger("Hotkeys", "UnlockAllThings", 116); //F5
	hotkeyAutoDrive = iniReader.ReadInteger("Hotkeys", "AutoDrive", 117); //F6
	EnableSaveLoadHotPos = iniReader.ReadInteger("Hotkeys", "EnableSaveLoadHotPos", 0) == 1;

	// LapControllers
	minLaps = iniReader.ReadInteger("LapControllers", "Minimum", 0);
	maxLaps = iniReader.ReadInteger("LapControllers", "Maximum", 127);
	maxLapsRandomQR = iniReader.ReadInteger("LapControllers", "RandomRace", 127);
	unfreezeKO = iniReader.ReadInteger("LapControllers", "UnfreezeKO", 1) == 1;

	// OpponentControllers
	minOpponents = iniReader.ReadInteger("OpponentControllers", "Minimum", 0);
	maxOpponents = iniReader.ReadInteger("OpponentControllers", "Maximum", 6);
	maxOpponentsRandomQR = iniReader.ReadInteger("OpponentControllers", "RandomRace", 6);
	MaxUniqueOpponentCars = iniReader.ReadInteger("OpponentControllers", "MaxUniqueOpponentCars", 3);
	MoreCarsForOpponents = iniReader.ReadInteger("OpponentControllers", "MoreCarsForOpponents", 0) == 1;
	CarbonStyleRandomCars = iniReader.ReadInteger("OpponentControllers", "CarbonStyleRandomCars", 0) == 1;

	// TrafficControllers
	lowTraffic = iniReader.ReadInteger("TrafficControllers", "Low", 10);
	medTraffic = iniReader.ReadInteger("TrafficControllers", "Medium", 30);
	highTraffic = iniReader.ReadInteger("TrafficControllers", "High", 50);

	// Menu
	ShowTollbooth = iniReader.ReadInteger("Menu", "ShowTollbooth", 1) == 1;
	ShowChallenge = iniReader.ReadInteger("Menu", "ShowChallenge", 0) == 1;
	ShowMoreRaceOptions = iniReader.ReadInteger("Menu", "ShowMoreRaceOptions", 1) == 1;
	ShowMoreCustomizationOptions = iniReader.ReadInteger("Menu", "ShowMoreCustomizationOptions", 0) == 1;
	HideOnline = iniReader.ReadInteger("Menu", "ShowOnline", 1) == 0;
	ShowOnlineOpts = iniReader.ReadInteger("Menu", "ShowOnlineOpts", 0) == 1;
	ShowSubs = iniReader.ReadInteger("Menu", "ShowSubs", 0) == 1;
	EnablePresetAndDebugCars = iniReader.ReadInteger("Menu", "ShowMoreCarCategories", 0) == 1;
	moreVinyls = iniReader.ReadInteger("Menu", "ShowSpecialVinyls", 1) == 1;
	ShowDebugCarCustomize = iniReader.ReadInteger("Menu", "ShowDebugCarCustomize", 0) == 1;
	ShowDebugEventID = iniReader.ReadInteger("Menu", "ShowDebugEventID", 0) == 1;
	ShowTimeOfDay = iniReader.ReadInteger("Menu", "ShowTimeOfDay", 0) == 1;
	ShowHiddenTracks = iniReader.ReadInteger("Menu", "ShowHiddenTracks", 0);
	BetterRandomRaces = iniReader.ReadInteger("Menu", "BetterRandomRaces", 0) == 1;
	maxBlacklist = iniReader.ReadInteger("Menu", "MaximumBlacklistRival", 15);
	ReplayBlacklistRaces = iniReader.ReadInteger("Menu", "ReplayBlacklistRaces", 0) == 1;
	csBlacklist = iniReader.ReadInteger("Menu", "ChallengeSeriesBlacklist", 19);
	randomizeCount = iniReader.ReadInteger("Menu", "RandomizeCount", 30);
	SplashScreenTimeLimit = iniReader.ReadFloat("Menu", "SplashScreenTimeLimit", 30.0f);
	CarSelectTireSteerAngle = iniReader.ReadFloat("Menu", "CarSelectTireSteerAngle", 21.6723f);
	GarageZoom = iniReader.ReadInteger("Menu", "ShowcaseCamInfiniteZoom", 0) == 1;
	GarageRotate = iniReader.ReadInteger("Menu", "ShowcaseCamInfiniteRotation", 0) == 1;
	GarageShowcase = iniReader.ReadInteger("Menu", "ShowcaseCamAlwaysEnable", 0) == 1;
	AllowLongerProfileNames = iniReader.ReadInteger("Menu", "AllowLongerProfileNames", 1) == 1;
	ExOptsTeamTakeOver = iniReader.ReadInteger("Menu", "DisableTakeover", 0) == 0;

	// Gameplay
	EnableCameras = iniReader.ReadInteger("Gameplay", "EnableHiddenCameraModes", 0) == 1;
	EnableDebugWorldCamera = iniReader.ReadInteger("Gameplay", "EnableDebugCamera", 0) == 1;
	NFSU2StyleLookBackCamera = iniReader.ReadInteger("Gameplay", "ShowHUDOnLookBack", 0) == 1;
	gameSpeed = iniReader.ReadFloat("Gameplay", "GameSpeed", 1.0f);
	WorldAnimationSpeed = iniReader.ReadFloat("Gameplay", "WorldAnimationSpeed", 45.0f);
	DebugCameraTurboSpeed = iniReader.ReadFloat("Gameplay", "DebugCamTurboSpeed", 3.06f);
	DebugCameraSuperTurboSpeed = iniReader.ReadFloat("Gameplay", "DebugCamSuperTurboSpeed", 7.16f);
	CarScale = iniReader.ReadFloat("Gameplay", "CarScale", 1.0f);
	removeSceneryGroupDoor = iniReader.ReadInteger("Gameplay", "RemoveOldBridgeBarrier", 0) == 1;
	removePlayerBarriers = iniReader.ReadInteger("Gameplay", "RemoveNeonBarriers", 0) == 1;
	CarbonStyleRaceProgress = iniReader.ReadInteger("Gameplay", "ShowPercentOn1LapRaces", 1) == 1;
	StartingCashDWORD = iniReader.ReadInteger("Gameplay", "StartingCash", 0);
	UnlockAllThings = iniReader.ReadInteger("Gameplay", "UnlockAllThings", 0) == 1;
	ForceBlackEdition = iniReader.ReadInteger("Gameplay", "ForceBlackEdition", 1) == 1;
	eatSomeBurgers = iniReader.ReadInteger("Gameplay", "UnlockBurgerKingChallenge", 0) == 1;
	EnableMaxPerfOnShop = iniReader.ReadInteger("Gameplay", "EnableMaxPerformanceOnShop", 1) == 1;
	ShowAllCarsInFE = iniReader.ReadInteger("Gameplay", "ShowAllCarsInFE", 1);
	// ShowPresetCarsInFE = iniReader.ReadInteger("Gameplay", "ShowPresetCarsInFE", 0);
	NoCatchUp = iniReader.ReadInteger("Gameplay", "NoCatchUp", 0) == 1;
	NoRevLimiter = iniReader.ReadInteger("Gameplay", "NoRevLimiter", 0) == 1;
	NosTrailRepeatCount = iniReader.ReadInteger("Gameplay", "NosTrailRepeatCount", 8);
	SelectableMarkerCount = iniReader.ReadInteger("Gameplay", "SelectableMarkerCount", 2);
	PurchasedCarLimit = iniReader.ReadInteger("Gameplay", "PurchasedCarLimit", 10);
	SBRechargeTime = iniReader.ReadFloat("Gameplay", "SBRechargeTime", 25.0f);
	SBRechargeSpeedLimit = iniReader.ReadFloat("Gameplay", "SBRechargeSpeedLimit", 100.0f);
	SBMassMultiplier = iniReader.ReadFloat("Gameplay", "SBMassMultiplier", 2.0f);

	// Pursuit
	EnableHeatLevelOverride = iniReader.ReadInteger("Pursuit", "HeatLevelOverride", 0) == 1;
	MinHeatLevel = iniReader.ReadFloat("Pursuit", "MinimumHeatLevel", 1.00f);
	MaxHeatLevel = iniReader.ReadFloat("Pursuit", "MaximumHeatLevel", 10.00f);
	PursuitActionMode = iniReader.ReadInteger("Pursuit", "PursuitActionMode", 0) == 1;
	ShowPursuitCops = iniReader.ReadInteger("Pursuit", "ShowPursuitCops", 1) == 1;
	ShowNonPursuitCops = iniReader.ReadInteger("Pursuit", "ShowNonPursuitCops", 0) == 1;
	CarbonStyleBustedScreen = iniReader.ReadInteger("Pursuit", "ShowBustedScreenFrame", 1) == 0;
	UncensoredBustedScreen = iniReader.ReadInteger("Pursuit", "UncensoredBustedScreen", 0) == 1;
	SpeedingLimit = iniReader.ReadFloat("Pursuit", "SpeedingLimit", 67.00f);
	ExcessiveSpeedingLimit = iniReader.ReadFloat("Pursuit", "ExcessiveSpeedingLimit", 80.50f);
	RecklessDrivingLimit = iniReader.ReadFloat("Pursuit", "RecklessDrivingLimit", 89.40f);

	// VisualTreatment
	EnableVTOverride = iniReader.ReadInteger("VisualTreatment", "EnableVisualTreatmentOverride", 0) == 1;
	VTRed = iniReader.ReadFloat("VisualTreatment", "Red", 0.88f);
	VTGreen = iniReader.ReadFloat("VisualTreatment", "Green", 0.8f);
	VTBlue = iniReader.ReadFloat("VisualTreatment", "Blue", 0.44f);
	VTColorBloom = iniReader.ReadFloat("VisualTreatment", "ColorBloomIntensity", 1.75f);
	VTSaturation = iniReader.ReadFloat("VisualTreatment", "Saturation", 0.5f);
	VTBlackBloom = iniReader.ReadFloat("VisualTreatment", "BlackBloomIntensity", 0.9f);

	// Weather
	AlwaysRain = iniReader.ReadInteger("Weather", "AlwaysRain", 0) == 1;
	RainAmount = iniReader.ReadFloat("Weather", "GeneralRainAmount", 1.0f);
	RoadReflection = iniReader.ReadFloat("Weather", "RoadReflectionAmount", 1.0f);
	FallingRainSize = iniReader.ReadFloat("Weather", "RainSize", 0.01f);
	RainIntensity = iniReader.ReadFloat("Weather", "RainIntensity", 0.45f);
	RainXing = iniReader.ReadFloat("Weather", "RainCrossing", 0.02f);
	RainFallSpeed = iniReader.ReadFloat("Weather", "RainSpeed", 0.03f);
	RainGravity = iniReader.ReadFloat("Weather", "RainGravity", 0.35f);

	// Fixes
	WheelFix = iniReader.ReadInteger("Fixes", "DisappearingWheelsFix", 1) == 1;
	ExperimentalSplitScreenFix = iniReader.ReadInteger("Fixes", "ExperimentalSplitScreenFix", 0) == 1;
	HelicopterFix = iniReader.ReadInteger("Fixes", "HelicopterBountyFix", 1) == 1;
	X10Fix = iniReader.ReadInteger("Fixes", "ZeroBountyFix", 1) == 1;
	//TimeBugFix = iniReader.ReadInteger("Fixes", "TimeBugFix", 0) == 1;
	CarSkinFix = iniReader.ReadInteger("Fixes", "CarSkinFix", 0) == 1;
	ImmobileColFix = iniReader.ReadInteger("Fixes", "ImmobileCollisionsFix", 0) == 1;
	DDayFix = iniReader.ReadInteger("Fixes", "DDayFix", 1) == 1;

	// Misc
	WindowedMode = iniReader.ReadInteger("Misc", "WindowedMode", 0);
	SkipMovies = iniReader.ReadInteger("Misc", "SkipMovies", 0) == 1;
	SkipNISs = iniReader.ReadInteger("Misc", "SkipNISs", 0) == 1;
	EnableSound = iniReader.ReadInteger("Misc", "EnableSound", 1) == 1;
	EnableMusic = iniReader.ReadInteger("Misc", "EnableMusic", 1) == 1;
	ShowMessage = iniReader.ReadInteger("Misc", "ShowMessage", 1) == 1;
	SkipCareerIntro = iniReader.ReadInteger("Misc", "SkipCareerIntro", 0) == 1;
	AllowMultipleInstances = iniReader.ReadInteger("Misc", "AllowMultipleInstances", 0) == 1;
	ThreadDelay = iniReader.ReadInteger("Misc", "ThreadDelay", 5);
	ExpandMemoryPools = iniReader.ReadInteger("Misc", "ExpandMemoryPools", 0) == 1;

	// Limit values to fix increment & decrement behaviour breaking

	// loop around values
	minLaps %= 128; 
	maxLaps %= 128;
	maxLapsRandomQR %= 128;

	minOpponents %= 16;
	maxOpponents %= 16;
	maxOpponentsRandomQR %= 16;

	headlightsMode = 1;

	randomizeCount %= 501;

	lowTraffic %= 101;
	medTraffic %= 101;
	highTraffic %= 101;

	SelectableMarkerCount--;
	SelectableMarkerCount %= 6;
	SelectableMarkerCount++;

	PurchasedCarLimit %= 26;
	PurchasedCarLimit--;

	// Allow Multiple Instances
	if (AllowMultipleInstances)
	{
		injector::WriteMemory<unsigned char>(0x6665A1, 0xEB, true);
	}

	// Expand Memory Pools (ty Berkay)
	if (ExpandMemoryPools)
	{
		injector::WriteMemory<short>(0x5F7396, 0x2C80, true); // GManager::PreloadTransientVaults (0x2C8000)
		injector::WriteMemory<short>(0x5F73B2, 0x2C80, true);

		injector::WriteMemory<unsigned char>(0x665FDC, 0x80, true); // InitializeEverything (0x8000)
		
		injector::WriteMemory<DWORD>(0x8F5790, 0x0BE6E0, true); // FEngMemoryPoolSize (InitFEngMemoryPool)
		injector::WriteMemory<DWORD>(0x8F7EF0, 0x01CC00, true); // CarLoaderPoolSizes

		// To prevent some crashes
		if (MaxUniqueOpponentCars > 7) MaxUniqueOpponentCars = 7;
	}

	// Remove 1-8 Lap Restriction
	injector::WriteMemory<unsigned char>(0x7AC3EC, minLaps, true); // Decrease lap count min lap controller
	injector::WriteMemory<unsigned char>(0x7AC3F0, maxLaps, true); // Decrease lap count max lap controller
	injector::WriteMemory<unsigned char>(0x7AC405, minLaps, true); // Increase lap count min lap controller
	injector::WriteMemory<unsigned char>(0x7AC401, maxLaps, true); // Increase lap count max lap controller
	// LAN Lobby
	injector::WriteMemory<unsigned char>(0x556bb5, maxLaps, true);
	injector::WriteMemory<unsigned char>(0x556bde, maxLaps, true);
	injector::WriteMemory<unsigned char>(0x556bc4, minLaps, true);
	injector::WriteMemory<unsigned char>(0x556bcf, minLaps, true);

	// Number of bots - Note: 4th and beyond opponents gets bugged cars.
	injector::WriteMemory<unsigned char>(0x7abff9, minOpponents, true); // Decrease opponents count min opponent controller
	injector::WriteMemory<unsigned char>(0x7abffd, maxOpponents + 0x01, true); // Decrease opponents count max opponent controller
	injector::WriteMemory<unsigned char>(0x7ac01a, minOpponents, true); // Increase opponents count min opponent controller
	injector::WriteMemory<unsigned char>(0x7ac00e, maxOpponents + 0x01, true); // Increase opponents count max opponent controller

	// Maximum Unique Opponent Cars (Increasing this value also increases opponent cars' variety)
	injector::WriteMemory<unsigned char>(0x61DE27, MaxUniqueOpponentCars, true);

	// Random Race Laps and Opponents
	injector::WriteMemory<unsigned char>(0x56DCC1, maxOpponentsRandomQR, true); // Random Race Max Opponent Controller
	injector::WriteMemory<unsigned char>(0x56DCE9, maxLapsRandomQR, true); // Random Race Max Lap Controller

	// Traffic Density Controllers (0-100)
	injector::WriteMemory<unsigned char>(0x56dd9E, lowTraffic, true); // Low (10)
	injector::WriteMemory<unsigned char>(0x56dd9A, medTraffic, true); // Medium (30)
	injector::WriteMemory<unsigned char>(0x56dd96, highTraffic, true); // High (50)

	// Blacklist Max Rival Controllers
	if (maxBlacklist == 0) // Unlimited blacklists
	{
		// Enable BL 0 for decrement
		injector::MakeNOP(0x551455, 7, true);
		injector::WriteMemory<unsigned char>(0x551455, 0x31, true);
		injector::WriteMemory<unsigned char>(0x551456, 0xD2, true);

		// Enable BL 0 for increment
		injector::MakeNOP(0x55a571, 7, true);
		injector::WriteMemory<unsigned char>(0x55a571, 0x31, true);
		injector::WriteMemory<unsigned char>(0x55a572, 0xC0, true);

		// Remove Maximum Restriction
		injector::WriteMemory<unsigned char>(0x55a56c, 0xeb, true);

		// Remove Minimum Restriction
		injector::MakeNOP(0x551460, 5, true);
	}

	else // Normal behaviour
	{
		injector::WriteMemory<unsigned char>(0x551461, maxBlacklist, true);
		injector::WriteMemory<unsigned char>(0x551473, maxBlacklist, true);
		injector::WriteMemory<unsigned char>(0x55a555, maxBlacklist, true);
		injector::WriteMemory<unsigned char>(0x55a566, maxBlacklist, true);
	}

	// Remove restrictions for new save games
	if (maxBlacklist != 15)
	{
		injector::MakeNOP(0x55a556, 6, true);
		injector::MakeNOP(0x551433, 6, true);
	}

	// Replay Blacklist Races
	if (ReplayBlacklistRaces)
	{
		injector::MakeNOP(0x5FD1B9, 2, true); //allow challenging defeated rival
		injector::WriteMemory<unsigned char>(0x52F202, 0x00, true); // remove defeated flag
		
		injector::WriteMemory<unsigned char>(0x603269, 0xEB, true); // fix boss race stats getting reset

		injector::MakeJMP(0x6243EF, ReplayBlacklistFixCodeCave, true);
		injector::MakeNOP(0x6243C8, 6, true); // visuals fix

		injector::WriteMemory<DWORD>(0x60075C, 0x91CA3C, true);
		injector::MakeRangedNOP(0x600760, 0x60076B, true); // Graphics and name fixes
	}

	// Challenge Series Bin
	injector::WriteMemory<unsigned char>(0x7ae969, csBlacklist, true);

	// Fix Unexistent Bin Crash?? (EXPERIMENTAL)
	if (maxBlacklist != 15 || csBlacklist != 19)
	{
		injector::WriteMemory<unsigned char>(0x5e39c6, 0x01, true);
		injector::WriteMemory<unsigned char>(0x5e39c7, 0xC8, true);
	}

	// Race Speed Multiplier
	injector::WriteMemory<float>(0x901B1C, gameSpeed, true);

	// Randomize Count
	injector::WriteMemory<int>(0x7b4648, randomizeCount, true);

	// Menu Front Tire Angle
	injector::WriteMemory<float>(0x905E20, CarSelectTireSteerAngle, true);

	// NOS Trail Repeat Count NosTrailRepeatCount
	injector::WriteMemory<int>(0x903694, NosTrailRepeatCount, true);

	// Unfreeze Lap Count for Lap Knockout
	if (unfreezeKO)
	{
		injector::MakeNOP(0x7acf83, 3, true); // Enable editing of lap count for KO
		injector::MakeNOP(0x7ac04b, 3, true); // Don't change laps when opponent count changed
	}

	// Remove Race Barriers and Locked Areas
	if (removePlayerBarriers)
	{
		injector::WriteMemory<unsigned char>(0x8B282B, 0x4A, true);
		injector::WriteMemory<unsigned char>(0x8B2838, 0x4A, true);
		injector::WriteMemory<unsigned char>(0x8B2844, 0x4A, true);

		// Always show whole map
		injector::WriteMemory<unsigned char>(0x89B15D, 0, true);
		injector::WriteMemory<unsigned char>(0x57bf01, 0xEB, true);
		injector::WriteMemory<unsigned char>(0x57bf19, 0xEB, true);
		injector::WriteMemory<unsigned char>(0x57bf5f, 0xEB, true);
		injector::WriteMemory<unsigned char>(0x57bf7c, 0xEB, true);
	}

	// Remove Old Bridge Area Barrier
	if (removeSceneryGroupDoor)
	{
		injector::WriteMemory<unsigned char>(0x8B2817, 0x20, true);
		injector::WriteMemory<unsigned char>(0x8B2810, 0x54, true);
	}

	// Add Tollbooth and Challenge options to quick race menu
	injector::MakeNOP(0x7aa924, 6, true);
	injector::MakeNOP(0x7AA97F, 6, true);
	injector::MakeJMP(0x7aaa5d, 0x7AAAEF, true);
	injector::WriteMemory<DWORD>(0x7AAB1A, 0x213CC8D1, true); // Challenge name hash
	injector::WriteMemory<DWORD>(0x7AAB1F, 0x9A962438, true); // Challenge icon hash
	injector::WriteMemory<unsigned char>(0x7AAB33, 0x08, true); // Challenge game mode

	if (!ShowTollbooth) // Remove Tollbooth if the option is 0
		injector::MakeJMP(0x7AAA11, 0x7AAA5D, true);

	if (!ShowChallenge) // Remove Challenge if the option is 0
		injector::MakeJMP(0x7AAAEF, 0x7AAB3B, true);

	// Show More Race Options
	if (ShowMoreRaceOptions)
	{
		// CopDensity::vtbl
		injector::WriteMemory<DWORD>(0x901300, 0x7B4C20, true); //Destroyer
		injector::WriteMemory(0x901304, &CopDensity_Act, true); // Act
		injector::WriteMemory<DWORD>(0x901308, 0x574540, true); // FEToggleWidget::CheckMouse
		injector::WriteMemory(0x90130C, &CopDensity_Draw, true); // Draw
		injector::WriteMemory<DWORD>(0x901310, 0x589390, true); // FEToggleWidget::Position
		injector::WriteMemory<DWORD>(0x901314, 0x5745B0, true); // FEToggleWidget::Show
		injector::WriteMemory<DWORD>(0x901318, 0x5745F0, true); // FEToggleWidget::Hide
		injector::WriteMemory<DWORD>(0x90131C, 0x589410, true); // FEToggleWidget::Enable
		injector::WriteMemory<DWORD>(0x901320, 0x589440, true); // FEToggleWidget::Disable
		injector::WriteMemory<DWORD>(0x901324, 0x574630, true); // FEToggleWidget::SetFocus
		injector::WriteMemory<DWORD>(0x901328, 0x574660, true); // FEToggleWidget::UnsetFocus
		injector::WriteMemory<DWORD>(0x90132C, 0x51A6C0, true); // FEStatWidget::SetPos
		injector::WriteMemory<DWORD>(0x901330, 0x574480, true); // FEStatWidget::SetPosX
		injector::WriteMemory<DWORD>(0x901334, 0x5744E0, true); // FEStatWidget::SetPosY
		injector::WriteMemory<DWORD>(0x901338, 0x574670, true); // FEToggleWidget::BlinkArrows

		// Repurpose Tollbooth menu to add Heat Level option
		injector::WriteMemory<unsigned char>(0x7AD359, 0xEB, true); // Jump to menu option
		injector::WriteMemory<DWORD>(0x7AD3C3, 0x901300, true); // Replace Traffic level with heat level
		// injector::WriteMemory<DWORD>(0x007AD3C3, 0x8B78C8, true); // Replace Traffic level with heat level

		// AI Opponents in tollbooth
		injector::MakeNOP(0x61DC9D, 6, true); // Enable opponents in tollbooth
		injector::MakeCALL(0x7BA9BD, 0x7ACAD0, true); // Use Sprint Menu

		injector::MakeCALL(0x7BA9B4, 0x7ACAD0, true); // Speedtrap -> Use Sprint Menu

		// Add to menus
		injector::MakeJMP(0x7ACA88, TrackOptionsHeatLevelCodeCave, true); // Circuit, Challenge & Others
		injector::MakeJMP(0x7ACC6B, TrackOptionsHeatLevelCodeCave, true); // Sprint (Speedtrap & Tollbooth)
		injector::MakeJMP(0x7AD0D2, TrackOptionsHeatLevelCodeCave2, true); // Lap KO

		// Read value before races
		injector::MakeJMP(0x56DD5E, GRaceCustomHeatLevelCodeCave, true);
	}

	// Show Hidden Tracks in QR Track select
	if (ShowHiddenTracks)
	{
		injector::MakeJMP(0x7BA650, ShowHiddenTracksCodeCave, true);
		injector::MakeJMP(0x5FAA2A, GetRaceTypeCodeCave, true);
		injector::MakeRangedNOP(0x7AAC7F, 0x7AAC9F, true);
		injector::MakeRangedNOP(0x7AACB5, 0x7AACC4, true);
	}

	// Better Random Races (more cars and tracks to choose)
	if (BetterRandomRaces)
	{
		// Make more tracks available
		injector::MakeRangedNOP(0x7BA2EE, 0x7BA354, true);

		// Make more cars available
		injector::MakeRangedNOP(0x7BA24F, 0x7BA258, true);
	}

	// Don't hide HUD on Look Back Cam
	if (NFSU2StyleLookBackCamera)
	{
		injector::WriteMemory<int>(0x4741DB, 0, true);
	}

	// Remove online option from main menu
	if (HideOnline)
		injector::WriteMemory<unsigned char>(0x5450f9, 0x46, true);

	if (ShowOnlineOpts) injector::MakeNOP(0x5290B2, 2, true); // Show Online Options menu

	// Fix headlights force on
	injector::WriteMemory<float>(0x742AB7, 0, true);
	injector::WriteMemory<float>(0x742ABF, 0, true);

	// Add Debug and Preset Categories to Car Select screen
	if (EnablePresetAndDebugCars)
	{
		injector::WriteMemory<unsigned char>(0x7BF9E9, 0x20, true);
		injector::WriteMemory<unsigned char>(0x7BF9FA, 0x20, true);
		// LAN Lobby
		injector::WriteMemory<unsigned char>(0x556787, 0x20, true);
		injector::WriteMemory<unsigned char>(0x5567bd, 0x20, true);
		injector::WriteMemory<unsigned char>(0x5567e7, 0x20, true);
		injector::WriteMemory<unsigned char>(0x556821, 0x20, true);
	}

	// Rain Options
	if (AlwaysRain)
	{
		injector::WriteMemory<unsigned char>(0x9B0A30, 0x01, true);
		injector::WriteMemory<unsigned char>(0x758293, 0xEB, true); // Road Reflection Fix??
	}
	injector::WriteMemory<float>(0x904B38, RoadReflection, true);
	injector::WriteMemory<float>(0x904B38, RoadReflection, true);
	injector::WriteMemory<float>(0x904A90, FallingRainSize, true);
	injector::WriteMemory<float>(0x904A14, RainAmount, true);
	injector::WriteMemory<float>(0x904A94, RainIntensity, true);
	injector::WriteMemory<float>(0x904A24, RainXing, true);
	injector::WriteMemory<float>(0x904A28, RainFallSpeed, true);
	injector::WriteMemory<float>(0x904A2C, RainGravity, true);

	// Options from ModLoader
	if (WindowedMode != 0)
	{
		injector::WriteMemory<unsigned char>(0x982BF0, 1, true);
	}

	if (SkipMovies)
	{
		injector::WriteMemory<unsigned char>(0x926144, SkipMovies, true);
	}

	if (SkipNISs)
	{
		injector::WriteMemory<unsigned char>(0x9260A4, SkipNISs, true);
	}

	if (!EnableSound)
	{
		injector::WriteMemory<unsigned char>(0x8F86F8, EnableSound, true);
	}

	if (!EnableMusic)
	{
		injector::WriteMemory<unsigned char>(0x8F86FC, EnableMusic, true);
	}
	if (ShowMessage)
	{
		injector::MakeCALL(0x0058F710, DialogBoxReturnValue, true);
	}
	if (SkipCareerIntro)
	{
		injector::WriteMemory<unsigned char>(0x926125, 1, true); // SkipDDayRaces
		injector::WriteMemory<unsigned char>(0x926126, 1, true); // SkipCareerIntro
	}

	// Enable hidden camera modes
	if (EnableCameras)
	{
		injector::MakeNOP(0x5A38AA, 2, true); // For C key
		injector::MakeNOP(0x51C8D9, 2, true); // Player Options Menu - Increment
		injector::MakeNOP(0x51C8AF, 2, true); // Player Options Menu - Decrement
		injector::MakeRangedNOP(0x51C95F, 0x0051C98C, true); // Clean the unused code
		injector::MakeJMP(0x51C95F, CameraNamesCodeCave, true); // Fix Names

		// Rearview Mirror Fix Addition for Widescreen Fix
		injector::MakeNOP(0x595DDD, 2, true);
		injector::MakeNOP(0x595DE2, 2, true);
		injector::MakeNOP(0x595DE7, 2, true);
		injector::MakeNOP(0x595DEC, 2, true);
		injector::WriteMemory<unsigned char>(0x595DF0, 0xFF, true);
	}

	// Splash Screen Time Limit
	injector::WriteMemory<float>(0x8F3CD0, SplashScreenTimeLimit, true);

	// Force Show Subtitles
	if (ShowSubs)
	{
		injector::MakeNOP(0x542dbc, 2, true);
		injector::MakeNOP(0x54ee78, 2, true);
		injector::MakeNOP(0x54f219, 2, true);
	}

	// Heat Level Overrides
	injector::MakeRangedNOP(0x443dc3, HeatLevelsCodeCaveExit, true); // Clean the unused code
	injector::MakeJMP(0x443dc3, HeatLevelsCodeCave, true); // Prepare the game for Ultimate force heat level hack

	// Show COMPLETED %x instead of LAP 1/1
	if (CarbonStyleRaceProgress)
	{
		injector::MakeRangedNOP(0x57AA2F, 0x57AA46, true);
		// call 0x5FAA20 (Just moved this call up)
		injector::MakeCALL(0x57AA2F, 0x5FAA20, true);
		// cmp dword ptr ds: [esi+0x3C],0x01 (if lap count = 1)
		injector::WriteMemory<unsigned int>(0x57AA34, 0x13C7E83, true);
		//je 0x57AA73 (If true, use COMPLETED %)
		injector::WriteMemory<unsigned short int>(0x57AA38, 0x3974, true);
	}

	// Burger King
	if (eatSomeBurgers)
		injector::WriteMemory<unsigned char>(0x576A8D, 0xEB, true);

	// Special Vinyl Category
	if (moreVinyls)
	{
		injector::MakeRangedNOP(0x7BC8FF, VinylMenuCodeCaveExit, true); // CustomizeSub::SetupVinylGroups
		injector::MakeRangedNOP(0x7BD26F, VinylMenuCodeCave2Exit, true); // CustomizeParts::Setup

		injector::MakeJMP(0x7BC8FF, VinylMenuCodeCave, true);
		injector::MakeJMP(0x7BD26F, VinylMenuCodeCave2, true);

		// Category Fixes
		//injector::WriteMemory<unsigned char>(0x7A5208, 0x0A, true); shows new icon
		injector::WriteMemory<unsigned char>(0x7A5823, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7AFE90, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7B600C, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7B7436, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7B75A2, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7BAEC3, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7BAF10, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7bca3c, 0x0A, true);
		injector::WriteMemory<unsigned char>(0x7c0ae7, 0x0A, true);
	}

	// Fix (ignore) starting heat level when a challenge executed from quick race screen
	injector::WriteMemory<unsigned char>(0x56DD55, 0xEB, true);

	// Unlock all things
	if (UnlockAllThings)
	{
		injector::WriteMemory<unsigned char>(0x926124, 0x01, true);
	}

	if (WheelFix) // Fix Invisible Wheels
		injector::WriteMemory<unsigned char>(0x74251D, 0x01, true);

	// Garage Hacks
	if (GarageZoom)
	{
		injector::MakeNOP(0x476B04, 5, true);
		injector::WriteMemory<unsigned char>(0x476B04, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x476B05, 0x01, true);
	}
	if (GarageRotate)
	{
		injector::MakeNOP(0x476AC3, 5, true);
		injector::WriteMemory<unsigned char>(0x476AC3, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x476AC4, 0x01, true);
	}
	if (GarageShowcase)
	{
		injector::MakeNOP(0x7A201B, 5, true);
		injector::WriteMemory<unsigned char>(0x7A201B, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x7A201C, 0x01, true);
	}

	// Force Black Edition
	if (ForceBlackEdition)
	{
		injector::MakeNOP(0x5994E6, 3, true);
		injector::WriteMemory<unsigned char>(0x5994E6, 0xB0, true);
		injector::WriteMemory<unsigned char>(0x5994E7, 0x01, true);
	}

	// World Animation Speed
	injector::WriteMemory<float>(0x904AEC, WorldAnimationSpeed, true);

	// Split-Screen Fix (Experimental)
	if (ExperimentalSplitScreenFix)
	{
		injector::WriteMemory<unsigned char>(0x7a3f68, 0x0D, true); // Fix Player 2 Ready control
		injector::WriteMemory<unsigned char>(0x666eb3, 0xEB, true); // Fix loading screen crash
		injector::MakeNOP(0x6cfc03, 2, true); // Fix black screen??

		// Add split screen to qr menu
		injector::MakeJMP(0x7AA821, SplitScreenCodeCave, true);
		// SplitScreen_vtbl
		injector::WriteMemory<DWORD>(0x901400, 0x7B4980, true);
		injector::WriteMemory(0x901404, &SplitScreen_React, true);
	}
	
	// String Replacement
	if (ExOptsTeamTakeOver)
		injector::MakeCALL(0x005A3260, GetGameVersionNumberStringHook, true);

	// Starting Cash
	injector::WriteMemory<unsigned char>(0x56D7D3, 0x81, true); // add
	injector::WriteMemory<unsigned char>(0x56D7D4, 0x41, true); // to [ecx]
	injector::WriteMemory<unsigned char>(0x56D7D5, 0x0C, true); // + 0x0c
	injector::WriteMemory<DWORD>(0x56D7D6, StartingCashDWORD, true); // Starting Cash
	injector::WriteMemory<unsigned char>(0x56D7DA, 0xC2, true); // ret 0004
	injector::WriteMemory<unsigned char>(0x56D7DB, 0x04, true);
	injector::WriteMemory<unsigned char>(0x56D7DC, 0x00, true);

	// Car size hack
	injector::WriteMemory<float>(0x737870, CarScale, true); // Length
	injector::WriteMemory<float>(0x7378A2, CarScale, true); // Width
	injector::WriteMemory<float>(0x7378D4, CarScale, true); // Height

	// Helicopter Bounty Fix
	if (HelicopterFix)
	{
		// Fix Bounty Announcement
		injector::MakeRangedNOP(0x595BED, HeliBountyFixCodeCaveExit, true);
		injector::MakeJMP(0x595BED, HeliBountyFixCodeCave, true);

		// Fix Bounty Amount
		injector::MakeRangedNOP(0x418F86, HeliBountyFixCodeCave2Exit, true);
		injector::MakeJMP(0x418F86, HeliBountyFixCodeCave2, true);
	}
	
	// X10 Bounty Fix
	if (X10Fix)
	{ 
		// Fix 0 Bounty
		injector::MakeRangedNOP(0x418F5B, X10FixCodeCaveExit, true);
		injector::MakeJMP(0x418F5B, X10FixCodeCave, true);

		// Fix combo timer
		injector::MakeRangedNOP(0x419019, X10FixCodeCave2Exit, true);
		injector::MakeJMP(0x419019, X10FixCodeCave2, true);
	}
	
	// Load Any Save Game
	// injector::WriteMemory<unsigned char>(0x5ACD4F, 0xEB, true); // Skip hash check, breaks compatibility for non-exopts users
	injector::MakeNOP(0x5ACBFA, 6, true); // Skip modded game check

	// Max Performance on Shop
	if (EnableMaxPerfOnShop)
	{
		// Make button active
		injector::MakeRangedNOP(0x7BBC4F, 0x7BBC6E, true);

		// Make button visible and give it a more proper position
		injector::MakeRangedNOP(0x7B11AB, 0x7B11C2, true);
		injector::MakeJMP(0x7B11CE, MaxPerfButtonCodeCave, true);
	}
		
	// Heat Level Override - Initialize
	if (EnableHeatLevelOverride)
	{
		// Keep heat level >x5 on car when used marker or evaded

		// CustomizeMeter::Init
		injector::WriteMemory<float>(0x7bb502, MaxHeatLevel, true); // CustomizeCategoryScreen
		injector::WriteMemory<float>(0x7b1387, MaxHeatLevel, true); // CustomizationScreenHelper
		injector::WriteMemory<float>(0x7b0c89, MaxHeatLevel, true); // CustomizeShoppingCart::Setup
		injector::WriteMemory<float>(0x7B4D7C, MaxHeatLevel, true); // UIQRCarSelect::InitStatsSliders

		// SetHeatLevel
		injector::WriteMemory<float*>(0x435079, &MaxHeatLevel, true); // AIVehicleHuman::~AIVehicleHuman
		injector::WriteMemory<float>(0x435088, MaxHeatLevel, true);

		// Safehouse car select icon stuff (HEAT_X%.0f)
		injector::WriteMemory<float*>(0x7A5B03, &MaxHeatLevel, true);
		injector::WriteMemory<float*>(0x7A5B12, &MaxHeatLevel, true);
	}

	// Visual Treatment Override
	if (EnableVTOverride)
	{
		injector::MakeRangedNOP(0x73E358, VTCodeCaveExit, true);
		injector::MakeJMP(0x73E358, VTCodeCave, true);
	}

	// Remove Busted Screen Frame
	if (CarbonStyleBustedScreen)
		injector::WriteMemory<unsigned char>(0x8948F8, 0x00, true);

	// Show Debug Car Customize
	if (ShowDebugCarCustomize)
	{
		injector::WriteMemory<DWORD>(0x544F80, 0x1AFD5BE6, true); // "My Cars"
		injector::WriteMemory<DWORD>(0x544F85, 0x74CE8C0B, true); // UI_ICON_DEBUG
		injector::MakeNOP(0x544FA6, 7, true); // UAT Fix
		injector::MakeJMP(0x5451D7, 0x544F5D, true); // Jump to unused?? duplicate menu option
		injector::WriteMemory<DWORD>(0x544F92, 0x89CADC, true); // Menu Code To Execute
		injector::WriteMemory<DWORD>(0x53CED7, 0x89FAD4, true); // "UI_DebugCarCustomize.fng"
	}

	// Show Debug Event ID
	if (ShowDebugEventID)
	{
		// Quick race EventID enable
		injector::MakeCALL(0x007AB0F4, FEngFindObjectHook, true);

		// Challenge Series EventID enable
		injector::MakeCALL(0x007A42AB, FEngFindObjectHook2, true);

		// SafeHouse EventID enable
		injector::MakeCALL(0x00547995, FindPackageHook, true);
	}

	// Pursuit Action Game Mode - Harder Pursuits in Quick Races
	if (PursuitActionMode)
	{
		// Replace Race Table (0x2283ECAF) with Heat Table (0xD4B0CC11)
		injector::WriteMemory<DWORD>(0x403476, 0xD4B0CC11, true);
		injector::WriteMemory<DWORD>(0x409176, 0xD4B0CC11, true);

		// Replace Race Support (0xE5332008) with Heat Support (0xF3918F68)
		injector::WriteMemory<DWORD>(0x403496, 0xF3918F68, true);
		injector::WriteMemory<DWORD>(0x4091D9, 0xF3918F68, true);

		// Can Spawn Roadblocks in Quick Race - AIPursuit::RequestRoadBlock
		injector::MakeNOP(0x419519, 6, true);

		// Cops Can Call Support in Quick Race - AIPursuit::RequestGroundSupport
		injector::WriteMemory<unsigned char>(0x4196FF, 0xEB, true);
		injector::MakeNOP(0x41974B, 2, true);

		// Helicopter - AIPursuit::CopRequest
		injector::MakeNOP(0x42BAD6, 6, true);
	}

	// More Stock Cars for Newly Created Save Games
	if (ShowAllCarsInFE != 0)
	{
		injector::WriteMemory<unsigned char>(0x926127, 0x01, true);
		injector::WriteMemory<unsigned char>(0x581663, ShowAllCarsInFE, true); // Add them to desired category
	}

	// Preset Mode (Only for experts)
	/*if (ShowPresetCarsInFE)
	{
		injector::WriteMemory<unsigned char>(0x926129, 0x01, true);

		// Disable bonus cars creation to save tuning slots
		injector::MakeRangedNOP(0x590E5E, 0x590E68, true);
		injector::WriteMemory<unsigned char>(0x590E68, 0xEB, true);

		// Only use Stock, My Cars and Preset categories in Car Select.
		injector::MakeRangedNOP(0x7BF9B4, 0x7BF9B6, true);
	}*/

	// More Opponent Cars
	if (MoreCarsForOpponents)
	{
		injector::MakeRangedNOP(0x61DA2B, 0x61DA30, true); // force RandomOpponent = 1
		injector::WriteMemory<unsigned char>(0x61DA2B, 0xB0, true); // Game_FindPerformanceCandidates
		injector::WriteMemory<unsigned char>(0x61DA2C, 0x01, true); // Opponents can use bonus cars and cop cars
	}

	// Uncensored Busted Screen
	if (UncensoredBustedScreen)
	{
		injector::WriteMemory<unsigned char>(0x6231D6, 0x00, true); // FacePixelation::mPixelationOn = 0
		injector::WriteMemory<unsigned char>(0x623227, 0x00, true);
	}

	// Show Pursuit Cops
	injector::WriteMemory<unsigned char>(0x8F3AAC, ShowPursuitCops, true);

	// Show Non-Pursuit Cops
	injector::WriteMemory<unsigned char>(0x91cf00, ShowNonPursuitCops, true);

	// Carbon-Style Random Cars (no Vinyls on Random Cars)
	if (CarbonStyleRandomCars)
	{
		injector::WriteMemory<unsigned char>(0x75B229, 0, true);
	}

	// Show More Customization Options
	if (ShowMoreCustomizationOptions)
	{
		// Show Backroom Parts on Customization
		injector::MakeNOP(0x7AF669, 2, true);

		// Enable All Customizations For All Cars
		injector::MakeNOP(0x7BCDD0, 2, true); //My Cars
		injector::MakeNOP(0x7BCE33, 2, true); //Backroom Categories
		injector::MakeNOP(0x7B123D, 2, true); //Backroom HUD Area
		injector::MakeNOP(0x7BFF49, 6, true); //Backroom Menu
		injector::MakeNOP(0x7B92DC, 6, true); //Backroom Parts
		injector::MakeNOP(0x7BC28C, 2, true); //Rim Paint
	}
	

	// Time Of Day (Replaces Level of Detail)
	if (ShowTimeOfDay)
	{
		// VOTimeOfDay::Act(char const *,uint)
		injector::WriteMemory<DWORD>(0x89BB44, 0x50F7A0, true); // change code location
		injector::MakeRangedNOP(0x50F7AE, 0x50F7B6, true); // cleanup

		char LoadToEcx[] = { 0x8B, 0x0D, 0x2C, 0x39, 0x9B, 0x00 }; // mov ecx,[009B392C]
		injector::WriteMemoryRaw(0x50F7AE, LoadToEcx, sizeof(LoadToEcx), true);
		injector::WriteMemory<unsigned char>(0x50F7BE, 0x08, true); // mov [ecx+8], edx


		// VOTimeOfDay::Draw(void)
		injector::WriteMemory<DWORD>(0x89BB4C, 0x51B5E0, true);
		injector::WriteMemory<DWORD>(0x51B5F5, 0x999D1383, true); // "Time of Day"
		char LoadToEcx2[] = { 0x8B, 0x0D, 0x2C, 0x39, 0x9B, 0x00, 0x90, 0x90, 0x90, 0x8B, 0x49, 0x08 }; // mov ecx,[009B392C]; mov ecx,[ecx+08]
		injector::WriteMemoryRaw(0x51B617, LoadToEcx2, sizeof(LoadToEcx2), true);


		// VOTimeOfDay::SetInitialValues(void)
		char LoadTimeOfDay[] = { 0x8B, 0x15, 0x2C, 0x39, 0x9B, 0x00, 0x8B, 0x52, 0x08 }; // mov edx,[009B392C]; mov edx,[edx+08]
		injector::WriteMemoryRaw(0x50F9E0, LoadTimeOfDay, sizeof(LoadTimeOfDay), true);

		injector::MakeRangedNOP(0x50F9F5, 0x50F9F8, true); // nop fstp
		injector::WriteMemory<unsigned char>(0x0050F9F4, 0x52, true); // push edx

		injector::WriteMemory<float>(0x50F9F9, 0.01f, true); // Step Size
		injector::WriteMemory<float>(0x50F9FE, 0.95f, true); // Max Value
		injector::WriteMemory<float>(0x50FA03, 0.05f, true); // Min Value
	}

	// No Catch Up
	if (NoCatchUp)
	{
		// AIPerpVehicle::GetCatchupCheat
		injector::WriteMemory(0x892240, &Return0fHook, true); // AIPerpVehicle::ICheater::`vtbl'
		injector::WriteMemory(0x8925C8, &Return0fHook, true); // AIVehicleRacecar::ICheater::`vtbl'

		// Make Catch Up Controller always 0
		injector::MakeRangedNOP(0x7AC203, 0x7AC206, true); // nop setz bl
		injector::WriteMemory<short>(0x7AC203, 0xDB30, true); // xor bl, bl
		injector::WriteMemory<unsigned char>(0x7AC245, 0xEB, true);
		
		// Freeze it in menu
		injector::WriteMemory<unsigned char>(0x7ACA68, 0, true); // Circuit
		injector::WriteMemory<unsigned char>(0x7ACC4B, 0, true); // Sprint
		injector::WriteMemory<unsigned char>(0x7ACDEE, 0, true); // Drag
		injector::WriteMemory<unsigned char>(0x7AD022, 0, true); // LAP KO
		injector::WriteMemory<unsigned char>(0x7AD26B, 0, true); // Speedtrap
	}

	// No rev limiter
	if (NoRevLimiter)
	{
		injector::WriteMemory(0x8AB78C, &Return0Hook, true); // EngineRacer::UseRevLimiter(void)
		injector::WriteMemory(0x8AC034, &Return0Hook, true); // EngineDragster::UseRevLimiter(void)
	}

	// Marker Count
	injector::WriteMemory<unsigned char>(0x7B3D7D, SelectableMarkerCount, true);
	injector::WriteMemory<unsigned char>(0x7B3E19, SelectableMarkerCount, true);
	injector::WriteMemory<unsigned char>(0x7A7A3C, SelectableMarkerCount, true);
	injector::WriteMemory<unsigned char>(0x7A7ABE, SelectableMarkerCount, true);

	// Garage Car Slot Limit
	injector::WriteMemory<unsigned char>(0x7C1A77, PurchasedCarLimit, true);

	// Debug Cam Speed
	injector::WriteMemory<float>(0x8EDEA4, DebugCameraTurboSpeed, true);
	injector::WriteMemory<float>(0x8EDEA8, DebugCameraSuperTurboSpeed, true);

	// Speedbreaker
	injector::WriteMemory<float>(0x901AE4, SBRechargeTime, true);
	injector::WriteMemory<float>(0x901AE8, SBRechargeSpeedLimit, true);
	injector::WriteMemory<float>(0x901AEC, SBMassMultiplier, true);

	// Infractions
	injector::WriteMemory<float>(0x8A3810, SpeedingLimit, true);
	injector::WriteMemory<float>(0x8A380C, ExcessiveSpeedingLimit, true);
	injector::WriteMemory<float>(0x8A3814, RecklessDrivingLimit, true);

	// Allow Longer Save Game Names
	if (AllowLongerProfileNames)
	{
		injector::WriteMemory<unsigned char>(0x527570, 15, true); // UIProfileMenu::ShowKeyboard - Input Limit on Save Game Creation
		injector::WriteMemory<unsigned char>(0x52758E, 0, true); // UIProfileMenu::ShowKeyboard - Allow every character
		injector::WriteMemory<unsigned char>(0x563A7A, 16, true); // UIMemcardList::NotificationMessage - Include Longer Names in the Load Save list
		injector::WriteMemory<unsigned char>(0x563A7E, '/', true); // UIMemcardList::NotificationMessage - Ignore saves with this character
	}

	// Car Skin Fix
	if (CarSkinFix)
	{
		injector::WriteMemory<unsigned char>(0x75D2B6, 16, true); 
		// injector::WriteMemory<unsigned char>(0x747F22, 12, true); // CARS\TEXTURES.BIN has 12 dummy skins

		injector::MakeJMP(0x747F19, CarSkinFixCodeCave, true);
		injector::MakeRangedNOP(0x73B324, 0x73B328, true); //crash fix??
	}

	// Immobile collisions fix
	if (ImmobileColFix)
	{
		injector::MakeRangedNOP(0x6982C5, 0x6982CB, true);
		injector::WriteMemory<short>(0x6982C5, 0x00B1, true);

		injector::WriteMemory<unsigned char>(0x69828F, 0, true); // RigidBody::IsImmobile
	}

	// DDay fix for save games with progress
	if (DDayFix)
	{
		injector::MakeJMP(0x57F5A9, DDayFixCodeCaveResumeCareer, true);
		injector::MakeJMP(0x531FCA, DDayFixCodeCaveCareerCribNotificationMessage, true);
	}


	// Other Things
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Thing, NULL, 0, NULL);
	injector::MakeCALL(0x663EE8, Thing, true); // Hook the hotkey detection to the main loop
}

void Thing()
{
	//Sleep(ThreadDelay);
	_asm pushad;

	raceOptions = *(DWORD*)0x91CF90; // Race Options Pointer (Thanks to samfednik)
	GameState = *(DWORD*)0x0925E90; // 3 = FE, 4&5 = Loading screen, 6 = Gameplay
	windowHandle = *(HWND*)0x982BF4;
	IsOnFocus = !(*(bool*)0x982C50);

	// Windowed Mode Related Fixes
	if (WindowedMode == 2 && windowHandle && !once2)
	{
		RECT o_cRect, n_cRect, n_wRect;
		GetClientRect(windowHandle, &o_cRect);

		DWORD wStyle = GetWindowLongPtr(windowHandle, GWL_STYLE) | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW;
		SetWindowLongPtr(windowHandle, GWL_STYLE, wStyle);

		// make window change style
		SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);

		GetWindowRect(windowHandle, &n_wRect);
		GetClientRect(windowHandle, &n_cRect);
		int n_wWidth = n_wRect.right - n_wRect.left;
		int n_wHeight = n_wRect.bottom - n_wRect.top;
		int dif_wWidth = o_cRect.right - n_cRect.right;
		int dif_wHeight = o_cRect.bottom - n_cRect.bottom;
		int newWidth = n_wWidth + dif_wWidth;
		int newHeight = n_wHeight + dif_wHeight;

		SetWindowPos(windowHandle, NULL, 0, 0, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

		once2 = 1;
	}

	// Ultimate Force Heat Level
	if ((GetAsyncKeyState(hotkeyToggleForceHeat) & 1) && (GameState == 6) && IsOnFocus) //When pressed "Toggle Force Heat"
	{
		forceHeatLevel = !forceHeatLevel; // Toggle option
		if (forceHeatLevel)
		{
			EnableHeatLevelOverride = 1;
			heatLevel = MinHeatLevel - 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled Ultimate Force Heat Level hack.^You can now press ChangeHeatLevel (PageUp as default) key to increase your heat level.");
			}
		}
		else
		{
			// Re-read the override boolean from config file
			CIniReader iniReader("NFSMWExtraOptionsSettings.ini");
			EnableHeatLevelOverride = iniReader.ReadInteger("Pursuit", "HeatLevelOverride", 0) == 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have disabled Ultimate Force Heat Level hack.");
			}
		}

		if (EnableHeatLevelOverride)
		{
			// CustomizeMeter::Init
			injector::WriteMemory<float>(0x7bb502, MaxHeatLevel, true); // CustomizeCategoryScreen
			injector::WriteMemory<float>(0x7b1387, MaxHeatLevel, true); // CustomizationScreenHelper
			injector::WriteMemory<float>(0x7b0c89, MaxHeatLevel, true); // CustomizeShoppingCart::Setup
			injector::WriteMemory<float>(0x7B4D7C, MaxHeatLevel, true); // UIQRCarSelect::InitStatsSliders

			// SetHeatLevel
			injector::WriteMemory<float*>(0x435079, &MaxHeatLevel, true); // AIVehicleHuman::~AIVehicleHuman
			injector::WriteMemory<float>(0x435088, MaxHeatLevel, true);
		}
		else
		{
			// CustomizeMeter::Init
			injector::WriteMemory<float>(0x7bb502, 5.0f, true); // CustomizeCategoryScreen
			injector::WriteMemory<float>(0x7b1387, 5.0f, true); // CustomizationScreenHelper
			injector::WriteMemory<float>(0x7b0c89, 5.0f, true); // CustomizeShoppingCart::Setup
			injector::WriteMemory<float>(0x7B4D7C, 5.0f, true); // UIQRCarSelect::InitStatsSliders

			// SetHeatLevel
			injector::WriteMemory<DWORD>(0x435079, 0x890DA4, true); // AIVehicleHuman::~AIVehicleHuman
			injector::WriteMemory<float>(0x435088, 5.0f, true);
		}
	}

	if ((GetAsyncKeyState(hotkeyForceHeatLevel) & 1) && forceHeatLevel && (GameState == 6) && IsOnFocus) //When pressed "Forced Heat Level"
	{
		heatLevel = (int)(floorf(heatLevel) - floorf(MinHeatLevel) + 1) % (int)(floorf(MaxHeatLevel) - floorf(MinHeatLevel) + 1) + MinHeatLevel;
			
		Game_SetWorldHeat(heatLevel); // Use direct function call to set the heat level
	}

	if ((GetAsyncKeyState(hotkeyToggleCops) & 1) && forceHeatLevel && (GameState == 6) && IsOnFocus) // Toggle Cops
	{
		ToggleCops = !ToggleCops;
		Game_SetCopsEnabled(ToggleCops);

		if (ToggleCops)
		{
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Cops are now enabled.");
			}
		}
		else
		{
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Cops are now disabled.");
			}
		}
	}

	// Cop Lights
	if ((GetAsyncKeyState(hotkeyToggleCopLights) & 1) && (GameState == 6) && IsOnFocus) // When pressed "Toggle Police Lights" key
	{
		copLightsEnabled = !copLightsEnabled;
			
		DWORD* PlayerPVehicle = *(DWORD**)0x0092CD28;

		if (PlayerPVehicle)
		{
			if (copLightsEnabled) // Enable cop lights - more proper method
			{
				if (!PVehicle_IsGlareOn(PlayerPVehicle, 0x7000)) PVehicle_GlareOn(PlayerPVehicle, 0x7000);
			}
			else // Disable cop lights
			{
				if (PVehicle_IsGlareOn(PlayerPVehicle, 0x7000)) PVehicle_GlareOff(PlayerPVehicle, 0x7000);
			}
		}
	}

	// Headlights
	if ((GetAsyncKeyState(hotkeyToggleHeadlights) & 1) && (GameState == 6) && IsOnFocus) // When pressed "Toggle Head Lights" key
	{
		headlightsMode = !headlightsMode;

		DWORD* PlayerPVehicle = *(DWORD**)0x0092CD28;

		if (PlayerPVehicle)
		{
			if (headlightsMode) // Enable headlights - more proper method
			{
				if (!PVehicle_IsGlareOn(PlayerPVehicle, 7))
				{
					injector::WriteMemory<unsigned char>(0x6703D6, 7, true);
					PVehicle_GlareOn(PlayerPVehicle, 7);
						
				}
			}
			else // Disable headlights
			{
				if (PVehicle_IsGlareOn(PlayerPVehicle, 7))
				{
					injector::WriteMemory<unsigned char>(0x6703D6, 0, true);
					PVehicle_GlareOff(PlayerPVehicle, 7);
				}
			}
		}
	}


	// Car Hack
	if ((GetAsyncKeyState(hotkeyCarHack) & 1) && IsOnFocus) // When pressed "Toggle Freeze Car" key
	{
		carHackEnabled = !carHackEnabled;

		if (carHackEnabled)
		{
			raceType = *(unsigned char*)(raceOptions + 0x18); // Get the race type
			raceMode = *(unsigned char*)(raceOptions + 0x12C); // Get the mode. (1 = Career, 4 = Quick Race, 32 = My Cars)
			careerCar = *(DWORD*)(*(DWORD*)(raceOptions + 0x10) + 0xA8);

			switch (raceMode)
			{
			case 1: selectedCar = *(DWORD*)(*(DWORD*)(raceOptions + 0x10) + 0xA8); break; //Freeze career car
			case 4: selectedCar = *(DWORD*)(raceOptions + 0x2C + (raceType * 0x18)); break; // Freeze QR Car
			case 32: selectedCar = *(DWORD*)(raceOptions + 0x2C); break; //Freeze my car
			default: carHackEnabled = 0; break; // Don't enable it while not in a car select screen
			}

			if (carHackEnabled && ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled Freeze Car Hack.^You can now use this car anywhere.");
			}

		}
		else
		{
			// Fix Challenge Series behaviour
			injector::WriteMemory<unsigned char>(0x6f48e7, 0x74, true);

			// Fix Career Car
			unsigned int CareerCarPointer = *(DWORD*)(raceOptions + 0x10) + 0xA8;
			*(unsigned int*)CareerCarPointer = careerCar;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have disabled Freeze Car Hack.^You can now select cars normally.");
			}
		}
	}

	if (carHackEnabled)
	{
		unsigned int PointerToWrite;
		// Freeze values
		PointerToWrite = raceOptions + 0x2C;  // Sprint (0)
		*(unsigned int*)PointerToWrite = selectedCar;
			
		PointerToWrite = raceOptions + 0x2C + (0x18); // Circuit (1)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (2 * 0x18); // Drag (2)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (3 * 0x18); // Lap Knockout (3)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (4 * 0x18); // Tollbooth (4)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (5 * 0x18); // Speedtrap (5)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = raceOptions + 0x2C + (8 * 0x18); // Challenge (8)
		*(unsigned int*)PointerToWrite = selectedCar;

		PointerToWrite = *(DWORD*)(raceOptions + 0x10) + 0xA8; // Career
		*(unsigned int*)PointerToWrite = selectedCar;

		injector::WriteMemory<unsigned char>(0x6f48e7, 0xEB, true); // Use selected cars at Challenge Series
	}

	// Drunk Driver
	if ((GetAsyncKeyState(hotkeyAutoDrive) & 1) && (GameState == 6) && IsOnFocus) // When pressed "Drunk Driver" key
	{
		AutoDrive = !AutoDrive;

		if (AutoDrive)
		{
			Game_ForceAIControl(1);
		}
		else
		{
			Game_ClearAIControl(1);
		}
	}

	// Freeze Camera
	if ((GetAsyncKeyState(hotkeyFreezeCamera) & 1) && (GameState == 6) && IsOnFocus)
	{
		*(bool*)0x911020 = !(*(bool*)0x911020);
	}

	// Unlock All Things
	if ((GetAsyncKeyState(hotkeyUnlockAllThings) & 1) && IsOnFocus) // When pressed "Unlock All Things" key
	{
		UnlockAllThings = !UnlockAllThings;
		CIniReader iniReader("NFSMWExtraOptionsSettings.ini");
		iniReader.WriteInteger("Gameplay", "UnlockAllThings", UnlockAllThings);

		if (UnlockAllThings)
		{
			*(unsigned char*)0x926124 = 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled UnlockAllThings hack.^Everything is now unlocked. Enjoy!");
			}

		}
		else
		{
			*(unsigned char*)0x926124 = 0;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have disabled UnlockAllThings hack.^Unlock status of everything is returned back to normal.");
			}
		}
	}
		
	// .hot Save And Load
	if (EnableSaveLoadHotPos && (GameState == 6) && IsOnFocus)
	{
		// Save
		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(49) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 1;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 1);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(50) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 2;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 2);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(51) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 3;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 3);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(52) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 4;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 4);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(53) & 0x8000)
		{
			*(unsigned int*)0x9B0908 = 5;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 5);
			}
		}

		// Load
		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(49) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 1;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 1);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(50) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 2;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 2);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(51) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 3;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 3);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(52) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 4;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 4);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(53) & 0x8000)
		{
			*(unsigned int*)0x009B090C = 5;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 5);
			}
		}
	}

	// Debug Camera
	if (EnableDebugWorldCamera && (GameState == 6) && IsOnFocus)
	{
		if ((GetAsyncKeyState(VK_BACK) & 1)) // Backspace : Toggle Debug World Camera
		{
			DebugWorldCamera = !DebugWorldCamera;

			if (*(int*)0x00925E90 == 6 && DebugWorldCamera)
			{
				injector::WriteMemory<unsigned char>(0x750F4C, 0xEB, true); // fix crash??
				*(unsigned int*)0x0091102C = 0;
				CameraAI_SetAction(1, "CDActionDebug");
			}

			if (*(int*)0x00925E90 == 6 && !DebugWorldCamera)
			{
				CameraAI_SetAction(1, "CDActionDrive");
				*(unsigned char*)0x911038 = 0;
				injector::WriteMemory<unsigned char>(0x750F4C, 0x75, true);
			}
		}

		if ((GetAsyncKeyState(VK_NEXT) & 1)) // Page Down : Toggle Debug Watch Car Camera
		{
			DebugWatchCarCamera = !DebugWatchCarCamera;
			*(unsigned char*)0x00911058 = 1;

			if (*(int*)0x00925E90 == 6 && DebugWatchCarCamera)
			{
				injector::WriteMemory<unsigned char>(0x750F4C, 0xEB, true); // fix crash??
				*(unsigned char*)0x911038 = 1;
				CameraAI_SetAction(1, "CDActionDebugWatchCar");
			}

			if (*(int*)0x00925E90 == 6 && !DebugWatchCarCamera)
			{
				CameraAI_SetAction(1, "CDActionDrive");
				*(unsigned char*)0x911038 = 0;
				injector::WriteMemory<unsigned char>(0x750F4C, 0x75, true);
			}
		}

		if ((GetAsyncKeyState(VK_ADD) & 1) && DebugWatchCarCamera) // + : Next car
		{
			*(unsigned char*)0x00911058 += 1;
		}

		if ((GetAsyncKeyState(VK_SUBTRACT) & 1) && DebugWatchCarCamera) // - : Previous car
		{
			*(unsigned char*)0x00911058 -= 1;
		}
	}

	// Continue after a dialog is closed
	if ((GameState == 6) && (ButtonResult == DialogBoxReturn))
	{
		ButtonResult = 0;
		UnPause(NULL);
	}

	_asm popad;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x7C4040) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Init();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.3 English speed.exe (5,75 MB (6.029.312 bytes)).", "NFSMW Extra Options", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}

