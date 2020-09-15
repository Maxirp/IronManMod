#include "MobilePlayerInfo.h"
#include "Settings.h"
#include "Utility.h"
#include "plugin.h"
#include "game_sa\CHudColours.h"
#include "game_sa\CWorld.h"
#include "game_sa\CStats.h"
#include "game_sa\common.h"
#include "game_sa\CSprite.h"
#include "game_sa\CFont.h"
#include "game_sa\CHud.h"
#include "game_sa\CTimer.h"
#include "game_sa\CTxdStore.h"
#include "game_sa\CModelInfo.h"
#include "game_sa\CKeyGen.h"
#include "game_sa\CRadar.h"
#include "game_sa\CDarkel.h"
#include <math.h>
#include "game_sa\CFileLoader.h"
#include "extensions\ScriptCommands.h"
#include "CCamera.h"
#include "Movtextures.h"

using namespace plugin;

unsigned int MobilePlayerInfo::m_CurrentStar;
/*static CSprite2d *sm2health;
static CSprite2d *sm2healthindic;
static CSprite2d *sm2healthbg;*/
//static CSprite2d *sm2rtbg;

static void Project(float *, float *, float *, float );

void Project(float *vel24, float *vel25, float *vel26, float diez)
{
	*vel24 *= -1.0f;
	*vel25 *= -1.0f;
	//float *vel26 = *vel26;
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS, boolvars.helperobj, *vel24, *vel26, *vel25, &*vel24, &*vel25, &*vel26);

	*vel24 /= *vel26;
	*vel25 /= *vel26;
	
	static float zoom, vel21, vel22;

	plugin::scripting::CallCommandById(COMMAND_GET_CAMERA_FOV, &zoom);
	zoom /= 2.0f;
	vel21 = cosf(zoom);
	vel22 = sinf(zoom);
	//plugin::scripting::CallCommandById(COMMAND_COS, zoom, &vel21);
	//plugin::scripting::CallCommandById(COMMAND_SIN, zoom, &vel22);

	vel21 /= vel22;
	*vel24 *= vel21;
	*vel25 *= vel21;



	*vel24 *= 320.0f;
	*vel25 *= 298.66f;

	plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, *vel24, *vel25, 0.0f, 0.0f, &zoom);
	zoom /= 144.0f;
	//fflush(stdin);
	if (0.0 > *vel26)
	{
		*vel24 *= -1.0f;
		*vel25 *= -1.0f;
	}
	*vel25 *= diez;
	*vel24 += 320.5f;
	*vel25 += 224.5f;
}


void MobilePlayerInfo::InstallPatches() {
    static float shadowOffsetMp = 0.0009765625f;
    plugin::patch::SetPointer(0x719D7C + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719D94 + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719DB5 + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719DD1 + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719DF2 + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719E0E + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719E2F + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719E4B + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719E6F + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719E97 + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719EBF + 2, &shadowOffsetMp);
    plugin::patch::SetPointer(0x719EE3 + 2, &shadowOffsetMp);
    plugin::patch::SetUInt(0x719D90, 0xC17048);
    plugin::patch::SetUInt(0x719DCD, 0xC17048);
    plugin::patch::SetUInt(0x719E0A, 0xC17048);
    plugin::patch::SetUInt(0x719E47, 0xC17048);
    plugin::patch::SetUInt(0x719E63, 0xC17048);
    plugin::patch::SetUInt(0x719E8B, 0xC17048);
    plugin::patch::RedirectJump(0x58D7D0, MyDrawWeaponIcon);
    plugin::patch::RedirectJump(0x5893B0, MyDrawWeaponAmmo);
    plugin::patch::RedirectCall(0x58EB4E, MySetClockScale);
    plugin::patch::RedirectCall(0x58EC21, MyDrawClock);
    plugin::patch::RedirectCall(0x58F56B, MySetMoneyScale);
    plugin::patch::RedirectCall(0x58F607, MyDrawMoney);
    plugin::patch::RedirectJump(0x589270, MyDrawHealth);
    plugin::patch::RedirectJump(0x5890A0, MyDrawArmor);
    plugin::patch::RedirectJump(0x589190, MyDrawBreath);
    plugin::patch::RedirectCall(0x58DD42, MyStoreCurrentStar);
    plugin::patch::RedirectCall(0x58DFD3, MyDrawWantedLevel);
    strcpy(reinterpret_cast<char *>(0x866C8C), "$-%d");
    strcpy(reinterpret_cast<char *>(0x866C94), "$%d");
}

void MobilePlayerInfo::MyDrawWeaponIcon(CPed *player, int, int, float alpha) {
	static int weapModel;
	float baseY = 0.0f;
	if (player == CWorld::Players[1].m_pPed)
		baseY = settings.fSecondPlayerOffsetY;

	float ofstx = 0.0f;
	float ofsty = 0.0f;
	
	
	CVector2D vecWeaponIconPosn;
	CRadar::TransformRadarPointToScreenSpace(vecWeaponIconPosn, CVector2D(0.0f, 1.0f));

	ofsty = vecWeaponIconPosn.y - SCREEN_COORD(baseY + settings.vecWeaponIconPosny);
	ofstx = vecWeaponIconPosn.x;
	

	if (settings.hud == 1)
	{
		
		ofstx = SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 2.75f;
		ofsty = SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 3.0f;
	}

	if (boolvars.menuisactive == false && boolvars.hudsetting == true && boolvars.alphafad == 0)
	{
		if (boolvars.activesuit == 0 && settings.standardhud==true) // || boolvars.suit[1] == true
		{
			/*
			weaponicon1[6]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
				SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6,
				SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 5.2,
				SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6 + SCREEN_HEIGHT / 4.9), CRGBA(255, 255, 255, 255));
				*/
			weapModel = CWeaponInfo::GetWeaponInfo((eWeaponType)player->m_aWeapons[player->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1;

			if (weapModel <= 0) {
				CHud::Sprites[HUDSPRITE_FIST].Draw(CRect(ofstx - SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
					ofsty - SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f),
					ofstx + SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
					ofsty + SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f)), CRGBA(255, 255, 255, (uchar)alpha));
			}
			else {
				CBaseModelInfo *model = CModelInfo::GetModelInfo(weapModel);
				TxdDef *txd = CTxdStore::ms_pTxdPool->GetAt(model->m_nTxdIndex);
				if (txd && txd->m_pRwDictionary) {
					RwTexture *iconTex = RwTexDictionaryFindHashNamedTexture(txd->m_pRwDictionary, CKeyGen::AppendStringToKey(model->m_nKey, "ICON"));
					if (iconTex) {
						RwRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
						RwRenderStateSet(rwRENDERSTATETEXTURERASTER, iconTex->raster);
						CSprite::RenderOneXLUSprite(ofstx,
							ofsty, 1.0f, SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
							SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f), 255, 255, 255, alpha, 1.0f, 255, 0, 0);
						RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
					}
				}
			}
		}
	}
}

void MobilePlayerInfo::MyDrawWeaponAmmo(CPed *player, int, int, float alpha) {
	if (boolvars.menuisactive == false && boolvars.hudsetting == true && boolvars.alphafad == 0)
	{
		if (settings.standardhud == true)
		{
			unsigned int ammoInClip = player->m_aWeapons[player->m_nActiveWeaponSlot].m_nAmmoInClip;
			unsigned int totalAmmo = player->m_aWeapons[player->m_nActiveWeaponSlot].m_nTotalAmmo;
			eWeaponType wepType = (eWeaponType)player->m_aWeapons[player->m_nActiveWeaponSlot].m_nType;
			unsigned short maxAmmoInClip = CWeaponInfo::GetWeaponInfo(wepType, player->GetWeaponSkill())->m_nAmmoClip;
			
			if (boolvars.activesuit == 0)
			{
				if (maxAmmoInClip <= 1 || maxAmmoInClip >= 1000)
					sprintf(gString, "%d", totalAmmo);
				else {
					if (wepType == WEAPON_FTHROWER) {
						unsigned int total = 9999;
						if ((totalAmmo - ammoInClip) / 10 <= 9999)
						{
							total = (totalAmmo - ammoInClip) / 10;
						}
						sprintf(gString, "%d-%d", total, ammoInClip / 10);
					}
					else {
						unsigned int total = totalAmmo - ammoInClip;
						if (totalAmmo - ammoInClip > 9999)
						{
							total = 9999;
						}
						sprintf(gString, "%d-%d", total, ammoInClip);
					}
				}
				CFont::SetBackground(false, false);
				CFont::SetScale(SCREEN_MULTIPLIER(settings.vecWeaponAmmoScale.x), SCREEN_MULTIPLIER(settings.vecWeaponAmmoScale.y));
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetCentreSize(SCREEN_WIDTH);
				CFont::SetProportional(true);
				CFont::SetEdge(2);
				CFont::SetDropColor(CRGBA(0, 0, 0, 255));
				CFont::SetFontStyle(FONT_SUBTITLES);
				if ((totalAmmo - ammoInClip) >= 9999
					|| CDarkel::FrenzyOnGoing()
					|| wepType == WEAPON_UNARMED
					|| wepType == WEAPON_DETONATOR
					|| wepType >= WEAPON_DILDO1 && wepType < WEAPON_GRENADE
					|| wepType == WEAPON_PARACHUTE
					|| CWeaponInfo::GetWeaponInfo(wepType, 1)->m_nWeaponFire == 5
					|| CWeaponInfo::GetWeaponInfo(wepType, 1)->m_nSlot <= 1)
				{
					CFont::SetEdge(0);
				}
				else {
					CFont::SetColor(HudColour.GetRGBA(HUD_COLOUR_WHITE));
					float baseY = 0.0f;
					if (player == CWorld::Players[1].m_pPed)
						baseY = settings.fSecondPlayerOffsetY;

					float ofstx = 0.0f;
					float ofsty = 0.0f;

					CVector2D WeaponIconPosn;
					CRadar::TransformRadarPointToScreenSpace(WeaponIconPosn, CVector2D(0.0f, 1.0f));

					ofstx = WeaponIconPosn.x;

					ofsty = WeaponIconPosn.y - SCREEN_COORD(baseY + settings.vecWeaponAmmoPosny);
					/*
					if (settings.hud == 1)
					{
						weaponicon1[6]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
							SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6,
							SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 5.2,
							SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6 + SCREEN_HEIGHT / 4.9), CRGBA(255, 255, 255, 255));

						ofstx = SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 2.45f;
						ofsty = SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 3.0 - SCREEN_COORD(baseY + settings.vecWeaponAmmoPosny);;
					}*/

					CFont::PrintString(ofstx, ofsty, gString);
					CFont::SetEdge(0);/*
					float progress = (ammoInClip + totalAmmo);
					float positionY = settings.vecWeaponAmmoPosn.y + SCREEN_COORD(10.0f);
					if (CWorld::Players[1].m_pPed)
						positionY += settings.fSecondPlayerOffsetY;

					DrawProgressBar_BilinearOffset_RightAlign(SCREEN_COORD_RIGHT(settings.vecWeaponAmmoPosn.x), SCREEN_COORD(positionY),
						SCREEN_COORD(settings.vecHealthScale.x), SCREEN_COORD(settings.vecHealthScale.y), progress, 1.0f,
						HudColour.GetRGBA(HUD_COLOUR_WHITE), CRGBA(0, 0, 0, 255));*/
				}
			}
		}
	}
}

void MobilePlayerInfo::MySetClockScale(float scaleX, float scaleY) {
    CFont::SetScale(SCREEN_MULTIPLIER(settings.vecClockScale.x), SCREEN_MULTIPLIER(settings.vecClockScale.y));
}

void MobilePlayerInfo::MyDrawClock(float x, float y, char *text) {
	/*if (boolvars.menuisactive == false)
	{
		if (boolvars.startfade == true)
		{
			if (boolvars.unfadetime + 2500 > CTimer::m_snTimeInMilliseconds)
			{
				CFont::PrintString(SCREEN_COORD_RIGHT(settings.vecClockPosn.x), SCREEN_COORD(settings.vecClockPosn.y), text);
			}
		}
		else
		{
			if (boolvars.suit[1]) {
				if (settings.hud == 0)
				{
					float dist;
					static std::string savedtime;
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEIGHT_ABOVE_GROUND, CWorld::Players[0].m_pPed, &dist);
					if (dist <= 998)
					{
						savedtime = text;
					}
					CFont::SetEdge(0);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetFontStyle(FONT_MENU);
					CFont::SetScale(SCREEN_WIDTH / SCREEN_COORD(1920.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(1.9f));

					float leftline = SCREEN_WIDTH * 0.7622f;
					float rightline = SCREEN_WIDTH * 0.83f;
					float posy = SCREEN_HEIGHT / (2.0f);
					float espaciado = SCREEN_HEIGHT / (100.0f) * (2.35f);
					if (RsGlobal.maximumWidth == 2732 || RsGlobal.maximumWidth == 2720 || RsGlobal.maximumWidth == 2048 || RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840) {
						leftline = SCREEN_WIDTH - SCREEN_COORD(1920.0f * (1000.0f / 1312.0f));
						rightline = SCREEN_WIDTH - SCREEN_COORD(1920.0f * (10.0f / 12.0f));
						posy = SCREEN_HEIGHT / (2.0f);
						espaciado = SCREEN_HEIGHT / (100.0f) * (2.35f);
					}
					float posx1 = leftline + ((rightline - leftline) / 2.0f);
					//CFont::PrintString(posx1, posy + espaciado * -5.2, gString);
					//std::string time = "Time: ";
					//time += text;
					CFont::PrintString(posx1, posy + espaciado * -3.9f, savedtime.data());
					//time.clear();
				}
			}
			else
			{
				CFont::PrintString(SCREEN_COORD_RIGHT(settings.vecClockPosn.x), SCREEN_COORD(settings.vecClockPosn.y), text);
			}
		}
	}*/
}

CVector2D radarPos;
static float clockposy;

void MobilePlayerInfo::MySetMoneyScale(float scaleX, float scaleY) {

	CFont::SetScale(SCREEN_MULTIPLIER(settings.vecMoneyScale.x), SCREEN_MULTIPLIER(settings.vecMoneyScale.y));
}

static void DrawIMprogressbar(CRect pos1, float totalstat, float currstat, CRGBA color);

void DrawIMprogressbar(CRect pos1, float totalstat, float currstat, CRGBA color) {
	CSprite2d::DrawRect(CRect(pos1.left, pos1.top, pos1.right + 1.0f, pos1.top + SCREEN_COORD(2.0f)), color);
	CSprite2d::DrawRect(CRect(pos1.left, pos1.bottom, pos1.right + 1.0f, pos1.bottom - SCREEN_COORD(2.0f)), color);
	float sizex = 1.0f * (pos1.right - pos1.left);
	if (totalstat > currstat)
	{
		sizex = currstat / totalstat * (pos1.right - pos1.left);
	}

	for (float x = pos1.left; x < pos1.left + sizex; x += 6.0f)
	{
		CSprite2d::DrawRect(CRect(x, pos1.bottom - SCREEN_COORD(4.0f), x + SCREEN_COORD(5.0f), pos1.top + SCREEN_COORD(4.0f)), color);
	}
}
#include "Color.h"

void MobilePlayerInfo::MyDrawMoney(float x, float y, char *text) {
	if (boolvars.menuisactive == false && boolvars.hudsetting == true && boolvars.alphafad == 0)
	{
		if(settings.standardhud==true)
		{
			int mony = plugin::patch::GetInt(0xB7CE50, 0);
			static bool listo;
			static int timmony, parte, difmony, difmony1, stordmony, stord1;
			std::string diftext, texto;
			if (stordmony != mony)
			{
				if (mony != stord1)
				{
					listo = false;
					difmony1 = mony - stord1;
					stord1 = mony;
				}
				difmony = mony - stordmony;

				int sz = 1, temp = abs(difmony / 10);

				while (temp / 10 > 0) {
					temp /= 10;
					sz = sz * 10 + 1;
				}

				stordmony += difmony / abs(difmony) * sz;
				timmony = CTimer::m_snTimeInMilliseconds;
			}
			else
			{
				stordmony = mony;
			}

			if (CTimer::m_snTimeInMilliseconds < timmony + 3000)
			{
				//CRadar::TransformRadarPointToScreenSpace(radarPos, CVector2D(0.0f, 1.0f));
				sprintf(texto.data(), "$%d", stordmony);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetFontStyle(FONT_MENU);
				static int alfa;
				static float pos2;
				//clockposy = radarPos.y - SCREEN_COORD(settings.vecMoneyPosny);
				CFont::PrintStringFromBottom(SCREEN_COORD_MAX_X - settings.vecWantedLevelPosn.x, settings.vecMoneyPosny, texto.data());
				if (!listo && CTimer::m_snTimeInMilliseconds > timmony + 50 && alfa > 0)
				{
					alfa -= 15;
					pos2 += 1.0f;
				}
				else
				{
					if (!listo)
					{
						alfa = 255;
						pos2 = 0.0f;
					}
				}
				if (alfa <= 1)
					listo = true;

				if (difmony1 != 0)
				{
					CFont::SetDropColor(CRGBA(0, 0, 0, alfa));
					if (difmony1 > 0) {
						sprintf(diftext.data(), "+$%d", difmony1);
						CFont::SetColor(CRGBA(plugin::color::Green.r, plugin::color::Green.g, plugin::color::Green.b, alfa));
					}
					else {
						sprintf(diftext.data(), "-$%d", abs(difmony1));
						CFont::SetColor(CRGBA(plugin::color::Red.r, plugin::color::Red.g, plugin::color::Red.b, alfa));
					}
					CFont::PrintStringFromBottom(SCREEN_COORD_MAX_X - settings.vecWantedLevelPosn.x, settings.vecMoneyPosny + SCREEN_COORD(50.0f + pos2), diftext.data());
					diftext.clear();
				}
			}
			else
			{
				stordmony = mony;
				diftext.clear();
				difmony = 0;
				difmony1 = 0;
			}
		}
	}
}

static void holatarola(char *text);
static void holatarola(char *text)
{
	if (boolvars.menuisactive == false && boolvars.alphafad == 0)
	{
		if (boolvars.startfade == true)
		{
			if (boolvars.unfadetime + 2500 > CTimer::m_snTimeInMilliseconds)
			{
				CVector2D radarPos;
				CRadar::TransformRadarPointToScreenSpace(radarPos, CVector2D(0.0f, 1.0f));

				CFont::SetBackground(0, 0);
				//CFont::SetColor(CRGBA(255, 255, 255, 255)); 
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetFontStyle(FONT_MENU);
				CFont::SetEdge(1);


				float clockposy = radarPos.y - SCREEN_COORD(15.0f);
				CFont::PrintStringFromBottom(radarPos.x, clockposy, text);
				//CFont::PrintString(SCREEN_COORD_RIGHT(settings.vecMoneyPosn.x), SCREEN_COORD(settings.vecMoneyPosn.y), text);
			}
		}

		/*else
		{
			if (boolvars.suit[1]) {
				if (settings.hud == 0)
				{
					float posx, posy, espaciado;
					posx = SCREEN_WIDTH / 8.0f * 6.9f;
					posy = SCREEN_HEIGHT / (2.0f);
					espaciado = SCREEN_HEIGHT / (100.0f) * (2.35f);
					float rtposx = SCREEN_WIDTH / 1.116955445544554;
					float rtposy = SCREEN_HEIGHT / 1.55559670781893;
					float leftline = SCREEN_WIDTH * 0.7622f;
					float rightline = SCREEN_WIDTH * 0.83f;
					if (RsGlobal.maximumWidth == 2732 || RsGlobal.maximumWidth == 2720 || RsGlobal.maximumWidth == 2048 || RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840) {
						leftline = SCREEN_WIDTH - SCREEN_COORD(1920.0f * (1000.0f / 1312.0f));
						rightline = SCREEN_WIDTH - SCREEN_COORD(1920.0f * (10.0f / 12.0f));
						posx = SCREEN_WIDTH - SCREEN_COORD(1920.0f / 8.0f * 1.1f);
						posy = SCREEN_HEIGHT / (2.0f);
						espaciado = SCREEN_HEIGHT / (100.0f) * (2.35f);
						rtposx = SCREEN_WIDTH - SCREEN_COORD(1920.0f * (1.116955445544554 / 1.0f));
					}
					CFont::SetBackground(0, 0);
					CFont::SetProportional(true);
					CFont::SetJustify(false);
					CFont::SetEdge(0);
					CFont::SetColor(CRGBA(255, 255, 255, 255));
					//CFont::SetScale(SCREEN_WIDTH / SCREEN_COORD(1920.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(1.9f));
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetFontStyle(FONT_MENU);

					float posx1 = leftline + ((rightline - leftline) / 2.0f);
					CFont::PrintString(posx1, posy + espaciado * -2.6, text);
					CFont::SetScale(SCREEN_WIDTH / SCREEN_COORD(1920.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(1.9f));
					int kevlar = (int)(CWorld::Players[0].m_pPed->m_fArmour / 10.0f);
					int pulmon = (int)(CWorld::Players[0].m_pPed->m_pPlayerData->m_fBreath / 11.5f);

					//CFont::SetScale(SCREEN_MULTIPLIER(0.8f), SCREEN_MULTIPLIER(1.9f));

					CVector speed;
					if (CWorld::Players[0].m_pPed->m_nPedState == PEDSTATE_DRIVING || CWorld::Players[0].m_pPed->m_nPedState == PEDSTATE_PASSENGER)
					{
						speed = CWorld::Players[0].m_pPed->m_pVehicle->m_vecMoveSpeed;
					}
					else {
						speed = CWorld::Players[0].m_pPed->m_vecMoveSpeed;
					}
					sprintf(gString, "%.0f MPH", speed.Magnitude() * 174.5f);
					//sprintf(gString, "%d", 100);
					CFont::PrintString(rtposx, rtposy, gString);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetScale(SCREEN_WIDTH / SCREEN_COORD(1920.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(1.9f));

					float topline = SCREEN_HEIGHT / (2.0f);
					float bottomline = SCREEN_HEIGHT / (1.9673f);

					DrawIMprogressbar(CRect(leftline, topline, rightline, bottomline), 1000.0f, CWorld::Players[0].m_pPed->m_fArmour, CRGBA(255, 255, 255, 178));

					topline = SCREEN_HEIGHT / (1.86f);
					bottomline = SCREEN_HEIGHT / (1.83f);

					DrawIMprogressbar(CRect(leftline, topline, rightline, bottomline), CStats::GetFatAndMuscleModifier(STAT_MOD_AIR_IN_LUNG), CWorld::Players[0].m_pPed->m_pPlayerData->m_fBreath, CRGBA(255, 255, 255, 178));

					topline = SCREEN_HEIGHT / (1.73f);
					bottomline = SCREEN_HEIGHT / (1.703f);

					DrawIMprogressbar(CRect(leftline, topline, rightline, bottomline), CWorld::Players[0].m_pPed->m_fMaxHealth, CWorld::Players[0].m_pPed->m_fHealth, CRGBA(255, 255, 255, 178));
					sprintf(gString, "ar: %d", kevlar);
					CFont::PrintString(leftline + SCREEN_COORD(50.0f), posy + espaciado * -1.15f, gString);
					sprintf(gString, "o2: %d", pulmon);
					CFont::PrintString(leftline + SCREEN_COORD(50.0f), posy + espaciado * 0.4f, gString);
				}
			}

			CVector2D radarPos;
			CRadar::TransformRadarPointToScreenSpace(radarPos, CVector2D(0.0f, 1.0f));

			CFont::SetBackground(0, 0);
			//CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetEdge(1);


			float clockposy = radarPos.y - SCREEN_COORD(settings.vecMoneyPosny);
			CFont::PrintStringFromBottom(radarPos.x, clockposy, text);
		}*/
	}
}

static bool isfreezeable();

bool isfreezeable() {
	CPed *player;
	player = FindPlayerPed(0);
	static int hulkbuster;
	static int mark;
	mark = boolvars.mark;
	if (player)
	{
		if ((player->m_nModelIndex != 0)) {
			if (mark > 7)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			int armorsuit = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17];
			if (armorsuit != 2061229512 && armorsuit != 1226861131 && armorsuit != 1817399610 && armorsuit != 900618200 && armorsuit != -559695285)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}




void MobilePlayerInfo::MyDrawHealth(int playerId, int, int) {
	
	if (boolvars.menuisactive == false &&
		!boolvars.systemerror) {
		if (CHud::m_ItemToFlash != 4 || CTimer::m_FrameCounter & 8) {
			if (CWorld::Players[playerId].m_pPed->m_fHealth >= 10 || CTimer::m_FrameCounter & 8)
			{
				if (playerId == 0)
				{
					movtextures.drawindexedtexture(0);
				}
			}
		}
	}

}


void MobilePlayerInfo::MyDrawArmor(int playerId, int, int) {
	if (boolvars.menuisactive == false &&
		!boolvars.systemerror && boolvars.alphafad == 0) {
		if (boolvars.suit[1] && boolvars.hudsetting==true && settings.ironmanhud == true)
		{
			if ((CHud::m_ItemToFlash != 3 || CTimer::m_FrameCounter & 8) && CWorld::Players[playerId].m_pPed->m_fArmour > 1.0f) {
				/*float progress = CWorld::Players[playerId].m_pPed->m_fArmour / 1000.0f;
				if (progress > 1.0f)
					progress = 1.0f;


				int scalex = 70.0f;

				if (scalex > 100)
					scalex = 100;

				if (scalex < 50)
					scalex = 50;

				float scalexf = (float)scalex / 50.0f;
				//Spiderman FINAL STYLE
				sm2health->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(768.0 * scalexf) / 4.0f * 2.0f, SCREEN_COORD(768.0 * scalexf) / 4.0f), CRGBA(255, 255, 255, 255));
				sm2health->Draw(CRect(SCREEN_WIDTH, SCREEN_COORD(0.0f), SCREEN_WIDTH - SCREEN_COORD(768.0 * scalexf) / 4.0f * 2.0f, SCREEN_COORD(768.0 * scalexf) / 4.0f), CRGBA(255, 255, 255, 255));

				//float progress = player->m_fHealth / static_cast<float>(FindPlayerPed(0)->GetPlayerInfoForThisPlayerPed()->m_nMaxHealth);
				//float progress = player->m_fArmour / static_cast<float>(FindPlayerPed(0)->GetPlayerInfoForThisPlayerPed()->m_nMaxArmour);

				float posx1radar = (739.0f * progress);

				sm2healthindic->Draw(CRect(SCREEN_WIDTH - (SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 0.70458715596330275229357798165138) * progress), SCREEN_COORD(768.0 * scalexf) / 4.0f / 11.815384615384615384615384615385 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 9.6) * progress, SCREEN_WIDTH - (SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 0.70458715596330275229357798165138) * progress), SCREEN_COORD(768.0 * scalexf) / 4.0f / 11.815384615384615384615384615385 + SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 9.6) * progress), CRGBA(255, 255, 255, 255));

				progress= CWorld::Players[playerId].m_pPed->m_fHealth / CWorld::Players[playerId].m_pPed->m_fMaxHealth;

				sm2healthindic->Draw(CRect(SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 0.70458715596330275229357798165138) * progress, SCREEN_COORD(768.0 * scalexf) / 4.0f / 11.815384615384615384615384615385 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 9.6) * progress, SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 0.70458715596330275229357798165138) * progress, SCREEN_COORD(768.0 * scalexf) / 4.0f / 11.815384615384615384615384615385 + SCREEN_COORD(768.0 * scalexf) / 4.0f / 3.6571428571428571428571428571429 + (SCREEN_COORD(768.0 * scalexf) / 4.0f / 9.6) * progress), CRGBA(255, 255, 255, 255));

				sm2healthbg->Draw(CRect(SCREEN_COORD(768.0 * scalexf) / 2.0f - SCREEN_COORD(64.0f), SCREEN_COORD(32.0f), SCREEN_COORD(768.0f * scalexf) / 2.0f + SCREEN_COORD(192.0f), SCREEN_COORD(160.0f)), CRGBA(255, 255, 255, 255));
				sm2healthbg->Draw(CRect(SCREEN_WIDTH - (SCREEN_COORD(768.0 * scalexf) / 2.0f - SCREEN_COORD(64.0f)), SCREEN_COORD(32.0f), SCREEN_WIDTH - (SCREEN_COORD(768.0f * scalexf) / 2.0f + SCREEN_COORD(192.0f)), SCREEN_COORD(160.0f)), CRGBA(255, 255, 255, 255));
				//sm2rtbg->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(768.0 * scalexf) / 2.0f - SCREEN_COORD(192.0f), SCREEN_COORD(32.0f), SCREEN_WIDTH - SCREEN_COORD(768.0f * scalexf) / 2.0f + SCREEN_COORD(64.0f), SCREEN_COORD(160.0f)), CRGBA(255, 255, 255, 255));


				char str[8];
				sprintf(str, "%.0f", CWorld::Players[playerId].m_pPed->m_fArmour);
				CFont::SetBackground(0, 0);
				CFont::SetOrientation(ALIGN_LEFT);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetColor(CRGBA(255, 255, 255, 255));
				CFont::SetFontStyle(FONT_SUBTITLES);
				CFont::SetEdge(0);
				CFont::SetScale(SCREEN_MULTIPLIER((768.0 * scalexf) / 5.0f / 102.4f / 4.0f), SCREEN_MULTIPLIER((768.0 * scalexf) / 5.0f / 102.4f / 2.0f));
				CFont::PrintStringFromBottom(SCREEN_WIDTH - (SCREEN_COORD(768.0 * scalexf) / 2.0f + SCREEN_COORD(64.0f)), SCREEN_COORD(96.0f), str);

				sprintf(str, "%.0f", CWorld::Players[playerId].m_pPed->m_fHealth);
				CFont::SetBackground(0, 0);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetColor(CRGBA(255, 255, 255, 255));
				CFont::SetFontStyle(FONT_SUBTITLES);
				CFont::SetEdge(0);
				CFont::SetScale(SCREEN_MULTIPLIER((768.0 * scalexf) / 5.0f / 102.4f / 4.0f), SCREEN_MULTIPLIER((768.0 * scalexf) / 5.0f / 102.4f / 2.0f));
				CFont::PrintStringFromBottom(SCREEN_COORD(768.0 * scalexf) / 2.0f + SCREEN_COORD(64.0f), SCREEN_COORD(96.0f), str);
				*/
				/*
				float positionY = settings.vecArmorPosn.y;
				if (playerId == 1)
				{
					positionY += settings.fSecondPlayerOffsetY;
					DrawProgressBar_BilinearOffset_RightAlign(SCREEN_COORD_RIGHT(settings.vecArmorPosn.x), SCREEN_COORD(positionY),
						SCREEN_COORD(settings.vecArmorScale.x), SCREEN_COORD(settings.vecArmorScale.y), progress, 1.0f,
						HudColour.GetRGBA(HUD_COLOUR_WHITE), CRGBA(0, 0, 0, 255));
				}
				else
				{
					if (boolvars.suit[1]) {
					}
					else
					{
						DrawProgressBar_BilinearOffset_RightAlign(SCREEN_COORD_RIGHT(settings.vecArmorPosn.x), SCREEN_COORD(positionY),
							SCREEN_COORD(settings.vecArmorScale.x), SCREEN_COORD(settings.vecArmorScale.y), progress, 1.0f,
							HudColour.GetRGBA(HUD_COLOUR_WHITE), CRGBA(0, 0, 0, 255));

					}
				}*/
			}
		}
	}
}

void MobilePlayerInfo::MyDrawBreath(int playerId, int, int) {
	if (boolvars.menuisactive == false && settings.standardhud && settings.ironmanhud && boolvars.alphafad == 0)
	{
		if (CHud::m_ItemToFlash != 10 || CTimer::m_FrameCounter & 8) {
			float progress = CWorld::Players[playerId].m_pPed->m_pPlayerData->m_fBreath / CStats::GetFatAndMuscleModifier(STAT_MOD_AIR_IN_LUNG);
			if (progress > 1.0f)
				progress = 1.0f;
			float positionY = SCREEN_COORD_CENTER_DOWN(10.0f);
			if (playerId == 1)
				positionY += settings.fSecondPlayerOffsetY;

			DrawProgressBar_BilinearOffset_RightAlign(SCREEN_COORD_CENTER_RIGHT(settings.vecBreathScale.x / 2.0f), SCREEN_COORD(positionY),
				SCREEN_COORD(settings.vecBreathScale.x), SCREEN_COORD(settings.vecBreathScale.y), progress, 1.0f,
				HudColour.GetRGBA(HUD_COLOUR_BLUELIGHT), CRGBA(0, 0, 0, 255));
		}
	}
}

__declspec(naked) void MobilePlayerInfo::MyStoreCurrentStar(short) {
    __asm mov m_CurrentStar, edi
    __asm retn
}

void MobilePlayerInfo::MyDrawWantedLevel(float x, float y, char *text) {
	if (boolvars.menuisactive == false && boolvars.hudsetting == true && boolvars.alphafad == 0)
	{
		if (FindPlayerPed(-1)->m_pPlayerData->m_pWanted->m_nWantedLevel > m_CurrentStar)
			CFont::SetEdge(2);
		else
			CFont::SetEdge(0);
		CFont::SetScale(SCREEN_MULTIPLIER(settings.vecWantedLevelStarScale.x), SCREEN_MULTIPLIER(settings.vecWantedLevelStarScale.y));
		if (boolvars.suit[1] && settings.hud!=1 && settings.ironmanhud == true) {
		}
		else
		{
			if(settings.standardhud == true)
			CFont::PrintString(SCREEN_COORD_MAX_X - SCREEN_COORD(settings.vecWantedLevelPosn.x) - SCREEN_COORD(settings.fWantedLevelStarSpace) * m_CurrentStar,
				SCREEN_COORD(settings.vecWantedLevelPosn.y), text);
		}
	}
}