#include "Movtextures.h"
#include "game_sa\CRadar.h"
#include "extensions\ScriptCommands.h"
#include "ShapeHelper.h"
#include "plugin.h"
#include "game_sa\common.h"
#include "CFont.h"
#include "extensions\ScriptCommands.h"
#include "CText.h"
#include "CTimer.h"
#include "CPed.h"
#include "CSprite.h"
#include "game_sa\CStats.h"
#include "MobileTextBox.h"
#include <windows.h>
#include <math.h>
#include "CWorld.h"
#include "game_sa\CStats.h"
#include "game_sa\CHud.h"
#include "game_sa\CSprite2d.h"

#include "extensions\Screen.h"
#include "CFont.h"
#include "CTxdStore.h"
#include "CPed.h"
#include "MMSystem.h"
#include "WinBase.h"
#include "extensions\ScriptCommands.h"
#include "CPad.h"
#include "CGeneral.h"
#include "CFileLoader.h"
#include <windows.h>
#include <math.h>
#include "Utility.h"
#include "CClothes.h"
#include "ePedState.h"
#include "game_sa/CGame.h"
#include "CWanted.h"
#include "CExplosion.h"
#include "BASS_funcs.h"

using namespace plugin;

Movtextures movtextures;

std::string location;
static int compasssize1;
static CSprite2d *playr[4];
static CSprite2d *statboxnull;
static CSprite2d *imhud2;
static CSprite2d *imhud3;
static CSprite2d *imhud4;
static CSprite2d *imhud5;
static CSprite2d *imhud6;
static CSprite2d *imhud7;
static CSprite2d *imhud8;
static CSprite2d *imhud9;
static CSprite2d *targetfly;
static CSprite2d *flashscreen;
static CSprite2d *weaponicons[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
static CSprite2d *weaponicon1[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
static CSprite2d *imhudbg1;
static CSprite2d *imhudbg2;
static CSprite2d *imhudbg3;
static CSprite2d *imhudwanted;
static CSprite2d *wpnarrow;
static CSprite2d *hudright;
static CSprite2d *hudright1;
static CSprite2d *hudbottom;
static CSprite2d *hudbottomicon[5] = { NULL,NULL,NULL,NULL,NULL };
static CSprite2d *rothudbottom;
static CSprite2d *rothudbottom1;
static CSprite2d *damagew;
static CSprite2d *flight;
static CSprite2d *online;
static CSprite2d *horizon;
static RwTexture *textbox;
static RwTexture *textcorner1;
static RwTexture *textcorner2;
static RwTexture *textcorner3;
static RwTexture *textcorner4;
static RwTexture *textborderu;
static RwTexture *textborderd;
static RwTexture *textborderl;
static RwTexture *textborderr;
static RwTexture *hpcircle;
static RwTexture *hpcircle0;
static RwTexture *hpcircle1;
static RwTexture *hpcircle2;
static RwTexture *beating;
static CSprite2d *compass;
static CSprite2d *compassbg;
static CSprite2d *indicator;
static CSprite2d *hudrigh0;
static CSprite2d *hudrigh1;
static CSprite2d *hudrigh2;
static CSprite2d *hudrigh3;
static CSprite2d *imhud0;
static CSprite2d *imhud00;
static CSprite2d *imhud11;
static CSprite2d *imhud1;

float ReadFloatt(char* szSection, char* szKey, float fltDefaultValue, LPCSTR m_szFileName);

float ReadFloatt(char* szSection, char* szKey, float fltDefaultValue, LPCSTR m_szFileName)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	sprintf(szDefault, "%f", fltDefaultValue);
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	fltResult = (float)atof(szResult);
	return fltResult;
}

void Movtextures::cargartexturas() {

	imhud0 = new CSprite2d();
	imhud0->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair.png"));

	imhud1 = new CSprite2d();
	imhud1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair_r.png"));

	imhud00 = new CSprite2d();
	imhud00->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair_target.png"));

	imhud11 = new CSprite2d();
	imhud11->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair_dam.png"));

	weaponicons[0] = new CSprite2d();
	weaponicons[0]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\WEAPONS\\melee.png")); //textura;
	weaponicons[1] = new CSprite2d();
	weaponicons[1]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\WEAPONS\\bullets.png"));//textura;
	weaponicons[2] = new CSprite2d();
	weaponicons[2]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\WEAPONS\\rep_hand.png")); //textura;
	weaponicons[3] = new CSprite2d();
	weaponicons[3]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\WEAPONS\\rockets.png")); //textura;
	weaponicons[4] = new CSprite2d();
	weaponicons[4]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\WEAPONS\\rep_chest.png"));//textura;
	weaponicons[5] = new CSprite2d();
	weaponicons[5]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\WEAPONS\\darts.png"));//textura;

	weaponicon1[0] = new CSprite2d();
	weaponicon1[0]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\melee.png")); //textura;
	weaponicon1[1] = new CSprite2d();
	weaponicon1[1]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\bullets.png"));//textura;
	weaponicon1[2] = new CSprite2d();
	weaponicon1[2]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\rep_hand.png")); //textura;
	weaponicon1[3] = new CSprite2d();
	weaponicon1[3]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\rockets.png")); //textura;
	weaponicon1[4] = new CSprite2d();
	weaponicon1[4]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\rep_chest.png"));//textura;
	weaponicon1[5] = new CSprite2d();
	weaponicon1[5]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\darts.png"));//textura;
	weaponicon1[6] = new CSprite2d();
	weaponicon1[6]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\WEAPONS\\wpns.png"));//textura;

	targetfly = new CSprite2d();
	targetfly->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\targetfly.png"));//textura;

																										 /*hbottom = new CSprite2d();
																										 hbottom->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\bottomhealth.png"));
																										 */
	hudright = new CSprite2d();
	hudright->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_RIGHT.png"));
	hudright1 = new CSprite2d();
	hudright1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_RIGHT01.png"));
	hudrigh0 = new CSprite2d();
	hudrigh0->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_RIGHT02.png"));
	statboxnull = new CSprite2d();
	statboxnull->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\HUD1\\stats\\statbars_null.png"));
	playr[0] = new CSprite2d();
	playr[0]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats\\hud_tony.png"));
	playr[1] = new CSprite2d();
	playr[1]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats\\hud_pepper.png"));
	playr[2] = new CSprite2d();
	playr[2]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats\\hud_rhodey.png"));
	playr[3] = new CSprite2d();
	playr[3]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats\\HUD_health.png"));
	hudbottom = new CSprite2d();
	hudbottom->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_bottom.png"));
	hudbottomicon[0] = new CSprite2d();
	hudbottomicon[0]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_spin2.png"));
	hudbottomicon[1] = new CSprite2d();
	hudbottomicon[1]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_static5.png"));
	hudbottomicon[2] = new CSprite2d();
	hudbottomicon[2]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_static4.png"));
	hudbottomicon[3] = new CSprite2d();
	hudbottomicon[3]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_static3.png"));
	hudbottomicon[4] = new CSprite2d();
	hudbottomicon[4]->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_spin1.png"));
	rothudbottom = new CSprite2d();
	rothudbottom->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_spin1.png"));
	rothudbottom1 = new CSprite2d();
	rothudbottom1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_spin2.png"));
	flight = new CSprite2d();
	flight->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\flight.png"));
	damagew = new CSprite2d();
	damagew->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\damage.png"));
	online = new CSprite2d();
	online->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\online.png"));
	horizon = new CSprite2d();
	horizon->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\horizon.png"));
	/*
	sm2health = new CSprite2d();
	sm2health->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\health.png"));

	sm2healthindic = new CSprite2d();
	sm2healthindic->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\indicator.png"));

	sm2healthbg = new CSprite2d();
	sm2healthbg->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\healthbg.png"));
	*/
	//sm2rtbg = new CSprite2d();
	//sm2rtbg->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\rtbg.png"));

	imhud3 = new CSprite2d();
	imhud3->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_bg00.png"));

	imhud5 = new CSprite2d();
	imhud5->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_bg01.png"));

	imhud4 = new CSprite2d();
	imhud4->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_comunic.png"));

	imhud6 = new CSprite2d();
	imhud6->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_data.png"));

	imhud7 = new CSprite2d();
	imhud7->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_diag.png"));

	imhud8 = new CSprite2d();
	imhud8->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_flight_mode.png"));

	imhud9 = new CSprite2d();
	imhud9->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_flight_speed.png"));

	imhud2 = new CSprite2d();
	imhud2->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_heading.png"));

	imhudwanted = new CSprite2d();
	imhudwanted->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud0\\rs_wanted.png"));

	flashscreen = new CSprite2d();
	flashscreen->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\background\\bg_flash.png"));

	imhudbg1 = new CSprite2d();
	imhudbg1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\background\\bg_tps_border.png"));

	imhudbg2 = new CSprite2d();
	imhudbg2->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\background\\bg_tps_middle.png"));

	imhudbg3 = new CSprite2d();
	imhudbg3->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\background\\bg_tps_frozen.png"));



	wpnarrow = new CSprite2d();
	wpnarrow->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\PlayerInfo\\wpnarrow.png"));
	/*
	if (settings.HUDTexDictionary) {
	//textura = RwTexDictionaryFindNamedTexture(m_pRwTexDictionary, "left_side");

	//textura = RwTexDictionaryFindNamedTexture(m_pRwTexDictionary, "right_side_b");
	//textura = RwTexDictionaryFindNamedTexture(m_pRwTexDictionary, "right_side");

	}*/
}

int activesuitwearing();

int activesuitwearing() {
	CPed *player;
	player = FindPlayerPed(0);
	static int hulkbuster;
	static unsigned int armorsuit;
	static int error;
	static int mark;
	mark = boolvars.mark;
	static int head, shades, reloj, zapas;
	head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
	reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
	shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
	zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
	if ((player->m_nModelIndex != 0)) {
		if (mark > 0)
		{
			return mark;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		armorsuit = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17];
		if (armorsuit == -1137657761)
		{
			if (mark != 1)
			{
				WritePrivateProfileString("CONFIG", "MARK", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
				//mobileTex.Returnsuiticon(1);
			}
			return 1;
		}
		else
		{
			if (armorsuit == -192727393)
			{
				if (mark != 2)
				{
					WritePrivateProfileString("CONFIG", "MARK", "2", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
					//mobileTex.Returnsuiticon(2);
				}
				return 2;
			}
			else
			{
				if (armorsuit == 2061229512)
				{
					if (mark != 3)
					{
						WritePrivateProfileString("CONFIG", "MARK", "3", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
						//mobileTex.Returnsuiticon(3);
					}
					return 3;
				}
				else
				{
					if (armorsuit == 1226861131)
					{
						if (mark != 4)
						{
							WritePrivateProfileString("CONFIG", "MARK", "4", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							//mobileTex.Returnsuiticon(4);
						}
						return 4;
					}
					else
					{
						if (armorsuit == 1817399610)
						{
							if (mark != 5)
							{
								WritePrivateProfileString("CONFIG", "MARK", "5", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
								//mobileTex.Returnsuiticon(5);
							}
							return 5;
						}
						else
						{
							if (armorsuit == 900618200)
							{
								if (mark != 6)
								{
									WritePrivateProfileString("CONFIG", "MARK", "6", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									//mobileTex.Returnsuiticon(6);
								}
								return 6;
							}
							else
							{
								if (armorsuit == -559695285)
								{
									if (mark != 7)
									{
										WritePrivateProfileString("CONFIG", "MARK", "7", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
										//mobileTex.Returnsuiticon(7);
									}
									return 7;
								}
								else
								{
									if ((reloj == 1142040938 || reloj == -1155750708) &&
										zapas == -592930753)
									{
										return -6;
									}
									else
									{
										if ((reloj != 1142040938 && reloj != -1155750708) &&
											zapas == -592930753)
										{
											return -5;
										}
										else
										{
											if ((reloj == 1142040938 || reloj == -1155750708) &&
												zapas != -592930753)
											{
												return -4;
											}
											else
											{
												return 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

static bool is_wearing_suit_area(int bp);

static bool is_wearing_suit_area(int bp) {
	//0 alguno
	//1 cabeza
	//2 pies
	//3 manos
	//4 extremidades
	//5 conjunto completo
	CPed *player;
	player = FindPlayerPed(0);
	static int hulkbuster;
	static unsigned int armorsuit;
	static int error;
	//static int hb;
	static int mark;
	//hb = GetPrivateProfileInt("CONFIG", "HB", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
	mark = boolvars.mark;
	error = 0;//GetPrivateProfileInt("CONFIG", "ERROR", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
	static int head, shades, reloj, zapas;
	head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
	reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
	shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
	zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
	if (error == 0)
	{
		if ((player->m_nModelIndex != 0)) {
			if (mark > 0)
			{
				return true;
			}
			else
			{
				if (mark != 0)
				{
					WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
					if (boolvars.iscjfrozen == true) {
						boolvars.iscjfrozen = false;
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
					}
				}
				return false;
			}
		}
		else
		{
			armorsuit = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17];
			if (armorsuit != 0)
			{
				if (armorsuit != 995481457)
				{
					if (armorsuit == -1137657761)
					{
						if (bp != 1)
							return true;
						else
							return false;
					}
					else
					{
						return true;
					}
				}
				else
				{
					if (bp == 0)
					{
						if (reloj == -1155750708 ||
							reloj == 1142040938 ||
							zapas == -592930753 ||
							head == 1348958410 ||
							shades == 672552983)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						if (bp == 4)
						{
							if ((reloj == 1142040938 || reloj == -1155750708) &&
								zapas == -592930753)
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							if (bp == 3)
							{
								if (reloj == 1142040938 || reloj == -1155750708)
								{
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								if (bp == 2)
								{
									if (zapas == -592930753)
									{
										return true;
									}
									else
									{
										return false;
									}
								}
								else
								{
									if (bp == 1)
									{
										if (head == 1348958410 ||
											shades == 672552983)
										{
											return true;
										}
										else
										{
											if (bp == 6)
											{
												if (head == 1348958410 ||
													shades == 672552983)
												{
													if (zapas == -592930753 && (reloj == 1142040938 || reloj == -1155750708))
													{
														return true;
													}
													else
													{
														return false;
													}
												}
												else
												{
													return false;
												}
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
							}
						}
					}
				}
			}
			else
			{
				if (bp == 0)
				{
					if (reloj == -1155750708 ||
						reloj == 1142040938 ||
						zapas == -592930753 ||
						head == 1348958410 ||
						shades == 672552983)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if (bp == 4)
					{
						if ((reloj == 1142040938 || reloj == -1155750708) &&
							zapas == -592930753)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						if (bp == 3)
						{
							if (reloj == 1142040938 || reloj == -1155750708)
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							if (bp == 2)
							{
								if (zapas == -592930753)
								{
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								if (bp == 1)
								{
									if (head == 1348958410 ||
										shades == 672552983)
									{
										return true;
									}
									else
									{
										if (bp == 6)
										{
											if (head == 1348958410 ||
												shades == 672552983)
											{
												if (zapas == -592930753 && (reloj == 1142040938 || reloj == -1155750708))
												{
													return true;
												}
												else
												{
													return false;
												}
											}
											else
											{
												return false;
											}
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
						}
					}
				}
			}
		}
	}
	else
	{
		return false;
	}
}

static void InitCompassTextures();

static void InitCompassTextures() {
	static int storedcompassstyle;
	static bool init;
	int compassstyle = GetPrivateProfileInt("OPTIONS", "Style", 0, PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Compass\\Compass.ini"));

	if (!init)
	{
		init = true;
		storedcompassstyle = compassstyle + 1;
	}

	if (compassstyle != storedcompassstyle)
	{
		storedcompassstyle = compassstyle;
		static char numstyle[10];
		std::string location0;
		location = PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Compass");
		location += "\\";
		sprintf(numstyle, "%d", compassstyle);
		location += numstyle;

		location0.clear();
		location0 = location.c_str();
		location0 += "\\compass.png";
		FILE *file = fopen(location0.c_str(), "r");

		if (file != NULL) {
			fclose(file);
			compass = new CSprite2d();
			compass->m_pTexture = mobileTex.LoadTexture(location0.data());
			fflush(stdin);
		}
		location0.clear();

		location0 = location.c_str();
		location0 += "\\background.png";
		FILE *file2 = fopen(location0.data(), "r");

		if (file2 != NULL) {
			fclose(file2);
			compassbg = new CSprite2d();
			compassbg->m_pTexture = mobileTex.LoadTexture(location0.data());
			fflush(stdin);
		}
		location0.clear();

		location0 = location.c_str();
		location0 += "\\indicator.png";
		FILE *file1 = fopen(location0.data(), "r");

		if (file1 != NULL) {
			fclose(file1);
			indicator = new CSprite2d();
			indicator->m_pTexture = mobileTex.LoadTexture(location0.data());
			fflush(stdin);
		}
		location0.clear();

		location0 = location.c_str();
		location0 += "\\Config.ini";
		FILE *file0 = fopen(location0.data(), "r");

		if (file0 != NULL) {
			fclose(file0);
			compasssize1 = GetPrivateProfileInt("TextureStructure", "PixelsBetweenCardinalPoints", 372, location0.data());
			fflush(stdin);
		}
		location0.clear();
		location += "\\config.ini";
	}
}

void Movtextures::readallhudtextures() {
	static float maxh;

	// Load txd and texture
	InitCompassTextures();
	maxh = 100.0f;
	hudrigh1 = new CSprite2d();
	hudrigh1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\HUD_RIGH1.png"));
	hudrigh2 = new CSprite2d();
	hudrigh2->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\stats\\HUD_RIGH2.png"));
	hudrigh3 = new CSprite2d();
	hudrigh3->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\stats\\HUD_RIGH3.png"));
	textbox = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\ALLHUDS\\HelpText\\help_bg.png"));
	hpcircle = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\ALLHUDS\\PlayerInfo\\hpcircle.png"));
	hpcircle0 = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\ALLHUDS\\PlayerInfo\\hpcircle1.png"));
	hpcircle1 = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\ALLHUDS\\PlayerInfo\\hpcircle2.png"));
	hpcircle2 = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\ALLHUDS\\PlayerInfo\\hpcircle3.png"));
	//staticon = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\Textures\\HUD1\\stats.png"));
	beating = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\hud1\\beating.png"));
	settings.hud = GetPrivateProfileInt("CONFIG", "HUD", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

	boolvars.maxarmor = static_cast<float>(FindPlayerPed(0)->GetPlayerInfoForThisPlayerPed()->m_nMaxArmour);
	// Prepare sin/cos table
	ShapeHelper::InitSinCosTable();
	Movtextures::cargartexturas();
}

void Movtextures::MyDrawWeaponIcon1() {
	CPed *player = FindPlayerPed(0);
	static int weapModel;

	if (boolvars.menuisactive == false && boolvars.hudsetting == true)
	{

		float enxvale;
		float enyvale;

		enyvale = settings.vecUnusedWeaponIconOffst.x;
		enxvale = settings.vecUnusedWeaponIconOffst.y;

		float compassang = (atan2f(-enyvale, -enxvale));
		//float compassang2= (atan2f(enyvale, enxvale));

		RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void *>(rwFILTERLINEAR));
		static float vecWeaponIconPosn1x;// = SCREEN_COORD_RIGHT(settings.vecWeaponIconPosn.x);
										 //float vecWeaponTextPosn1x = SCREEN_COORD_RIGHT(settings.vecWeaponAmmoPosn.x);

		static float ofsty;


		CVector2D WeaponIconPosn;
		CRadar::TransformRadarPointToScreenSpace(WeaponIconPosn, CVector2D(0.0f, 1.0f));

		ofsty = WeaponIconPosn.y - SCREEN_COORD(settings.vecWeaponIconPosny);
		vecWeaponIconPosn1x = WeaponIconPosn.x;


		float offsetx;
		float offsety;
		offsetx = SCREEN_COORD(settings.vecUnusedWeaponIconOffst.x);
		offsety = SCREEN_COORD(settings.vecUnusedWeaponIconOffst.y);
		int activesuit = activesuitwearing();

		CVector2D posn1[4], posn2[4];;
		posn1[0].x = vecWeaponIconPosn1x + offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn1[0].y = ofsty + offsety + SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);
		posn1[1].x = vecWeaponIconPosn1x + offsetx + SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn1[1].y = ofsty + offsety + SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);
		posn1[2].x = vecWeaponIconPosn1x + offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn1[2].y = ofsty + offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);
		posn1[3].x = vecWeaponIconPosn1x + offsetx + SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn1[3].y = ofsty + offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);

		posn2[0].x = vecWeaponIconPosn1x - offsetx + SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn2[0].y = ofsty - offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);
		posn2[1].x = vecWeaponIconPosn1x - offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn2[1].y = ofsty - offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);
		posn2[2].x = vecWeaponIconPosn1x - offsetx + SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn2[2].y = ofsty - offsety + SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);
		posn2[3].x = vecWeaponIconPosn1x - offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f);
		posn2[3].y = ofsty - offsety + SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f);

		RotateVertices(posn1, 4, vecWeaponIconPosn1x + offsetx, ofsty + offsety, compassang);
		RotateVertices(posn2, 4, vecWeaponIconPosn1x - offsetx, ofsty - offsety, compassang);
		//RotateVertices(posn2, 4, vecWeaponIconPosn1x, ofsty, -compassang2);

		if (is_wearing_suit_area(1) == true
			&& settings.hud == 1 && settings.ironmanhud == true)
		{
			if (boolvars.hudindex > 5)
			{
				CVector2D radarPos3;
				CRadar::TransformRadarPointToScreenSpace(radarPos3, CVector2D(1.0f, -1.0f));

				35.0f; 63.0f;
				35.0f; 100.0f;
				207.0f; 28.0f;
				207.0f; 96.0f;


				weaponicon1[boolvars.wpn]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 1.9f,
					SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6f,
					SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 5.2f,
					SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6f + SCREEN_HEIGHT / 4.9f), CRGBA(255, 255, 255, 255));

				/*weaponicons[boolvars.wpn]->Draw(CRect(radarPos3.x + SCREEN_COORD(50.0f + 63.0f) + SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
				radarPos3.y - SCREEN_COORD(275.0f / 2.0f - 35.0f) - SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f),
				radarPos3.x + SCREEN_COORD(50.0f + 63.0f) - SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
				radarPos3.y - SCREEN_COORD(275.0f / 2.0f - 35.0f) + SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f)), CRGBA(255, 255, 255, 255));

				CHud::Sprites[HUDSPRITE_FIST].Draw(CRect(radarPos3.x + SCREEN_COORD(50.0f + 207.0f) - SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
				radarPos3.y - SCREEN_COORD(275.0f / 2.0f - 96.0f) - SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f),
				radarPos3.x + SCREEN_COORD(50.0f + 207.0f) + SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
				radarPos3.y - SCREEN_COORD(275.0f / 2.0f - 96.0f) + SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f)), CRGBA(255, 255, 255, 255));

				CHud::Sprites[HUDSPRITE_FIST].Draw(CRect(radarPos3.x + SCREEN_COORD(50.0f + 63.0f) + SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
				radarPos3.y - SCREEN_COORD(275.0f / 2.0f - 103.0f) - SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f),
				radarPos3.x + SCREEN_COORD(50.0f + 63.0f) - SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
				radarPos3.y - SCREEN_COORD(275.0f / 2.0f - 103.0f) + SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f)), CRGBA(255, 255, 255, 255));
				*/
			}
		}
		else
		{
			if (settings.standardhud == true)
			{

				if (settings.hud == 1)
				{

					weaponicon1[6]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 1.9f,
						SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 5.2f,
						SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 4.6f + SCREEN_HEIGHT / 4.9f), CRGBA(255, 255, 255, 255));

					vecWeaponIconPosn1x = SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 2.75f;
					ofsty = SCREEN_COORD_CENTER_Y + SCREEN_HEIGHT / 3.0f;
				}
				if (activesuit != 0)
				{

					int pre = boolvars.wpn - 1;
					if (pre < 0)
					{
						pre = 5;
					}

					int nex = boolvars.wpn + 1;
					if (nex > 5)
					{
						nex = 0;
					}

					weaponicons[boolvars.wpn]->Draw(CRect(vecWeaponIconPosn1x - SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
						ofsty - SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f),
						vecWeaponIconPosn1x + SCREEN_COORD(settings.vecWeaponIconScale.x / 2.0f),
						ofsty + SCREEN_COORD(settings.vecWeaponIconScale.y / 2.0f)), CRGBA(255, 255, 255, 255));
					/*
					weaponicons[nex]->Draw(vecWeaponIconPosn1x + offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f),
					ofsty + offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.x),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.y),
					CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));

					weaponicons[pre]->Draw(vecWeaponIconPosn1x - offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f),
					ofsty - offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.x),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.y),
					CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));*/
				}
				else
				{
					/*

					static int currslot, currwpn, weapon, ammo, model, prevslot;
					static int prevmodel, nextmodel;
					static int comparechk;
					int stat = 0;
					bool found = false;
					prevslot = -1;
					comparechk = player->m_nActiveWeaponSlot;
					currslot = player->m_nActiveWeaponSlot;
					currslot++;
					for (int i = 0; i < 13; i++)
					{
					if (player)
					{
					currslot -= 1;
					if (currslot < 1)
					{
					currslot = 13;
					}
					prevmodel = -1;
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_WEAPON_IN_SLOT, player, currslot, &weapon, &ammo, &model);
					if (currslot == 1 || model != -1)
					{
					found = true;

					int memory = weapon * 112 + 13150904 + 32;

					int value = plugin::patch::GetInt(memory, false);

					if (value > 0)
					{
					plugin::scripting::CallCommandById(COMMAND_GET_AMMO_IN_CHAR_WEAPON, player, weapon, &ammo);
					if (ammo == 0)
					{
					plugin::scripting::CallCommandById(COMMAND_REMOVE_WEAPON_FROM_CHAR, player, weapon);
					}
					else
					{
					prevslot = weapon;
					prevmodel = model;
					break;
					}
					}
					else
					{
					prevslot = weapon;
					prevmodel = model;
					break;
					}
					}
					}
					else
					{
					break;
					}
					}


					found = false;
					int nextslot = -1;

					currslot = player->m_nActiveWeaponSlot;
					currslot += 1;
					nextmodel = -1;
					while (found == false && currslot != player->m_nActiveWeaponSlot)
					{
					if (player)
					{
					currslot += 1;
					if (currslot > 13)
					{
					currslot = 1;
					}
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_WEAPON_IN_SLOT, player, currslot, &weapon, &ammo, &model);
					if (currslot == 1 || model != -1)
					{
					found = true;

					int memory = weapon * 112 + 13150904 + 32;

					int value = plugin::patch::GetInt(memory, false);

					if (value > 0)
					{
					plugin::scripting::CallCommandById(COMMAND_GET_AMMO_IN_CHAR_WEAPON, player, weapon, &ammo);
					if (ammo == 0)
					{
					plugin::scripting::CallCommandById(COMMAND_REMOVE_WEAPON_FROM_CHAR, player, weapon);
					}
					else
					{
					nextslot = weapon;
					nextmodel = model;
					break;
					}
					}
					else
					{
					nextslot = weapon;
					nextmodel = model;
					break;
					}
					}
					}
					else
					{
					break;
					}
					}
					plugin::scripting::CallCommandById(COMMAND_GET_CURRENT_CHAR_WEAPON, player, &currwpn);


					if (currwpn != nextslot)
					{
					RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void *>(rwFILTERLINEAR));
					int weapModel = nextmodel;

					if (weapModel <= 0) {
					float baseY = 0.0f;
					if (player == CWorld::Players[1].m_pPed)
					baseY = settings.fSecondPlayerOffsetY;

					CHud::Sprites[HUDSPRITE_FIST].Draw(vecWeaponIconPosn1x + offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f),
					ofsty + offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.x),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.y),
					CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));
					}
					else {
					CBaseModelInfo *model = CModelInfo::GetModelInfo(weapModel);
					TxdDef *txd = CTxdStore::ms_pTxdPool->GetAt(model->m_nTxdIndex);
					if (txd && txd->m_pRwDictionary) {
					RwTexture *iconTex = RwTexDictionaryFindHashNamedTexture(txd->m_pRwDictionary, CKeyGen::AppendStringToKey(model->m_nKey, "ICON"));
					if (iconTex) {
					//RwRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
					unsigned int savedAlpha;
					RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);

					RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, iconTex->raster);
					float baseY = 0.0f;
					if (player == CWorld::Players[1].m_pPed)
					baseY = settings.fSecondPlayerOffsetY;
					CSprite2d::SetVertices(CRect(vecWeaponIconPosn1x + offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f),
					ofsty + offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f),
					vecWeaponIconPosn1x + SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f) + offsetx,
					ofsty + offsety + SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f)),
					CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha), CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha), CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha), CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));
					RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
					RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));

					}
					}
					}
					}
					else
					{
					wpnarrow->Draw(posn1[0].x, posn1[0].y, posn1[1].x, posn1[1].y, posn1[2].x, posn1[2].y, posn1[3].x, posn1[3].y, CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));
					}
					if (currwpn != prevslot)
					{
					RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void *>(rwFILTERLINEAR));
					int weapModel = prevmodel;

					if (weapModel <= 0) {
					float baseY = 0.0f;
					if (player == CWorld::Players[1].m_pPed)
					baseY = settings.fSecondPlayerOffsetY;
					CHud::Sprites[HUDSPRITE_FIST].Draw(vecWeaponIconPosn1x - offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f),
					ofsty - offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.x),
					SCREEN_COORD(settings.vecUnusedWeaponIconScale.y),
					CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));
					}
					else {
					CBaseModelInfo *model = CModelInfo::GetModelInfo(weapModel);
					TxdDef *txd = CTxdStore::ms_pTxdPool->GetAt(model->m_nTxdIndex);
					if (txd && txd->m_pRwDictionary) {
					RwTexture *iconTex = RwTexDictionaryFindHashNamedTexture(txd->m_pRwDictionary, CKeyGen::AppendStringToKey(model->m_nKey, "ICON"));
					if (iconTex) {
					//RwRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);

					unsigned int savedAlpha;
					RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);

					RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, iconTex->raster);
					float baseY = 0.0f;
					if (player == CWorld::Players[1].m_pPed)
					baseY = settings.fSecondPlayerOffsetY;

					CSprite2d::SetVertices(CRect(vecWeaponIconPosn1x - offsetx - SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f),
					ofsty - offsety - SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f),
					vecWeaponIconPosn1x + SCREEN_COORD(settings.vecUnusedWeaponIconScale.x / 2.0f) - offsetx,
					ofsty - offsety + SCREEN_COORD(settings.vecUnusedWeaponIconScale.y / 2.0f)),
					CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha), CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha), CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha), CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));
					RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
					RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));
					}
					}
					}
					}
					else
					{
					wpnarrow->Draw(posn2[0].x, posn2[0].y, posn2[1].x, posn2[1].y, posn2[2].x, posn2[2].y, posn2[3].x, posn2[3].y, CRGBA(255, 255, 255, (uchar)settings.vecUnusedWeaponIconAlpha));
					}
					*/
				}
			}
		}
	}
}

static void MyHelpBox_DrawBox(CRect const& rect, CRGBA const& color);

static void MyHelpBox_DrawBox(CRect const& rect, CRGBA const& color) {
	/*static float rectright;
	rectright = rect.right + 50.0f;
	boolvars.bottomtextboxcoord = rect.top + 10.0f;
	static bool en3;
	if (!en3) {
	//if (settings.HUDTexDictionary) {

	fondo1 = new CSprite2d();
	fondo1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\HelpText\\help_bg.png"));
	border11 = new CSprite2d();
	border11->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\HelpText\\help_border1.png"));
	border2 = new CSprite2d();
	border2->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\HelpText\\help_border.png"));
	corner1 = new CSprite2d();
	corner1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\HelpText\\help_corner.png"));
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

	rigsid.left = BilinearOffset(rectright) + 10.0f;
	rigsid.right = BilinearOffset(rectright);
	rigsid.top = BilinearOffset(rect.top);
	rigsid.bottom = BilinearOffset(rect.bottom);

	topup.left = BilinearOffset(rect.left);
	topup.right = BilinearOffset(rectright);
	topup.top = BilinearOffset(rect.top);
	topup.bottom = BilinearOffset(rect.top) + 10.0f;

	downund.left = BilinearOffset(rect.left);
	downund.right = BilinearOffset(rectright);
	downund.top = BilinearOffset(rect.bottom);
	downund.bottom = BilinearOffset(rect.bottom) - 10.0f;

	border11->Draw(topup, CRGBA(255, 255, 255, 190));
	border11->Draw(downund, CRGBA(255, 255, 255, 190));
	border2->Draw(rigsid, CRGBA(255, 255, 255, 190));
	border2->Draw(lefsid, CRGBA(255, 255, 255, 190));

	corner1->Draw(CRect(BilinearOffset(rect.left) - 10.0f, BilinearOffset(rect.top) + 10.0f, BilinearOffset(rect.left), BilinearOffset(rect.top)), CRGBA(255, 255, 255, 190));
	corner1->Draw(CRect(BilinearOffset(rectright) + 10.0f, BilinearOffset(rect.top) + 10.0f, BilinearOffset(rectright), BilinearOffset(rect.top)), CRGBA(255, 255, 255, 190));
	corner1->Draw(CRect(BilinearOffset(rect.left) - 10.0f, BilinearOffset(rect.bottom) - 10.0f, BilinearOffset(rect.left), BilinearOffset(rect.bottom)), CRGBA(255, 255, 255, 190));
	corner1->Draw(CRect(BilinearOffset(rectright) + 10.0f, BilinearOffset(rect.bottom) - 10.0f, BilinearOffset(rectright), BilinearOffset(rect.bottom)), CRGBA(255, 255, 255, 190));

	fondo1->Draw(CRect(BilinearOffset(rect.left), BilinearOffset(rect.bottom), BilinearOffset(rectright), BilinearOffset(rect.top)), CRGBA(255,255,255,190));
	*/
	boolvars.bottomtextboxcoord = rect.top + 25.0f;
	unsigned int savedShade;
	unsigned int savedAlpha;
	RwRenderStateGet(rwRENDERSTATESHADEMODE, &savedShade);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(rwSHADEMODEGOURAUD));
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	CRect rect1;

	if (rect.bottom > rect.top)
	{
		float aux = rect.top;
		rect1.top = rect.bottom;
		rect1.bottom = aux;
	}
	else
	{
		rect1.top = rect.top;
		rect1.bottom = rect.bottom;
	}
	if (rect.left > rect.right)
	{
		float aux = rect.right;
		rect1.right = rect.left;
		rect1.left = aux;
	}
	else
	{
		rect1.right = rect.right;
		rect1.left = rect.left;
	}

	ShapeHelper::DrawTriangleSectorBorder(CRect(BilinearOffset(rect1.left) - SCREEN_COORD(5.0f), BilinearOffset(rect1.bottom) - SCREEN_COORD(20.0f),
		BilinearOffset(rect1.right), BilinearOffset(rect1.top) + SCREEN_COORD(30.0f)),
		CRGBA(56, 56, 56, 255)); //CRGBA(0, 27, 63, 255));

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, textbox->raster);


	ShapeHelper::DrawTriangleSectorTextured(CRect(BilinearOffset(rect1.left) - SCREEN_COORD(5.0f), BilinearOffset(rect1.bottom) - SCREEN_COORD(20.0f),
		BilinearOffset(rect1.right), BilinearOffset(rect1.top) + SCREEN_COORD(30.0f)),
		CRGBA(255, 255, 255, 255));

	/*RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);


	ShapeHelper::DrawTriangleSectorBorder(CRect(BilinearOffset(rect.left - settings.fTextBoxBorderSize), BilinearOffset(rect.bottom - settings.fTextBoxBorderSize),
	BilinearOffset(rect.right + settings.fTextBoxBorderSize), BilinearOffset(rect.top + settings.fTextBoxBorderSize)),
	CRGBA(255, 255, 255, 105));*/
	RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(savedShade));
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));



}

void Movtextures::InstallPatches() {
	plugin::patch::RedirectCall(0x573EF4, MyHelpBox_DrawBox);
	plugin::patch::RedirectCall(0x71A7EE, MyHelpBox_DrawBox);
}

bool was_player_damaged1(float *damage, float *damage1);

bool was_player_damaged1(float *damage, float *damage1) {
	static int contador;

	CPed *player;
	player = FindPlayerPed(0);

	if ((player->m_fHealth < *damage) || (player->m_fArmour < *damage1))
	{
		*damage = player->m_fHealth;
		*damage1 = player->m_fArmour;
		contador = CTimer::m_snTimeInMillisecondsNonClipped;
		return true;
	}
	else
	{
		*damage = player->m_fHealth;
		*damage1 = player->m_fArmour;
		if (contador + 300 > CTimer::m_snTimeInMillisecondsNonClipped) {
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool is_on_air_or_water1();
bool is_on_air_or_water1() {
	int gpoint;
	ReadProcessMemory(GetCurrentProcess(), (LPCVOID)0xB6F5F0, &gpoint, 4, 0);
	int playerstate = gpoint + 0x46C;
	char actorstate = plugin::patch::GetChar(playerstate, false);
	while (actorstate > 3) {
		actorstate -= 4;
		if (actorstate <= 3)
		{
			break;
		}
	}
	if (actorstate == 0)
		return true;
	else
		return false;
}

bool h1as_obstacles(CPed *player, float offsetx, float offsety, float offsetz, int solid, int car, int actor, int object, int particle);

bool h1as_obstacles(CPed *player, float offsetx, float offsety, float offsetz, int solid, int car, int actor, int object, int particle) {
	float coordx, coordy, coordz;
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, offsetx, offsety, offsetz, &coordx, &coordy, &coordz);
	CVector position = player->GetPosition();
	if ((plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, position.x, position.y, position.z, coordx, coordy, coordz, solid, car, actor, object, particle)) == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_over_ground1(float distance);

bool is_over_ground1(float distance) {
	static float dist;
	CPed *player;
	player = FindPlayerPed(0);
	CVector coords;

	if (player)
	{
		coords = player->GetPosition();
	}

	if (is_on_air_or_water1() == true) {
		plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, coords.x, coords.y, coords.z, &dist);
		if (dist != 0.0f) {
			if (coords.z - dist > distance) {
				if (h1as_obstacles(player, 0.0, 0.0, 0.0f - distance, 1, 1, 0, 1, 0) == false)
				{
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void c1oordstoscreen(float x, float y, float z, float *dx, float *dy, float *wid, float *hei);
void c1oordstoscreen(float x, float y, float z, float *dx, float *dy, float *wid, float *hei) {
	RwV3d coord3d, coord2d;
	float w, h;
	coord3d.x = x;
	coord3d.y = y;
	coord3d.z = z;
	CSprite::CalcScreenCoors(coord3d, &coord2d, &w, &h, true, true);
	*dx = coord2d.x;
	*dy = coord2d.y;
	*wid = w;
	*hei = h;
}

void Movtextures::drawsizedtexture(CRect siz, CSprite2d *tex, int index) {

	CRect size;
	int tex1[4];
	float siz4[4], extendedsize;
	int alfa[4], extendedalfa;
	int i = 0;
	bool b = false;
	while (i < 4)
	{
		if (index == tex1[i])
		{
			extendedalfa = alfa[i] * 0.75f;
			extendedsize = siz4[i] * 0.25f;
			if (extendedsize < 0.01)
			{
				extendedsize = 0.0f;
			}
			b = true;
			break;
		}
		i++;
	}

	if (b == false)
	{
		tex1[i] = index;
		alfa[i] = 100.0f;
		siz4[i] = SCREEN_HEIGHT / 2.0f;
	}
	

	size = { siz.left - extendedsize,siz.top - extendedsize,siz.right + extendedsize,siz.bottom + extendedsize };

	tex->Draw(size, CRGBA(255, 255, 255, 255 - (unsigned char)extendedalfa));
}

void Movtextures::drawindexedtexture(int index) {
	CVector2D radarPos2, radarPos3;
	CPed *player = FindPlayerPed(0);
	CRadar::TransformRadarPointToScreenSpace(radarPos2, CVector2D(0.0f, 0.0f));
	CRadar::TransformRadarPointToScreenSpace(radarPos3, CVector2D(1.0f, -1.0f));
	static CRGBA colorhealt;
	static float progress;
	float maxh = static_cast<float>(FindPlayerPed(0)->GetPlayerInfoForThisPlayerPed()->m_nMaxHealth);

	static int compassalign;
	static float offsetcompassx;
	static float offsetcompassy;
	static int compasssize;
	static float salud;
	char *numb00;
	static float kevlar, angle;
	static bool load;
	static bool hudactive;
	static bool initaudiohud;

	if (boolvars.hudactive == true)
	{
		hudactive = false;
		boolvars.hudactive = false;
	}
	if (index == 0)
	{
		int playerId = 0;
		settings.healthdisplayed = true;
		static float angle1;
		//CPed *player = FindPlayerPed(0);
		float barSize = static_cast<float>(CWorld::Players[playerId].m_nMaxHealth) / CStats::GetFatAndMuscleModifier((eStatModAbilities)10);
		if (barSize > 1.0f)
			barSize = 1.0f;
		float progress = CWorld::Players[playerId].m_pPed->m_fHealth / static_cast<float>(CWorld::Players[playerId].m_nMaxHealth);
		if (progress > 1.0f)
			progress = 1.0f;
		/*float positionY = settings.vecHealthPosn.y;
		if (playerId == 1)
		positionY += settings.fSecondPlayerOffsetY;
		*/

		int level1;
		plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level1);

		if (!load)
		{
			angle = 0.0f;
			salud = CWorld::Players[playerId].m_pPed->m_fHealth;
			kevlar = CWorld::Players[playerId].m_pPed->m_fArmour;
			load = true;
		}
		static int alha;
		if (alha > 150)
		{
			alha = 175;
		}
		CRGBA colorbg = CRGBA(0, 50, 60, alha);
		settings.redcodecolour = CRGBA(settings.redcodecolour.r, settings.redcodecolour.g, settings.redcodecolour.b, alha);
		settings.blueregular = CRGBA(settings.blueregular.r, settings.blueregular.g, settings.blueregular.b, alha);
		float posx, posy, espaciado;

		CVector2D radarPos;
		CRadar::TransformRadarPointToScreenSpace(radarPos, CVector2D(0.0f, 1.0f));
		if (is_wearing_suit_area(1) &&
			boolvars.hudsetting == true &&
			settings.ironmanhud == true) {
			static float hudicnposhoriz;
			float offcoordshoriz = SCREEN_WIDTH / 2.5f;
			float offcoordshoriz1 = SCREEN_HEIGHT / 2.0f;
			float texcoordshoriz = 0.0f;
			float sizey;
			static int alfaflash;
			static int hudicontimer;
			static float sizetexture[6];
			static float offsettexture[6];
			static float offsettextury[6];
			static int opacity[6];
			if (!hudactive)
			{
				boolvars.timehud = CTimer::m_snTimeInMillisecondsNonClipped;
				hudicontimer = CTimer::m_snTimeInMillisecondsNonClipped;
				hudicnposhoriz = offcoordshoriz;
				alfaflash = 255;
				sizey = 0.0f;
				hudactive = true;
				boolvars.hudindex = 0;
				sizetexture[0] = offcoordshoriz1;
				sizetexture[1] = offcoordshoriz1;
				sizetexture[2] = offcoordshoriz1;
				sizetexture[3] = offcoordshoriz1;
				sizetexture[4] = offcoordshoriz1;
				sizetexture[5] = offcoordshoriz1;

				offsettexture[0] = 0.0f;
				offsettexture[1] = 0.0f;
				offsettexture[2] = 0.0f;
				offsettexture[3] = 0.0f;
				offsettexture[4] = 0.0f;
				offsettexture[5] = 0.0f;

				offsettextury[0] = 0.0f;
				offsettextury[1] = 0.0f;
				offsettextury[2] = 0.0f;
				offsettextury[3] = 0.0f;
				offsettextury[4] = 0.0f;
				offsettextury[5] = 0.0f;

				opacity[0] = 70;
				opacity[1] = 0;
				opacity[2] = 0;
				opacity[3] = 0;
				opacity[4] = 0;
				opacity[5] = 0;
			}
			static bool init;
			static float divfactor = (offcoordshoriz - texcoordshoriz) * 100;
			static float divfactor1 = 255;

			int retardo = 1500;
			int hudAnimTime = 250;

			static float divfactor2 = (offcoordshoriz1 - texcoordshoriz) * 100;
			
			int retardo1 = 10;
			int hudAnimTime1 = 200;

			if (hudactive == true && hudicontimer + retardo1 < CTimer::m_snTimeInMillisecondsNonClipped)
			{
				if (boolvars.hudindex < 6)
				{
					if (CTimer::m_snTimeInMillisecondsNonClipped <= hudicontimer + retardo1 + hudAnimTime1)
					{
						if (sizetexture[boolvars.hudindex] < texcoordshoriz + divfactor2 / hudAnimTime1)
						{
							sizetexture[boolvars.hudindex] = texcoordshoriz;
						}
						else
						{
							sizetexture[boolvars.hudindex] -= SCREEN_HEIGHT * (CTimer::m_snTimeInMillisecondsNonClipped - (hudicontimer + retardo1 + hudAnimTime1)) / hudAnimTime1;
							if (sizetexture[boolvars.hudindex] <= 0.0f)
							{
								sizetexture[boolvars.hudindex] = 0.0f;
							}
						}
						opacity[boolvars.hudindex] = 255 * (CTimer::m_snTimeInMillisecondsNonClipped - (hudicontimer + retardo1 + hudAnimTime1)) / hudAnimTime1;
						
						if (opacity[boolvars.hudindex] > 240)
						{
							opacity[boolvars.hudindex] = 255;
						}
						else
						{
							opacity[boolvars.hudindex] += 15;
						}
					}
					else
					{
						boolvars.hudindex++;
						for (int i = 0; i < boolvars.hudindex; i++)
						{
							offsettexture[i] += 93.0f;
							offsettextury[i] += 5.0f;
						}
						hudicontimer = CTimer::m_snTimeInMillisecondsNonClipped;
						opacity[boolvars.hudindex] = 70;
						if (boolvars.hudindex < 6)
							AudioLib.PlayMenuSFX(4);
						else
							AudioLib.PlayVoiceEvent(19);
					}
				}
			}


			if (hudactive == true && boolvars.timehud + retardo < CTimer::m_snTimeInMillisecondsNonClipped)
			{
				if (CTimer::m_snTimeInMillisecondsNonClipped <= boolvars.timehud + retardo + hudAnimTime)
				{
					if (hudicnposhoriz < texcoordshoriz + divfactor / hudAnimTime)
					{
						hudicnposhoriz = texcoordshoriz;
					}
					else
					{
						hudicnposhoriz -= offcoordshoriz * (CTimer::m_snTimeInMillisecondsNonClipped - (boolvars.timehud + retardo + hudAnimTime)) / hudAnimTime;
						if (hudicnposhoriz <= 0.0f)
						{
							hudicnposhoriz = 0.0f;
						}
					}
					
					sizey += SCREEN_COORD_CENTER_UP(5.0f) * SCREEN_MULTIPLIER(500.0f / hudAnimTime);
					if (sizey > SCREEN_COORD_CENTER_UP(5.0f))
					{
						sizey = SCREEN_COORD_CENTER_UP(5.0f);
					}
				}
				else
				{
					sizey = SCREEN_COORD_CENTER_UP(5.0f);
					hudicnposhoriz = texcoordshoriz;
					alfaflash = 0;
				}

			}

			if (alfaflash < 0)
			{
				alfaflash = 0;
			}
			else
			{
				alfaflash -= divfactor1 * 100 / hudAnimTime;
				if (alfaflash < 0)
				{
					alfaflash = 0;
				}
			}
			boolvars.ofstappearx = hudicnposhoriz;
			if (playerId == 0)
			{
				if (settings.hud == 0)
				{
					//imhud6->Draw(CRect(altline.left, altline.top, altline.left + SCREEN_COORD(200.0f), altline.top + SCREEN_COORD(100.0f)), coloraltimeter);

					if (level1 > 0)
					{
						imhudwanted->Draw(CRect(SCREEN_WIDTH / 1.63f, SCREEN_HEIGHT / 3.76f, SCREEN_WIDTH / 1.63f + SCREEN_COORD(250.0f), (SCREEN_HEIGHT / 3.76f) + SCREEN_COORD(125.0f)), CRGBA(255, 255, 255, 255));

						numb00 = new char[17];
						sprintf(numb00, "x%d", level1);
						CFont::SetBackground(0, 0);
						CFont::SetColor(CRGBA(255, 255, 255, 255));
						CFont::SetOrientation(ALIGN_LEFT);
						CFont::SetProportional(true);
						CFont::SetJustify(false);
						CFont::SetFontStyle(FONT_MENU);
						//CFont::SetScale(SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_WIDTH / SCREEN_COORD(2420.0f) * SCREEN_MULTIPLIER(1.9f));
						CFont::SetScale(SCREEN_MULTIPLIER(0.65f), SCREEN_MULTIPLIER(1.9f));
						CFont::PrintStringFromBottom(SCREEN_WIDTH / 1.63f + SCREEN_COORD(50.0f), SCREEN_HEIGHT / 3.76f + SCREEN_COORD(100.0f), numb00);
						delete[] numb00;
					}

					CFont::SetBackground(0, 0);
					CFont::SetProportional(true);
					CFont::SetJustify(false);
					CFont::SetColor(CRGBA(255, 255, 255, 255));
					//vecVehicleNameScale.x
					//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecAreaNameScale.x / 2.0f), SCREEN_MULTIPLIER(settings.vecAreaNameScale.y / 2.0f));
					CFont::SetScale(SCREEN_WIDTH / SCREEN_COORD(1920.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(1.9f));
					float leftline, rightline;

					if (RsGlobal.maximumWidth == 2732 || RsGlobal.maximumWidth == 2720 || RsGlobal.maximumWidth == 2048 || RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840) {
						if (SCREEN_COORD(0.0F) < SCREEN_HEIGHT - SCREEN_COORD(1080.0f))
						{
							//imhud2->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(1920.0f), SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
							imhud3->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(1920.0f), SCREEN_COORD(0.0f), SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
							//imhud4->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(1920.0f), SCREEN_COORD(0.0f), SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
							imhud5->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(1920.0f), SCREEN_COORD(0.0f), SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
						}
						else {
							//imhud2->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_HEIGHT - SCREEN_COORD(1080.0f), SCREEN_COORD(1920.0f), SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
							imhud3->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(1920.0f), SCREEN_HEIGHT - SCREEN_COORD(1080.0f), SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
							//imhud4->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(1920.0f), SCREEN_HEIGHT - SCREEN_COORD(1080.0f), SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
							imhud5->Draw(CRect(SCREEN_WIDTH - SCREEN_COORD(1920.0f), SCREEN_HEIGHT - SCREEN_COORD(1080.0f), SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
						}
						posx = SCREEN_WIDTH - SCREEN_COORD(1920.0f / 8.0f * 1.1f);
						leftline = SCREEN_WIDTH - SCREEN_COORD(1920.0f * 1.0f / 1.3120f);
						rightline = SCREEN_WIDTH - SCREEN_COORD(1920.0f * 1.0f / 1.2f);
						//posy = SCREEN_HEIGHT - SCREEN_COORD((1080.0f / 7 * 6) - 10.0f);
						posy = SCREEN_HEIGHT / (2.0f);// SCREEN_MULTIPLIER(100.0f) * SCREEN_MULTIPLIER(12.7);
						espaciado = SCREEN_HEIGHT / (100.0f) * (2.35f);
					}
					else {
						imhud3->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
						//imhud4->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
						imhud5->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
						//imhud2->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
						posx = SCREEN_WIDTH / 8.0f * 6.9f;
						leftline = SCREEN_WIDTH * 0.7622f;
						rightline = SCREEN_WIDTH * 0.83f;
						//posy = SCREEN_COORD((1080.0f / 7 * 1) +10.0f);
						posy = SCREEN_HEIGHT / (2.0f);// SCREEN_MULTIPLIER(100.0f) * SCREEN_MULTIPLIER(12.7);
						espaciado = SCREEN_HEIGHT / (100.0f) * (2.35f);
					}
					/*
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetFontStyle(FONT_MENU);
					float posx1 = leftline + ((rightline - leftline) / 2.0f);
					int salud1 = (int)(CWorld::Players[playerId].m_pPed->m_fHealth);
					//CFont::SetScale(SCREEN_WIDTH / SCREEN_COORD(1920.0f) * SCREEN_MULTIPLIER(0.65f), SCREEN_HEIGHT / SCREEN_COORD(1080.0f) * SCREEN_MULTIPLIER(1.9f));

					//CFont::SetScale(SCREEN_MULTIPLIER(0.8f), SCREEN_MULTIPLIER(1.9f));
					sprintf(gString, "hp: %d", salud1);
					CFont::PrintString(leftline + SCREEN_COORD(50.0f), posy + espaciado * 2.0f, gString);
					*/






					if (was_player_damaged1(&salud, &kevlar))
					{
						colorbg = settings.redcodecolour;
					}
					else
					{
						colorbg = settings.blueregular;
					}
					if (alfaflash <= 10)
					{
						if (alha < 100)
						{
							alha += 50;
						}
						else
						{
							alha = 175;
						}
						if (RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840)
						{
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_HEIGHT, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
							imhudbg2->Draw(CRect(SCREEN_COORD(0.0f) + SCREEN_HEIGHT, SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
						}
						else
						{
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_COORD_CENTER_X, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_COORD_CENTER_X, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
						}
					}
					else
					{
						alha = 0;
					}
				}



				float ofstcord = SCREEN_HEIGHT * 0.1f;

				angle1 += CWorld::Players[playerId].m_pPed->m_vecMoveSpeed.Magnitude() / 10.0f;

				if (settings.hud == 1) {
					if (boolvars.hudindex <= 5)
					{
						for (int i = 0; i < 5; i++)
						{
							hudbottomicon[i]->Draw(CRect(SCREEN_COORD_CENTER_LEFT(33.0f) - sizetexture[i] + SCREEN_COORD(offsettexture[i]),
								SCREEN_COORD_BOTTOM(150.0f) - sizetexture[i] * 2.0f + SCREEN_COORD(offsettextury[i]),
								SCREEN_COORD_CENTER_RIGHT(33.0f) + sizetexture[i] + SCREEN_COORD(offsettexture[i]),
								SCREEN_COORD_BOTTOM(84.0f) + SCREEN_COORD(offsettextury[i])), CRGBA(255, 255, 255, opacity[i]));

						}

						if (boolvars.hudindex == 5)
						{
							hudright->Draw(CRect(SCREEN_WIDTH - sizetexture[boolvars.hudindex] * 1.5 - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
								SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f - sizetexture[boolvars.hudindex],
								SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f + sizetexture[boolvars.hudindex] * 1.5f,
								SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f + sizetexture[boolvars.hudindex]), CRGBA(255, 255, 255, opacity[boolvars.hudindex]));
						}
					}
					else
					{
					if (settings.bhudflight == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, CWorld::Players[playerId].m_pPed, "fly_Fast") != false
							|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, CWorld::Players[playerId].m_pPed, "fly_FastSurf") != false)
						{
							CVector horizon0, horizon00, horizon1, horizon2;
							CVector2D posn1[4], posn2[4];
							CVector2D coord2d, coord2d1, height3;
							static float w, h, w1, h1;
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, CWorld::Players[playerId].m_pPed, 0.0, 0.0f, 0.0f, &horizon0.x, &horizon0.y, &horizon0.z);
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, CWorld::Players[playerId].m_pPed, 0.0, 1000.0f, 0.0f, &horizon00.x, &horizon00.y, &horizon00.z);
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, CWorld::Players[playerId].m_pPed, -25.0, 1000.0f, 0.0f, &horizon1.x, &horizon1.y, &horizon1.z);
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, CWorld::Players[playerId].m_pPed, 25.0, 1000.0f, 0.0f, &horizon2.x, &horizon2.y, &horizon2.z);


							c1oordstoscreen(horizon1.x, horizon1.y, horizon1.z, &coord2d.x, &coord2d.y, &w, &h);
							c1oordstoscreen(horizon2.x, horizon2.y, horizon2.z, &coord2d1.x, &coord2d1.y, &w1, &h1);

							plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, coord2d1.x - coord2d.x, coord2d1.y - coord2d.y, &angle);
							//plugin::scripting::CallCommandById(COMMAND_LIMIT_ANGLE, angle, &angle);

							c1oordstoscreen(horizon00.x, horizon00.y, horizon00.z, &coord2d.x, &coord2d.y, &w, &h);

							static float orx, ory, orz;

							CWorld::Players[playerId].m_pPed->GetOrientation(orx, ory, orz);
							float cospit = cosf(orx);
							float sinpit = sinf(orx);

							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, CWorld::Players[playerId].m_pPed, 0.0f, cospit * 1000.0f, -sinpit * 1000.0f, &horizon0.x, &horizon0.y, &horizon0.z);


							c1oordstoscreen(horizon0.x, horizon0.y, horizon0.z, &coord2d1.x, &coord2d1.y, &w, &h);

							posn1[0].x = coord2d1.x + SCREEN_COORD(1024.0f / 2.0f);
							posn1[0].y = coord2d1.y + SCREEN_COORD(128.0f / 2.0f);
							posn1[1].x = coord2d1.x - SCREEN_COORD(1024.0f / 2.0f);
							posn1[1].y = coord2d1.y + SCREEN_COORD(128.0f / 2.0f);
							posn1[2].x = coord2d1.x + SCREEN_COORD(1024.0f / 2.0f);
							posn1[2].y = coord2d1.y - SCREEN_COORD(128.0f / 2.0f);
							posn1[3].x = coord2d1.x - SCREEN_COORD(1024.0f / 2.0f);
							posn1[3].y = coord2d1.y - SCREEN_COORD(128.0f / 2.0f);

							posn2[0].x = coord2d.x + SCREEN_COORD(256.0f / 2.0f);
							posn2[0].y = coord2d.y + SCREEN_COORD(128.0f / 2.0f);
							posn2[1].x = coord2d.x - SCREEN_COORD(256.0f / 2.0f);
							posn2[1].y = coord2d.y + SCREEN_COORD(128.0f / 2.0f);
							posn2[2].x = coord2d.x + SCREEN_COORD(256.0f / 2.0f);
							posn2[2].y = coord2d.y - SCREEN_COORD(128.0f / 2.0f);
							posn2[3].x = coord2d.x - SCREEN_COORD(256.0f / 2.0f);
							posn2[3].y = coord2d.y - SCREEN_COORD(128.0f / 2.0f);

							RotateVertices(posn1, 4, coord2d1.x, coord2d1.y, -boolvars.horizonfloor);

							RotateVertices(posn2, 4, coord2d.x, coord2d.y, -((angle + 90.0f) * 3.1415926535897932384f / 180.0f));

							if (coord2d.x > SCREEN_COORD(20.0f)
								&& coord2d.y > SCREEN_COORD(20.0f)
								&& coord2d.x < SCREEN_WIDTH - SCREEN_COORD(20.0f)
								&& coord2d.y < SCREEN_HEIGHT - SCREEN_COORD(20.0f))
							{
								targetfly->Draw(posn2[0].x, posn2[0].y, posn2[1].x, posn2[1].y, posn2[2].x, posn2[2].y, posn2[3].x, posn2[3].y, CRGBA(255, 255, 255, 90));
							}
							if (coord2d1.x > SCREEN_COORD(20.0f)
								&& coord2d1.y > SCREEN_COORD(20.0f)
								&& coord2d1.x < SCREEN_WIDTH - SCREEN_COORD(20.0f)
								&& coord2d1.y < SCREEN_HEIGHT - SCREEN_COORD(20.0f))
							{
								horizon->Draw(posn1[0].x, posn1[0].y, posn1[1].x, posn1[1].y, posn1[2].x, posn1[2].y, posn1[3].x, posn1[3].y, CRGBA(255, 255, 255, 70));
							}

						}
					}

					static int timedam;
					bool damaged = was_player_damaged1(&salud, &kevlar);
					if (damaged == true &&
						CTimer::m_snTimeInMillisecondsNonClipped > timedam + 70)
					{
						if (CTimer::m_snTimeInMillisecondsNonClipped > timedam + 150)
						{
							timedam = CTimer::m_snTimeInMillisecondsNonClipped;
						}
						else
						{
							damagew->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 2.01f,
								SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
								SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT / 153.6f,
								SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));
						}
					}


					if (damaged == true)
					{
						colorbg = settings.redcodecolour;
					}
					else
					{
						colorbg = settings.blueregular;
					}

					if (alfaflash <= 10)
					{
						if (alha <= 100)
						{
							alha += 50;
						}
						else
						{
							alha = 175;
						}

						if (RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840)
						{
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_HEIGHT, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
							imhudbg2->Draw(CRect(SCREEN_COORD(0.0f) + SCREEN_HEIGHT, SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
						}
						else
						{
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_COORD_CENTER_X, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
							imhudbg1->Draw(CRect(SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_COORD_CENTER_X, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), colorbg);
						}
					}
					else
					{
						alha = 0;
					}

					/*
					if (damaged == true)
					{
					hbottom->Draw(CRect(SCREEN_COORD(0.0f),
					SCREEN_COORD_BOTTOM(256.0f),
					SCREEN_WIDTH,
					SCREEN_HEIGHT), CRGBA(255, 79, 79, 150));
					}
					else
					{
					hbottom->Draw(CRect(SCREEN_COORD(0.0f),
					SCREEN_COORD_BOTTOM(256.0f),
					SCREEN_WIDTH,
					SCREEN_HEIGHT), CRGBA(179, 255, 255, 150));
					}*/

					
						hudright1->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
							SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
							SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
							SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));

						hudrigh0->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
							SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
							SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
							SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));

						if (settings.IsStatsBoxOpen == 0)
						{
							statboxnull->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.9f - ofstcord,
								SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
								SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT / 7.9f - ofstcord,
								SCREEN_COORD_CENTER_DOWN(83.0f)), CRGBA(255, 255, 255, 255));
						}

						CVector2D poshud[4];
						poshud[0].x = SCREEN_COORD_RIGHT(589.0f) - SCREEN_COORD(32.0f);
						poshud[0].y = SCREEN_COORD_BOTTOM(106.0f) + SCREEN_COORD(32.0f);
						poshud[1].x = SCREEN_COORD_RIGHT(589.0f) + SCREEN_COORD(32.0f);
						poshud[1].y = SCREEN_COORD_BOTTOM(106.0f) + SCREEN_COORD(32.0f);
						poshud[2].x = SCREEN_COORD_RIGHT(589.0f) - SCREEN_COORD(32.0f);
						poshud[2].y = SCREEN_COORD_BOTTOM(106.0f) - SCREEN_COORD(32.0f);
						poshud[3].x = SCREEN_COORD_RIGHT(589.0f) + SCREEN_COORD(32.0f);
						poshud[3].y = SCREEN_COORD_BOTTOM(106.0f) - SCREEN_COORD(32.0f);

						if (angle1 < 360.0f)
						{
							angle1 += 0.05f;
						}
						else
						{
							angle1 = 0.0f;
						}
						RotateVertices(poshud, 4, SCREEN_COORD_RIGHT(589.0f), SCREEN_COORD_BOTTOM(106.0f), angle1);
						rothudbottom->Draw(poshud[0].x, poshud[0].y, poshud[1].x, poshud[1].y, poshud[2].x, poshud[2].y, poshud[3].x, poshud[3].y, CRGBA(255, 255, 255, 255));

						poshud[0].x = SCREEN_COORD_RIGHT(220.0f) - SCREEN_COORD(32.0f);
						poshud[0].y = SCREEN_COORD_BOTTOM(67.0f) + SCREEN_COORD(32.0f);
						poshud[1].x = SCREEN_COORD_RIGHT(220.0f) + SCREEN_COORD(32.0f);
						poshud[1].y = SCREEN_COORD_BOTTOM(67.0f) + SCREEN_COORD(32.0f);
						poshud[2].x = SCREEN_COORD_RIGHT(220.0f) - SCREEN_COORD(32.0f);
						poshud[2].y = SCREEN_COORD_BOTTOM(67.0f) - SCREEN_COORD(32.0f);
						poshud[3].x = SCREEN_COORD_RIGHT(220.0f) + SCREEN_COORD(32.0f);
						poshud[3].y = SCREEN_COORD_BOTTOM(67.0f) - SCREEN_COORD(32.0f);

						RotateVertices(poshud, 4, SCREEN_COORD_RIGHT(220.0f), SCREEN_COORD_BOTTOM(67.0f), -angle1);
						rothudbottom1->Draw(poshud[0].x, poshud[0].y, poshud[1].x, poshud[1].y, poshud[2].x, poshud[2].y, poshud[3].x, poshud[3].y, CRGBA(255, 255, 255, 255));


						if (is_over_ground1(2.0f))
						{
							flight->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 2.0f,
								SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
								SCREEN_WIDTH - (boolvars.ofstappearx),
								SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));

						}
						else
						{
							online->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 2.0f,
								SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
								SCREEN_WIDTH - (boolvars.ofstappearx),
								SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));
						}

						hudbottom->Draw(CRect(SCREEN_COORD_RIGHT(687.0f),
							SCREEN_COORD_BOTTOM(147.0f),
							SCREEN_COORD_RIGHT(150.0f),
							SCREEN_COORD_BOTTOM(20.0f)), CRGBA(255, 255, 255, 255));
					}
				}

			}
			flashscreen->Draw(CRect(SCREEN_COORD(0.0f),
				SCREEN_COORD(0.0f) + sizey,
				SCREEN_WIDTH,
				SCREEN_HEIGHT - sizey), CRGBA(255, 255, 255, alfaflash));
		}
		else
		{
			if (settings.hud == 1 &&
				settings.standardhud) {
				playr[3]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
					SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
					SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
					SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));

				playr[0]->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
					SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
					SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
					SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));
			}
			hudactive = false;
			/*
			DrawProgressBar_BilinearOffset_RightAlign(SCREEN_COORD_RIGHT(settings.vecHealthPosn.x), SCREEN_COORD(positionY),
			SCREEN_COORD(settings.vecHealthScale.x) * barSize, SCREEN_COORD(settings.vecHealthScale.y), progress, 1.0f,
			HudColour.GetRGBA(HUD_COLOUR_RED), CRGBA(0, 0, 0, 255));
			*/
		}
	}



	if (index == 7)
	{
		if (is_wearing_suit_area(1) == true && settings.ironmanhud == true)
		{
			if (settings.hud == 0)
			{
				colorhealt = CRGBA(255, 255, 255, 255);
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle2->raster);

				ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
					radarPos2.x, radarPos2.y,
					SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 130.0f, 130.0f + 100.0f * 1.0f, CRGBA(255, 255, 255, 255));

				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle2->raster);

				ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
					radarPos2.x, radarPos2.y,
					-SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 130.0f, 130.0f + 100.0f * 1.0f, CRGBA(255, 255, 255, 255));
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle0->raster);

				progress = player->m_fHealth / maxh;
				ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
					radarPos2.x, radarPos2.y,
					SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 130.0f, 130.0f + 100.0f * progress, colorhealt);

				progress = player->m_fArmour / boolvars.maxarmor;
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle0->raster);

				ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
					radarPos2.x, radarPos2.y,
					-SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 130.0f, 130.0f + 100.0f * progress, colorhealt);
			}
			if (settings.hud == 1)
			{
				if (boolvars.hudindex > 5)
				{
					colorhealt = CRGBA(255, 68, 53, 255);

					static float angle;

					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, beating->raster);

					ShapeHelper::DrawRotatedCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(340.0f), SCREEN_COORD(340.0f), 155.0f, 155.0f + 59.0f, CRGBA(255, 255, 255, 255), angle);
					if (angle < 360.0f)
					{
						angle += 0.25f;
					}
					else
					{
						angle = 0.0f;
					}
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle2->raster);

					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(275.0f), SCREEN_COORD(275.0f), 155.0f, 155.0f + 59.0f, CRGBA(255, 255, 255, 255));

					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(315.0f), SCREEN_COORD(315.0f), 155.0f, 155.0f + 59.0f, CRGBA(255, 255, 255, 255));


					progress = player->m_fArmour / boolvars.maxarmor;
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle0->raster);

					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(315.0f), SCREEN_COORD(315.0f), 155.0f, 155.0f + 59.0f * progress, CRGBA(255, 255, 255, 255));

					progress = player->m_fHealth / maxh;

					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(275.0f), SCREEN_COORD(275.0f), 155.0f, 155.0f + 59.0f * progress, colorhealt);

					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);

					hudrigh1->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
						SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
						SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));
				}
			}

			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
		}
		else
		{
			if (settings.standardhud == true)
			{
				if (settings.hud == 1)
				{
					colorhealt = CRGBA(255, 255, 255, 255);

					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle1->raster);

					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(275.0f), SCREEN_COORD(275.0f), 155.0f, 155.0f + 59.0f, CRGBA(0, 0, 0, 255));

					colorhealt = CRGBA(255, 68, 53, 255);
					progress = player->m_fHealth / maxh;
					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
						SCREEN_COORD(275.0f), SCREEN_COORD(275.0f), 155.0f, 155.0f + 59.0f * progress, colorhealt);
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
					progress = player->m_fArmour / boolvars.maxarmor;
					hudrigh2->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
						SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
						SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
						SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));

					if (player->m_fArmour > 0.0f)
					{
						RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle1->raster);
						ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
							SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
							SCREEN_COORD(315.0f), SCREEN_COORD(315.0f), 155.0f, 155.0f + 59.0f, CRGBA(0, 0, 0, 255));

						ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
							SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT / 7.576f, SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 42.67f,
							SCREEN_COORD(315.0f), SCREEN_COORD(315.0f), 155.0f, 155.0f + 59.0f * progress, CRGBA(255, 255, 255, 255));
						RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
						hudrigh3->Draw(CRect(SCREEN_WIDTH - (boolvars.ofstappearx) - SCREEN_HEIGHT * 6.0f / 10.0f,
							SCREEN_COORD_CENTER_DOWN(83.0f) - SCREEN_HEIGHT / 4.0f,
							SCREEN_WIDTH - (boolvars.ofstappearx) + SCREEN_HEIGHT * 4.0f / 10.0f,
							SCREEN_COORD_CENTER_DOWN(83.0f) + SCREEN_HEIGHT / 4.0f), CRGBA(255, 255, 255, 255));

					}
				}
				else
				{
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle->raster);
					progress = player->m_fHealth / maxh;
					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						radarPos2.x, radarPos2.y,
						SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 130.0f, 130.0f + 100.0f * 1.0f, CRGBA(0, 0, 0, 255));

					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle1->raster);
					ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
						radarPos2.x, radarPos2.y,
						SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 131.0f, 131.0f + 98.0f * progress, CRGBA(255, 68, 53, 255));

					if (player->m_fArmour > 0.0f)
					{
						progress = player->m_fArmour / boolvars.maxarmor;
						RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle->raster);
						ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
							radarPos2.x, radarPos2.y,
							-SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 130.0f, 130.0f + 100.0f * 1.0f, CRGBA(0, 0, 0, 255));

						RwRenderStateSet(rwRENDERSTATETEXTURERASTER, hpcircle1->raster);
						ShapeHelper::DrawCircleSectorTextured(512.0f, 512.0f, 0.0f, 0.0f, 512.0f, 512.0f,
							radarPos2.x, radarPos2.y,
							-SCREEN_COORD(275.0f / 2.0f), SCREEN_COORD(275.0f / 2.0f), 131.0f, 131.0f + 98.0f * progress, CRGBA(255, 68, 53, 255));
					}
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
				}
			}
		}
	}

	if (index == 9)
	{

		InitCompassTextures();
		static float bk;
		static CVector camaimpoint;
		static CVector objrot;
		static float dist, dist2, heading, seno, coseno, datosiete;
		static int estructura;
		static float angulo;
		static float datoseis;
		static float datocinco;
		static float dato28;
		static float posx;
		static float colocacionx;
		static int cardinal;
		static int angulo1;
		CPed *player = FindPlayerPed(0);
		if (player)
		{
			//primera parte
			plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &heading);
			heading = heading * 180.0f / 3.141529f;
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &objrot.x, &objrot.y, &objrot.z);
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 1.0f, &camaimpoint.x, &camaimpoint.y, &camaimpoint.z);

			camaimpoint.x -= objrot.x;
			camaimpoint.y -= objrot.y;
			camaimpoint.z -= objrot.z;

			objrot.z = heading;
			objrot.z -= 90.0f;

			objrot.x = camaimpoint.x;
			objrot.y = camaimpoint.y;

			seno = sinf(objrot.z);
			coseno = cosf(objrot.z);

			objrot.x *= coseno;
			objrot.y *= coseno;
			camaimpoint.x *= seno;
			camaimpoint.y *= seno;

			objrot.y *= -1.0f;
			camaimpoint.x += objrot.y;
			camaimpoint.y += objrot.x;

			plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, camaimpoint.y, camaimpoint.z, 0.0f, 0.0f, &dist2);
			datosiete = atan2f(camaimpoint.x, dist2) *180.0f / 3.141529f;;

			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, -1.0f, &objrot.x, &objrot.y, &objrot.z);
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 1.0f, &camaimpoint.x, &camaimpoint.y, &camaimpoint.z);

			if (objrot.z > camaimpoint.z)
			{
				datosiete -= 180.0f;
				datosiete *= -1.0f;
			}

			//segunda parte


			plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &objrot.x, &objrot.y, &objrot.z);
			plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &camaimpoint.x, &camaimpoint.y, &camaimpoint.z);

			camaimpoint.x -= objrot.x;
			camaimpoint.y -= objrot.y;
			camaimpoint.z -= objrot.z;

			plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, camaimpoint.x, camaimpoint.y, camaimpoint.z, 0.0f, 0.0f, 0.0f, &dist);
			camaimpoint.x /= dist;
			camaimpoint.y /= dist;
			camaimpoint.z /= dist;

			objrot.y = atan2f(camaimpoint.y, camaimpoint.x) *180.0f / 3.141529f;

			plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, camaimpoint.x, camaimpoint.y, 0.0f, 0.0f, &dist2);
			objrot.x = atan2f(camaimpoint.z, dist2) *180.0f / 3.141529f;

			plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &heading);
			heading = heading *180.0f / 3.141529f;
			heading -= objrot.y;
			plugin::scripting::CallCommandById(COMMAND_LIMIT_ANGLE, heading, &heading);
			heading = cosf(heading);
			bk = heading;
			bk = abs(bk);
			heading /= bk;
			plugin::scripting::CallCommandById(COMMAND_GET_PLAYER_IN_CAR_CAMERA_MODE, &bk);
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_ON_ANY_BIKE, player) == 1)
			{
				estructura = plugin::patch::GetInt((int)player, 0);
				estructura += 1608;
				angulo = plugin::patch::GetFloat(estructura, 0);
				angulo *= 180.0f;
				angulo /= 3.141529f;
				datosiete += angulo;
			}

			angulo = 0.0f;
			if (bk == 0)
			{
				if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == 0 ||
					plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 7) == 0)
				{
					angulo = datosiete;
				}
			}
			if (bk > 5)
			{
				angulo = datosiete;
				angulo *= 1.0f;
				//contador *= 0 o 1
			}

			if (bk == 5)
			{
				datosiete *= heading;
				//contador *= 0 o 1
			}

			//setup de todo
			datosiete -= angulo;
			datosiete *= -1.0f;

			//-objrot.z es el angulo y (altura donde mira cj)

			objrot.z = angulo;
			objrot.z *= -1.0f;
			//listo

			angulo = objrot.z;
			boolvars.horizonfloor = angulo;
			boolvars.pith = objrot.x;
			//boolvars.rot = objrot.y * -1.0f;
			datoseis = objrot.y * -1.0f + 450.0f;
			datocinco = objrot.y * -1.0f + 450.0f;
			while (datoseis < 0.0f || datoseis >= 360.0f)
			{
				if (0.0f > datoseis)
				{
					datoseis += 360.0f;
				}
				if (datoseis >= 360.0f)
				{
					datoseis -= 360.0f;
				}
			}

			dato28 = (float((int)datocinco / 10))*(10.0f) - 60.0f;
			//dato28 = (float((int)datocinco / 10))*(10.0f) - 50.0f;
			bk = -60;

			static float offsetforcenter;

			static float offsetfortop;

			static float offsetx2, offsety2;



			compassalign = GetPrivateProfileInt("CompassConfig", "Align", 0, location.data());
			offsetcompassx = ReadFloatt("CompassConfig", "PositionOffsetX", 0.0f, location.data());
			offsetcompassy = ReadFloatt("CompassConfig", "PositionOffsetY", 0.0f, location.data());

			if (offsetcompassx < 0.0f)
			{
				offsetx2 = 0.0f - SCREEN_COORD(-offsetcompassx);
			}
			else
			{
				offsetx2 = SCREEN_COORD(offsetcompassx);
			}

			if (offsetcompassy < 0.0f)
			{
				offsety2 = 0.0f - SCREEN_COORD(-offsetcompassy);
			}
			else
			{
				offsety2 = SCREEN_COORD(offsetcompassy);
			}

			CVector2D radarPos;
			CRadar::TransformRadarPointToScreenSpace(radarPos, CVector2D(0.0f, 1.0f));

			if (compassalign == 1)
			{
				offsetfortop = radarPos.y - SCREEN_COORD(30.0f);
				offsetforcenter = 0.0f - SCREEN_COORD_CENTER_X + radarPos.x;
			}
			else
			{
				offsetfortop = SCREEN_COORD(30.0f);
				offsetforcenter = SCREEN_COORD(0.0f);
			}

			////////////////////////////////////////////////////////////////////
			static float posx1radar, posx2radar;
			static float posx3radar, posx4radar;
			static float posx1radaros;

			if (datoseis >= 180.0f)
			{
				datoseis = -(360.0f - datoseis);
			}

			compasssize = GetPrivateProfileInt("CompassConfig", "Scale", 100, location.data());

			if (compasssize > 100)
			{
				compasssize = 100;
			}
			if (compasssize < 1)
			{
				compasssize = 1;
			}

			posx1radaros = ((datoseis)* compasssize1 / 90.0f) + 1043.0f;
			posx1radar = (((datoseis)* compasssize1 / 90.0f) - 300.0f * (float)compasssize / 100.0f) + 1043.0f;
			posx2radar = (((datoseis)* compasssize1 / 90.0f) + 300.0f * (float)compasssize / 100.0f) + 1043.0f;

			posx3radar = (((datoseis)* compasssize1 / 90.0f) - 150.0f * (float)compasssize / 100.0f) + 1043.0f;
			posx4radar = (((datoseis)* compasssize1 / 90.0f) + 150.0f * (float)compasssize / 100.0f) + 1043.0f;

			float compassmiddle = 300.0f * (float)compasssize / 100;
			float compassquarter = 150.0f * (float)compasssize / 100;

			float indicatormiddle = 256.0f * (float)compasssize / 100;
			float indicatorquarter = 128.0f * (float)compasssize / 100;

			float indicatorx1 = 1024.0f - 1024.0f * (float)compasssize / 100;
			float indicatorx2 = 1024.0f + 1024.0f * (float)compasssize / 100;

			compassbg->Draw(CRect(SCREEN_COORD_CENTER_LEFT(compassmiddle) + offsetforcenter + offsetx2,
				offsetfortop - SCREEN_COORD(30.0f) + offsety2,
				SCREEN_COORD_CENTER_RIGHT(compassmiddle) + offsetforcenter + offsetx2,
				offsetfortop + SCREEN_COORD(30.0f) + offsety2), settings.colorcompassbg);

			int compassletterstyle = GetPrivateProfileInt("CompassConfig", "LimitsFade", 0, location.data());

			if (compassletterstyle > 0)
			{
				unsigned int savedShade;
				unsigned int savedAlpha;
				RwRenderStateGet(rwRENDERSTATESHADEMODE, &savedShade);
				RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(rwSHADEMODEGOURAUD));
				RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));

				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, compass->m_pTexture->raster);

				// Background
				ShapeHelper::DrawFadedRectangle(CRect(SCREEN_COORD_CENTER_RIGHT(compassquarter) + offsetforcenter + offsetx2,
					offsetfortop - SCREEN_COORD(30.0f) + offsety2,
					SCREEN_COORD_CENTER_RIGHT(compassmiddle) + offsetforcenter + offsetx2,
					offsetfortop + SCREEN_COORD(30.0f) + offsety2),
					settings.colorcompassbar,
					posx4radar / 2088.0f, 0.0f, posx2radar / 2088.0f, 0.0f, posx4radar / 2088.0f, 1.0f, posx2radar / 2088.0f, 1.0f);

				ShapeHelper::DrawFadedRectangle(CRect(SCREEN_COORD_CENTER_LEFT(compassquarter) + offsetforcenter + offsetx2,
					offsetfortop - SCREEN_COORD(30.0f) + offsety2,
					SCREEN_COORD_CENTER_LEFT(compassmiddle) + offsetforcenter + offsetx2,
					offsetfortop + SCREEN_COORD(30.0f) + offsety2),
					settings.colorcompassbar,
					posx3radar / 2088.0f, 0.0f, posx1radar / 2088.0f, 0.0f, posx3radar / 2088.0f, 1.0f, posx1radar / 2088.0f, 1.0f);


				ShapeHelper::DrawTexturedRectangle(CRect(SCREEN_COORD_CENTER_LEFT(compassquarter) + offsetforcenter + offsetx2,
					offsetfortop - SCREEN_COORD(30.0f) + offsety2,
					SCREEN_COORD_CENTER_RIGHT(compassquarter) + offsetforcenter + offsetx2,
					offsetfortop + SCREEN_COORD(30.0f) + offsety2),
					settings.colorcompassbar,
					posx3radar / 2088.0f, 0.0f, posx4radar / 2088.0f, 0.0f, posx3radar / 2088.0f, 1.0f, posx4radar / 2088.0f, 1.0f);
				/*
				ShapeHelper::DrawTexturedRectangle(CRect(SCREEN_COORD_RIGHT(64.0f + 290.0f),
				SCREEN_COORD_BOTTOM(40.0f + 290.0f), SCREEN_COORD_RIGHT(64.0f), SCREEN_COORD_BOTTOM(40.0f)),
				CRGBA(255, 255, 255, 255),
				0.0f, 0.0f, 291.0f / 1024.0f, 0.0f, 0.0f, 291.0f / 512.0f, 291.0f / 1024.0f, 291.0f / 512.0f);
				*/
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);

				RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(savedShade));
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));
			}
			else
			{
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, compass->m_pTexture->raster);

				// Background
				ShapeHelper::DrawTexturedRectangle(CRect(SCREEN_COORD_CENTER_LEFT(compassmiddle) + offsetforcenter + offsetx2,
					offsetfortop - SCREEN_COORD(30.0f) + offsety2,
					SCREEN_COORD_CENTER_RIGHT(compassmiddle) + offsetforcenter + offsetx2,
					offsetfortop + SCREEN_COORD(30.0f) + offsety2),
					settings.colorcompassbar,
					posx1radar / 2088.0f, 0.0f, posx2radar / 2088.0f, 0.0f, posx1radar / 2088.0f, 1.0f, posx2radar / 2088.0f, 1.0f);

				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
			}

			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, indicator->m_pTexture->raster);

			// Background
			ShapeHelper::DrawTexturedRectangle(CRect(SCREEN_COORD_CENTER_LEFT(indicatormiddle) + offsetforcenter + offsetx2,
				offsetfortop - SCREEN_COORD(30.0f) + offsety2,
				SCREEN_COORD_CENTER_RIGHT(indicatormiddle) + offsetforcenter + offsetx2,
				offsetfortop + SCREEN_COORD(30.0f) + offsety2),
				settings.colorcompassmark,
				indicatorx1 / 2048.0f, 0.0f, indicatorx2 / 2048.0f, 0.0f, indicatorx1 / 2048.0f, 1.0f, indicatorx2 / 2048.0f, 1.0f);

			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);

		}
	}

	if (index == 10) {
		static int alfa;
		static float coord2dx, coord2dy, w, h;
		static float coord2dx1, coord2dy1, w1, h1;
		c1oordstoscreen(boolvars.pricel.x, boolvars.pricel.y, boolvars.pricel.z, &coord2dx, &coord2dy, &w, &h);
		if (is_wearing_suit_area(1) == true) {


			if (boolvars.fartarget == true)
			{
				alfa = 255;
				imhud1->Draw(CRect((coord2dx - 56.0f), (coord2dy - 56.0f), (coord2dx + 56.0f), (coord2dy + 56.0f)), CRGBA(255, 255, 255, alfa));
			}
			else
			{
				if (alfa >= 51)
					alfa -= 51;
				else
					alfa = 0;
				imhud1->Draw(CRect((coord2dx - 56.0f), (coord2dy - 56.0f), (coord2dx + 56.0f), (coord2dy + 56.0f)), CRGBA(255, 255, 255, alfa));
				c1oordstoscreen(boolvars.target.x, boolvars.target.y, boolvars.target.z, &coord2dx1, &coord2dy1, &w1, &h1);

				imhud00->Draw(CRect((coord2dx - 56.0f), (coord2dy - 56.0f), (coord2dx + 56.0f), (coord2dy + 56.0f)), CRGBA(255, 255, 255, 255));
				imhud0->Draw(CRect((coord2dx1 - w1 / 2 - 28.0f), (coord2dy1 - 56.0f), (coord2dx1 - w1 / 2), (coord2dy1 + 56.0f)), CRGBA(255, 255, 255, 255));
				imhud0->Draw(CRect((coord2dx1 + w1 / 2 + 28.0f), (coord2dy1 - 56.0f), (coord2dx1 + w1 / 2), (coord2dy1 + 56.0f)), CRGBA(255, 255, 255, 255));
			}
		}
		else
		{
			imhud1->Draw(CRect((coord2dx - 56.0f), (coord2dy - 56.0f), (coord2dx + 56.0f), (coord2dy + 56.0f)), CRGBA(255, 255, 255, 255));
			imhud11->Draw(CRect((coord2dx - 56.0f), (coord2dy - 56.0f), (coord2dx + 56.0f), (coord2dy + 56.0f)), CRGBA(255, 255, 255, 255));
		}
	}
	if (index == 12) {
		imhud1->Draw(CRect(SCREEN_COORD_CENTER_LEFT(35.0f), SCREEN_COORD_CENTER_UP(35.0f), SCREEN_COORD_CENTER_RIGHT(35.0f), SCREEN_COORD_CENTER_DOWN(35.0f)), CRGBA(255, 255, 255, 255));
	}
}