#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include "..\includes\IniReader.h"

void Thing()
{
	//Sleep(ThreadDelay);
	_asm pushad;

	raceOptions = *(DWORD*)_FEDatabase; // Race Options Pointer (Thanks to samfednik)
	GameState = *(DWORD*)_TheGameFlowManager; // 3 = FE, 4&5 = Loading screen, 6 = Gameplay
	windowHandle = *(HWND*)_hWnd;
	IsOnFocus = !(*(bool*)_IsLostFocus);

	// Windowed Mode Related Fixes (Center and Resize)
	if (WindowedMode && windowHandle && !once2)
	{
		RECT o_cRect, n_cRect, n_wRect;
		GetClientRect(windowHandle, &o_cRect);

		DWORD wStyle = GetWindowLongPtr(windowHandle, GWL_STYLE);

		switch (WindowedMode)
		{
		case 1: wStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU); break;
		case 2: default: wStyle |= (WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW); break;
		}

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

		HWND hDesktop = GetDesktopWindow();
		RECT desktop;
		GetWindowRect(hDesktop, &desktop);

		int newXPos = ((desktop.right - desktop.left) - newWidth) / 2;
		int newYPos = ((desktop.bottom - desktop.top) - newHeight) / 2;

		SetWindowPos(windowHandle, NULL, newXPos, newYPos, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);

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

		DWORD* PlayerPVehicle = *(DWORD**)_PlayerPVehicle;

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

		DWORD* PlayerPVehicle = *(DWORD**)_PlayerPVehicle;

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
		*(bool*)_Camera_StopUpdating = !(*(bool*)_Camera_StopUpdating);
	}

	// Unlock All Things
	if ((GetAsyncKeyState(hotkeyUnlockAllThings) & 1) && IsOnFocus) // When pressed "Unlock All Things" key
	{
		UnlockAllThings = !UnlockAllThings;
		CIniReader iniReader("NFSMWExtraOptionsSettings.ini");
		iniReader.WriteInteger("Gameplay", "UnlockAllThings", UnlockAllThings);

		if (UnlockAllThings)
		{
			*(unsigned char*)_UnlockAllThings = 1;

			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "You have enabled UnlockAllThings hack.^Everything is now unlocked. Enjoy!");
			}

		}
		else
		{
			*(unsigned char*)_UnlockAllThings = 0;

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
			*(unsigned int*)_SaveHotPosition = 1;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 1);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(50) & 0x8000)
		{
			*(unsigned int*)_SaveHotPosition = 2;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 2);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(51) & 0x8000)
		{
			*(unsigned int*)_SaveHotPosition = 3;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 3);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(52) & 0x8000)
		{
			*(unsigned int*)_SaveHotPosition = 4;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 4);
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) & GetAsyncKeyState(53) & 0x8000)
		{
			*(unsigned int*)_SaveHotPosition = 5;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Your current position data is saved to Slot %d.", 5);
			}
		}

		// Load
		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(49) & 0x8000)
		{
			*(unsigned int*)_JumpToHotPosition = 1;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 1);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(50) & 0x8000)
		{
			*(unsigned int*)_JumpToHotPosition = 2;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 2);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(51) & 0x8000)
		{
			*(unsigned int*)_JumpToHotPosition = 3;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 3);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(52) & 0x8000)
		{
			*(unsigned int*)_JumpToHotPosition = 4;
			if (ShowMessage)
			{
				DialogInterface_ShowNButtons(1, NULL, DLGTITLE_INFO, 0, DialogBoxReturn, 0, 0, DialogBoxButtonOK, 0, 0, "Loaded position data from Slot %d.", 4);
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) & GetAsyncKeyState(53) & 0x8000)
		{
			*(unsigned int*)_JumpToHotPosition = 5;
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

			if (*(int*)_TheGameFlowManager == 6 && DebugWorldCamera)
			{
				injector::WriteMemory<unsigned char>(0x750F4C, 0xEB, true); // fix crash??
				*(unsigned int*)_WeHaveCheckedIfJR2ServerExists = 0;
				CameraAI_SetAction(1, "CDActionDebug");
			}

			if (*(int*)_TheGameFlowManager == 6 && !DebugWorldCamera)
			{
				CameraAI_SetAction(1, "CDActionDrive");
				*(unsigned char*)_WeHaveCheckedIfJR2ServerExists = 0;
				injector::WriteMemory<unsigned char>(0x750F4C, 0x75, true);
			}
		}

		if ((GetAsyncKeyState(VK_NEXT) & 1)) // Page Down : Toggle Debug Watch Car Camera
		{
			DebugWatchCarCamera = !DebugWatchCarCamera;
			*(unsigned char*)_mToggleCar = 1;

			if (*(int*)_TheGameFlowManager == 6 && DebugWatchCarCamera)
			{
				injector::WriteMemory<unsigned char>(0x750F4C, 0xEB, true); // fix crash??
				*(unsigned char*)_CameraDebugWatchCar = 1;
				CameraAI_SetAction(1, "CDActionDebugWatchCar");
			}

			if (*(int*)_TheGameFlowManager == 6 && !DebugWatchCarCamera)
			{
				CameraAI_SetAction(1, "CDActionDrive");
				*(unsigned char*)_CameraDebugWatchCar = 0;
				injector::WriteMemory<unsigned char>(0x750F4C, 0x75, true);
			}
		}

		if ((GetAsyncKeyState(VK_ADD) & 1) && DebugWatchCarCamera) // + : Next car
		{
			*(unsigned char*)_mToggleCar += 1;
		}

		if ((GetAsyncKeyState(VK_SUBTRACT) & 1) && DebugWatchCarCamera) // - : Previous car
		{
			*(unsigned char*)_mToggleCar -= 1;
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