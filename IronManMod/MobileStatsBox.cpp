#include <plugin.h>
#include "game_sa\common.h"
#include "game_sa\CClock.h"
#include "game_sa\CFont.h"
#include "game_sa\CHudColours.h"
#include "game_sa\CMenuManager.h"
#include "game_sa\CPad.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CStats.h"
#include "game_sa\CText.h"
#include "MobileStatsBox.h"
#include "Settings.h"
#include "Utility.h"
#include "MobileTextures.h"
#include "CFileLoader.h"

static CSprite2d *statbox[2];

void MobileStatsBox::InstallPatches() {
	Events::initRwEvent += [] {
		// Load png and texture
		statbox[0] = new CSprite2d();
		statbox[0]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats\\statbars_unarmed.png"));
		statbox[1] = new CSprite2d();
		statbox[1]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats\\statbars_armed.png"));
	};
    plugin::patch::Set<BYTE>(0x58FC2C, 0xEB);
    plugin::Events::drawHudEvent += DrawVitalStats;
}

void __cdecl MobileStatsBox::DrawVitalStats() {
	if (boolvars.menuisactive == false) {
		int CURWSKL;
		int CURWSKL_ALPHA;
		CPed *player = FindPlayerPed(-1);
		CPad *pad = CPad::GetPad(0);

		switch (CWeaponInfo::GetWeaponInfo((eWeaponType)player->m_aWeapons[player->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1) {
		case MODEL_COLT45:
			CURWSKL = 69;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_SILENCED:
			CURWSKL = 70;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_DESERT_EAGLE:
			CURWSKL = 71;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_CHROMEGUN:
			CURWSKL = 72;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_SAWNOFF:
			CURWSKL = 73;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_SHOTGSPA:
			CURWSKL = 74;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_MICRO_UZI:
			CURWSKL = 75;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_MP5LNG:
			CURWSKL = 76;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_AK47:
			CURWSKL = 77;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_M4:
			CURWSKL = 78;
			CURWSKL_ALPHA = 255;
			break;
		case MODEL_SNIPER:
			CURWSKL = 79;
			CURWSKL_ALPHA = 255;
			break;
		default:
			CURWSKL = -1;
			CURWSKL_ALPHA = 50;
			break;
		}

		CVector2D vecStatsBoxScale;
		vecStatsBoxScale.x = 260.0f;
		vecStatsBoxScale.y = 255.0f;
		float vecStatsBoxScalex = SCREEN_COORD(vecStatsBoxScale.x);
		float vecStatsBoxScaley = SCREEN_COORD(vecStatsBoxScale.y);
		float settingsvecStatsBoxPosnx = SCREEN_COORD(settings.vecStatsBoxPosn.x);
		float settingsvecStatsBoxPosny = SCREEN_COORD_BOTTOM(settings.vecStatsBoxPosn.y);
		boolvars.toptextboxcoord = (settings.vecStatsBoxPosn.y) - vecStatsBoxScaley + SCREEN_COORD(15.0f);
		
		float ofstcord = SCREEN_HEIGHT * 0.1f;

		if (!pad->GetDisplayVitalStats(player) || FindPlayerVehicle(-1, 0)) {
			// Set Statsbox is closed

			settings.IsStatsBoxOpen = 0;
			
		}
		else {
			// Set Statsbox is open


			char *indiceee;
			indiceee = new char[4];
			sprintf(indiceee, "%d", boolvars.indx);
			CFont::SetBackground(0, 0);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetScale(SCREEN_MULTIPLIER(settings.vecVehicleNameScale.x), SCREEN_MULTIPLIER(settings.vecVehicleNameScale.y));
			CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesPosnY), indiceee);

			delete[] indiceee;



			settings.IsStatsBoxOpen = 1;
			if (settings.hud == 1)
			{
				if (settings.marktraje > 1)
				{
					statbox[1]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 8.0f - ofstcord,
						SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 2.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT / 8.0f - ofstcord,
						SCREEN_COORD_CENTER_DOWN(83.0f)), CRGBA(255, 255, 255, 255));
					CFont::SetColor(CRGBA(255, 255, 255, 255));
				}
				else {
					statbox[0]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 8.0f - ofstcord,
						SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 2.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT / 8.0f - ofstcord,
						SCREEN_COORD_CENTER_DOWN(83.0f)), CRGBA(255, 255, 255, 255));
					CFont::SetColor(CRGBA(255, 255, 255, 255));
				}
				settingsvecStatsBoxPosnx = SCREEN_WIDTH - (SCREEN_HEIGHT /4.75f) - (boolvars.ofstappearx);
				settingsvecStatsBoxPosny = SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 7.0f;
				vecStatsBoxScalex = SCREEN_HEIGHT / 4.35f;
				vecStatsBoxScaley = SCREEN_HEIGHT / 4.0f - SCREEN_COORD(40.0f);
			}
			else
			{
				// Window
				CSprite2d::DrawRect(CRect(settingsvecStatsBoxPosnx, settingsvecStatsBoxPosny, settingsvecStatsBoxPosnx + vecStatsBoxScalex, settingsvecStatsBoxPosny - vecStatsBoxScaley), CRGBA(0, 0, 0, 255));

				// Header
				CFont::SetProportional(true);
				CFont::SetOrientation(ALIGN_LEFT);
				CFont::SetFontStyle(FONT_GOTHIC);
				CFont::SetEdge(2);
				CFont::SetScale(SCREEN_MULTIPLIER(1.7f), SCREEN_MULTIPLIER(2.9f));
				float width = CFont::GetStringWidth(TheText.Get("FEH_STA"), true, false);
				if (width > SCREEN_MULTIPLIER(225.0f))
					CFont::SetScale(SCREEN_MULTIPLIER(1.7f * SCREEN_MULTIPLIER(225.0f) / width), SCREEN_MULTIPLIER(2.9f));
				CFont::SetColor(CRGBA(255, 255, 255, 225));
				CFont::SetDropColor(CRGBA(0, 0, 0, 255));
				CFont::PrintStringFromBottom(settingsvecStatsBoxPosnx + SCREEN_COORD(15.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley +SCREEN_COORD(35.0f), TheText.Get("FEH_STA"));
			}

			/*
			char *shades;
			shades = new char[100];
			sprintf(shades, "nodo: %d", boolvars.indx);

			CFont::SetBackground(0, 0);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetScale(SCREEN_MULTIPLIER(settings.vecVehicleNameScale.x), SCREEN_MULTIPLIER(settings.vecVehicleNameScale.y));
			CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesPosnY), shades);
			delete[] shades;
			*/

			//

			/*
			*/
			// Stats
			CFont::SetOrientation(ALIGN_LEFT);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetEdge(0);

			// RESPECT_TOTAL
			
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetScale(SCREEN_MULTIPLIER(0.50f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f), TheText.Get("STAT068"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(64) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, 255), CRGBA(0, 170, 213, 255));

			// UNDERWATER_STAMINA
			CFont::SetScale(SCREEN_MULTIPLIER(0.35f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(30.0f), TheText.Get("STAT225"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f) + SCREEN_COORD(30.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(225) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, 255), CRGBA(0, 170, 213, 255));

			// WEAPON_SKILLS
			CFont::SetScale(SCREEN_MULTIPLIER(0.40f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(60.0f), TheText.Get("CURWSKL"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f) + SCREEN_COORD(60.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(CURWSKL) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, CURWSKL_ALPHA), CRGBA(0, 170, 213, CURWSKL_ALPHA));

			// STAMINA
			CFont::SetScale(SCREEN_MULTIPLIER(0.50f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(90.0f), TheText.Get("STAT022"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f) + SCREEN_COORD(90.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(22) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, 255), CRGBA(0, 170, 213, 255));

			// BODY_MUSCLE
			CFont::SetScale(SCREEN_MULTIPLIER(0.40f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(120.0f), TheText.Get("STAT023"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f) + SCREEN_COORD(120.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(23) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, 255), CRGBA(0, 170, 213, 255));

			// FAT
			CFont::SetScale(SCREEN_MULTIPLIER(0.50f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(150.0f), TheText.Get("STAT021"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f) + SCREEN_COORD(150.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(21) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, 255), CRGBA(0, 170, 213, 255));

			// SEX_APPEAL
			CFont::SetScale(SCREEN_MULTIPLIER(0.40f), SCREEN_MULTIPLIER(1.2f));
			CFont::PrintString(settingsvecStatsBoxPosnx + SCREEN_COORD(10.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(180.0f), TheText.Get("STAT025"));
			CSprite2d::DrawBarChart(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(110.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(35.0f) + SCREEN_COORD(180.0f), SCREEN_MULTIPLIER(80.0f), SCREEN_MULTIPLIER(15.0f), CStats::GetStatValue(25) * 0.001 * 100.0, 0, 0, 0, CRGBA(0, 170, 213, 255), CRGBA(0, 170, 213, 255));

			// DAY
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetFontStyle(FONT_PRICEDOWN);
			CFont::SetScale(SCREEN_MULTIPLIER(0.9f), SCREEN_MULTIPLIER(1.3f));
			sprintf(gString, "DAY_%d", (unsigned __int8)CClock::ms_nGameClockDays);
			CFont::PrintString(settingsvecStatsBoxPosnx + vecStatsBoxScalex - SCREEN_COORD(30.0f), (settingsvecStatsBoxPosny) - vecStatsBoxScaley + SCREEN_COORD(30.0f) + SCREEN_COORD(205.0f), TheText.Get(gString));
		}
	}
}
/*
CSprite2d *fondo = NULL;
CSprite2d *border1 = NULL;
CSprite2d *corner = NULL;
CSprite2d *border = NULL;

void MobileStatsBox::DrawRect(CRect const& rect, bool bFading, CRGBA const& color) {
	static bool en3;
	if (!en3) {
		//if (settings.HUDTexDictionary) {

			fondo = new CSprite2d();
			fondo->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\StatusText\\stat_bg.png"));
			border1 = new CSprite2d();
			border1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\StatusText\\stat_border1.png"));
			border = new CSprite2d();
			border->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\StatusText\\stat_border.png"));
			corner = new CSprite2d();
			corner->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\StatusText\\stat_corner.png"));
		//}
		en3 = true;
	}
	CRect topup;
	CRect rigsid;
	CRect downund;
	CRect lefsid;
	lefsid.left = BilinearOffset(rect.left) - 10.0f;
	lefsid.right = BilinearOffset(rect.left);
	lefsid.top = BilinearOffset(rect.top);
	lefsid.bottom = BilinearOffset(rect.bottom);

	rigsid.left = BilinearOffset(rect.right) + 10.0f;
	rigsid.right = BilinearOffset(rect.right);
	rigsid.top = BilinearOffset(rect.top);
	rigsid.bottom = BilinearOffset(rect.bottom);

	topup.left = BilinearOffset(rect.left);
	topup.right = BilinearOffset(rect.right);
	topup.top = BilinearOffset(rect.top);
	topup.bottom = BilinearOffset(rect.top) - 10.0f;

	downund.left = BilinearOffset(rect.left);
	downund.right = BilinearOffset(rect.right);
	downund.top = BilinearOffset(rect.bottom);
	downund.bottom = BilinearOffset(rect.bottom) + 10.0f;

	border1->Draw(topup, CRGBA(255, 255, 255, 190));
	border1->Draw(downund, CRGBA(255, 255, 255, 190));
	border->Draw(rigsid, CRGBA(255, 255, 255, 190));
	border->Draw(lefsid, CRGBA(255, 255, 255, 190));

	corner->Draw(CRect(BilinearOffset(rect.left) - 10.0f, BilinearOffset(rect.top) - 10.0f, BilinearOffset(rect.left), BilinearOffset(rect.top)), CRGBA(255, 255, 255, 190));
	corner->Draw(CRect(BilinearOffset(rect.right) + 10.0f, BilinearOffset(rect.top) - 10.0f, BilinearOffset(rect.right), BilinearOffset(rect.top)), CRGBA(255, 255, 255, 190));
	corner->Draw(CRect(BilinearOffset(rect.left) - 10.0f, BilinearOffset(rect.bottom) + 10.0f, BilinearOffset(rect.left), BilinearOffset(rect.bottom)), CRGBA(255, 255, 255, 190));
	corner->Draw(CRect(BilinearOffset(rect.right) + 10.0f, BilinearOffset(rect.bottom) + 10.0f, BilinearOffset(rect.right), BilinearOffset(rect.bottom)), CRGBA(255, 255, 255, 190));

	fondo->Draw(CRect(BilinearOffset(rect.left), BilinearOffset(rect.bottom), BilinearOffset(rect.right), BilinearOffset(rect.top)), CRGBA(255, 255, 255, 190));
	
	
	/*
	unsigned int savedShade;
    unsigned int savedAlpha;
    RwRenderStateGet(rwRENDERSTATESHADEMODE, &savedShade);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(rwSHADEMODEGOURAUD));
    RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
    if (bFading)
        CSprite2d::DrawRect(CRect(BilinearOffset(rect.left), BilinearOffset(rect.bottom),
            BilinearOffset(rect.right), BilinearOffset(rect.top)),
            CRGBA(color.red, color.green, color.blue, 255), CRGBA(color.red, color.green, color.blue, color.alpha),
            CRGBA(color.red, color.green, color.blue, 255), CRGBA(color.red, color.green, color.blue, color.alpha));
    else
        CSprite2d::DrawRect(CRect(BilinearOffset(rect.left), BilinearOffset(rect.bottom),
            BilinearOffset(rect.right), BilinearOffset(rect.top)), color);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(savedShade));
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));*/
/*}*/