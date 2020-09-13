#pragma once
#include "game_sa\CRect.h"
#include "CRGBA.h"

class MobileTextBox {
public:
    static void InstallPatches();
    static void MyHelpBox_DrawBox(CRect const& rect, CRGBA const& color);
    static void MyHelpBox_Draw();
    static float GetHelpBoxXShift();
    static bool IsMenuEnabled();
};