// The NFS Most Wanted (and Underground 2?) DialogInterface hook
// by Xanvier / xan1242

#include "stdio.h"
#include "DialogInterfaceAddresses.h"

#define MESSAGEOBJECTHASH 0x1E2640FA
#define BUTTON1OBJECTHASH 0xF9363F30
#define BUTTON2OBJECTHASH 0xFB8B67D1
#define BUTTON3OBJECTHASH 0xFDE09072

// Unnecessary but left for reference
/*#define ANIMATING_FNG "OL_Dialog.fng"
#define STACKED_FNG "OL_Dialog_Stacked_Buttons.fng"
#define NORMAL_FNG "Dialog.fng"
#define INGAME_FNG "InGameDialog.fng"*/

#define DLG_ANIMATING "animating"
#define DLG_3BUTTON "3button"
#define DLG_STACKED "3buttons_stacked"

#define DLGTITLE_INFO 0
#define DLGTITLE_ATTN 1
#define DLGTITLE_SEL 3

struct feDialogConfig
{
	char MsgBuffer[512]; // The message buffer
	int eDialogTitle; // Dialog title type
	unsigned int Button1LangHash; // Button 1 language hash
	unsigned int Button1Hash; // Button 1 message hash
	unsigned int Button2LangHash; // Button 2 language hash
	unsigned int Button2Hash; // Button 2 message hash
	unsigned int Button3LangHash; // Button 3 language hash
	unsigned int Button3Hash; // Button 3 message hash
	unsigned int v22; // Unknown
	int eDialogFirstButtons; // First selected button
	char* SomeFNG; // Unknown
	const char* Type; // Type of dialog box
	int NumberOfButtons; // Number of buttons in a dialog box
	int v27; // Unknown
	unsigned int unk1; // Unknown 
	unsigned int unk2; // Unknown
	unsigned int unk3; // Unknown
	unsigned int unk4; // Unknown
	char* SomeFNG2; // Unknown
	char* unk6; // Unknown
	unsigned int unk7; // Unknown
};

struct FEColor
{
	unsigned int Blue;
	unsigned int Green;
	unsigned int Red;
	unsigned int Alpha;
};

int(__cdecl *DialogInterface_ShowDialog)(feDialogConfig* DialogConfig) = (int(__cdecl*)(feDialogConfig*))SHOWDIALOG_ADDRESS; // return value is its DialogNum (dialog number)
bool(__cdecl *DialogInterface_SetButtonText)(int ButtonNumber, const char* Text, bool IsWideString) = (bool(__cdecl*)(int, const char*, bool))SETBUTTONTEXT_ADDRESS;
int(__cdecl *FEPrintf)(const char* pkg_name, unsigned int obj_hash, const char* format, ...) = (int(__cdecl*)(const char*, unsigned int, const char*, ...))FEPRINTF_ADDRESS;
int(__cdecl *FEPrintf_Obj)(const char* pkg_name, void *FEObject, const char* format, ...) = (int(__cdecl*)(const char*, void*, const char*, ...))FEPRINTF2_ADDRESS;
int(__cdecl *DialogInterface_DismissDialog)(int DialogNum) = (int(__cdecl*)(int))DISMISSDIALOG_ADDRESS;
void*(__cdecl *FEObject_FindObject)(const char *pkg_name, unsigned int obj_hash) = (void*(__cdecl*)(const char*, unsigned int))FENGFINDOBJECT_ADDRESS;
void(__thiscall *FEObject_SetColor)(void* FEObject, FEColor &color, bool bRelative) = (void(__thiscall*)(void*, FEColor&, bool))FEOBJECT_SETCOLOR_ADDRESS;

int __cdecl CurrentDialog_Dismiss()
{
	return DialogInterface_DismissDialog(*(int*)CURRENTDIALOGNUM_ADDRESS);
}

int DialogInterface_ShowDialog_Custom(feDialogConfig* DialogConfig, const char* Button1Text, const char* Button2Text, const char* Button3Text)
{
	int Result;
	feDialogConfig ButtonChecker = *DialogConfig;

	Result = DialogInterface_ShowDialog(DialogConfig);

	Sleep(20); // hack to satisfy race conditions

	switch (ButtonChecker.NumberOfButtons)
	{
	case 3:
		if (Button3Text)
			DialogInterface_SetButtonText(2, Button3Text, 0);
	case 2:
		if (Button2Text)
			DialogInterface_SetButtonText(1, Button2Text, 0);
	case 1:
		if (Button1Text)
			DialogInterface_SetButtonText(0, Button1Text, 0);
	default:
		break;
	}

	return Result;
}

int DialogInterface_ShowMessage(const char* Type, int eDialogTitle, const char* format, ...)
{
	va_list Args;
	feDialogConfig TheDialogConfig = { 0 };

	TheDialogConfig.Type = Type;
	TheDialogConfig.eDialogTitle = eDialogTitle;

	va_start(Args, format);
	vsprintf(TheDialogConfig.MsgBuffer, format, Args);
	va_end(Args);

	return DialogInterface_ShowDialog(&TheDialogConfig);
}

int DialogInterface_ShowNButtons(unsigned int NumberOfButtons, const char* Type, int eDialogTitle, int eDialogFirstButtons, unsigned int Button1Hash, unsigned int Button2Hash, unsigned int Button3Hash, unsigned int Button1LangHash, unsigned int Button2LangHash, unsigned int Button3LangHash, const char* format, ...)
{
	va_list Args;
	feDialogConfig TheDialogConfig = { 0 };

	TheDialogConfig.Type = Type;
	TheDialogConfig.eDialogTitle = eDialogTitle;
	TheDialogConfig.eDialogFirstButtons = eDialogFirstButtons;

	if (NumberOfButtons > 3)
		NumberOfButtons = 3;

	TheDialogConfig.NumberOfButtons = NumberOfButtons;

	switch (NumberOfButtons)
	{
	case 3:
		TheDialogConfig.Button3Hash = Button3Hash;
		TheDialogConfig.Button3LangHash = Button3LangHash;
	case 2:
		TheDialogConfig.Button2Hash = Button2Hash;
		TheDialogConfig.Button2LangHash = Button2LangHash;
	case 1:
		TheDialogConfig.Button1Hash = Button1Hash;
		TheDialogConfig.Button1LangHash = Button1LangHash;
	default:
		break;
	}

	va_start(Args, format);
	vsprintf(TheDialogConfig.MsgBuffer, format, Args);
	va_end(Args);

	return DialogInterface_ShowDialog(&TheDialogConfig);
}

int DialogInterface_ShowNButtons_Custom(unsigned int NumberOfButtons, const char* Type, int eDialogTitle, int eDialogFirstButtons, unsigned int Button1Hash, unsigned int Button2Hash, unsigned int Button3Hash, const char* Button1Text, const char* Button2Text, const char* Button3Text, const char* format, ...)
{
	va_list Args;
	feDialogConfig TheDialogConfig = { 0 };

	TheDialogConfig.Type = Type;
	TheDialogConfig.eDialogTitle = eDialogTitle;
	TheDialogConfig.eDialogFirstButtons = eDialogFirstButtons;

	if (NumberOfButtons > 3)
		NumberOfButtons = 3;

	TheDialogConfig.NumberOfButtons = NumberOfButtons;

	switch (NumberOfButtons)
	{
	case 3:
		TheDialogConfig.Button3Hash = Button3Hash;
	case 2:
		TheDialogConfig.Button2Hash = Button2Hash;
	case 1:
		TheDialogConfig.Button1Hash = Button1Hash;
	default:
		break;
	}

	va_start(Args, format);
	vsprintf(TheDialogConfig.MsgBuffer, format, Args);
	va_end(Args);

	return DialogInterface_ShowDialog_Custom(&TheDialogConfig, Button1Text, Button2Text, Button3Text);
}

#pragma warning(disable:4100)
int __cdecl CurrentDialog_FEPrintf(const char* Reserved, unsigned int obj_hash, const char* format, ...)
{
	_asm
	{
		pop ebp
		add esp, 8
		mov eax, ds:CURRENTDIALOGPKG_ADDRESS
		push eax
		sub esp, 4
		jmp FEPrintf
	}
}
#pragma warning(default:4100)