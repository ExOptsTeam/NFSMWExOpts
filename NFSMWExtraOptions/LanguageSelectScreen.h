#include "stdio.h"
#include "InGameFunctions.h"

DWORD __fastcall ChooseLanguage_React(DWORD* IconOption, void* EDX_Unused, char const* FEPackageName, unsigned int Data, DWORD* FEObject, unsigned int Param1, unsigned int Param2)
{
    if (Data == 0xC407210)
    {
        SetCurrentLanguage(IconOption[19]);
        DWORD* BootFlowManager = BootFlowManager_Get();
        return BootFlowManager_ChangeToNextBootFlowScreen(BootFlowManager, -1);
    }

    return 0;
}

DWORD ChooseLanguage_vtable[2] = {
    0x7B4980,
    (DWORD)ChooseLanguage_React
};

DWORD* __fastcall LanguageSelectScreen_LanguageSelectScreen(DWORD* LanguageSelectScreenPointer, void* EDX_Unused, DWORD* ScreenConstructorData)
{
    IconScrollerMenu_Create(LanguageSelectScreenPointer, ScreenConstructorData);
    LanguageSelectScreenPointer[0] = LanguageSelectScreen_vtable;
    LanguageSelectScreenPointer[82] = 0;

    DWORD* TheIconOption;
    CIniReader RaceFixesSettings("NFSMWRaceFixesSettings.ini");

    if (bFileExists("Languages\\English.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_ENGLISH"), bStringHash("LANG_ENGLISH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 0;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\French.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_FRENCH"), bStringHash("LANG_FRENCH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 1;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\German.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_GERMAN"), bStringHash("LANG_GERMAN"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 2;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Italian.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_ITALIAN"), bStringHash("LANG_ITALIAN"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 3;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Spanish.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_SPANISH"), bStringHash("LANG_SPANISH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 4;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Dutch.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_DUTCH"), bStringHash("LANG_DUTCH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 5;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Swedish.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_SWEDISH"), bStringHash("LANG_SWEDISH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 6;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Danish.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_DANISH"), bStringHash("LANG_DANISH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 7;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Finnish.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_FINNISH"), bStringHash("LANG_FINNISH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 13;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }

    if (bFileExists("Languages\\Polish.bin"))
    {
        TheIconOption = (DWORD*)j_malloc(80);
        if (TheIconOption)
        {
            IconOption_Create(TheIconOption, bStringHash("LANGSELECT_ICON_POLISH"), bStringHash("LANG_POLISH"), 0);
            TheIconOption[0] = (DWORD)ChooseLanguage_vtable;
            TheIconOption[19] = 12;
        }
        else
        {
            TheIconOption = 0;
        }
        IconScrollerMenu_AddOption(LanguageSelectScreenPointer, TheIconOption);
    }
    
    if (*((BYTE*)LanguageSelectScreenPointer + 297))
    {
        *((BYTE*)LanguageSelectScreenPointer + 284) = 0;
        *((BYTE*)LanguageSelectScreenPointer + 281) = 1;
        *((BYTE*)LanguageSelectScreenPointer + 282) = 0;
        LanguageSelectScreenPointer[68] = 0;
    }
    (*(void(__thiscall**)(DWORD*, DWORD))(LanguageSelectScreenPointer[11] + 64))(LanguageSelectScreenPointer + 11, 0);
    IconScrollerMenu_RefreshHeader(LanguageSelectScreenPointer);
    LanguageSelectScreenPointer[82] = *(DWORD*)_RealTimer;
    return LanguageSelectScreenPointer;
}