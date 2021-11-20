#pragma once

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
DWORD CreateGameWindow = 0x6E6310;
DWORD WindowedModeCodeCaveExit = 0x6E6C13;
DWORD ReplayBlacklistFixCodeCaveExit = 0x6243F4;
DWORD CarSkinFixCodeCaveExit = 0x747F3F;
DWORD CarSkinFixCodeCaveExit2 = 0x747F2B;

DWORD GlobalTimerAddress = 0x009885D8;
DWORD TimerAddress = 0x009142DC;
float PreviousRaceTime = 0.0f;

DWORD GRaceDatabase_GetRaceParameters = 0x005DC930;
DWORD GRaceParameters_GetEventHash = 0x005FAB40;
DWORD SplitScreenCodeCaveExit = 0x7AA82A;

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
		mov dword ptr ds : [edi + 0x15C] , 0x1C619FD8
			mov ebp, 0xF7352706
			mov dword ptr ds : [esp + 0x18] , 0x00000002
			jmp VinylMenuCodeCave2Exit

			raceflags :
		mov dword ptr ds : [edi + 0x15C] , 0x9C1B8935
			mov ebp, 0x1223CC89
			mov dword ptr ds : [esp + 0x18] , 0x00000003
			jmp VinylMenuCodeCave2Exit

			nflags :
		mov dword ptr ds : [edi + 0x15C] , 0x7956F7B0
			mov ebp, 0xBC44BBCB
			mov dword ptr ds : [esp + 0x18] , 0x00000004
			jmp VinylMenuCodeCave2Exit

			body :
		mov dword ptr ds : [edi + 0x15C] , 0x2D5BFF0F
			mov ebp, 0x694CA0CA
			mov dword ptr ds : [esp + 0x18] , 0x00000005
			jmp VinylMenuCodeCave2Exit

			uniques :
		mov dword ptr ds : [edi + 0x15C] , 0x209A9158
			mov ebp, 0x1B3A8DD3
			mov dword ptr ds : [esp + 0x18] , 0x00000006
			jmp VinylMenuCodeCave2Exit

			contest :
		mov dword ptr ds : [edi + 0x15C] , 0xCD057D21
			mov ebp, 0x1BA508FC
			mov dword ptr ds : [esp + 0x18] , 0x00000007
			jmp VinylMenuCodeCave2Exit

			specials :
		mov dword ptr ds : [edi + 0x15C] , 0xB715070A
			mov ebp, 0x55778E5A
			mov dword ptr ds : [esp + 0x18] , 0x0000008
			jmp VinylMenuCodeCave2Exit

			ifabove :
		jmp VinylMenuCodeCave2ifAbove
	}

}

void __declspec(naked) ShowHiddenTracksCodeCave()
{
	_asm
	{
	resetcounter:
		xor edi, edi

			letsgo :
		push edi
			mov ecx, dword ptr ds : [0x91E004] // GRaceDatabase::mObj
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
		cmp dword ptr ds : [eax] , 0x9C614397 // 20.5.1
			jne fix_20_8_1
			mov byte ptr ds : [eax + 0x2B] , 5
			jmp originalcode

			fix_20_8_1 :
		cmp dword ptr ds : [eax] , 0x102E6410 // 20.8.1
			jne fix_20_1_1
			mov byte ptr ds : [eax + 0x2B] , 8
			jmp originalcode

			fix_20_1_1 :
		cmp dword ptr ds : [eax] , 0xF984278B // 20.1.1
			jne fix_20_2_1
			mov byte ptr ds : [eax + 0x2B] , 1
			jmp originalcode

			fix_20_2_1 :
		cmp dword ptr ds : [eax] , 0x2D442AD8 // 20.2.1
			jne fix_20_2_2
			mov byte ptr ds : [eax + 0x2B] , 0
			jmp originalcode

			fix_20_2_2 :
		cmp dword ptr ds : [eax] , 0xC2AC6CB3 // 20.2.2
			jne fix_maxload
			mov byte ptr ds : [eax + 0x2B] , 0
			jmp originalcode

			fix_maxload :
		cmp dword ptr ds : [eax] , 0xA137ED5B // MaxLoad
			jne originalcode
			mov byte ptr ds : [eax + 0x2B] , 1
			jmp originalcode

			originalcode :
		movsx eax, byte ptr ds : [eax + 0x2B]
			pop esi
			retn
	}
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
		mov dword ptr ds : [esi + 0xF4] , eax

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
		mov dword ptr ds : [esi + 0xEC] , ecx

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
		mov	dword ptr ds : [eax + 0xC0] , ecx //red

		mov ecx, VTGreen
		mov	dword ptr ds : [eax + 0xc4] , ecx //green

		mov ecx, VTBlue
		mov	dword ptr ds : [eax + 0xc8] , ecx //blue

		mov ecx, VTColorBloom
		mov dword ptr ds : [eax + 0xd0] , ecx //color bloom

		mov ecx, VTSaturation
		mov dword ptr ds : [eax + 0xd4] , ecx //saturation

		mov ecx, VTBlackBloom
		mov	dword ptr ds : [eax + 0xdc] , ecx //black bloom

		fld dword ptr[eax + 0xC4]

		mov ecx, VTecx
		jmp VTCodeCaveExit
	}
}


void __declspec(naked) ReplayBlacklistFixCodeCave()
{
	_asm
	{
		mov dword ptr ds : [0x91CA3C] , edx
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

		caveexit :
		mov esi, ecx
			mov byte ptr ds : [esi + 07] , 1
			jmp CarSkinFixCodeCaveExit

			fixskinid : // swap around DUMMY_SKIN/WHEEL2,3 and 4
		sub edi, 03
			cmp edi, 04
			jg fixskinid
			jmp caveexit

			caveexit2 :
		jmp CarSkinFixCodeCaveExit2
	}
}

void __declspec(naked) SplitScreen_React()
{

	_asm
	{
		cmp dword ptr ds : [esp + 0x8] , 0x0C407210
		jnz funcexit
		mov dword ptr ds : [0x9B9E68] , 2

		funcexit :
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
		mov dword ptr ds : [esp + 0x8] , edi
		test edi, edi
		mov dword ptr ds : [esp + 0x14] , 00000003
		je ssjump1
		push 0
		push 0x841D518A
		push 0xF365B5F5
		mov ecx, edi
		call IconOption_Create
		push ebx
		mov ebx, 0x901400
		mov dword ptr ds : [edi] , ebx
		pop ebx
		jmp ssjump2

		ssjump1 :
		xor edi, edi

			ssjump2 :
		mov ecx, esi
			push edi
			mov[esp + 0x18], 0xFFFFFFFF
			call IconScrollerMenu_AddOption


			originalcode :
		mov eax, [esi + 0x10]
			push eax
			call FEngGetLastButton

			caveexit :
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
		test al, al
		jnz StartFreeRoamOrFinalPursuit

		mov eax, dword ptr ds : [0x91CF90] // FEDatabase
		test eax, eax
		jz StartDDay
		mov eax, [eax + 0x10]
		test eax, eax
		jz StartDDay
		movzx eax, byte ptr ds : [eax + 0xB0]
		cmp eax, 16
		jl StartFreeRoamOrFinalPursuit

		StartDDay :
		push 0x57F5B2
			retn

			StartFreeRoamOrFinalPursuit :
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
		movzx eax, byte ptr ds : [eax + 0xB0]
		cmp eax, 16
		jl StartFreeRoamOrFinalPursuit

		StartDDay :
		call GRaceDatabase_GetNextDDayRace
			push 0x531FD6
			retn

			StartFreeRoamOrFinalPursuit :
		push 0x532015
			retn
	}
}