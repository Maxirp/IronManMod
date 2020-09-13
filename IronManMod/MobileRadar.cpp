#include "MobileRadar.h"
#include "Settings.h"
#include "Utility.h"
#include "plugin.h"
#include "game_sa\common.h"
#include "game_sa\CPad.h"
#include "MobileTextures.h"
#include "game_sa\CFileLoader.h"
#include "game_sa\CCamera.h"
#include "JarvisVoice.h"
#include "game_sa\CGeneral.h"
#include "CTimer.h"
#include "BASS_funcs.h"
#include "extensions\ScriptCommands.h"

MobileRadarTextures mobileradar;

bool IsMenuEnabled();

bool IsMenuEnabled() {
	return CMenuSystem::MenuInUse[0] || CMenuSystem::MenuInUse[1];
}

static unsigned int m_nLastTimeWhenAnyActionWasEnabled1;
template <typename retType, typename... Args>
struct sa_func_def
{
	typedef retType(__cdecl*function_t)(Args...);

	inline sa_func_def(DWORD ptr)
	{
		this->func_ptr = (void*)ptr;
	}

	inline retType operator () (Args... args) const
	{
		return ((function_t)this->func_ptr)(args...);
	}

private:
	void *func_ptr;
};

static const sa_func_def <RwTexture*, RwStream*> RwTextureStreamReadRockstar(0x00730E60);

void MobileRadar::InstallPatches() {
	
		//mobileradar.textura = new(RwTexture);
		mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\hudradarwaves.png"));
		//mobileradar.waterdrop = new CSprite2d();
		mobileradar.waterdrop->m_pTexture = mobileradar.textura;
		mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\bugradarwaves.png"));
		//mobileradar.bugdrop = new CSprite2d();
		mobileradar.bugdrop->m_pTexture = mobileradar.textura;
		mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\hudcoloreddisk.png"));
		//mobileradar.radarglow = new CSprite2d();
		mobileradar.radarglow->m_pTexture = mobileradar.textura;
		mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\bugcoloreddisk.png"));
		//mobileradar.radarbug = new CSprite2d();
		mobileradar.radarbug->m_pTexture = mobileradar.textura;
		mobileradar.textura = NULL;
		//mobileradar.altpos = new CSprite2d();
		mobileradar.altpos->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\altimeter\\alt01.png"));

		//mobileradar.altcut = new CSprite2d();
		mobileradar.altcut->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\altimeter\\alt02.png"));

		//mobileradar.altend = new CSprite2d();
		mobileradar.altend->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\altimeter\\alt03.png"));
		
		//mobileradar.textur = new RwTexture();
		mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudtriang.png"));
		//mobileradar.center = new CSprite2d();
		mobileradar.center->m_pTexture = mobileradar.textur;
		mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudbigarrow.png"));
		//mobileradar.center2 = new CSprite2d();
		mobileradar.center2->m_pTexture = mobileradar.textur;
		mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudsmallarrow.png"));
		//mobileradar.center1 = new CSprite2d();
		mobileradar.center1->m_pTexture = mobileradar.textur;
		mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudfail.png"));
		//mobileradar.texfail = new CSprite2d();
		mobileradar.texfail->m_pTexture = mobileradar.textur;
		mobileradar.textur = NULL;
	
		plugin::patch::RedirectCall(0x58AA25, MyDrawRadarCircle);
		plugin::patch::RedirectCall(0x58A551, MyDrawRadarPlane);
		plugin::patch::RedirectCall(0x58A649, MyDrawPlaneHeightBorder);
		plugin::patch::RedirectCall(0x58A77A, MyDrawPlaneHeight);
		plugin::patch::RedirectJump(0x583480, MyTransformRadarPointToScreenSpace);
		plugin::patch::Nop(0x58A818, 16);
		plugin::patch::Nop(0x58A8C2, 16);
		plugin::patch::Nop(0x58A96C, 16);

}



void MobileRadar::DrawRadarRectangle(CSprite2d *sprite, CRect const& rect, CRGBA const& color) {
	
		if (sprite) {
			if (settings.bRadarTop)
				sprite->Draw(CRect(SCREEN_COORD(rect.left), SCREEN_COORD(rect.top), SCREEN_COORD(rect.right), SCREEN_COORD(rect.bottom)), color);
			else
				sprite->Draw(CRect(SCREEN_COORD(rect.left), SCREEN_HEIGHT - SCREEN_COORD(rect.top), SCREEN_COORD(rect.right), SCREEN_HEIGHT - SCREEN_COORD(rect.bottom)), color);
		}
		else {
			if (settings.bRadarTop)
				CSprite2d::DrawRect(CRect(SCREEN_COORD(rect.left), SCREEN_COORD(rect.top), SCREEN_COORD(rect.right), SCREEN_COORD(rect.bottom)), color);
			else
				CSprite2d::DrawRect(CRect(SCREEN_COORD(rect.left), SCREEN_HEIGHT - SCREEN_COORD(rect.top), SCREEN_COORD(rect.right), SCREEN_HEIGHT - SCREEN_COORD(rect.bottom)), color);
		}
	
}

void MobileRadar::DrawRadarRectangle1(CSprite2d *sprite, CRect const& rect, CRGBA const& color) {

	if (sprite) {
		if (settings.bRadarTop)
			sprite->Draw(CRect((rect.left), (rect.top), (rect.right), (rect.bottom)), color);
		else
			sprite->Draw(CRect((rect.left), SCREEN_HEIGHT - (rect.top), (rect.right), SCREEN_HEIGHT - (rect.bottom)), color);
	}
	else {
		if (settings.bRadarTop)
			CSprite2d::DrawRect(CRect((rect.left), (rect.top), (rect.right), (rect.bottom)), color);
		else
			CSprite2d::DrawRect(CRect((rect.left), SCREEN_HEIGHT - (rect.top), (rect.right), SCREEN_HEIGHT - (rect.bottom)), color);
	}

}

float SCREEN(float a)
{
    RwInt32 base;
    if (RsGlobal.maximumWidth <= RsGlobal.maximumHeight) {
        base = RsGlobal.maximumWidth;
    }
    else {
        base = RsGlobal.maximumHeight;
    }
    return a * (float)base / 900.0f;
    return a*(float)RsGlobal.maximumHeight / 900.0f;
}

void MobileRadar::MyTransformRadarPointToScreenSpace(CVector2D *out, CVector2D *in) {
	CPed *player = FindPlayerPed(-1);
	CPad *pad = CPad::GetPad(0);
	__asm push edx

	if (!boolvars.systemerror)
	{
		if (FrontEndMenuManager.drawRadarOrMap) {
			out->x = FrontEndMenuManager.m_fMapZoom * in->x + FrontEndMenuManager.m_fMapBaseX;
			out->y = FrontEndMenuManager.m_fMapBaseY - FrontEndMenuManager.m_fMapZoom * in->y;
		}
		else {
			if (settings.bRadarTop)
				out->x = SCREEN_COORD(settings.vecRadarPosn.x) + in->x * SCREEN_COORD(settings.fRadarWidthHalf);
			else
				//if (!settings.IsStatsBoxOpen)
					out->x = SCREEN_COORD(settings.vecRadarPosn.x) + in->x * SCREEN_COORD(settings.fRadarWidthHalf);
				/*else
					out->x = SCREEN_COORD(settings.vecRadarPosn.x + 270.0f) + in->x * SCREEN_COORD(settings.fRadarWidthHalf);
					*/
			if (settings.bRadarTop)
				out->y = SCREEN_COORD(settings.vecRadarPosn.y) - in->y * SCREEN_COORD(settings.fRadarHeightHalf);
			else
				out->y = SCREEN_COORD_MAX_Y - SCREEN_COORD(settings.vecRadarPosn.y) - in->y * SCREEN_COORD(settings.fRadarHeightHalf);
		}
	}

	__asm pop edx
}


void __fastcall MobileRadar::MyDrawRadarCircle(CSprite2d *sprite, int, CRect const &rect, CRGBA const &color) {	
	
	CPed *player = FindPlayerPed(0);
	CVector2D radarPos = { SCREEN_COORD(settings.vecRadarPosn.x) , SCREEN_HEIGHT - SCREEN_COORD(settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf) };
	CVector2D radarPos1 = { SCREEN_COORD(settings.vecRadarPosn.x) , SCREEN_HEIGHT - SCREEN_COORD(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf) };


	if (!IsMenuEnabled())
	{
		static std::string para_text;
		static std::string area_text;
		char namepos[16], exactname[16], intareaname[16];
		//CRadar::TransformRadarPointToScreenSpace(radarPos, CVector2D(0.0f, 1.0f));
		//CRadar::TransformRadarPointToScreenSpace(radarPos1, CVector2D(0.0f, -1.0f));
		/*float areaoffsety = radarPos.y + SCREEN_COORD(40.0f);
		float posycarname = radarPos.y - SCREEN_COORD(180.0f);
		float radoffsetx = radarPos.x;
		float areaoffsetx = radarPos.x - SCREEN_COORD(15.0f);
		float radoffsety = radarPos.y - SCREEN_COORD(30.0f);
		float offsty = SCREEN_COORD(0.0f);
		*/
		static float clockofst = 50.0f;
		float areaoffsetx = radarPos1.x - SCREEN_COORD(15.0f);
		float offsty = SCREEN_COORD(0.0f);
		float areaoffsety = radarPos1.y + 8.0f * static_cast<float>(RsGlobal.maximumHeight) / 448.0f + SCREEN_COORD(130.0f);
		
		if (settings.clockdata == true)
		{
			float clockposy = radarPos.y - SCREEN_COORD(settings.vecClockPosny);

			static int hora1, min1;
			plugin::scripting::CallCommandById(COMMAND_GET_TIME_OF_DAY, &hora1, &min1);

			char tiempo[16];
			sprintf(tiempo, "%02d:%02d", hora1, min1);

			CFont::SetBackground(0, 0);
			CFont::SetColor(settings.clockcolor);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetEdge(0);

			CFont::SetScale(SCREEN_MULTIPLIER(0.5), SCREEN_MULTIPLIER(1.5));

			CFont::PrintStringFromBottom(areaoffsetx + SCREEN_COORD(15.0f), clockposy + SCREEN_COORD(clockofst), tiempo);
		}
		if (settings.areadata == true)
		{
			if (settings.gpsshow == true)// && settings.bRadarTop == true)
			{
				offsty = SCREEN_COORD(30.0f);
			}
			//plugin::scripting::CallCommandById(COMMAND_DISPLAY_ZONE_NAMES, 0);
			static bool isinside;
			static int activeinterior, townnumb;
			CVector positionplayer;
			plugin::scripting::CallCommandById(COMMAND_GET_AREA_VISIBLE, &activeinterior);
			if (activeinterior == 0)
			{
				isinside = false;
			}
			else
			{
				isinside = true;
			}

			CFont::SetBackground(0, 0);
			CFont::SetColor(settings.areacolor);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetEdge(0);

			CFont::SetScale(SCREEN_MULTIPLIER(0.5), SCREEN_MULTIPLIER(1.5));

			if (!isinside)
			{
				//positionplayer = player->GetPosition();
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, player, &positionplayer.x, &positionplayer.y, &positionplayer.z);
				plugin::scripting::CallCommandById(COMMAND_GET_NAME_OF_ZONE, positionplayer.x, positionplayer.y, positionplayer.z, &namepos);

				plugin::scripting::CallCommandById(COMMAND_GET_CITY_PLAYER_IS_IN, (CPlayerPed *)player, &townnumb);
				if (townnumb == 0)
				{
					//exactly country side
					para_text.clear();
					para_text = "Country Side";
				}
				else
				{
					if (townnumb == 1)
					{
						//exactly Los Santos
						sprintf(exactname, "LA");
						para_text.clear();
						para_text = TheText.Get(exactname);
						fflush(stdin);
					}
					else
					{
						if (townnumb == 2)
						{
							//exactly San Fierro
							sprintf(exactname, "SF");
							para_text.clear();
							para_text = TheText.Get(exactname);
							fflush(stdin);
						}
						else
						{
							if (townnumb == 3)
							{
								//exactly Venturas
								sprintf(exactname, "VE");
								para_text.clear();
								para_text = TheText.Get(exactname);
								fflush(stdin);
							}
							else
							{
								if (strcmp(namepos, "SAN_AND") == 0)
								{
									sprintf(exactname, "SAN_AND");
									para_text.clear();
									para_text = TheText.Get(exactname);
									fflush(stdin);
								}
								else
								{
									if (strcmp(namepos, "LA") == 0)
									{
										sprintf(exactname, "LA");
										para_text.clear();
										para_text = TheText.Get(exactname);
										fflush(stdin);
									}
									else
									{
										if (strcmp(namepos, "SF") == 0)
										{
											sprintf(exactname, "SF");
											para_text.clear();
											para_text = TheText.Get(exactname);
											fflush(stdin);
										}
										else
										{
											if (strcmp(namepos, "VE") == 0)
											{
												sprintf(exactname, "VE");
												para_text.clear();
												para_text = TheText.Get(exactname);
												fflush(stdin);
											}
										}
									}
								}
							}
						}
					}
				}


				CFont::PrintString(areaoffsetx + SCREEN_COORD(15.0f), areaoffsety - SCREEN_COORD(140.0f) + offsty, para_text.data());


				if (strcmp(namepos, "SAN_AND") == 0)
				{
					//Ocean
					area_text.clear();
					area_text = "Ocean";
				}
				else
				{
					area_text.clear();
					area_text = TheText.Get(namepos);
				}
				if (area_text.compare(para_text.data()) == 0)
				{
					CFont::PrintString(areaoffsetx + SCREEN_COORD(15.0f), areaoffsety - SCREEN_COORD(110.0f) + offsty, "Country");
				}
				else
				{
					CFont::PrintString(areaoffsetx + SCREEN_COORD(15.0f), areaoffsety - SCREEN_COORD(110.0f) + offsty, area_text.data());
				}
			}
			else
			{

				plugin::scripting::CallCommandById(COMMAND_GET_NAME_OF_ENTRY_EXIT_CHAR_USED, player, &intareaname);
				para_text.clear();
				para_text = TheText.Get(intareaname);
				CFont::PrintString(areaoffsetx + SCREEN_COORD(15.0f), areaoffsety - SCREEN_COORD(110.0f) + offsty, para_text.data());
			}
		}
	}
	
	
	if (!boolvars.systemerror && boolvars.menuisactive == false)
	{
		CPad *pad = CPad::GetPad(0);


		static float x1;
		static float y1;
		static float x2;
		static float y2;
		static float unit = -0.5;

		static float salud;
		static float kevlar;
		static bool asd;
		if (!asd) {
			x1 = settings.vecRadarPosn.x;
			y1 = settings.vecRadarPosn.y;
			salud = player->m_fHealth;
			kevlar = player->m_fArmour;
			asd = true;
		}

		if (was_player_damaged(&salud, &kevlar)) {
		}
		else
		{
			if (x1 <= settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf || x1 > settings.vecRadarPosn.x)
			{
				x1 = settings.vecRadarPosn.x;
				y1 = settings.vecRadarPosn.y;
			}
			else {
				x1 += unit;
				y1 += unit;
			}
		}

		if (x1 < settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf / 2.0f && (x2 <= settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf || x2 > settings.vecRadarPosn.x)) {
			x2 = settings.vecRadarPosn.x;
			y2 = settings.vecRadarPosn.y;
		}
		else {
			if (was_player_damaged(&salud, &kevlar)) {

			}
			else {
				x2 += unit;
				y2 += unit;
			}
		}

		CRect rect1;
		rect1.left = settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf;
		rect1.right = settings.vecRadarPosn.x;
		rect1.top = settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf;
		rect1.bottom = settings.vecRadarPosn.y;

		if (boolvars.suit[1]) {
			int aux = (int)(pow(255, (x1 + rect1.right) / (rect1.right + rect1.right)));
			int aux1 = (int)(pow(255, (x2 + rect1.right) / (rect1.right + rect1.right)));
			if (was_player_damaged(&salud, &kevlar)) {
				if (settings.bRadarTop) {
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf),
						(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x),
						(settings.vecRadarPosn.y)),
						settings.colorradar);
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);

					mobileradar.bugdrop->Draw(CRect(SCREEN_COORD(x1 + 5), SCREEN_COORD(y1 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x1)) - 5), SCREEN_COORD(rect1.bottom + (rect1.bottom - (y1)) - 5)), CRGBA(255, 255, 255, aux));
					if (x2 > settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf && x2 <= settings.vecRadarPosn.x)
					{
						mobileradar.bugdrop->Draw(CRect(SCREEN_COORD(x2 + 5), SCREEN_COORD(y2 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x2)) - 5), SCREEN_COORD(rect1.bottom + (rect1.bottom - (y2)) - 5)), CRGBA(255, 255, 255, aux1));
					}
				}
				else
				{
					//if (!settings.IsStatsBoxOpen) {
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf),
						(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x),
						(settings.vecRadarPosn.y)),
						settings.colorradar);
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarbug, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					mobileradar.bugdrop->Draw(CRect(SCREEN_COORD(x1 + 5), SCREEN_HEIGHT - SCREEN_COORD(y1 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x1)) - 5), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y1)) - 5)), CRGBA(255, 255, 255, aux));
					if (x2 > settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf && x2 <= settings.vecRadarPosn.x)
					{
						mobileradar.bugdrop->Draw(CRect(SCREEN_COORD(x2 + 5), SCREEN_HEIGHT - SCREEN_COORD(y2 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x2)) - 5), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y2)) - 5)), CRGBA(255, 255, 255, aux1));
					}
				}
				/*}
				else {
					DrawRadarRectangle(radarbug, CRect((settings.vecRadarPosn.x + 270.0f - settings.fRadarBorderWidthHalf),
						(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x + 270.0f),
						(settings.vecRadarPosn.y)),
						settings.colorradar);
					DrawRadarRectangle(radarbug, CRect((settings.vecRadarPosn.x + 270.0f + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x + 270.0f), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(radarbug, CRect((settings.vecRadarPosn.x + 270.0f - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x + 270.0f), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(radarbug, CRect((settings.vecRadarPosn.x + 270.0f + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x + 270.0f), (settings.vecRadarPosn.y)), settings.colorradar);
					bugdrop->Draw(CRect(SCREEN_COORD(x1 + 275.0f), SCREEN_HEIGHT - SCREEN_COORD(y1 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x1)) + 265.0f), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y1)) - 5)), CRGBA(255, 255, 255, aux));
					if (x2 > settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf && x2 <= settings.vecRadarPosn.x)
					{
						bugdrop->Draw(CRect(SCREEN_COORD(x2 + 275.0f), SCREEN_HEIGHT - SCREEN_COORD(y2 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x2)) + 265.0f), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y2)) - 5)), CRGBA(255, 255, 255, aux1));
					}
				}*/
			}
			else
			{
				if (settings.bRadarTop) {
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf),
						(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x),
						(settings.vecRadarPosn.y)),
						settings.colorradar);
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					mobileradar.waterdrop->Draw(CRect(SCREEN_COORD(x1 + 5), SCREEN_COORD(y1 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x1)) - 5), SCREEN_COORD(rect1.bottom + (rect1.bottom - (y1)) - 5)), CRGBA(255, 255, 255, aux));
					if (x2 > settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf && x2 <= settings.vecRadarPosn.x)
					{
						mobileradar.waterdrop->Draw(CRect(SCREEN_COORD(x2 + 5), SCREEN_COORD(y2 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x2)) - 5), SCREEN_COORD(rect1.bottom + (rect1.bottom - (y2)) - 5)), CRGBA(255, 255, 255, aux1));
					}
				}
				else
				{
					//if (!settings.IsStatsBoxOpen) {
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf),
						(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x),
						(settings.vecRadarPosn.y)),
						settings.colorradar);
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					DrawRadarRectangle(mobileradar.radarglow, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
						(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
					mobileradar.waterdrop->Draw(CRect(SCREEN_COORD(x1 + 5), SCREEN_HEIGHT - SCREEN_COORD(y1 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x1)) - 5), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y1)) - 5)), CRGBA(255, 255, 255, aux));
					if (x2 > settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf && x2 <= settings.vecRadarPosn.x)
					{
						mobileradar.waterdrop->Draw(CRect(SCREEN_COORD(x2 + 5), SCREEN_HEIGHT - SCREEN_COORD(y2 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x2)) - 5), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y2)) - 5)), CRGBA(255, 255, 255, aux1));
					}
					/*}
					else {
						DrawRadarRectangle(radarglow, CRect((settings.vecRadarPosn.x + 270.0f - settings.fRadarBorderWidthHalf),
							(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
							(settings.vecRadarPosn.x + 270.0f),
							(settings.vecRadarPosn.y)),
							settings.colorradar);
						DrawRadarRectangle(radarglow, CRect((settings.vecRadarPosn.x + 270.0f + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
							(settings.vecRadarPosn.x + 270.0f), (settings.vecRadarPosn.y)), settings.colorradar);
						DrawRadarRectangle(radarglow, CRect((settings.vecRadarPosn.x + 270.0f - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
							(settings.vecRadarPosn.x + 270.0f), (settings.vecRadarPosn.y)), settings.colorradar);
						DrawRadarRectangle(radarglow, CRect((settings.vecRadarPosn.x + 270.0f + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
							(settings.vecRadarPosn.x + 270.0f), (settings.vecRadarPosn.y)), settings.colorradar);
						waterdrop->Draw(CRect(SCREEN_COORD(x1 + 275.0f), SCREEN_HEIGHT - SCREEN_COORD(y1 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x1)) + 265.0f), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y1)) - 5)), CRGBA(255, 255, 255, aux));
						if (x2 > settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf && x2 <= settings.vecRadarPosn.x)
						{
							waterdrop->Draw(CRect(SCREEN_COORD(x2 + 275.0f), SCREEN_HEIGHT - SCREEN_COORD(y2 + 5), SCREEN_COORD(rect1.right + (rect1.right - (x2)) + 265.0f), SCREEN_HEIGHT - SCREEN_COORD(rect1.bottom + (rect1.bottom - (y2)) - 5)), CRGBA(255, 255, 255, aux1));
						}
					}*/
				}
			}
		
		if (!IsMenuEnabled())
		{
			char *numb0;
			char *numb1;
			float posy;


			unsigned int radarmode;
			unsigned int isradarshown;
			radarmode = plugin::patch::GetInt(0xBA676C, false);
			isradarshown = plugin::patch::GetInt(0xBAA3FB, false);
			float topofall;
			static CRect altline;
			if (radarmode != 2 || isradarshown != 0) //hay radar
			{
				if (!settings.bRadarTop) {
					altline.bottom = radarPos.y + SCREEN_COORD(50.0f) - SCREEN_COORD(settings.vecClockPosny) - SCREEN_COORD(settings.vecClockScale.y * 15.0f);
					/*if (settings.IsStatsBoxOpen == 1)
					{
					altline.bottom = boolvars.toptextboxcoord;
					}
					else
					{
					altline.bottom = SCREEN_HEIGHT - SCREEN_COORD(70.0f) - SCREEN_COORD(settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf);
					}*/
					if (plugin::scripting::CallCommandById(COMMAND_IS_HELP_MESSAGE_BEING_DISPLAYED) == true)
					{
						if (boolvars.bottomtextboxcoord > SCREEN_COORD_TOP(157.0f))
						{
							altline.top = boolvars.bottomtextboxcoord;
							topofall = boolvars.bottomtextboxcoord;// + SCREEN_COORD(100.0f);
						}
						else
						{
							altline.top = SCREEN_COORD_TOP(157.0f);
							topofall = SCREEN_COORD_TOP(157.0f);// +SCREEN_COORD(100.0f);
						}
					}
					else
					{
					altline.top = SCREEN_COORD_TOP(157.0f);
					topofall = SCREEN_COORD_TOP(157.0f);// +SCREEN_COORD(100.0f);
					}
				}
				else
				{
					altline.top = settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf;
					topofall = settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf + SCREEN_COORD(100.0f);
					if (settings.IsStatsBoxOpen == 1 && settings.hud != 1)
					{
						altline.bottom = boolvars.toptextboxcoord;
					}
					else
					{
						altline.bottom = SCREEN_COORD_BOTTOM(57.0f);
					}
				}
			}
			else
			{
				if (plugin::scripting::CallCommandById(COMMAND_IS_HELP_MESSAGE_BEING_DISPLAYED) == true)
				{
				if (boolvars.bottomtextboxcoord > SCREEN_COORD_TOP(157.0f))
				{
				altline.top = boolvars.bottomtextboxcoord;
				topofall = boolvars.bottomtextboxcoord;// +SCREEN_COORD(100.0f);
				}
				else
				{
				altline.top = SCREEN_COORD_TOP(157.0f);
				topofall = SCREEN_COORD_TOP(157.0f);// +SCREEN_COORD(100.0f);
				}
				}
				else
				{
				altline.top = SCREEN_COORD_TOP(157.0f);
				topofall = SCREEN_COORD_TOP(157.0f);// +SCREEN_COORD(100.0f);
													}
				if (settings.IsStatsBoxOpen == 1 && settings.hud != 1)
				{
					altline.bottom = boolvars.toptextboxcoord;
				}
				else
				{
					altline.bottom = SCREEN_COORD_BOTTOM(57.0f);
				}
			}

			altline.left = SCREEN_COORD(settings.vecRadarPosn.x + 20.0f) + (boolvars.ofstappearx);
			altline.right = SCREEN_COORD(settings.vecRadarPosn.x + 25.0f) + (boolvars.ofstappearx);


			float offset = 93.0f;

			//error de mobileradar.textura aqui
			mobileradar.altpos->Draw(CRect(SCREEN_COORD(settings.vecRadarPosn.x - 50.0f) + (boolvars.ofstappearx), altline.top, SCREEN_COORD(settings.vecRadarPosn.x + 30.0f) + (boolvars.ofstappearx), altline.bottom), settings.coloraltimeter);
			CSprite2d::DrawRect(CRect(altline.right, altline.top, altline.left, SCREEN_COORD_CENTER_UP(25.5f)), settings.coloraltimeter);
			CSprite2d::DrawRect(CRect(altline.right, SCREEN_COORD_CENTER_DOWN(25.5f), altline.left, altline.bottom), settings.coloraltimeter);
			

			numb0 = new char[17];
			sprintf(numb0, "Alt: %.0f ft", boolvars.altitude);

			float ofsetakt = CFont::GetStringWidth(numb0, 1, 0);
			CSprite2d::DrawRect(CRect(altline.left + SCREEN_COORD(offset * 0.0f), SCREEN_COORD_CENTER_UP(23.5f), altline.left + ofsetakt, SCREEN_COORD_CENTER_UP(19.5f)), settings.coloraltimetermark);
			CSprite2d::DrawRect(CRect(altline.left + SCREEN_COORD(offset * 0.0f), SCREEN_COORD_CENTER_DOWN(25.5f), altline.left + ofsetakt + SCREEN_COORD(offset * 0.12f), SCREEN_COORD_CENTER_DOWN(21.5f)), settings.coloraltimetermark);

			//error de mobileradar.textura aqui
			mobileradar.altcut->Draw(CRect(altline.left - SCREEN_COORD(offset), SCREEN_COORD_CENTER_UP(25.5f), altline.left, SCREEN_COORD_CENTER_DOWN(25.5f)), settings.coloraltimetermark);

			//error de mobileradar.textura aqui
			mobileradar.altend->Draw(CRect(altline.left + ofsetakt + SCREEN_COORD(offset * 0.0f), SCREEN_COORD_CENTER_UP(25.5f), altline.left + ofsetakt + SCREEN_COORD(offset * 0.6f), SCREEN_COORD_CENTER_DOWN(25.5f)), settings.coloraltimetermark);

			
			CFont::SetBackground(0, 0);
			CFont::SetColor(settings.coloraltimetermark);
			CFont::SetOrientation(ALIGN_LEFT);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			//CFont::SetScale(SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_WIDTH / SCREEN_COORD(2420.0f) * SCREEN_MULTIPLIER(1.9f));
			CFont::SetScale(SCREEN_MULTIPLIER(0.5), SCREEN_MULTIPLIER(1.5));
			CFont::PrintStringFromBottom(altline.left - SCREEN_COORD(offset * 0.0f), SCREEN_COORD_CENTER_DOWN(15.0f), numb0);

			

			static int pasoaint;
			pasoaint = (int)boolvars.altitude;
			pasoaint /= 10;
			static float vein;
			vein = (float)pasoaint;
			vein *= -10.0f;
			vein += -23.0f;
			int i = -10;

			static float posi;
			static int cua, numh;
			static float mult;

			mult = SCREEN_HEIGHT / SCREEN_HEIGHT;
			while (i < 10)
			{
				vein += 10.0f;
				posy = vein;
				posy += boolvars.altitude;
				posy *= SCREEN_MULTIPLIER(7.8f);
				cua = (int)boolvars.altitude;
				cua /= 10;
				cua -= i;
				numh = cua;
				//cua /= 5;
				//cua *= 5;
				cua -= numh;
				posy += SCREEN_COORD_CENTER_UP(0.0f);
				posi = 480.0f / SCREEN_HEIGHT * posy;

				if ((SCREEN_COORD(posi - 2.0f) >= topofall && SCREEN_COORD(posi - 2.0f) < SCREEN_COORD_CENTER_UP(25.5f)) || SCREEN_COORD(posi - 2.0f) > SCREEN_COORD_CENTER_DOWN(25.5f) && (SCREEN_COORD(posi + 2.0f) <= altline.bottom))
				{
					if (cua == 0)
					{
						CSprite2d::DrawRect(CRect(altline.right, SCREEN_COORD(posi - 2.0f), altline.right + 10.0f, SCREEN_COORD(posi + 2.0f)), settings.coloraltimeter);
						if ((SCREEN_COORD(posi - 4.0f) >= topofall && SCREEN_COORD(posi - 4.0f) < SCREEN_COORD_CENTER_UP(25.5f)) || SCREEN_COORD(posi) > SCREEN_COORD_CENTER_DOWN(49.5f) && (SCREEN_COORD(posi - 4.0f) <= altline.bottom))
						{
							numb1 = new char[17];
							sprintf(numb1, "%d", numh);
							CFont::SetBackground(0, 0);
							CFont::SetColor(settings.coloraltimeter);
							CFont::SetOrientation(ALIGN_LEFT);
							CFont::SetProportional(true);
							CFont::SetJustify(false);
							CFont::SetFontStyle(FONT_MENU);
							CFont::SetScale(SCREEN_MULTIPLIER(0.5), SCREEN_MULTIPLIER(1.5));
							CFont::PrintStringFromBottom(altline.left + 20.0f, SCREEN_COORD(posi - 4.0f), numb1);
							numb1;
						}
					}
					else
					{
						CSprite2d::DrawRect(CRect(altline.right, SCREEN_COORD(posi - 2.0f), altline.right + 6.0f, +SCREEN_COORD(posi + 2.0f)), settings.coloraltimeter);
					}
				}
				i += 1;
			}


		}
		}
		else {
			if (settings.bRadarTop) {
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf),
					(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x),
					(settings.vecRadarPosn.y)),
					settings.colorradar);
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
			}
			else
			{
				//if (!settings.IsStatsBoxOpen) {
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf),
					(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x),
					(settings.vecRadarPosn.y)),
					settings.colorradar);
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
				DrawRadarRectangle(sprite, CRect((settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), (settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf),
					(settings.vecRadarPosn.x), (settings.vecRadarPosn.y)), settings.colorradar);
			}
		}
	}
}

void __fastcall MobileRadar::MyDrawRadarPlane(CSprite2d *sprite, int, float x1, float y1, float x2, float y2, float x3,
    float y3, float x4, float y4, CRGBA const &color)
{
	if (boolvars.menuisactive == false &&
		!boolvars.systemerror)
	{
		CVector2D posn[4];
		posn[0].x = SCREEN_COORD(settings.vecRadarPosn.x) - SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn[0].y = SCREEN_COORD(settings.vecRadarPosn.y) + SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn[1].x = SCREEN_COORD(settings.vecRadarPosn.x) + SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn[1].y = SCREEN_COORD(settings.vecRadarPosn.y) + SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn[2].x = SCREEN_COORD(settings.vecRadarPosn.x) - SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn[2].y = SCREEN_COORD(settings.vecRadarPosn.y) - SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn[3].x = SCREEN_COORD(settings.vecRadarPosn.x) + SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn[3].y = SCREEN_COORD(settings.vecRadarPosn.y) - SCREEN_COORD(settings.fRadarBorderHeightHalf);
		CMatrix *planeMat = FindPlayerVehicle(-1, false)->m_matrix;
		float angle = atan2f(planeMat->at.z, -planeMat->right.z) - 1.57079632679f;
		RotateVertices(posn, 4, SCREEN_COORD(settings.vecRadarPosn.x), SCREEN_COORD(settings.vecRadarPosn.y), settings.bRadarTop ? angle : -angle);
		if (settings.bRadarTop)
			sprite->Draw(posn[0].x, posn[0].y, posn[1].x, posn[1].y, posn[2].x, posn[2].y, posn[3].x, posn[3].y, CRGBA(255, 255, 255, 255));
		else {
			sprite->Draw(posn[2].x, SCREEN_COORD_MAX_Y - posn[2].y - SCREEN_COORD(1.0f),
				posn[3].x, SCREEN_COORD_MAX_Y - posn[3].y - SCREEN_COORD(1.0f),
				posn[0].x, SCREEN_COORD_MAX_Y - posn[0].y - SCREEN_COORD(1.0f),
				posn[1].x, SCREEN_COORD_MAX_Y - posn[1].y - SCREEN_COORD(1.0f),
				CRGBA(255, 255, 255, 255));
		}
	}
}

void MobileRadar::MyDrawPlaneHeightBorder(CRect const& rect, CRGBA const& color) {
	if (boolvars.menuisactive == false &&
		!boolvars.systemerror)
	{
		CPed *player = FindPlayerPed();
		CPad *pad = CPad::GetPad(0);
		if (settings.bRadarTop) {
			DrawRadarRectangle1(NULL, CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x)), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.y)),
				BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) + SCREEN_COORD(settings.vecPlaneRadarSize.x)), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.y)
					+ SCREEN_COORD(settings.vecPlaneRadarSize.y))), CRGBA(0, 0, 0, 255));
			DrawRadarRectangle1(NULL, CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x)),
				BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y)),
				BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
				BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y))), CRGBA(120, 120, 120, 255));
		}
		else {
			//if (!settings.IsStatsBoxOpen) {
				DrawRadarRectangle1(NULL, CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x)), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.y)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) + SCREEN_COORD(settings.vecPlaneRadarSize.x)), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.y)
						+ SCREEN_COORD(settings.vecPlaneRadarSize.y))), CRGBA(0, 0, 0, 255));
				DrawRadarRectangle1(NULL, CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y))), CRGBA(120, 120, 120, 255));
			/*}
			else {
				DrawRadarRectangle1(NULL, CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x + 270.0f)), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.y)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x + 270.0f) + SCREEN_COORD(settings.vecPlaneRadarSize.x)), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.y)
						+ SCREEN_COORD(settings.vecPlaneRadarSize.y))), CRGBA(0, 0, 0, 255));
				DrawRadarRectangle1(NULL, CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x + 270.0f)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x + 270.0f) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y))), CRGBA(120, 120, 120, 255));
			}*/
		}
	}
}

void MobileRadar::MyDrawPlaneHeight(CRect const& rect, CRGBA const& color) {
	if (boolvars.menuisactive == false &&
		!boolvars.systemerror)
	{
		float playerHeight = 0.0f;
		float blackLineBaseY = 0.0f;
		CPed *playa = FindPlayerPed();
		CPad *pad = CPad::GetPad(0);
		CVehicle *playaVeh = FindPlayerVehicle(-1, false);
		if (playaVeh)
			playerHeight = playaVeh->GetPosition().z;
		else if (playa)
			playerHeight = playa->GetPosition().z;
		if (playerHeight < 0.0f)
			playerHeight = 0.0f;
		else
			playerHeight /= 900.0f;
		if (playerHeight > 1.0f)
			playerHeight = 1.0f;
		if (playerHeight > 0.0f) {
			if (settings.bRadarTop) {
				CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * (1.0f - playerHeight)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
					BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y))), CRGBA(255, 255, 255, 255));
			}
			else {
				if (!pad->GetDisplayVitalStats(playa) || FindPlayerVehicle(-1, 0)) {
					CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x)),
						BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) -
							SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight),
						BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
						BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y))), CRGBA(255, 255, 255, 255));
				}
				else {
					CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x + 270.0f)),
						BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) -
							SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight),
						BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.x + 270.0f) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.x)),
						BilinearOffset(SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y))), CRGBA(255, 255, 255, 255));
				}
			}
		}
		if (settings.bRadarTop) {
			blackLineBaseY = playerHeight * (settings.vecPlaneRadarLightPartSize.y) > 4.0f ?
				SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * (1.0f - playerHeight) :
				SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) + SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) - SCREEN_COORD(4.0f);
			CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) - SCREEN_COORD(3.0f)), BilinearOffset(blackLineBaseY),
				BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) + SCREEN_COORD(settings.vecPlaneRadarSize.x) + SCREEN_COORD(3.0f)),
				BilinearOffset(blackLineBaseY + SCREEN_COORD(4.0f))), CRGBA(0, 0, 0, 255));
		}
		else {
			if (!pad->GetDisplayVitalStats(playa) || FindPlayerVehicle(-1, 0)) {
				blackLineBaseY = playerHeight * (settings.vecPlaneRadarLightPartSize.y) > 4.0f ?
					SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight :
					SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(4.0f);
				CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) - SCREEN_COORD(3.0f)),
					BilinearOffset(blackLineBaseY), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x) + SCREEN_COORD(settings.vecPlaneRadarSize.x) + SCREEN_COORD(3.0f)),
					BilinearOffset(blackLineBaseY + SCREEN_COORD(4.0f))), CRGBA(0, 0, 0, 255));
			}
			else {
				blackLineBaseY = playerHeight * (settings.vecPlaneRadarLightPartSize.y) > 4.0f ?
					SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(settings.vecPlaneRadarLightPartSize.y) * playerHeight :
					SCREEN_COORD_MAX_Y + 1.0f - SCREEN_COORD(settings.vecPlaneRadarLightPartPosn.y) - SCREEN_COORD(4.0f);
				CSprite2d::DrawRect(CRect(BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x + 270.0f) - SCREEN_COORD(3.0f)),
					BilinearOffset(blackLineBaseY), BilinearOffset(SCREEN_COORD(settings.vecPlaneRadarPosn.x + 270.0f) + SCREEN_COORD(settings.vecPlaneRadarSize.x) + SCREEN_COORD(3.0f)),
					BilinearOffset(blackLineBaseY + SCREEN_COORD(4.0f))), CRGBA(0, 0, 0, 255));
			}
		}
	}
}

void MobileRadar::DrawRadarCenter() {
	if (boolvars.menuisactive == false &&
		!boolvars.systemerror)
	{
		CPed *player = FindPlayerPed(-1);
		static bool asdf;
		static float salud;
		static float kevlar;
		static bool xds;
		if (!asdf)
		{
			//if (settings.HUDTexDictionary) {
				
				
				salud = player->m_fHealth;
				kevlar = player->m_fArmour;
				asdf = TRUE;
			//}
		}
		CVector actorpos = player->GetPosition();

		CVector campos = TheCamera.GetPosition();

		CVector camaimpoint = TheCamera.m_vecAimingTargetCoors;

		float enxvale;
		float enyvale;

		enyvale = -actorpos.y + campos.y;
		enxvale = -actorpos.x + campos.x;

		float actorangle = player->GetHeading();
		float compassang = -((CGeneral::GetATanOfXY(enyvale, enxvale)) + actorangle);
		float compassang2 = (CGeneral::GetATanOfXY(-enyvale, -enxvale)) + actorangle;

		CVector2D posn1[4], posn2[4];
		posn1[0].x = SCREEN_COORD(settings.vecRadarPosn.x) - SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn1[0].y = SCREEN_COORD(settings.vecRadarPosn.y) + SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn1[1].x = SCREEN_COORD(settings.vecRadarPosn.x) + SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn1[1].y = SCREEN_COORD(settings.vecRadarPosn.y) + SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn1[2].x = SCREEN_COORD(settings.vecRadarPosn.x) - SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn1[2].y = SCREEN_COORD(settings.vecRadarPosn.y) - SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn1[3].x = SCREEN_COORD(settings.vecRadarPosn.x) + SCREEN_COORD(settings.fRadarBorderWidthHalf);
		posn1[3].y = SCREEN_COORD(settings.vecRadarPosn.y) - SCREEN_COORD(settings.fRadarBorderHeightHalf);
		posn2[0] = posn1[2];
		posn2[1] = posn1[3];
		posn2[2] = posn1[0];
		posn2[3] = posn1[1];
		/*if (!settings.bRadarTop) && settings.IsStatsBoxOpen)
		{
			posn1[0].x = SCREEN_COORD(settings.vecRadarPosn.x + 270.0f) - SCREEN_COORD(settings.fRadarBorderWidthHalf);
			posn1[1].x = SCREEN_COORD(settings.vecRadarPosn.x + 270.0f) + SCREEN_COORD(settings.fRadarBorderWidthHalf);
			posn1[2].x = SCREEN_COORD(settings.vecRadarPosn.x + 270.0f) - SCREEN_COORD(settings.fRadarBorderWidthHalf);
			posn1[3].x = SCREEN_COORD(settings.vecRadarPosn.x + 270.0f) + SCREEN_COORD(settings.fRadarBorderWidthHalf);
			posn2[0] = posn1[2];
			posn2[1] = posn1[3];
			posn2[2] = posn1[0];
			posn2[3] = posn1[1];
			RotateVertices(posn1, 4, SCREEN_COORD(settings.vecRadarPosn.x + 270.0f), SCREEN_COORD(settings.vecRadarPosn.y), compassang);
		}
		else
		{
			/*if (settings.bRadarTop)
			{
			RotateVertices(posn1, 4, SCREEN_COORD(settings.vecRadarPosn.x), SCREEN_COORD(settings.vecRadarPosn.y), compassang2);
			}
			else
			{*/
				RotateVertices(posn1, 4, SCREEN_COORD(settings.vecRadarPosn.x), SCREEN_COORD(settings.vecRadarPosn.y), compassang);// settings.bRadarTop ? compassang : -compassang);
			/*}
		}*/
		if ((*(unsigned __int8 *)0xBA676C) != 2)
		{
			static bool ismortal;

			if (boolvars.suit[1])
			{
				ismortal = true;
				if (settings.bRadarTop) {
					mobileradar.center2->Draw(posn2[0].x, posn2[0].y, posn2[1].x, posn2[1].y, posn2[2].x, posn2[2].y, posn2[3].x, posn2[3].y, CRGBA(255, 255, 255, 255));
					mobileradar.center->Draw(posn1[0].x, posn1[0].y, posn1[1].x, posn1[1].y, posn1[2].x, posn1[2].y, posn1[3].x, posn1[3].y, CRGBA(255, 255, 255, 255));
				}
				else {
					//if (!settings.IsStatsBoxOpen) {
					mobileradar.center2->Draw(posn2[0].x, SCREEN_HEIGHT - posn2[0].y, posn2[1].x, SCREEN_HEIGHT - posn2[1].y, posn2[2].x, SCREEN_HEIGHT - posn2[2].y, posn2[3].x, SCREEN_HEIGHT - posn2[3].y, CRGBA(255, 255, 255, 255));
					mobileradar.center->Draw(posn1[0].x, SCREEN_HEIGHT - posn1[0].y, posn1[1].x, SCREEN_HEIGHT - posn1[1].y, posn1[2].x, SCREEN_HEIGHT - posn1[2].y, posn1[3].x, SCREEN_HEIGHT - posn1[3].y, CRGBA(255, 255, 255, 255));
					/*}
					else {
						center2->Draw(posn2[0].x, SCREEN_HEIGHT - posn2[0].y, posn2[1].x, SCREEN_HEIGHT - posn2[1].y, posn2[2].x, SCREEN_HEIGHT - posn2[2].y, posn2[3].x, SCREEN_HEIGHT - posn2[3].y, CRGBA(255, 255, 255, 255));
						center->Draw(posn1[0].x, SCREEN_HEIGHT - posn1[0].y, posn1[1].x, SCREEN_HEIGHT - posn1[1].y, posn1[2].x, SCREEN_HEIGHT - posn1[2].y, posn1[3].x, SCREEN_HEIGHT - posn1[3].y, CRGBA(255, 255, 255, 255));
					}*/
				}



				if (was_player_damaged(&salud, &kevlar) && mobileradar.texfail) {
					DrawFailReport(mobileradar.texfail);

					if ((!xds || CTimer::m_snTimeInMilliseconds > m_nLastTimeWhenAnyActionWasEnabled1) && jarvisvoice.IsAudioVoicePlayed(19) == false)
					{
						AudioLib.PlayVoiceEvent(20);
						m_nLastTimeWhenAnyActionWasEnabled1 = CTimer::m_snTimeInMilliseconds + 2000;
						xds = true;
					}
				}
				else
				{
					xds = false;

				}
			}
			else {

				if (ismortal == true)
				{
					plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 0, 0, 0);
					ismortal = false;
				}
				if (settings.bRadarTop) {
					mobileradar.center1->Draw(posn1[0].x, posn1[0].y, posn1[1].x, posn1[1].y, posn1[2].x, posn1[2].y, posn1[3].x, posn1[3].y, CRGBA(255, 255, 255, 255));
				}
				else
				{
					//if (!settings.IsStatsBoxOpen) {
					mobileradar.center1->Draw(posn1[0].x, SCREEN_HEIGHT - posn1[0].y, posn1[1].x, SCREEN_HEIGHT - posn1[1].y, posn1[2].x, SCREEN_HEIGHT - posn1[2].y, posn1[3].x, SCREEN_HEIGHT - posn1[3].y, CRGBA(255, 255, 255, 255));
					/*}
					else {
						center1->Draw(posn1[0].x, SCREEN_HEIGHT - posn1[0].y, posn1[1].x, SCREEN_HEIGHT - posn1[1].y, posn1[2].x, SCREEN_HEIGHT - posn1[2].y, posn1[3].x, SCREEN_HEIGHT - posn1[3].y, CRGBA(255, 255, 255, 255));
					}*/
				}
			}
		}
	}
}

void MobileRadar::DrawFailReport(CSprite2d *fail) {
	if(boolvars.menuisactive == false &&
		!boolvars.systemerror)
	{
		if (settings.bRadarTop) {
			fail->Draw(CRect(SCREEN_COORD(settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), SCREEN_COORD(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf), SCREEN_COORD(settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), SCREEN_COORD(settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf)), CRGBA(255, 255, 255, 255));
		}
		else {
			//if (!settings.IsStatsBoxOpen)
				fail->Draw(CRect(SCREEN_COORD(settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf), SCREEN_HEIGHT - SCREEN_COORD(settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf), SCREEN_COORD(settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf), SCREEN_HEIGHT - SCREEN_COORD(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf)), CRGBA(255, 255, 255, 255));
			/*else
				fail->Draw(CRect(SCREEN_COORD(settings.vecRadarPosn.x - settings.fRadarBorderWidthHalf + 275.0f), SCREEN_HEIGHT - SCREEN_COORD(settings.vecRadarPosn.y + settings.fRadarBorderHeightHalf), SCREEN_COORD(settings.vecRadarPosn.x + settings.fRadarBorderWidthHalf + 265.0f), SCREEN_HEIGHT - SCREEN_COORD(settings.vecRadarPosn.y - settings.fRadarBorderHeightHalf)), CRGBA(255, 255, 255, 255));
		*/
		}
	}
}