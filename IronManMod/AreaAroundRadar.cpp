#include <plugin.h>
#include "game_sa\CVector2D.h"
#include "game_sa\CSprite2d.h"

#include "Settings.h"
#include "game_sa\common.h"
#include "CFont.h"
#include "extensions\ScriptCommands.h"
#include "CText.h"
#include "CPed.h"
#include "game_sa\CStats.h"
#include "MobileTextBox.h"
#include <windows.h>
#include <math.h>
#include "CWorld.h"
#include "game_sa\CStats.h"
#include "MobileTextures.h"
#include "game_sa\CHud.h"
#include "Movtextures.h"

//static RwTexture *staticon;


using namespace plugin;

class AreaAroundRadar {
public:
	AreaAroundRadar() {
		Events::initRwEvent += [] {
			movtextures.readallhudtextures();
		};

		Events::drawHudEvent.after += [] {

			if (settings.healthdisplayed == true)
			{
				DrawPlaceData();
				movtextures.MyDrawWeaponIcon1();
				DrawCompass();
				settings.healthdisplayed = false;
			}
		};
		
		Events::drawingEvent.before += [] {
			boolvars.hudsetting = (plugin::patch::GetUChar(0xBA6769, false) == 1);
			getcharalt();
		};
	}

	static void getcharalt() {
		CPed *player = FindPlayerPed(0);
		if (player)
		{
			CVector coords;

			coords = player->GetPosition();
			CVector waterpos, playerpos;

			float waterh, grz;
			plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, coords.x, coords.y, coords.z, &grz);
			plugin::scripting::CallCommandById(COMMAND_GET_WATER_HEIGHT_AT_COORDS, coords.x, coords.y, 0, &waterh);
			if (waterh > grz)
			{
				grz = waterh;
			}
			grz -= coords.z;
			grz *= -1.0f;
			grz -= 1;
			coords.z *= 1.0f;
			if (600.0f > grz)
			{
				coords.z = grz;
			}
			boolvars.altitude = coords.z;
			/*
			char indice[3];
			sprintf(indice, "%d", boolvars.indx);
			CFont::SetBackground(0, 0);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetEdge(2);
			CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
			CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
			CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_CENTER_UP(0.0f), indice);
			*/
		}
	}


	static void DrawCompass()
	{
		static int hudradarcolor;
		if (settings.ironmanhud)
		{			
			if (!boolvars.systemerror && boolvars.hudsetting == true)
			{
				if (settings.marktraje > 1)
				{
					if (hudradarcolor == 0)
					{
						plugin::patch::SetInt(5792972, 0, 1);
						plugin::patch::SetInt(5793213, 0, 1);
						plugin::patch::SetInt(5793243, 0, 1);
						plugin::patch::SetInt(5793273, 0, 1);
						plugin::patch::SetInt(5793303, 0, 1);
						hudradarcolor = 1;
					}
					movtextures.drawindexedtexture(9);
				}
				else
				{
					if (hudradarcolor == 1)
					{
						plugin::patch::SetInt(5792972, 255, 1);
						plugin::patch::SetInt(5793213, 255, 1);
						plugin::patch::SetInt(5793243, 255, 1);
						plugin::patch::SetInt(5793273, 255, 1);
						plugin::patch::SetInt(5793303, 255, 1);

						hudradarcolor = 0;
					}
				}
			}
		}
		else
		{
			if (hudradarcolor == 1)
			{
				plugin::patch::SetInt(5792972, 255, 1);
				plugin::patch::SetInt(5793213, 255, 1);
				plugin::patch::SetInt(5793243, 255, 1);
				plugin::patch::SetInt(5793273, 255, 1);
				plugin::patch::SetInt(5793303, 255, 1);

				hudradarcolor = 0;
			}
		}
	}
	

	static void DrawPlaceData()
	{
		movtextures.drawindexedtexture(7);
	}

	
} areaaroundradar;

