#include <plugin.h>
#include "game_sa\CFont.h"
#include "MobileAreaName.h"
#include "Settings.h"
#include "CTimer.h"

void MobileAreaName::InstallPatches() {
    plugin::patch::RedirectCall(0x58AE5D, DrawAreaName);
}

void MobileAreaName::DrawAreaName(float x, float y, char *str) {
	/*if (boolvars.menuisactive == false) {
		if (boolvars.startfade == true)
		{
			if (boolvars.unfadetime + 2500 > CTimer::m_snTimeInMilliseconds)
			{
				CFont::SetBackground(0, 0);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetFontStyle(FONT_GOTHIC);
				CFont::SetEdge(3);
				CFont::SetScale(SCREEN_MULTIPLIER(settings.vecAreaNameScale.x), SCREEN_MULTIPLIER(settings.vecAreaNameScale.y));

				CFont::PrintString(SCREEN_COORD_RIGHT(settings.vecAreaNamePosn.x), SCREEN_COORD_BOTTOM(settings.vecAreaNamePosn.y), str);
			}
		}
		else
		{
			CFont::SetBackground(0, 0);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_GOTHIC);
			CFont::SetEdge(3);
			CFont::SetScale(SCREEN_MULTIPLIER(settings.vecAreaNameScale.x), SCREEN_MULTIPLIER(settings.vecAreaNameScale.y));

			CFont::PrintString(SCREEN_COORD_RIGHT(settings.vecAreaNamePosn.x), SCREEN_COORD_BOTTOM(settings.vecAreaNamePosn.y), str);
		}
	}*/
}