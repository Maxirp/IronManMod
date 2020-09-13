// mobile-hud
#include "Settings.h"
#include <game_sa/CFileMgr.h>
#include "MobileText.h"
Settings settings;



void Settings::Read() {
    plugin::config_file config(PLUGIN_PATH("IronMan\\maxirpHud.dat"));


	vecUnusedWeaponIconAlpha = config["WEAPON_UNUSED_ICON_OPACITY"].asInt(255);
	vecUnusedWeaponIconOffst = config["WEAPON_UNUSED_ICON_OFFSET"].asVec2d(CVector2D(110.0f, 0.0f));
	vecUnusedWeaponIconScale = config["WEAPON_UNUSED_ICON_SCALE"].asVec2d(CVector2D(114.0f, 114.0f));
	areadata		                      = config["ENABLE_AREA_NAMES"						].asBool(true);
	clockdata		                      = config["ENABLE_CLOCK"							].asBool(true);
	standardhud		                      = config["ENABLE_STANDARD_HUD"					].asBool(true);
	 ironmanhud		                      = config["ENABLE_IRONMAN_HUD"						].asBool(true);	 
	 bhudflight		                      = config["ENABLE_IM_FLIGHT_HUD"                   ].asBool(true);
	 bEnableColors                        = config["ENABLE_COLORS"                          ].asBool(true);
    bEnablePlayerInfo                     = config["ENABLE_PLAYER_INFO"                     ].asBool(true);
	bWEnablealkstyle					  = config["ENABLE_WALKSTYLE"	                    ].asBool(true);
	bEnableRadar = true;
	bDonewithradar = false;
	bEnableRadar1                         = config["ENABLE_RADAR"                           ].asBool(true);
    bEnableSubtitles                      = config["ENABLE_SUBTITLES"                       ].asBool(true);
    bEnableRadioNames                     = config["ENABLE_RADIO_NAMES"                     ].asBool(true);
    bEnableAreaNames                      = config["ENABLE_AREA_NAMES"                      ].asBool(true);
    bEnableVehicleNames                   = config["ENABLE_VEHICLE_NAMES"                   ].asBool(true);
    bEnableStatsBox                       = config["ENABLE_STATS_BOX"                       ].asBool(true);
    bEnableTextBox                        = config["ENABLE_TEXT_BOX"                        ].asBool(true);
    bEnableTimers                         = config["ENABLE_TIMERS"                          ].asBool(true);
    bEnableProgressBars                   = config["ENABLE_PROGRESS_BARS"                   ].asBool(true);
    bEnableMenuSystem                     = config["ENABLE_MENU_SYSTEM"                     ].asBool(true);
    bEnableLoadingScreens                 = config["ENABLE_LOADING_SCREENS"                 ].asBool(true);
    bEnableMenuPages                      = config["ENABLE_MENU_PAGES"                      ].asBool(true);
    bEnableFrontends                      = config["ENABLE_FRONTEND"                        ].asBool(true);
	radarmode = plugin::patch::GetInt(0xBA676C, false);
    vecWeaponIconPosny                     = config["WEAPON_ICON_POSN_Y"                       ].asFloat(40.0f);
    vecWeaponIconScale                    = config["WEAPON_ICON_SCALE"                      ].asVec2d(CVector2D(124.0f, 124.0f));
    vecWeaponAmmoPosny                     = config["WEAPON_AMMO_POSN_Y"                       ].asFloat(35.0f);
    vecWeaponAmmoScale                    = config["WEAPON_AMMO_SCALE"                      ].asVec2d(CVector2D(0.65f, 1.2f));
    vecClockPosny                          = config["CLOCK_POSN_Y"                           ].asFloat(115.0f);
    vecClockScale                         = config["CLOCK_SCALE"                            ].asVec2d(CVector2D(1.0f, 1.9f));
    vecMoneyPosny                          = config["MONEY_POSN_Y"                           ].asFloat(75.0f);
    vecMoneyScale                         = config["MONEY_SCALE"                            ].asVec2d(CVector2D(0.9f, 1.7f));
    vecBreathPosn                         = config["BREATH_POSN"                            ].asVec2d(CVector2D(156.0f, 148.0f));
    vecBreathScale                        = config["BREATH_SCALE"                           ].asVec2d(CVector2D(170.0f, 13.0f));
    vecWantedLevelPosn                    = config["WANTED_LEVEL_POSN"                      ].asVec2d(CVector2D(18.0f, 162.0f));
    vecWantedLevelStarScale               = config["WANTED_LEVEL_STAR_SCALE"                ].asVec2d(CVector2D(1.2f, 2.3f));
    fWantedLevelStarSpace                 = config["WANTED_LEVEL_STAR_SPACE"                ].asFloat(46.0f);
    fSecondPlayerOffsetY                  = config["SECOND_PLAYER_OFFSET_Y"                 ].asFloat(190.0f);
    vecRadarPosn                          = config["RADAR_POSN"                             ].asVec2d(CVector2D(105.0f, 130.0f));
    fRadarWidthHalf                       = config["RADAR_WIDTH_HALF"                       ].asFloat(93.0f);
    fRadarHeightHalf                      = config["RADAR_HEIGHT_HALF"                      ].asFloat(93.0f);
    fRadarBorderWidthHalf                 = config["RADAR_BORDER_WIDTH_HALF"                ].asFloat(96.0f);
    fRadarBorderHeightHalf                = config["RADAR_BORDER_HEIGHT_HALF"               ].asFloat(96.0f);
    vecPlaneRadarPosn                     = config["PLANE_RADAR_POSN"                       ].asVec2d(CVector2D(222.0f, 67.0f));
    vecPlaneRadarSize                     = config["PLANE_RADAR_SIZE"                       ].asVec2d(CVector2D(14.0f, 145.0f));
    vecPlaneRadarLightPartPosn            = config["PLANE_RADAR_LIGHT_PART_POSN"            ].asVec2d(CVector2D(225.0f, 70.0f));
    vecPlaneRadarLightPartSize            = config["PLANE_RADAR_LIGHT_PART_SIZE"            ].asVec2d(CVector2D(8.0f, 139.0f));
    bRadioNameTop                         = config["RADIO_NAME_TOP"                         ].asBool(false);
    fRadioNameTopPosnY                    = config["RADIO_NAME_TOP_POSN_Y"                  ].asFloat(5.0f);
    vecRadioNamePosn                      = config["RADIO_NAME_POSN"                        ].asVec2d(CVector2D(20.0f, 37.0f));
    vecRadioNameScale                     = config["RADIO_NAME_SCALE"                       ].asVec2d(CVector2D(0.8f, 1.5f));
    vecTextBoxPosn                        = config["TEXT_BOX_POSN"                          ].asVec2d(CVector2D(25.0f, 40.0f));
    fTextBoxPosnXWithRadar                = config["TEXT_BOX_POSN_X_WITH_RADAR"             ].asFloat(231.0f);
    fTextBoxPosnXWithRadarAndPlane        = config["TEXT_BOX_POSN_X_WITH_RADAR_AND_PLANE"   ].asFloat(259.0f);
    vecTextBoxFontScale                   = config["TEXT_BOX_FONT_SCALE"                    ].asVec2d(CVector2D(0.7f, 1.5f));
    fTextBoxWidth                         = config["TEXT_BOX_WIDTH"                         ].asFloat(421.0f);
    fTextBoxBorderSize                    = config["TEXT_BOX_BORDER_SIZE"                   ].asFloat(-5.0f);
    vecTextBoxProgressBarSize             = config["TEXT_BOX_PROGRESS_BAR_SIZE"             ].asVec2d(CVector2D(140.0f, 20.0f));
    fTextBoxProgressBarYShift             = config["TEXT_BOX_PROGRESS_BAR_Y_SHIFT"          ].asFloat(3.0f);
    fTextBoxWidthWithProgressBar          = config["TEXT_BOX_WIDTH_WITH_PROGRESS_BAR"       ].asFloat(164.0f);
    fTextBoxPlusMinTextXShift             = config["TEXT_BOX_PLUS_MIN_TEXT_X_SHIFT"         ].asFloat(152.0f);
    vecTextBoxTextPosnWhenMenuEnabled     = config["TEXT_BOX_TEXT_POSN_WHEN_MENU_ENABLED"   ].asVec2d(CVector2D(18.0f, 10.0f));
    fTextBoxHeightWhenMenuEnabled         = config["TEXT_BOX_HEIGHT_WHEN_MENU_ENABLED"      ].asFloat(55.0f);
    bMissionTimersTop                     = config["MISSIONTIMERS_TOP"                      ].asBool(true);
    fMissionTimersTopPosnY                = config["MISSIONTIMERS_TOP_POSN_Y"               ].asFloat(36.0f);
    fMissionTimersBoxPosnX                = config["MISSIONTIMERS_BOX_POSN_X"               ].asFloat(20.0f);
    fMissionTimersBoxPosnMinY             = config["MISSIONTIMERS_BOX_POSN_MIN_Y"           ].asFloat(230.0f);
    fMissionTimersBoxPosnMinY2P           = config["MISSIONTIMERS_BOX_POSN_MIN_Y_2P"        ].asFloat(380.0f);
    vecMissionTimersBoxSize               = config["MISSIONTIMERS_BOX_SIZE"                 ].asVec2d(CVector2D(240.0f, 37.0f));
    vecMissionTimersTextScale             = config["MISSIONTIMERS_TEXT_SCALE"               ].asVec2d(CVector2D(0.7f, 1.3f));
    fMissionTimersTextMaxWidth            = config["MISSIONTIMERS_TEXT_MAX_WIDTH"           ].asFloat(128.0f);
    fMissionTimersTextOffsetY             = config["MISSIONTIMERS_TEXT_OFFSET_Y"            ].asFloat(6.0f);
    fMissionTimersTextDescriptionOffsetX  = config["MISSIONTIMERS_TEXT_DESCRIPTION_OFFSET_X"].asFloat(8.0f);
    fMissionTimersTextValueOffsetX        = config["MISSIONTIMERS_TEXT_VALUE_OFFSET_X"      ].asFloat(8.0f);
    vecMissionTimersProgressBarOffset     = config["MISSIONTIMERS_PROGRESSBAR_OFFSET"       ].asVec2d(CVector2D(6.0f, 11.0f));
    vecMissionTimersProgressBarSize       = config["MISSIONTIMERS_PROGRESSBAR_SIZE"         ].asVec2d(CVector2D(92.0f, 18.0f));
    fSubtitlesPosnY                       = config["SUBTITLES_POSN_Y"                       ].asFloat(60.0f);
    fSubtitlesWidePosnY                   = config["SUBTITLES_WIDESCREEN_POSN_Y"            ].asFloat(5.0f);
    vecSubtitlesScale                     = config["SUBTITLES_SCALE"                        ].asVec2d(CVector2D(1.0f, 1.9f));
    vecAreaNamePosn                       = config["AREA_NAME_POSN"                         ].asVec2d(CVector2D(25.0f, 50.0f));
    vecAreaNameScale                      = config["AREA_NAME_SCALE"                        ].asVec2d(CVector2D(1.1f, 2.1f));
    vecVehicleNamePosn                    = config["VEHICLE_NAME_POSN"                      ].asVec2d(CVector2D(25.0f, 40.0f));
    vecVehicleNameScale                   = config["VEHICLE_NAME_SCALE"                     ].asVec2d(CVector2D(0.8f, 1.6f));
    vecStatsBoxPosn                       = config["STATS_BOX_POSN"                         ].asVec2d(CVector2D(10.0f, 10.0f));

    bReloadWKey                            = config["RELOAD_SETTINGS"                        ].asBool(false);
    
	colorradar = config["RADAR_BORDER_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	coloraltimeter = config["ALTIMETER_BG_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	coloraltimeterbar = config["ALTIMETER_BAR_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	coloraltimetermark = config["ALTIMETER_MARK_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	colorcompassbg = config["COMPASS_BG_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	colorcompassbar = config["COMPASS_BAR_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	colorcompassmark = config["COMPASS_MARK_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	blueregular = config["IM_BACKGROUND_COLOUR"].asRGBA(CRGBA(0, 50, 60, 255));
	redcodecolour = config["IM_BG_DAM_COLOUR"].asRGBA(CRGBA(72, 25, 25, 255));
	areacolor = config["AREA_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	clockcolor = config["CLOCK_COLOUR"].asRGBA(CRGBA(255, 255, 255, 255));
	if(bReloadWKey)
        iReloadKey = config["RELOAD_KEY"].asInt(-1);

	if(ironmanhud)
	{
		numtextures[0] = 0; //bg
		numtextures[1] = 0;
		numtextures[2] = 0;
		numtextures[3] = 0;
		numtextures[4] = 0;
		numtextures[5] = 0; //iconitos de abajo
		numtextures[6] = 0; //diagnostics rueda
		numtextures[7] = 0; //diagnostics health pestaña
		numtextures[8] = 0; //altimeter
		numtextures[9] = 0; //compass
		numtextures[10] = 0; //flightaimpoint or aimpoint
		numtextures[11] = 0; //targets
		numtextures[12] = 0; //missiles
	}
}

#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

void traje::deletealldata()
{
	this->hasini = false;
	this->iconexists=false;
	this->mdlexists = false;
	this->texexists = false;
	this->id=0;
	strcpy(this->name, "");
	strcpy(this->pngname, "");
	this->rightfoot.x = 0;
	this->rightfoot.y = 0;
	this->rightfoot.z = 0;
	this->leftfoot.x = 0;
	this->leftfoot.y = 0;
	this->leftfoot.z = 0;
	this->righthand.x = 0;
	this->righthand.y = 0;
	this->righthand.z = 0;
	this->lefthand.x = 0;
	this->lefthand.y = 0;
	this->lefthand.z = 0;
	this->flyrightfoot.x = 0;
	this->flyrightfoot.y = 0;
	this->flyrightfoot.z = 0;
	this->flyleftfoot.x = 0;
	this->flyleftfoot.y = 0;
	this->flyleftfoot.z = 0;
	this->flyrighthand.x = 0;
	this->flyrighthand.y = 0;
	this->flyrighthand.z = 0;
	this->flylefthand.x = 0;
	this->flylefthand.y = 0;
	this->flylefthand.z = 0;
}

bool Settings::storesuits()
{
	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	//size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	std::string directorios[84];
	std::string inidir;

	StringCchCopy(szDir, MAX_PATH, GAME_PATH("modloader\\IronManMod\\Skins"));
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return false;
	}
	// List all the files in the directory with some info about them.
	int contador = 0;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ('.' != ffd.cFileName[0])
			{
				sprintf(directorios[contador].data(), "%s", ffd.cFileName);
				contador++;
			}
		}
		else
		{
			//not a dir

			//filesize.LowPart = ffd.nFileSizeLow;
			//filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{

	}

	FindClose(hFind);
	

	settings.amount = contador;
	int trajenum=0;
	for (int ii = 0; ii < contador; ii++)
	{
		strcpy(settings.folderdirs[ii].name, directorios[ii].data());
		HANDLE hFind1 = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA ffd1;
		TCHAR szDir1[MAX_PATH];
		Tsuitfiles archivostrajes;
		std::string directoriosdetrajes[84]; //subdirs con 4 elems c u
		//int contadorcarpconelem = 0;

		StringCchCopy(szDir1, MAX_PATH, GAME_PATH("modloader\\IronManMod\\Skins"));
		StringCchCat(szDir1, MAX_PATH, TEXT("\\"));
		StringCchCat(szDir1, MAX_PATH, settings.folderdirs[ii].name);
		StringCchCat(szDir1, MAX_PATH, TEXT("\\*"));
		hFind1 = FindFirstFile(szDir1, &ffd1);
		if (INVALID_HANDLE_VALUE == hFind1)
		{
			//folder empty
		}
		else
		{
			do
			{
				if (ffd1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if ('.' != ffd1.cFileName[0])
					{
						sprintf(directoriosdetrajes[trajenum].data(), "%s", ffd1.cFileName);
						trajenum++;
					}
				}
			} while (FindNextFile(hFind1, &ffd1) != 0);
			dwError = GetLastError();
			if (dwError != ERROR_NO_MORE_FILES)
			{

			}
			FindClose(hFind1);
			int contadorcarpconelem = 1;

			//trajenum tiene la cantidad de subdirectorios
			for (int i = 0; i < trajenum; i++)
			{
				bool lacarpetaesvalida = false;

				HANDLE hFind2 = INVALID_HANDLE_VALUE;
				WIN32_FIND_DATA ffd2;
				TCHAR szDir2[MAX_PATH];
				std::string filename = "";
				std::string stored = "";
				std::string trajeconcreto;

				Telemento elem[4];
				elem[0].existe = false;
				strcpy(elem[0].nombre, "");
				elem[1].existe = false;
				strcpy(elem[1].nombre, ""); 
				elem[2].existe = false;
				strcpy(elem[2].nombre, ""); 
				elem[3].existe = false;
				strcpy(elem[3].nombre, "");


				StringCchCopy(szDir2, MAX_PATH, GAME_PATH("modloader\\IronManMod\\Skins"));
				StringCchCat(szDir2, MAX_PATH, TEXT("\\"));
				StringCchCat(szDir2, MAX_PATH, settings.folderdirs[ii].name);
				StringCchCat(szDir2, MAX_PATH, TEXT("\\"));
				StringCchCat(szDir2, MAX_PATH, directoriosdetrajes[i].data()); //subdirs recien encontrados
				StringCchCat(szDir2, MAX_PATH, TEXT("\\*"));
				hFind2 = FindFirstFile(szDir2, &ffd2);
				if (INVALID_HANDLE_VALUE == hFind2)
				{
					//folder empty
				}
				else
				{
					do
					{
						if ((ffd2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false)
						{
							if ('.' != ffd2.cFileName[0])
							{
								sprintf(trajeconcreto.data(), "%s", ffd2.cFileName);
								if (strlen(trajeconcreto.data()) > 4)
								{
									for (int y = 0; y < strlen(ffd2.cFileName); y++)
									{
										// error en esta llave
										if (ffd2.cFileName[y] != '.')
										{
											char letter = ffd2.cFileName[y];
											filename += letter;
										}
										else
										{
											if (tolower(ffd2.cFileName[y + 1]) == 'd' &&
												tolower(ffd2.cFileName[y + 2]) == 'a' &&
												tolower(ffd2.cFileName[y + 3]) == 't' &&
												elem[0].existe == false)
											{
												strcpy(elem[0].nombre, filename.data());
												elem[0].tipo = 1;
												elem[0].existe = true;
											}
											else
											{

												if (tolower(ffd2.cFileName[y + 1]) == 'd' &&
													tolower(ffd2.cFileName[y + 2]) == 'f' &&
													tolower(ffd2.cFileName[y + 3]) == 'f' &&
													elem[1].existe == false)
												{
													elem[1].existe = true;
													strcpy(elem[1].nombre, filename.data());
													elem[1].tipo = 2;
												}
												else
												{
													if (tolower(ffd2.cFileName[y + 1]) == 'p' &&
														tolower(ffd2.cFileName[y + 2]) == 'n' &&
														tolower(ffd2.cFileName[y + 3]) == 'g' &&
														elem[2].existe == false)
													{
														elem[2].existe = true;
														strcpy(elem[2].nombre, filename.data());
														elem[2].tipo = 3;
													}
													else
													{
														if (tolower(ffd2.cFileName[y + 1]) == 't' &&
															tolower(ffd2.cFileName[y + 2]) == 'x' &&
															tolower(ffd2.cFileName[y + 3]) == 'd' &&
															elem[3].existe == false)
														{
															elem[3].existe = true;
															strcpy(elem[3].nombre, filename.data());
															elem[3].tipo = 4;
														}
													}
												}
											}
											filename.clear();
											break;
										}
									}
									filename.clear();
								}
							}
						}
						trajeconcreto.clear();
					} while (FindNextFile(hFind2, &ffd2) != 0);
					dwError = GetLastError();
					if (dwError != ERROR_NO_MORE_FILES)
					{

					}
					FindClose(hFind2);

					if (elem[2].existe == true || (elem[1].existe == true && elem[3].existe == true))
					{
						lacarpetaesvalida = true;
						if (strlen(elem[1].nombre) > 0)
						{
							strcpy(archivostrajes.nombre, elem[1].nombre);
						}
						else {
							if (strlen(elem[2].nombre) > 0)
							{
								strcpy(archivostrajes.nombre, elem[2].nombre);
							}
							else {
								if (strlen(elem[3].nombre) > 0) {
									strcpy(archivostrajes.nombre, elem[3].nombre);
								}
							}
						}

						strcpy(archivostrajes.nombrecarpeta, directoriosdetrajes[i].data());

						if (elem[1].existe == true)
						{
							archivostrajes.hasmodel = true;
							elem[1].existe = false;
						}
						if (elem[3].existe == true)
						{
							archivostrajes.hastexture = true;
							elem[3].existe = false;
						}

						if (elem[0].existe == true)
						{
							archivostrajes.hasini = true;
							elem[0].existe = false;
						}
						if (elem[2].existe == true)
						{
							archivostrajes.hasicon = true;
							elem[2].existe = false;
						}
					}
					else
					{
						lacarpetaesvalida = false;
						archivostrajes.hasicon = false;
						archivostrajes.hastexture = false;
						archivostrajes.hasmodel = false;
						archivostrajes.hasini = false;
						strcpy(archivostrajes.nombre, "");
						strcpy(archivostrajes.nombrecarpeta, "");
					}
					elem[0].existe = false;
					elem[1].existe = false;
					elem[2].existe = false;
					elem[3].existe = false;
					strcpy(elem[0].nombre, "");
					strcpy(elem[1].nombre, "");
					strcpy(elem[2].nombre, "");
					strcpy(elem[3].nombre, "");
					elem[0].tipo = 0;
					elem[1].tipo = 0;
					elem[2].tipo = 0;
					elem[3].tipo = 0;
				}
				
				int cant = 0;
				std::string szDir3;
				szDir3 = GAME_PATH("modloader\\IronManMod\\Skins");
				szDir3 += "\\";
				szDir3 += settings.folderdirs[ii].name;
				szDir3 += "\\";
				szDir3 += archivostrajes.nombrecarpeta; //subdirs recien encontrados
				szDir3 += "\\";
				szDir3 += archivostrajes.nombre;
				szDir3 += ".dat";


				if (lacarpetaesvalida == true)
				{
					settings.folderdirs[ii].suits[contadorcarpconelem].hasini = archivostrajes.hasini;
					settings.folderdirs[ii].suits[contadorcarpconelem].mdlexists = archivostrajes.hasmodel;
					settings.folderdirs[ii].suits[contadorcarpconelem].texexists = archivostrajes.hastexture;
					settings.folderdirs[ii].suits[contadorcarpconelem].iconexists = archivostrajes.hasicon;
					settings.folderdirs[ii].suits[contadorcarpconelem].id = contadorcarpconelem;
					if (archivostrajes.hasini == true)
					{

						plugin::config_file config0(szDir3);
						settings.folderdirs[ii].suits[contadorcarpconelem].rightfoot.x = config0["IDLErightfootX"].asFloat(0.165f);
						settings.folderdirs[ii].suits[contadorcarpconelem].rightfoot.y = config0["IDLErightfootY"].asFloat(-0.24f);
						settings.folderdirs[ii].suits[contadorcarpconelem].rightfoot.z = config0["IDLErightfootZ"].asFloat(-0.94f);
						settings.folderdirs[ii].suits[contadorcarpconelem].leftfoot.x = config0["IDLEleftfootX"].asFloat(-0.18f);
						settings.folderdirs[ii].suits[contadorcarpconelem].leftfoot.y = config0["IDLEleftfootY"].asFloat(-0.09f);
						settings.folderdirs[ii].suits[contadorcarpconelem].leftfoot.z = config0["IDLEleftfootZ"].asFloat(-0.91f);
						settings.folderdirs[ii].suits[contadorcarpconelem].righthand.x = config0["IDLErighthandX"].asFloat(0.375f);
						settings.folderdirs[ii].suits[contadorcarpconelem].righthand.y = config0["IDLErighthandY"].asFloat(-0.045f);
						settings.folderdirs[ii].suits[contadorcarpconelem].righthand.z = config0["IDLErighthandZ"].asFloat(0.0049f);
						settings.folderdirs[ii].suits[contadorcarpconelem].lefthand.x = config0["IDLElefthandX"].asFloat(-0.36f);
						settings.folderdirs[ii].suits[contadorcarpconelem].lefthand.y = config0["IDLElefthandY"].asFloat(-0.015f);
						settings.folderdirs[ii].suits[contadorcarpconelem].lefthand.z = config0["IDLElefthandZ"].asFloat(0.0349f);

						settings.folderdirs[ii].suits[contadorcarpconelem].flyrightfoot.x = config0["FLYrightfootX"].asFloat(0.12f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrightfoot.y = config0["FLYrightfootY"].asFloat(-1.05f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrightfoot.z = config0["FLYrightfootZ"].asFloat(0.1549f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyleftfoot.x = config0["FLYleftfootX"].asFloat(-0.105f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyleftfoot.y = config0["FLYleftfootY"].asFloat(-1.05f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyleftfoot.z = config0["FLYleftfootZ"].asFloat(0.1549f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrighthand.x = config0["FLYrighthandX"].asFloat(0.435f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrighthand.y = config0["FLYrighthandY"].asFloat(-0.06f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrighthand.z = config0["FLYrighthandZ"].asFloat(0.1399f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flylefthand.x = config0["FLYlefthandX"].asFloat(-0.435f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flylefthand.y = config0["FLYlefthandY"].asFloat(-0.015f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flylefthand.z = config0["FLYlefthandZ"].asFloat(0.1549f);
						
					}
					else
					{

						plugin::config_file config1(GAME_PATH("modloader\\IronManMod\\Skins\\GenericArmor.dat"));
						settings.folderdirs[ii].suits[contadorcarpconelem].rightfoot.x = config1["IDLErightfootX"].asFloat(0.165f);
						settings.folderdirs[ii].suits[contadorcarpconelem].rightfoot.y = config1["IDLErightfootY"].asFloat(-0.24f);
						settings.folderdirs[ii].suits[contadorcarpconelem].rightfoot.z = config1["IDLErightfootZ"].asFloat(-0.94f);
						settings.folderdirs[ii].suits[contadorcarpconelem].leftfoot.x = config1["IDLEleftfootX"].asFloat(-0.18f);
						settings.folderdirs[ii].suits[contadorcarpconelem].leftfoot.y = config1["IDLEleftfootY"].asFloat(-0.09f);
						settings.folderdirs[ii].suits[contadorcarpconelem].leftfoot.z = config1["IDLEleftfootZ"].asFloat(-0.91f);
						settings.folderdirs[ii].suits[contadorcarpconelem].righthand.x = config1["IDLErighthandX"].asFloat(0.375f);
						settings.folderdirs[ii].suits[contadorcarpconelem].righthand.y = config1["IDLErighthandY"].asFloat(-0.045f);
						settings.folderdirs[ii].suits[contadorcarpconelem].righthand.z = config1["IDLErighthandZ"].asFloat(0.0049f);
						settings.folderdirs[ii].suits[contadorcarpconelem].lefthand.x = config1["IDLElefthandX"].asFloat(-0.36f);
						settings.folderdirs[ii].suits[contadorcarpconelem].lefthand.y = config1["IDLElefthandY"].asFloat(-0.015f);
						settings.folderdirs[ii].suits[contadorcarpconelem].lefthand.z = config1["IDLElefthandZ"].asFloat(0.0349f);

						settings.folderdirs[ii].suits[contadorcarpconelem].flyrightfoot.x = config1["FLYrightfootX"].asFloat(0.12f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrightfoot.y = config1["FLYrightfootY"].asFloat(-1.05f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrightfoot.z = config1["FLYrightfootZ"].asFloat(0.1549f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyleftfoot.x = config1["FLYleftfootX"].asFloat(-0.105f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyleftfoot.y = config1["FLYleftfootY"].asFloat(-1.05f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyleftfoot.z = config1["FLYleftfootZ"].asFloat(0.1549f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrighthand.x = config1["FLYrighthandX"].asFloat(0.435f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrighthand.y = config1["FLYrighthandY"].asFloat(-0.06f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flyrighthand.z = config1["FLYrighthandZ"].asFloat(0.1399f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flylefthand.x = config1["FLYlefthandX"].asFloat(-0.435f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flylefthand.y = config1["FLYlefthandY"].asFloat(-0.015f);
						settings.folderdirs[ii].suits[contadorcarpconelem].flylefthand.z = config1["FLYlefthandZ"].asFloat(0.1549f);
						archivostrajes.hasini = true;
					}

					fflush(stdin);
					strcpy(settings.folderdirs[ii].suits[contadorcarpconelem].pngname, archivostrajes.nombre);
					if (strcmp(archivostrajes.nombre, "mark01") == 0)
					{
						boolvars.posironskinsfold = ii;
					}
					strcpy(settings.folderdirs[ii].suits[contadorcarpconelem].name, archivostrajes.nombrecarpeta);
					fflush(stdin);
					contadorcarpconelem++;
				}
				archivostrajes.hasicon = false;
				archivostrajes.hasini = false;
				archivostrajes.hasmodel = false;
				archivostrajes.hastexture = false;
				strcpy(archivostrajes.nombre, "");
				strcpy(archivostrajes.nombrecarpeta, "");
				szDir3.clear();
			}
			settings.folderdirs[ii].subcontentamnt = contadorcarpconelem;
		}
	}
	return true;
}