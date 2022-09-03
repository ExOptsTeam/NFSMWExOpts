#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"

#define SCREEN_PRINT_ITEM_COUNT 64
#define SCREEN_PRINT_BUFFER_SIZE 128

#define AudioMemoryPool 0x8EE558
#define BuildVersionChangelistName 0x8F8690
#define RenderTimingStart 0x925828
#define RenderTimingEnd 0x92582C
#define FrameTimingStartTime 0x925830
#define FrameTimingEndTime 0x925834
#define RealTimeElapsed 0x925960
#define BuildVersionChangelistNumber 0x8F8694
#define _DistanceToNextSection 0x9AAB5C
#define _TrackStreamNameThing 0x9AAB64

char ScreenPrintfBuffer[SCREEN_PRINT_BUFFER_SIZE];
int mCurPursuitState = 0, mCurPursuitLevel = 0, mCurPursuitForm = 0;

typedef struct 
{
	DWORD* pFEString;
	short PosX;
	short PosY;
	float Lifetime;
} ScreenPrintItem;

ScreenPrintItem ScreenPrintItemTable[SCREEN_PRINT_ITEM_COUNT];

ScreenPrintItem* GetUnusedScreenPrintItem()
{
	for (int i = 0; i < SCREEN_PRINT_ITEM_COUNT; i++)
	{
		if (ScreenPrintItemTable[i].Lifetime < 0.0f)
			return &ScreenPrintItemTable[i];
	}
	return 0;
}

ScreenPrintItem* GetScreenPrintItemAtLocation(int x, int y)
{
	for (int i = 0; i < SCREEN_PRINT_ITEM_COUNT; i++)
	{
		if (ScreenPrintItemTable[i].PosX == x && ScreenPrintItemTable[i].PosY == y)
			return &ScreenPrintItemTable[i];
	}
	return 0;
}

void FixWidescreen(int *x)
{
	/*if (eIsWidescreen()) // Original Code
	{
		if (*x >= 11) *x += 120;
		else if (*x <= -10) *x -= 120;
	}*/

	// Code ported from HUD Adapter for convenience
	int ResX, ResY, iDifference;
	float Difference;

	ResX = *(int*)0x93CAD8;
	ResY = *(int*)0x93CADC;

	if (ResX != 0 && ResY != 0) Difference = (((float)ResX / (float)ResY) * 240) - 320; // Calculate position difference for current aspect ratio
	else Difference = 0; // 4:3 if we can't get any values
	
	iDifference = floor(Difference + 0.5f);

	if (*x >= 11) *x += iDifference;
	else if (*x <= -10) *x -= iDifference;
}

int ScreenPrintf(int x, int y, float duration, unsigned int color, char const* fmt, ...)
{
	va_list Args;
	ScreenPrintItem* ItemToPrint = 0;
	DWORD* FEStringItem = 0;
	FEColor FEStringColor; // struct defined in DialogInterfaceHook.h

	FixWidescreen(&x);

	if (DoScreenPrintf)
	{
		if ((duration == 0) || ((ItemToPrint = GetScreenPrintItemAtLocation(x, y)) == 0))
			ItemToPrint = GetUnusedScreenPrintItem();

		if (ItemToPrint)
		{
			ItemToPrint->Lifetime = duration;
			ItemToPrint->PosX = x;
			ItemToPrint->PosY = y;
			FEStringItem = ItemToPrint->pFEString;

			va_start(Args, fmt);
			vsnprintf(ScreenPrintfBuffer, SCREEN_PRINT_BUFFER_SIZE - 1, fmt, Args);
			va_end(Args);

			if (FEStringItem)
			{
				FEColor_Create(&FEStringColor, color);
				FEObject_SetColor(FEStringItem, FEStringColor, 0);
				FEPrintf_FEString(FEStringItem, ScreenPrintfBuffer);
				FEngSetTopLeft(FEStringItem, x, y);
				FEngSetVisible(FEStringItem);
			}
		}
	}

	return strlen(ScreenPrintfBuffer);
}

int ScreenShadowPrintf(int x, int y, float duration, unsigned int color, char const* fmt, ...)
{
	va_list Args;

	va_start(Args, fmt);
	vsnprintf(ScreenPrintfBuffer, SCREEN_PRINT_BUFFER_SIZE - 1, fmt, Args);
	va_end(Args);

	ScreenPrintf(x + 1, y + 1, duration, 0x80000000, ScreenPrintfBuffer);
	return ScreenPrintf(x, y, duration, color, ScreenPrintfBuffer);
}

void DisplayDebugScreenPrints()
{
	DWORD *PlayerPVehicle, * PlayerISuspension, *PlayerSuspension, * DebugVehicleSelectionThis;
	float WaitForBufferSwapTime = 0; // GetWaitForBufferSwapTime();
	float VifTime = 0; //GetVifTime();
	float PreviousRenderFrameTime = bGetTickerDifference(*(unsigned int*)RenderTimingStart, *(unsigned int*)RenderTimingEnd) - WaitForBufferSwapTime;
	float PreviousCpuFrameTime = bGetTickerDifference(*(unsigned int*)FrameTimingStartTime, *(unsigned int*)FrameTimingEndTime) - WaitForBufferSwapTime;
	float PreviousGpuFrameRate = 0;
	if (VifTime > 0.1) PreviousGpuFrameRate = 1000.0f / VifTime;
	if (PreviousCpuFrameTime < 0.1f) PreviousCpuFrameTime = 0.1f;
	float PreviousCpuFrameRate = 1000.0f / PreviousCpuFrameTime;
	if (PreviousGpuFrameRate < 1.0)
	{
		PreviousGpuFrameRate = PreviousCpuFrameRate;
		VifTime = PreviousCpuFrameTime;
	}

	ScreenPrintf(-300, -30, 1.0f, 0xFFFFFFFF, "C: %.2f ms", PreviousCpuFrameTime);
	ScreenPrintf(-200, -30, 1.0f, 0xFFFFFFFF, "G: %.2fms", VifTime);
	ScreenPrintf(-300, -15, 1.0f, 0xFFFFFFFF, "F: %.2f fps", PreviousGpuFrameRate);

	// Memory
	int FreeMemoryPool0 = bCountFreeMemory(0);
	if (FreeMemoryPool0 <= -1) FreeMemoryPool0 += 1023;
	int FreeMemoryAudioPool = 0;
	if (EnableSound) FreeMemoryAudioPool = bCountFreeMemory(*(int*)AudioMemoryPool); // AudioMemoryPool

	ScreenPrintf(-300, 45, 1.0f, 0xFFFFFFFF, "Main = %dK  Fast = %dK", FreeMemoryPool0 >> 10, FreeMemoryAudioPool >> 10);

	if (GameState == 6)
	{
		PlayerPVehicle = GetPlayerPVehicle();

		// Event ID and Coordinates
		DWORD* GRaceStatus = (DWORD*)*(DWORD*)GRaceStatus_fObj;
		char* EventID = "";
		float* Coords = NULL;

		if (GRaceStatus)
		{
			DWORD* GRaceParams = (DWORD*)GRaceStatus[1626]; // [GRaceStatus + 0x1968]
			if (GRaceParams)
			{
				EventID = GRaceParameters_GetEventID(GRaceParams);
			}

			if (PlayerPVehicle)
			{
				Coords = PVehicle_GetPosition(PlayerPVehicle);
			}

			if (Coords)
			{
				ScreenPrintf(-300, 15, 1.0f, 0xFFFFFFFF, "(%.0f,%.0f,%.0f) %s", Coords[0], Coords[1], Coords[2], EventID ? EventID : "");
			}
		}


		// Track Streamer
		int TSFree = bCountFreeMemory(7);
		int TSSize = TrackStreamer_GetMemoryPoolSize((DWORD*)TheTrackStreamer);
		int TSUsed = TSSize - TSFree;
		TSSize = TSUsed + 1023;
		if (TSUsed > -1) TSSize = TSUsed;

		char DistanceToNextSectionFmtStr[8] = "";
		float DistanceToNextSection = *(float*)_DistanceToNextSection;
		if (DistanceToNextSection > 0.0)
			bSPrintf(DistanceToNextSectionFmtStr, "%3.1f", DistanceToNextSection);

		ScreenPrintf(-300, 30, 1.0f, 0xFFFFFFFF, "TS = %d / %dK %s %s %s", TSUsed >> 10, TSSize >> 10, (char*)(_TrackStreamNameThing), DistanceToNextSectionFmtStr, "");

		// Vehicle
		DebugVehicleSelectionThis = *(DWORD**)DebugVehicleSelection_mThis;

		if (PlayerPVehicle)
		{
			char* VehicleName = PVehicle_GetVehicleName(PlayerPVehicle);
			ScreenShadowPrintf(-300, 220, 1.0f, 0xFFFFFFFF, "Vehicle: %s", VehicleName ? VehicleName : "null");
			PlayerISuspension = (DWORD*)PlayerPVehicle[17]; // ISuspension
		}
		else
		{
			PlayerISuspension = NULL;
		}

		// Surface & Collision
		if (PlayerISuspension && DebugVehicleSelectionThis)
		{
			PlayerSuspension = PlayerISuspension - 19;
			if (PlayerSuspension)
			{
				DWORD* Surface = (*(DWORD* (__thiscall**)(DWORD*, unsigned int))(PlayerISuspension[0] + 0x44))(PlayerISuspension, 0); // SimSurface
				if (Surface)
				{
					ScreenShadowPrintf(-100, 220, 1.0f, 0xFFFFFFFF, "Surface: %s", *(char**)(Surface[2] + 0xDC));
				}
				else
				{
					ScreenShadowPrintf(-100, 220, 1.0f, 0xFFFFFFFF, "Surface: %s", "null");
				}
			}

			ScreenShadowPrintf(50, 220, 1.0f, 0xFFFFFFFF, "Collision: %s --- %s", DebugVehicleSelectionThis[11], DebugVehicleSelectionThis[12]);
		}
	}
	else
	{
		ScreenPrintf(-300, 30, 1.0f, 0xFFFFFFFF, "CL %s", *(char**)BuildVersionChangelistName);
	}
}

// 0x542A42
char *MovieName;
void __declspec(naked) MoviePlayer_Play_ScreenPrintf()
{
	_asm
	{
		pushad
		mov MovieName, edi
	}

	if (MovieName) ScreenPrintf(-300, -80, 5.0f, 0xFFFFFFFF, "FMV: [%s]\n", MovieName);

	_asm
	{
		popad
		push 0x86FCFB
		push 0x542A47
		retn
	}
}

// 0x564473
char* PackageName;
void __declspec(naked) FEManager_Update_ScreenPrintf()
{
	_asm
	{
		pushad
		mov ecx, dword ptr ds : [cFEng_mInstance]
		call cFEng_FindPackageWithControl
		test eax, eax
		jz writename
		mov eax, dword ptr ds : [eax + 0xC]

		writename :
		mov PackageName, eax

		continuetocode :
	}

	if (PackageName) ScreenPrintf(-300, 70, 1.0f, 0xFFFFFFFF, "%s\n", PackageName);

	_asm
	{
		popad
		//mov ecx, dword ptr ds : [cFEng_mInstance]
		push 0x56449E
		retn
	}

}

void InitScreenPrintf()
{
	char ScreenPrintfObjBuf[18];
	DWORD* FEStringItem;
	int FEStringHash;

	void* cFEng = (void*)*(DWORD*)cFEng_mInstance;
	cFEng_PushNoControlPackage(cFEng, "ScreenPrintf.fng", 104);
	DWORD* ScreenPrintfPackage = FEPackageManager_FindPackage(FEPackageManager_Get(), "ScreenPrintf.fng");
	if (ScreenPrintfPackage)
	{
		//ScreenPrintfPackage[24] = 1; // SetUseIdleList
		//ScreenPrintfPackage[32] = 1; // SetPermanent
		FEPackageManager_SetUseIdleList(FEPackageManager_Get(), "ScreenPrintf.fng", 1);
		FEPackageManager_SetPermanent(FEPackageManager_Get(), "ScreenPrintf.fng", 1);

		for (int i = 0; i < SCREEN_PRINT_ITEM_COUNT; i++)
		{
			sprintf(ScreenPrintfObjBuf, "SCREENPRINTF%d", i);
			FEStringHash = FEHashUpper(ScreenPrintfObjBuf);
			FEStringItem = (DWORD*)FEngFindString("ScreenPrintf.fng", FEStringHash);
			if (FEStringItem)
			{
				FEngSetInvisible(FEStringItem);
				ScreenPrintItemTable[i].pFEString = FEStringItem;
			}
			else
			{
				ScreenPrintItemTable[i].pFEString = 0;
			}
			
			ScreenPrintItemTable[i].PosX = 0;
			ScreenPrintItemTable[i].PosY = 0;
			ScreenPrintItemTable[i].Lifetime = -1.0f;
		}
	}
}

void JuicePrintf()
{
	if (DoScreenPrintf)
	{
		ScreenPrintf(80, -200, 1.0f, 0xFFAA2222, "Juice Disabled");
		ScreenPrintf(80, -180, 1.0f, 0xFFAA2222, "%d", *(int*)BuildVersionChangelistNumber);
	}
}

void __fastcall ResetScreenPrintf(DWORD* cFEngGameInterface, void* edx_unused, DWORD* FEPkg)
{
	for (int i = 0; i < SCREEN_PRINT_ITEM_COUNT; i++)
	{
		if (ScreenPrintItemTable[i].Lifetime >= 0.0f)
		{
			ScreenPrintItemTable[i].Lifetime -= *(float*)RealTimeElapsed;

			if ((ScreenPrintItemTable[i].Lifetime < 0.0f) && (ScreenPrintItemTable[i].pFEString != 0))
			{
				FEngSetInvisible(ScreenPrintItemTable[i].pFEString);
			}
		}
	}
}

// 0x666110
void __declspec(naked) InitScreenPrintfCodeCave()
{
	_asm
	{
		call EventSequencer_Init
		call InitScreenPrintf
		call JuicePrintf
		push 0x666115
		retn
	}
}

// 0x7A8D3B
char* FECameraName;
void __declspec(naked) FindScreenInfo_ScreenPrintf()
{
	_asm
	{
		lea ecx, [esp + 0x38]
		mov FECameraName, ecx
	}

	if (FECameraName) ScreenPrintf(-300, -235, 1.0f, 0xFFFFFFFF, "FE camera name: %s\n", FECameraName);

	_asm
	{
		lea ecx, [esp + 0x38]
		push ecx
		push 0x7A8D40
		retn
	}
}

// 0x7A8EBD
void __declspec(naked) FindScreenInfo_ScreenPrintf2()
{
	_asm
	{
		lea eax, [esp + 0x38]
		mov FECameraName, eax
	}

	if (FECameraName) ScreenPrintf(-300, -235, 1.0f, 0xFFFFFFFF, "FE camera name: %s\n", FECameraName);

	_asm
	{
		lea eax, [esp + 0x38]
		push eax
		push 0x7A8EC2
		retn
	}
}

char* GetFormationStr(int Form)
{
	switch (Form)
	{
	case 1:
		return "PIT";
	case 2:
		return "BOX_IN";
	case 3:
		return "ROLLING_BLOCK";
	case 4:
		return "FOLLOW";
	case 5:
		return "HELI_PURSUIT";
	case 6:
		return "HERD";
	case 7:
		return "STAGGER_FOLLOW";
	}
	return "NONE";
}

int PursuitPrintf()
{
	switch (mCurPursuitState)
	{
	case 1:
		ScreenPrintf(-300, -130, 2.0f, 0xFFFFFFFF, "SEEN");
		ScreenPrintf(-300, -115, 1.0f, 0xFFFFFFFF, "Pursuit: IDLE - %d", mCurPursuitLevel);
		break;
	case 2:
		ScreenPrintf(-300, -115, 1.0f, 0xFFFFFFFF, "Pursuit: PURSUIT - %d", mCurPursuitLevel);
		break;
	case 3:
		ScreenPrintf(-300, -115, 1.0f, 0xFFFFFFFF, "Pursuit: SEARCHING - %d", mCurPursuitLevel);
		break;
	case 0:
		ScreenPrintf(-300, -115, 1.0f, 0xFFFFFFFF, "Pursuit: IDLE - %d", mCurPursuitLevel);
		break;
	}

	return ScreenPrintf(-300, -100, 1.0f, 0xFFFFFFFF, "Formation: %s", GetFormationStr(mCurPursuitForm));
}

int SessionIDPrintf()
{
	/*
	DWORD* Instance = LobbyGameSessions_Instance();
	DWORD* MySession = LobbyGameSessions_GetMySession(Instance);

	if (MySession)
	{
		return ScreenPrintf(-300, -120, 1.0f, 0xFFFFFFFF, "Session ID: %s", (char const*)MySession + 40);
	}
	return ScreenPrintf(-300, -120, 1.0f, 0xFFFFFFFF, "Session ID: none");
	*/
	return 0;
}

int OnlineScreenPrintf()
{
	return 0;
}

void DoScreenPrintfs()
{
	DisplayDebugScreenPrints();

	if (*(DWORD*)_TheGameFlowManager == 6)
	{
		if (mCurPursuitForm || mCurPursuitState) PursuitPrintf();

		//SessionIDPrintf();
		//OnlineScreenPrintf();
	}
}

// 0x4404B9
void __declspec(naked) AIPursuit_InitFormation_GetValues()
{
	_asm
	{
		push ebx
		mov ebx,2
		mov mCurPursuitState, ebx
		pop ebx

		push edx
		mov edx, dword ptr ds:[esi+0xE0]
		mov mCurPursuitLevel, edx
		pop edx

		mov ecx, [esp + 8]
		pop esi
		push 0x4404BE
		retn
	}
}

// 0x8927DC = 0x40AA80
void __fastcall AIPursuit_EndCurrentFormation(DWORD* AIPursuit, void* EDX_unused)
{
	AIPursuit[49] = 0;
	AIPursuit[44] = -1.0f;
	mCurPursuitLevel = AIPursuit[56]; // ?
	mCurPursuitState = 0;
	mCurPursuitForm = 0;
}

// 0x4440B3
void __declspec(naked) AIPursuit_OnTask_SetValues()
{
	_asm
	{
		mov mCurPursuitForm, eax
		call AIPursuit_InitFormation

		push 0x4440B8
		retn
	}
}

// 0x4337AD
void __declspec(naked) AIPursuit_Destructor_SetValues()
{
	_asm
	{
		call Sim_Activity_DetachAll

		push eax
		mov eax,0
		mov mCurPursuitForm, eax
		mov mCurPursuitState, eax
		pop eax

		push 0x4337B2
		retn
	}
}

// 0x429DBC
void __declspec(naked) AIPerpVehicle_OnCausedCollision_SetValues()
{
	_asm
	{
		call AITarget_Acquire

		push eax
		mov eax, 2
		mov mCurPursuitState, eax
		pop eax

		push 0x429DC1
		retn
	}
}

// 0x42A06C
void __declspec(naked) AIVehicleCopCar_CheckForPursuit_SetValues()
{
	_asm
	{
		mov eax, [esp+0x14]
		mov edx, [ebp+0]

		push ebx
		mov ebx, 2
		mov mCurPursuitState, ebx
		pop ebx

		push 0x42A073
		retn
	}
}
