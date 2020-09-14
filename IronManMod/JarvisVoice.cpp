#include "extensions\Screen.h"
#include "game_sa\CSprite2d.h"
#include "CFont.h"
#include "CTxdStore.h"
#include "CPed.h"
#include "MobileTextures.h"
#include "CText.h"
#include "JarvisVoice.h"
#include "CTimer.h"
#include "MMSystem.h"
#include "WinBase.h"
#include "extensions\ScriptCommands.h"
#include "CPad.h"
#include "CGeneral.h"
#include "CFileLoader.h"
#include "CSprite.h"
#include <windows.h>
#include <math.h>
#include "CWorld.h"
#include "Utility.h"
#include "CClothes.h"
#include <plugin.h>
#include "ePedState.h"
#include "game_sa/CGame.h"
#include "CWanted.h"
#include "CExplosion.h"
#include "Movtextures.h"

using namespace plugin;

BoolVars boolvars;
JarvisVoice jarvisvoice;
unsigned int armoralfa = 255;//100

CVector Getorientbetweenpoints(CVector origin, CVector dir);

CVector Getorientbetweenpoints(CVector origin, CVector dir) {
	static float head, fx;
	plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);
	float fy = dir.z - origin.z;

	float xangle = (CGeneral::GetATanOfXY(fy, fx));
	//plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, fx, fy, &xangle);

	float headx = dir.x - origin.x;
	float heady = dir.y - origin.y;
	plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
	CVector orient = { xangle, 2, head };
	return orient;
}


Taudiofile IMStream[3];
Taudiofile MenuSound;
Taudiofile JarvisSpeech;
Taudiofile JarvisMalarm;
Taudiofile beams;
Taudiofile audiostream[26];

CSprite2d *fondo;
CSprite2d *fondo1;


CSprite2d *check;
CSprite2d *uncheck;
CSprite2d *menusel;

static bool beginend;


void Playuniloop(int id);

void Playuniloop(int id)
{
	if (AudioLib.isset())
	{
		//if (AudioLib.Init(-1, 44100, 4, 0, 0) == NULL)
		//{
			if (AudioLib.ChannelIsActive(beams.audio) == 1)
			{
				if (id == 0)
				{
					AudioLib.ChannelStop(beams.audio);
				}
			}
			else
			{
				if (id == 1)
				{
					AudioLib.ChannelPlay(beams.audio, true);
				}
			}
		//}
	}
}

//std::string lng1;

CSprite2d *malarm = NULL;
CSprite2d *armoricom[85] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL 
,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL 
,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL 
,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL 
,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL 
,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL 
,NULL ,NULL ,NULL ,NULL ,NULL }; 
CSprite2d *imhud2;
CSprite2d *imhud3;
//CSprite2d *menu1;
CSprite2d *menu3;
CSprite2d *menu4;
CSprite2d *menu5;
CSprite2d *chosen;
CSprite2d *notchosen;
CSprite2d *closewdw;
CSprite2d *settingmenu;
//CSprite2d *settingmenuh;
CSprite2d *closebtn;
//CSprite2d *closebtnh;
CSprite2d *cursor;
CSprite2d *signo1;
CSprite2d *signo;
CSprite2d *marktony;

CSprite2d *wpmenu;
CSprite2d *wp1;
CSprite2d *wp2;
CSprite2d *wp3;
CSprite2d *wp4;
CSprite2d *wp5;

CSprite2d *hwp1;
CSprite2d *hwp2;
CSprite2d *hwp3;
CSprite2d *hwp4;
CSprite2d *hwp5;

bool JarvisVoice::isvulnerablecharinarea(CPed *charact, float posx, float posy, float posz, float radius)
{
	if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, charact, posx, posy, posz, radius, radius, radius, 0) == true && boolvars.iscalculating == false)
	{
		if (boolvars.isinvincible == false)
		{
			return true;
		}
	}
	return false;
}

void JarvisVoice::Loadsuiticons()
{
	//for (int ynde = 0; ynde < settings.amount; ynde++)
	//{			
	std::string iconPath;
	std::string numerodetraje;
	
	char *rutaimg;

	int hj = 1;
		while( hj < settings.folderdirs[boolvars.yndex].subcontentamnt)
		{
			fflush(stdin);
			//iconPath.reserve(524288);
			iconPath += GAME_PATH("modloader\\IronManMod\\Skins");
			iconPath += "\\";
			iconPath += settings.folderdirs[boolvars.yndex].name;
			iconPath += "\\";
			iconPath += settings.folderdirs[boolvars.yndex].suits[hj].name;
			iconPath += "\\";
			iconPath += settings.folderdirs[boolvars.yndex].suits[hj].pngname;

			iconPath += ".png";

			rutaimg = new char[iconPath.length() + 1];
			strcpy(rutaimg, iconPath.data());
			//fflush(stdin);
			
			FILE *file = fopen(rutaimg, "r");

			if (file != NULL) {
				fclose(file);
				armoricom[hj] = new CSprite2d();
				armoricom[hj]->m_pTexture = mobileTex.LoadTexture(rutaimg);
				fflush(stdin);
			}
			else
			{
				armoricom[hj] = new CSprite2d();
				armoricom[hj]->m_pTexture = NULL;
			}
			delete[] rutaimg;
			iconPath.clear();
			fflush(stdin);
			hj++;
		}
		if (settings.folderdirs[boolvars.yndex].subcontentamnt < 84)
		{
			for (int hj = settings.folderdirs[boolvars.yndex].subcontentamnt; hj < 84; hj++)
			{
				armoricom[hj] = new CSprite2d();
				armoricom[hj]->m_pTexture = NULL;
			}
		}
	//}
}

void JarvisVoice::storeenviroment(int* enviroment_ID) {
	CPed *player;
	player = FindPlayerPed(0);
	if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_IN_WATER, player)) {
		*enviroment_ID = 1;
	}
	else {
		*enviroment_ID = 0;
	}
}

void JarvisVoice::ManageMaxirpBass() {
	if (AudioLib.library==NULL)
	{
		AudioLib.Readfromdll();
		//JarvisVoice::LoadAudios();
		
		//AudioLib.Start();
		//AudioLib.Init(-1, 44100, 4, 0, 0);
		boolvars.bassinit = true;

	}
}
/*
void JarvisVoice::PauseBass() {
	if (AudioLib.library != NULL)
	{
		/*AudioLib.library = LoadLibrary(GAME_PATH("AudioLib.dll"));
		boolvars.bassinit = false;
		JarvisVoice::LoadAudios();
	}
	else
	{
		
		//AudioLib.Pause();
	}
}*/

static bool isentered, hastoloadfriend;

void JarvisVoice::IronManPatch() {
	jarvisvoice.ironmanpowers();
	//jarvisvoice.jarvisbotpowers();
}

bool JarvisVoice::notwastednotbusted() {
	if (plugin::scripting::CallCommandById(COMMAND_IS_PLAYER_PLAYING, 0) == true)
	{
		CPed *player;
		player = FindPlayerPed(0);
		if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, player) == false && plugin::scripting::CallCommandById(COMMAND_HAS_PLAYER_BEEN_ARRESTED, player) == false)
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

void DrawSuitClickIconAtCoords(bool available, bool noicon, int id, float texposx, float texposy, float texposx2, float texposy2, float mouseposx, float mouseposy, bool *ishovered);
bool DrawClickIconAtCoords(int chang, bool, CSprite2d *sprite, CSprite2d *spriteh, float texposx, float texposy, float texposx2, float texposy2, float mouseposx, float mouseposy);

void DrawSuitClickIconAtCoords(bool available, bool noicon, int id, float texposx, float texposy, float texposx2, float texposy2, float mouseposx, float mouseposy, bool *ishovered)
{
	if (boolvars.settingisactive == false)
	{
		if (id == 0)
		{
			marktony->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
			if (mouseposx >= texposx && mouseposx <= texposx2 && mouseposy >= texposy && mouseposy <= texposy2)
			{
				*ishovered = true;
				chosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
			}
			else
			{
				*ishovered = false;
				notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
			}
		}
		else
		{
			if (mouseposx >= texposx && mouseposx <= texposx2 && mouseposy >= texposy && mouseposy <= texposy2)
			{
				*ishovered = true;
				if (!available)
				{
					if (armoricom[id]->m_pTexture != NULL)
					{
						armoricom[id]->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
						CSprite2d::DrawRect(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 50));
					}
					else
					{
						signo->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					}
					notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
				}
				else
				{
					if (armoricom[id]->m_pTexture != NULL)
					{
						armoricom[id]->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					}
					else
					{
						signo1->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					}
					chosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
				}
			}
			else
			{
				*ishovered = false;

				if (!available)
				{
					if (armoricom[id]->m_pTexture != NULL)
					{
						armoricom[id]->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa)); 
						CSprite2d::DrawRect(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 50));
					}
					else
					{
						signo->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					}
					notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
				}
				else
				{
					if (armoricom[id]->m_pTexture != NULL)
					{
						armoricom[id]->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));

					}
					else
					{
						signo1->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					}
					notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
				}
			}
		}
	}
	else
	{
		if (id == 0)
		{
			marktony->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
			*ishovered = false;
			notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
		}
		else
		{
			*ishovered = false;

			if (!available)
			{
				if (armoricom[id]->m_pTexture != NULL)
				{
					armoricom[id]->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					CSprite2d::DrawRect(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 50));
				}
				else
				{
					signo->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
				}
				notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
			}
			else
			{
				if (armoricom[id]->m_pTexture != NULL)
				{
					armoricom[id]->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));

				}
				else
				{
					signo1->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
				}
				notchosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
			}
		}
	}
	fflush(stdin);
}

bool DrawClickIconAtCoords(int chang, bool setingcheck, CSprite2d *sprite, CSprite2d *spriteh, float texposx, float texposy, float texposx2, float texposy2, float mouseposx, float mouseposy)
{
	fflush(stdin);
	if (chang == 0)
	{
		if (boolvars.settingisactive == false || setingcheck == false)
		{
			if (texposx < texposx2)
			{
				if (mouseposx >= texposx && mouseposx <= texposx2 && mouseposy >= texposy && mouseposy <= texposy2)
				{
					sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					chosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					return true;
				}
				else
				{
					sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					return false;
				}
			}
			else
			{
				if (mouseposx >= texposx2 && mouseposx <= texposx && mouseposy >= texposy && mouseposy <= texposy2)
				{
					sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					chosen->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					return true;
				}
				else
				{
					sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					return false;
				}
			}
		}
		else
		{
			sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
			return false;
		}
	}
	else
	{
		if (boolvars.settingisactive == false || setingcheck == false)
		{
			if (texposx < texposx2)
			{
				if (mouseposx >= texposx && mouseposx <= texposx2 && mouseposy >= texposy && mouseposy <= texposy2)
				{
					spriteh->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					return true;
				}
				else
				{
					sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					return false;
				}
			}
			else
			{
				if (mouseposx >= texposx2 && mouseposx <= texposx && mouseposy >= texposy && mouseposy <= texposy2)
				{
					spriteh->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, 255));
					return true;
				}
				else
				{
					sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
					return false;
				}
			}
		}
		else
		{
			sprite->Draw(CRect(texposx, texposy, texposx2, texposy2), CRGBA(255, 255, 255, armoralfa));
			return false;
		}
	}
}

void Drawfondo(int alfa);

void Drawfondo(int alfa)
{
	if (alfa > 255)
	{
		alfa = 255;
	}

	if (alfa < 0)
	{
		alfa = 0;
	}
	
	static int alfa2;

	static bool strt1;
	static bool end1;
	if (beginend == true)
	{
		fondo->Draw(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(255, 255, 255, alfa));
		if (alfa > 150 || end1 == true) {
			if (alfa2 < 240) {
				fondo1->Draw(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(255, 255, 255, alfa2));
				alfa2 += 20;
				end1 = true;
			}
			else
			{
				alfa2 = 255;
				fondo1->Draw(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(255, 255, 255, alfa2));

				strt1 = true;
			}
		}

		if (strt1 == true)
		{
			if (alfa2 >= 25)
			{
				fondo1->Draw(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(255, 255, 255, alfa2));
				alfa2 -= 25;
				end1 = false;
			}
			else
			{
				alfa2 = 0;
				strt1 = false;
			}

		}
	}
	else
	{
		CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)alfa));
	}
	
}

bool JarvisVoice::LoadAudios() {
	if (AudioLib.isset())
	{
		//DWORD AudioLib.DEVICE_3D = NULL;//(DWORD)GetProcAddress(AudioLib.library, "AudioLib.DEVICE_3D");
		//DWORD AudioLib.SAMPLE_3D = NULL;//(DWORD)GetProcAddress(AudioLib.library, "AudioLib.SAMPLE_3D");
		BASS_3DVECTOR pos(0, 0, 0), vel(0, 0, 0), front(0, -1, 0), top(0, 0, 1);

		if (//aquivalibinit
			AudioLib.Set3DFactors(1.0f, 0.3f, 1.0f) &&
			AudioLib.Set3DPosition(&pos, &vel, &front, &top))
		{
			if (AudioLib.StreamCreateFile != NULL)
			{
				fflush(stdin);
				beams.audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\uniloop.mp3"), 0, 0, 12);
				beams.is3d = true;
				beams.ispaused = false;
				audiostream[0].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\dart.mp3"), 0, 0, 8);
				audiostream[0].is3d = true;
				audiostream[1].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\repulsorStart.mp3"), 0, 0, 8);
				audiostream[1].is3d = true;
				audiostream[2].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\repulsorBlast.mp3"), 0, 0, 8);
				audiostream[2].is3d = true;
				audiostream[3].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\jet_soundFlying.mp3"), 0, 0, 8);
				audiostream[3].is3d = true; 
				audiostream[4].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\fireBullets.mp3"), 0, 0, 8);
				audiostream[4].is3d = true;
				audiostream[5].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\launchM.mp3"), 0, 0, 8);
				audiostream[5].is3d = true; 
				audiostream[6].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\hover_thrusters.mp3"), 0, 0, 12);
				audiostream[6].is3d = true; 
				audiostream[7].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\underwater_thrusters.mp3"), 0, 0, 12);
				audiostream[7].is3d = true; 
				audiostream[8].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\flying_thrusters.mp3"), 0, 0, 12);
				audiostream[8].is3d = true; 
				audiostream[9].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\takeoff.mp3"), 0, 0, 8);
				audiostream[9].is3d = true; 
				audiostream[10].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\flyfast.mp3"), 0, 0, 8);
				audiostream[10].is3d = true; 
				audiostream[11].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\targetSet.wav"), 0, 0, 0);
				audiostream[12].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\landing.mp3"), 0, 0, 8);
				audiostream[12].is3d = true;
				audiostream[13].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\flystop.mp3"), 0, 0, 8);
				audiostream[13].is3d = true; 
				audiostream[14].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\unibeam.mp3"), 0, 0, 8);
				audiostream[14].is3d = true; 
				audiostream[15].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\uniaim.mp3"), 0, 0, 8);
				audiostream[15].is3d = true;
				audiostream[16].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\alarm.mp3"), 0, 0, 0);
				audiostream[17].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\dcareful.mp3"), 0, 0, 0);
				audiostream[18].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\dwarning.mp3"), 0, 0, 0);
				audiostream[19].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\online.mp3"), 0, 0, 0);
				audiostream[20].audio = audiostream[16 + rand() % 3].audio;
				audiostream[21].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\enemyAI.mp3"), 0, 0, 0);
				audiostream[22].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\Well_done.wav"), 0, 0, 0);
				audiostream[23].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\fallstop.mp3"), 0, 0, 8);
				audiostream[23].is3d = true; 
				audiostream[24].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\beep.wav"), 0, 0, 0);
				audiostream[25].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\j_warning2.wav"), 0, 0, 0);
				AudioLib.MenuStream[0].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\hover.mp3"), 0, 0, 0);
				AudioLib.MenuStream[1].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\clic.mp3"), 0, 0, 0);
				AudioLib.MenuStream[2].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\choose01.mp3"), 0, 0, 0);
				AudioLib.MenuStream[3].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\choose03.mp3"), 0, 0, 0);
				AudioLib.MenuStream[4].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\choose04.mp3"), 0, 0, 0);
				AudioLib.MenuStream[5].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\choose02.mp3"), 0, 0, 0);
				AudioLib.MenuStream[6].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\choose05.mp3"), 0, 0, 0);
				fflush(stdin);
				for (int i = 0; i < 26; i++)
				{
					if (audiostream[i].is3d)
					{
						AudioLib.ChannelSet3DAttributes(audiostream[i].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
						AudioLib.Apply3D();
						AudioLib.ChannelSetAttribute(audiostream[i].audio, 2, boolvars.gamevolume);
					}
					else
					{
						AudioLib.ChannelSetAttribute(audiostream[i].audio, 2, boolvars.gamevolume);
					}
				}
				AudioLib.ChannelSet3DAttributes(beams.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
				AudioLib.Apply3D();
				AudioLib.ChannelSetAttribute(beams.audio, 2, boolvars.gamevolume);
				return true;
			}
			else
			{
				//CFont::SetBackground(0, 0);
				//CFont::SetOrientation(ALIGN_CENTER);
				//CFont::SetProportional(true);
				//CFont::SetJustify(false);
				//CFont::SetColor(CRGBA(255, 255, 255, 255));
				//CFont::SetFontStyle(FONT_SUBTITLES);
				//CFont::SetEdge(2);
				//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
				//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
				//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
				//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass stream create file failed");
				return false;
			}
		}
		else
		{
			//CFont::SetBackground(0, 0);
			//CFont::SetOrientation(ALIGN_CENTER);
			//CFont::SetProportional(true);
			//CFont::SetJustify(false);
			//CFont::SetColor(CRGBA(255, 255, 255, 255));
			//CFont::SetFontStyle(FONT_SUBTITLES);
			//CFont::SetEdge(2);
			//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
			//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
			//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
			//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass init failed");
			return false;
		}
	}
	else
	{
		//CFont::SetBackground(0, 0);
		//CFont::SetOrientation(ALIGN_CENTER);
		//CFont::SetProportional(true);
		//CFont::SetJustify(false);
		//CFont::SetColor(CRGBA(255, 255, 255, 255));
		//CFont::SetFontStyle(FONT_SUBTITLES);
		//CFont::SetEdge(2);
		//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
		//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
		//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
		//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass failed");
		return false;
	}
}

void JarvisVoice::unloadaudios() {
	FreeLibrary(AudioLib.library);
}

void JarvisVoice::PlayAudiostream(Taudiofile audiostreams, int user, Taudiofile *MP3Stream) {
	if (AudioLib.isset())
	{
		

		
			
		
			if (user == 0)
			{
				if (audiostreams.audio == audiostream[20].audio)
				{
					int ran = 16 + rand() % 3;
					MP3Stream->audio = audiostream[ran].audio;
					MP3Stream->is3d = audiostream[ran].is3d;
				}
				else
				{
					MP3Stream->audio = audiostreams.audio;
					MP3Stream->is3d = audiostreams.is3d;
				}
				
			}
			else
			{
				MP3Stream->audio = audiostreams.audio;
				MP3Stream->is3d = audiostreams.is3d;
			}
			if (MP3Stream->audio != NULL) {
				
				
				if (AudioLib.ChannelPlay != false) {
					AudioLib.ChannelPlay(MP3Stream->audio, false);
				}
								
				if (MP3Stream->is3d)
				{
					AudioLib.ChannelSet3DAttributes(MP3Stream->audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
				}

			}
		
	}
}

void JarvisVoice::PlayShot(Taudiofile *MP3Stream) {
	static int timer;
	if (AudioLib.isset())
	{
		 
		
		if (CTimer::m_snTimeInMillisecondsNonClipped > (timer + 10))//aquivalibinit
		{
			timer = 0;
			MP3Stream->audio = audiostream[fireBullets].audio;
			MP3Stream->is3d = audiostream[fireBullets].is3d;
			
			if (MP3Stream->audio != NULL) {
				
				AudioLib.ChannelPlay(MP3Stream->audio, true);

				if (MP3Stream->is3d)
				{
					AudioLib.ChannelSet3DAttributes(MP3Stream->audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
				}

			}
		}
		else
		{
			timer=CTimer::m_snTimeInMillisecondsNonClipped;
		}
	}
}


void AUDIOLIB::PlayMenuSFX(int id) {
	if (AudioLib.isset())
	{
		
		 
		//aquivalibinit
		//if (AudioLib.Init(-1, 44100, 4, 0, 0) == NULL)
		//{
			
			MenuSound.audio = AudioLib.MenuStream[id].audio;
			AudioLib.ChannelPlay(MenuSound.audio, true);

			if (MenuSound.is3d)
			{
				AudioLib.ChannelSet3DAttributes(MenuSound.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
				AudioLib.Apply3D();
				AudioLib.ChannelSetAttribute(MenuSound.audio, 2, boolvars.gamevolume);
			}
			else
			{
				AudioLib.ChannelSetAttribute(MenuSound.audio, 2, boolvars.gamevolume);
			}
		//}
	}
}

void JarvisVoice::PlayBeep(Taudiofile *MP3Stream) {
	static int timer;
	if (AudioLib.isset())
	{
		//aquivalibinit
		if (CTimer::m_snTimeInMillisecondsNonClipped > (timer + 150))//AudioLib.Init(-1, 44100, 4, 0, 0) == NULL && 
		{
			timer = CTimer::m_snTimeInMillisecondsNonClipped;
			MP3Stream->audio = audiostream[24].audio;
			if (MP3Stream->audio != NULL) {
				
				AudioLib.ChannelPlay(MP3Stream->audio, true);
				
				if (MP3Stream->is3d)
				{
					AudioLib.ChannelSet3DAttributes(MP3Stream->audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
				}
			}
		}
	}
}

void JarvisVoice::PauseAllStreams() {	
	
	if (AudioLib.isset())
	{
		JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
		if (propellers.manoizqfx != 0)
		{
			kill_thrusters(&propellers);
		}
		Playuniloop(0);
		int i = 0;
		while (i < dimfriends)
		{
			if ((int)jarvisfriend[i].actorchar != NULL)
			{
				if (jarvisfriend[i].jarvisprop.manoizqfx > 0)
				{
					kill_thrusters(&jarvisfriend[i].jarvisprop);
				}
			}
			for (int j = 0; j < 3; j++)
			{
				if (AudioLib.ChannelIsActive(jarvisfriend[i].botstream[j].audio) == 1)
				{
					AudioLib.ChannelPause(jarvisfriend[i].botstream[j].audio);
					jarvisfriend[i].botstream[j].ispaused = true;
				}
			}
			i++;
		}
		i = 0;
		while (i < dimenemy)
		{
			if ((int)jarvisenemy[i].actorchar != NULL)
			{
				if (jarvisenemy[i].jarvisprop.manoizqfx > 0)
				{
					kill_thrusters(&jarvisenemy[i].jarvisprop);
				}
			}
			for (int j = 0; j < 3; j++)
			{
				if (AudioLib.ChannelIsActive(jarvisenemy[i].botstream[j].audio) == 1)
				{
					AudioLib.ChannelPause(jarvisenemy[i].botstream[j].audio);
					jarvisenemy[i].botstream[j].ispaused = true;
				}
			}
			i++;
		}
		if (AudioLib.ChannelIsActive(IMStream[0].audio) == 1)
		{
			AudioLib.ChannelPause(IMStream[0].audio);
			IMStream[0].audio = NULL;
			IMStream[0].ispaused = true;
		}
		if (AudioLib.ChannelIsActive(IMStream[1].audio) == 1)
		{
			AudioLib.ChannelPause(IMStream[1].audio);
			IMStream[1].audio = NULL;
			IMStream[1].ispaused = true;
		}
		if (AudioLib.ChannelIsActive(JarvisSpeech.audio) == 1)
		{
			AudioLib.ChannelPause(JarvisSpeech.audio);
			JarvisSpeech.audio = NULL;
			JarvisSpeech.ispaused = true;
		}
		if (AudioLib.ChannelIsActive(JarvisMalarm.audio) == 1)
		{
			AudioLib.ChannelPause(JarvisMalarm.audio);
			JarvisMalarm.audio = NULL;
			JarvisMalarm.ispaused = true;
		}
		if (AudioLib.ChannelIsActive(IMStream[2].audio) == 1)
		{
			AudioLib.ChannelPause(IMStream[2].audio);
			IMStream[2].audio = NULL;
			IMStream[2].ispaused = true;
		}
		if (AudioLib.ChannelIsActive(MenuSound.audio) == 1)
		{
			AudioLib.ChannelPause(MenuSound.audio);
			MenuSound.audio = NULL;
			MenuSound.ispaused = true;
		}
	}
}

void JarvisVoice::ResumeAllStreams()
{
	if (AudioLib.isset())
	{
		if (AudioLib.ChannelPlay != false) {

			if (IMStream[0].ispaused == true) {
				AudioLib.ChannelPlay(IMStream[0].audio, false);
				IMStream[0].ispaused = false;

				if (IMStream[0].is3d)
				{
					AudioLib.ChannelSet3DAttributes(IMStream[0].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(IMStream[0].audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(IMStream[0].audio, 2, boolvars.gamevolume);
				}
			}
			if (IMStream[1].ispaused == true) {
				AudioLib.ChannelPlay(IMStream[1].audio, false);
				IMStream[1].ispaused = false;
				if (IMStream[1].is3d)
				{
					AudioLib.ChannelSet3DAttributes(IMStream[1].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(IMStream[1].audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(IMStream[1].audio, 2, boolvars.gamevolume);
				}
			}
			if (JarvisSpeech.ispaused == true) {
				AudioLib.ChannelPlay(JarvisSpeech.audio, false);
				JarvisSpeech.ispaused = false;
				if (JarvisSpeech.is3d)
				{
					AudioLib.ChannelSet3DAttributes(JarvisSpeech.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(JarvisSpeech.audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(JarvisSpeech.audio, 2, boolvars.gamevolume);
				}
			}
			if (JarvisMalarm.ispaused == true) {
				AudioLib.ChannelPlay(JarvisMalarm.audio, false);
				JarvisMalarm.ispaused = false;
				if (JarvisMalarm.is3d)
				{
					AudioLib.ChannelSet3DAttributes(JarvisMalarm.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(JarvisMalarm.audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(JarvisMalarm.audio, 2, boolvars.gamevolume);
				}
			}
			if (IMStream[2].ispaused == true) {
				AudioLib.ChannelPlay(IMStream[2].audio, false);
				IMStream[2].ispaused = false;
				if (IMStream[2].is3d)
				{
					AudioLib.ChannelSet3DAttributes(IMStream[2].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(IMStream[2].audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(IMStream[2].audio, 2, boolvars.gamevolume);
				}
			}
			if (MenuSound.ispaused == true) {
				AudioLib.ChannelPlay(MenuSound.audio, false);
				MenuSound.ispaused = false;
				if (MenuSound.is3d)
				{
					AudioLib.ChannelSet3DAttributes(MenuSound.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(MenuSound.audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(MenuSound.audio, 2, boolvars.gamevolume);
				}
			}

			int i = 0;
			while (i < dimfriends)
			{
				for (int j = 0; j < 3; j++)
				{
					if (jarvisfriend[i].botstream[j].ispaused == true)
					{
						AudioLib.ChannelPlay(jarvisfriend[i].botstream[j].audio, false);
						jarvisfriend[i].botstream[j].ispaused = false;
						if (jarvisfriend[i].botstream[j].is3d)
						{
							AudioLib.ChannelSet3DAttributes(jarvisfriend[i].botstream[j].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
							AudioLib.Apply3D();
							AudioLib.ChannelSetAttribute(jarvisfriend[i].botstream[j].audio, 2, boolvars.gamevolume);
						}
						else
						{
							AudioLib.ChannelSetAttribute(jarvisfriend[i].botstream[j].audio, 2, boolvars.gamevolume);
						}
					}
				}
				i++;
			}
			i = 0;
			while (i < dimenemy)
			{
				for (int j = 0; j < 3; j++)
				{
					if (jarvisenemy[i].botstream[j].ispaused == true)
					{
						AudioLib.ChannelPlay(jarvisenemy[i].botstream[j].audio, false);
						jarvisenemy[i].botstream[j].ispaused = false;
						if (jarvisenemy[i].botstream[j].is3d)
						{
							AudioLib.ChannelSet3DAttributes(jarvisenemy[i].botstream[j].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
							AudioLib.Apply3D();
							AudioLib.ChannelSetAttribute(jarvisenemy[i].botstream[j].audio, 2, boolvars.gamevolume);
						}
						else
						{
							AudioLib.ChannelSetAttribute(jarvisenemy[i].botstream[j].audio, 2, boolvars.gamevolume);
						}
					}
				}
				i++;
			}
		}
	}
}

void AUDIOLIB::PlayVoiceEvent(int id) {
	if (AudioLib.isset())
	{
		
		if (AudioLib.ChannelIsActive(JarvisSpeech.audio) != 1)
		{
			//aquivalibinit
			//if (AudioLib.Init(-1, 44100, 4, 0, 0) == NULL)
			//{
				if (id == 20)
				{
					int ran = 16 + rand() % 3;
					JarvisSpeech.audio = audiostream[ran].audio;
					JarvisSpeech.is3d = audiostream[ran].is3d;
				}
				else
				{
					JarvisSpeech.audio = audiostream[id].audio;
					JarvisSpeech.is3d = audiostream[id].is3d;
				}
				
				if (JarvisSpeech.audio != NULL) {
					AudioLib.ChannelPlay(JarvisSpeech.audio, false);
					if (JarvisSpeech.is3d)
					{
						AudioLib.ChannelSet3DAttributes(JarvisSpeech.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
						AudioLib.Apply3D();
						AudioLib.ChannelSetAttribute(JarvisSpeech.audio, 2, boolvars.gamevolume);
					}
					else
					{
						AudioLib.ChannelSetAttribute(JarvisSpeech.audio, 2, boolvars.gamevolume);
					}
				}
			//}
		}
	}
}

bool JarvisVoice::IsAudioStreamPlayed(Taudiofile MP3Stream, Taudiofile audiostreams) {
	if (AudioLib.isset())
	{
		if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1 && MP3Stream.audio == audiostreams.audio)// audiostream[id])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		return false;
	}
}

bool JarvisVoice::IsAudioVoicePlayed(int id) {
	if (AudioLib.isset())
	{
		if (AudioLib.ChannelIsActive(JarvisSpeech.audio) == 1 && JarvisSpeech.audio == audiostream[id].audio)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		return false;
	}
}

void JarvisVoice::Updateaudiopositions()
{
	BASS_3DVECTOR pos, posaim, posaim2, oriplay;
	BASS_3DVECTOR poscam;
	CMatrix camera;
	boolvars.gamevolume = plugin::patch::GetFloat(0xB5FCCC, false);
	camera = TheCamera.m_mCameraMatrix;
	CPed *player = FindPlayerPed(0);
	if (AudioLib.isset())
	{

		if (AudioLib.ChannelSet3DPosition != NULL && AudioLib.Apply3D != NULL && AudioLib.ChannelSetAttribute != NULL)
		{
			if (AudioLib.ChannelIsActive(JarvisSpeech.audio) == 1)
			{
				if (JarvisSpeech.is3d)
				{
					AudioLib.ChannelSet3DAttributes(JarvisSpeech.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(JarvisSpeech.audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(JarvisSpeech.audio, 2, boolvars.gamevolume);
				}
			}
			if (AudioLib.ChannelIsActive(JarvisMalarm.audio) == 1)
			{
				if (JarvisMalarm.is3d)
				{
					AudioLib.ChannelSet3DAttributes(JarvisMalarm.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
					AudioLib.Apply3D();
					AudioLib.ChannelSetAttribute(JarvisMalarm.audio, 2, boolvars.gamevolume);
				}
				else
				{
					AudioLib.ChannelSetAttribute(JarvisMalarm.audio, 2, boolvars.gamevolume);
				}
			}


			CVector playerpos;
			//CVector campos;// campos1;
			if (player)
			{
				//playerpos = player->GetPosition();
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, player, &playerpos.x, &playerpos.y, &playerpos.z);
			}
			//plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, player, &playerpos.x, &playerpos.y, &playerpos.z);
			//plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &campos.x, &campos.y, &campos.z);
			//plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &campos1.x, &campos1.y, &campos1.z);
			//campos.x -= campos1.x;
			//campos.y -= campos1.y;
			//campos.z -= campos1.z;

			CVector camcoors, camaimpoint8;// caposcalc; unit, camvec,
			//static float disttotal, magnus1;
			plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &camcoors.x, &camcoors.y, &camcoors.z);
			plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &camaimpoint8.x, &camaimpoint8.y, &camaimpoint8.z);
			//CVector camaimpoint = TheCamera.m_vecAimingTargetCoors;
			//camcoors = TheCamera.GetPosition();
			//TheCamera.GetOrientation(camaimpoint8.x, camaimpoint8.y, camaimpoint8.z);

			//TheCamera.Find3rdPersonCamTargetVector(10, camcoors, &camvec, &camaimpoint8);
			/*
			plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, camvec.x, camvec.y, camvec.z, playerpos.x, playerpos.y, playerpos.z, &disttotal);
			plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, camcoors.x, camcoors.y, camcoors.z, playerpos.x, playerpos.y, playerpos.z, &magnus1);
			*/
			/*
			static float distx;
			plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, camcoors.x, camcoors.y, playerpos.x, playerpos.y, &distx);

			float disty = playerpos.z - camcoors.z;

			float xangle = (CGeneral::GetATanOfXY(disty, distx));
			float yangle = 0.0f;
			static float zangle;
			float headx = playerpos.x - camcoors.x;
			float heady = playerpos.y - camcoors.y;
			plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &zangle);

			CVector2D newdirectionx, newdirectionz;
			CVector hipotenusas;

			newdirectionz.x = cosf(zangle);
			newdirectionz.y = sinf(zangle);

			newdirectionx.x = cosf(xangle);
			newdirectionx.y = sinf(xangle);

			hipotenusas.y =
			*/





			CVector orientplay = Getorientbetweenpoints(playerpos, camcoors);
			oriplay.x = orientplay.x;
			oriplay.y = orientplay.y;
			oriplay.z = orientplay.z;
			/*
			float headx = playerpos.x - camcoors.x;
			float heady = playerpos.y - camcoors.y;
			plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &oriplay.z);
			*/

			/*

			unit.x = (camvec.x - playerpos.x);
			unit.y = (camvec.y - playerpos.y);
			unit.z = (camvec.z - playerpos.z);

			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, -unit.x, -unit.y, -unit.z, &caposcalc.x, &caposcalc.y, &caposcalc.z);

			poscam.x = caposcalc.x;
			poscam.y = caposcalc.y;
			poscam.z = caposcalc.z;*/
			/*
			posaim.x = camaimpoint8.x;
			posaim.y = camaimpoint8.y;
			posaim.z = camaimpoint8.z;
			*/
			//CVector angles = TheCamera.m_vecFixedModeUpOffSet;
			/*CVector campos = TheCamera.GetPosition();*/
			poscam.x = camcoors.x; //camaimpoint8.x - camcoors.x;
			poscam.y = camcoors.y; //camaimpoint8.y - camcoors.y;
			poscam.z = camcoors.z; //camaimpoint8.z - camcoors.z;
			//CVector offset = { camaimpoint8.x - caposcalc.x ,camaimpoint8.y - caposcalc.y, camaimpoint8.z - caposcalc.z };

			//plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, offset.y, offset.z);


			posaim2.x = camera.up.x;
			posaim2.y = camera.up.y;
			posaim2.z = camera.up.z;
			//CVector angles = Getorientbetweenpoints(camcoors, camaimpoint8);
			posaim.x = camaimpoint8.x;
			posaim.y = camaimpoint8.y;
			posaim.z = camaimpoint8.z;
			//TheCamera.GetOrientation(angles.x, angles.y, angles.z);
			//float ang1 = angles.x + 90.0f;

			AudioLib.Set3DPosition(&poscam, nullptr, &posaim, &posaim2);
			AudioLib.Apply3D();

			pos.x = playerpos.x;
			pos.y = playerpos.y;
			pos.z = playerpos.z;


			for (int i = 0; i < 3; i++)
			{
				if (IMStream[i].audio != NULL)
				{
					if (IMStream[i].is3d)
					{
						AudioLib.ChannelSet3DPosition(IMStream[i].audio, &pos, &oriplay, NULL);
						AudioLib.Apply3D();
						AudioLib.ChannelSet3DAttributes(IMStream[i].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
						AudioLib.Apply3D();
						AudioLib.ChannelSetAttribute(IMStream[i].audio, 2, boolvars.gamevolume);
					}
					else
					{
						AudioLib.ChannelSetAttribute(IMStream[i].audio, 2, boolvars.gamevolume);
					}
				}
			}

			AudioLib.ChannelSet3DPosition(beams.audio, &pos, &oriplay, NULL);
			AudioLib.Apply3D();
			AudioLib.ChannelSet3DAttributes(beams.audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
			AudioLib.Apply3D();
			AudioLib.ChannelSetAttribute(beams.audio, 2, boolvars.gamevolume);

			if (dimfriends > 0)
			{
				for (int i = 0; i < dimfriends; i++)
				{
					if ((int)jarvisfriend[i].actorchar != NULL)
					{
						if (AudioLib.StreamCreateFile != NULL)// && AudioLib.ChannelSet3DAttributes != NULL)
						{
							if (jarvisfriend[i].audios[0].audio == NULL) {
								jarvisfriend[i].audios[0].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\hover_thrusters.mp3"), 0, 0, 12);
								jarvisfriend[i].audios[0].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[0], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisfriend[i].audios[1].audio == NULL) {
								jarvisfriend[i].audios[1].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\flying_thrusters.mp3"), 0, 0, 12);
								jarvisfriend[i].audios[1].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[1], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisfriend[i].audios[2].audio == NULL) {
								jarvisfriend[i].audios[2].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\fireBullets.mp3"), 0, 0, 8);
								jarvisfriend[i].audios[2].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[2], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisfriend[i].audios[3].audio == NULL) {
								jarvisfriend[i].audios[3].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\repulsorBlast.mp3"), 0, 0, 8);
								jarvisfriend[i].audios[3].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[3], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisfriend[i].audios[4].audio == NULL) {
								jarvisfriend[i].audios[4].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\launchM.mp3"), 0, 0, 8);
								jarvisfriend[i].audios[4].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[4], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisfriend[i].audios[5].audio == NULL) {
								jarvisfriend[i].audios[5].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\underwater_thrusters.mp3"), 0, 0, 12);
								jarvisfriend[i].audios[5].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[5], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisfriend[i].audios[6].audio == NULL) {
								jarvisfriend[i].audios[6].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\at_your_service.mp3"), 0, 0, 8);
								jarvisfriend[i].audios[6].is3d = true;

								jarvisfriend[i].botstream[2].audio = jarvisfriend[i].audios[6].audio;

								AudioLib.ChannelSet3DAttributes(jarvisfriend[i].botstream[2].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
								AudioLib.Apply3D();
								AudioLib.ChannelSetAttribute(jarvisfriend[i].botstream[2].audio, 2, boolvars.gamevolume);
								//poner volumen

								//aquivalibinit
								//AudioLib.Init(-1, 44100, 4, 0, 0);
								AudioLib.ChannelPlay(jarvisfriend[i].botstream[2].audio, false);
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[5], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							for (int j = 0; j < 3; j++)
							{
								if (jarvisfriend[i].botstream[j].audio != NULL) {
									CVector playerpos1;// = jarvisfriend[i].actorchar->GetPosition();
									plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, jarvisfriend[i].actorchar, &playerpos1.x, &playerpos1.y, &playerpos1.z);

									if (jarvisfriend[i].botstream[j].is3d)
									{
										AudioLib.ChannelSet3DAttributes(jarvisfriend[i].botstream[j].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
										AudioLib.Apply3D();
										AudioLib.ChannelSetAttribute(jarvisfriend[i].botstream[j].audio, 2, boolvars.gamevolume);
									}
									else
									{
										AudioLib.ChannelSetAttribute(jarvisfriend[i].botstream[j].audio, 2, boolvars.gamevolume);
									}
									//poner volumen
									BASS_3DVECTOR pos1;
									pos1.x = playerpos1.x;
									pos1.y = playerpos1.y;
									pos1.z = playerpos1.z;

									CVector angles = Getorientbetweenpoints(playerpos1, camera.pos);
									posaim.x = angles.x;
									posaim.y = angles.y;
									posaim.z = angles.z;

									AudioLib.ChannelSet3DPosition(jarvisfriend[i].botstream[j].audio, &pos1, &posaim, NULL);
									AudioLib.Apply3D();
								}
							}
						}
					}
				}
			}
			if (dimenemy > 0)
			{
				for (int i = 0; i < dimenemy; i++)
				{
					if ((int)jarvisenemy[i].actorchar != NULL)
					{
						if (AudioLib.StreamCreateFile != NULL)// && AudioLib.ChannelSet3DAttributes != NULL)
						{
							if (jarvisenemy[i].audios[0].audio == NULL) {
								jarvisenemy[i].audios[0].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\hover_thrusters.mp3"), 0, 0, 12);
								jarvisenemy[i].audios[0].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisenemy[i].audios[0], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisenemy[i].audios[1].audio == NULL) {
								jarvisenemy[i].audios[1].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\flying_thrusters.mp3"), 0, 0, 12);
								jarvisenemy[i].audios[1].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisenemy[i].audios[1], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisenemy[i].audios[2].audio == NULL) {
								jarvisenemy[i].audios[2].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\fireBullets.mp3"), 0, 0, 8);
								jarvisenemy[i].audios[2].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisenemy[i].audios[2], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisenemy[i].audios[3].audio == NULL) {
								jarvisenemy[i].audios[3].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\repulsorBlast.mp3"), 0, 0, 8);
								jarvisenemy[i].audios[3].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisenemy[i].audios[3], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisenemy[i].audios[4].audio == NULL) {
								jarvisenemy[i].audios[4].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\launchM.mp3"), 0, 0, 8);
								jarvisenemy[i].audios[4].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisenemy[i].audios[4], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisenemy[i].audios[5].audio == NULL) {
								jarvisenemy[i].audios[5].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\SFX\\underwater_thrusters.mp3"), 0, 0, 12);
								jarvisenemy[i].audios[5].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisenemy[i].audios[5], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							if (jarvisenemy[i].audios[6].audio == NULL) {
								jarvisenemy[i].audios[6].audio = AudioLib.StreamCreateFile(false, PLUGIN_PATH("IronMan\\sound\\Jarvis\\at_your_service.mp3"), 0, 0, 8);
								jarvisenemy[i].audios[6].is3d = true;
								//AudioLib.ChannelSet3DAttributes(jarvisfriend[i].audios[5], 2, 0.0f, 30.0f, 360, 360, -1);
								//AudioLib.Apply3D();
							}
							for (int j = 0; j < 3; j++)
							{
								if (jarvisenemy[i].botstream[j].audio != NULL) {
									CVector playerpos2;// = jarvisenemy[i].actorchar->GetPosition();

									plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, jarvisenemy[i].actorchar, &playerpos2.x, &playerpos2.y, &playerpos2.z);

									if (jarvisenemy[i].botstream[j].is3d)
									{
										AudioLib.ChannelSet3DAttributes(jarvisenemy[i].botstream[j].audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
										AudioLib.Apply3D();
										AudioLib.ChannelSetAttribute(jarvisenemy[i].botstream[j].audio, 2, boolvars.gamevolume);
									}
									else
									{
										AudioLib.ChannelSetAttribute(jarvisenemy[i].botstream[j].audio, 2, boolvars.gamevolume);
									}
									BASS_3DVECTOR pos2;
									pos2.x = playerpos2.x;
									pos2.y = playerpos2.y;
									pos2.z = playerpos2.z;

									CVector angles = Getorientbetweenpoints(playerpos2, camera.pos);
									posaim.x = angles.x;
									posaim.y = angles.y;
									posaim.z = angles.z;

									AudioLib.ChannelSet3DPosition(jarvisenemy[i].botstream[j].audio, &pos2, &posaim, NULL);
									AudioLib.Apply3D();
								}
							}
						}
					}
				}
			}
		}
	}
}

static bool isplayingfly(bool isplaying, Taudiofile MP3Stream, int hovfly, int env, int user, int index);

bool isplayingfly(bool isplaying, Taudiofile MP3Stream, int hovfly, int env, int user, int index)
{
	if (env == 1)
	{
		if (user == NULL)
		{
			if (MP3Stream.audio == audiostream[underwater].audio)
			{
				if (!isplaying)
				{
					return true;
				}
				else
				{
					if (AudioLib.isset())
					{
						if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (user == 1)
			{
				if ((int)jarvisfriend[index].actorchar != NULL)
				{
					if (MP3Stream.audio == jarvisfriend[index].audios[5].audio)
					{
						if (!isplaying)
						{
							return true;
						}
						else
						{
							if (AudioLib.isset())
							{
								if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
				if (user == 2)
				{
					if ((int)jarvisenemy[index].actorchar != NULL)
					{
						if (MP3Stream.audio = jarvisenemy[index].audios[5].audio)
						{
							if (!isplaying)
							{
								return true;
							}
							else
							{
								if (AudioLib.isset())
								{
									if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
	}
	else
	{
		if (hovfly == 0)
		{
			if (user == NULL)
			{
				if (MP3Stream.audio == audiostream[thrustersfx].audio)
				{
					if (!isplaying)
					{
						return true;
					}
					else
					{
						if (AudioLib.isset())
						{
							if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (user == 1)
				{
					if ((int)jarvisfriend[index].actorchar != NULL)
					{
						if (MP3Stream.audio == jarvisfriend[index].audios[0].audio)
						{
							if (!isplaying)
							{
								return true;
							}
							else
							{
								if (AudioLib.isset())
								{
									if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
					if (user == 2)
					{
						if ((int)jarvisenemy[index].actorchar != NULL)
						{
							if (MP3Stream.audio = jarvisenemy[index].audios[0].audio)
							{
								if (!isplaying)
								{
									return true;
								}
								else
								{
									if (AudioLib.isset())
									{
										if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
		}
		else
		{
			if (user == NULL)
			{
				if (MP3Stream.audio == audiostream[flying_thrusters].audio)
				{
					if (!isplaying)
					{
						return true;
					}
					else
					{
						if (AudioLib.isset())
						{
							if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (user == 1)
				{
					if ((int)jarvisfriend[index].actorchar != NULL)
					{
						if (MP3Stream.audio == jarvisfriend[index].audios[1].audio)
						{
							if (!isplaying)
							{
								return true;
							}
							else
							{
								if (AudioLib.isset())
								{
									if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
					if (user == 2)
					{
						if ((int)jarvisenemy[index].actorchar != NULL)
						{
							if (MP3Stream.audio = jarvisenemy[index].audios[1].audio)
							{
								if (!isplaying)
								{
									return true;
								}
								else
								{
									if (AudioLib.isset())
									{
										if (AudioLib.ChannelIsActive(MP3Stream.audio) == 1)
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
		}
	}
	return false;
}

bool JarvisVoice::PlayThrustersIDLE(int user, int index, Taudiofile *MP3Stream, int pedid, Ttraje suit, int id, int code, Tthrusters *jets) {
	if (AudioLib.isset())
	{
		if (jets->manoizqfx == 0)
		{
			jarvisvoice.create_thrusters(pedid, &suit, code, &*jets);
		}
		if (isplayingfly(false, *MP3Stream, id, jets->enviroment_ID, user, index) == true)
			{
				
				return true;
			}
			else
			{
				if (id == 0) {
					goto playhover;
				}
				else
				{
					goto playflight;
				}
			}
		
	}
	else
	{
		//CFont::SetBackground(0, 0);
		//CFont::SetOrientation(ALIGN_CENTER);
		//CFont::SetProportional(true);
		//CFont::SetJustify(false);
		//CFont::SetColor(CRGBA(255, 255, 255, 255));
		//CFont::SetFontStyle(FONT_SUBTITLES);
		//CFont::SetEdge(2);
		//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
		//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
		//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
		//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass failed");
		return false;
	}
	//		else
	//FreeLibrary(AudioLib.library);

playhover:
	{
		if (MP3Stream->audio == NULL)
		{
			
			//aquivalibinit
			//if (AudioLib.Init(-1, 44100, 4, 0, 0) == NULL)
			//{
				if (jets->enviroment_ID == 1)
				{
					if (user == NULL)
					{
						MP3Stream->audio = audiostream[underwater].audio;
					}
					else
					{
						if (user == 1)
						{
							if ((int)jarvisfriend[index].actorchar != NULL)
							{
								MP3Stream->audio = jarvisfriend[index].audios[5].audio;
							}
						}
						else
						{
							if (user == 2)
							{
								if ((int)jarvisenemy[index].actorchar != NULL)
								{
									MP3Stream->audio = jarvisenemy[index].audios[5].audio;
								}
							}
						}
					}
				}
				else
				{
					if (user == NULL)
					{
						MP3Stream->audio = audiostream[thrustersfx].audio;
					}
					else
					{
						if (user == 1)
						{
							if ((int)jarvisfriend[index].actorchar != NULL)
							{
								MP3Stream->audio = jarvisfriend[index].audios[0].audio;
							}
						}
						else
						{
							if (user == 2)
							{
								if ((int)jarvisenemy[index].actorchar != NULL)
								{
									MP3Stream->audio = jarvisenemy[index].audios[0].audio;
								}
							}
						}
					}
				}
				if (MP3Stream->audio != NULL) {
					AudioLib.ChannelPlay(MP3Stream->audio, false);
					
					for (int i = 0; i < 26; i++)
					{
						if (MP3Stream->is3d)
						{
							AudioLib.ChannelSet3DAttributes(MP3Stream->audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
							AudioLib.Apply3D();
							AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
						}
						else
						{
							AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
						}
					}
					//AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
					return true;
					
				}
				else
				{
					//CFont::SetBackground(0, 0);
					//CFont::SetOrientation(ALIGN_CENTER);
					//CFont::SetProportional(true);
					//CFont::SetJustify(false);
					//CFont::SetColor(CRGBA(255, 255, 255, 255));
					//CFont::SetFontStyle(FONT_SUBTITLES);
					//CFont::SetEdge(2);
					//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
					//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
					//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
					//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "IMStream[ failed");
					return false;
				}
			/*}
			else
			{
				//CFont::SetBackground(0, 0);
				//CFont::SetOrientation(ALIGN_CENTER);
				//CFont::SetProportional(true);
				//CFont::SetJustify(false);
				//CFont::SetColor(CRGBA(255, 255, 255, 255));
				//CFont::SetFontStyle(FONT_SUBTITLES);
				//CFont::SetEdge(2);
				//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
				//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
				//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
				//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass init failed");
				return false;
			}*/
		}
		else
		{
			//CFont::SetBackground(0, 0);
			//CFont::SetOrientation(ALIGN_CENTER);
			//CFont::SetProportional(true);
			//CFont::SetJustify(false);
			//CFont::SetColor(CRGBA(255, 255, 255, 255));
			//CFont::SetFontStyle(FONT_SUBTITLES);
			//CFont::SetEdge(2);
			//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
			//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
			//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
			//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "error audio not null");
			return false;
		}
		goto endgame;
	}

playflight:
	{
		if (MP3Stream->audio == NULL)
		{
			
			//aquivalibinit
			//if (AudioLib.Init(-1, 44100, 4, 0, 0) == NULL)
			//{
				if (jets->enviroment_ID == 1)
				{
					if (user == NULL)
					{
						MP3Stream->audio = audiostream[underwater].audio;
					}
					else
					{
						if (user == 1)
						{
							if ((int)jarvisfriend[index].actorchar != NULL)
							{
								MP3Stream->audio = jarvisfriend[index].audios[5].audio;
							}
						}
						else
						{
							if (user == 2)
							{
								if ((int)jarvisenemy[index].actorchar != NULL)
								{
									MP3Stream->audio = jarvisenemy[index].audios[5].audio;
								}
							}
						}
					}
				}
				else
				{
					if (user == NULL)
					{
						MP3Stream->audio = audiostream[flying_thrusters].audio;
					}
					else
					{
						if (user == 1)
						{
							if ((int)jarvisfriend[index].actorchar != NULL)
							{
								MP3Stream->audio = jarvisfriend[index].audios[1].audio;
							}
						}
						else
						{
							if (user == 2)
							{
								if ((int)jarvisenemy[index].actorchar != NULL)
								{
									MP3Stream->audio = jarvisenemy[index].audios[1].audio;
								}
							}
						}
					}
				}


				if (MP3Stream->audio != NULL) {
					AudioLib.ChannelPlay(MP3Stream->audio, false);
					
					for (int i = 0; i < 26; i++)
					{
						if (MP3Stream->is3d)
						{
							AudioLib.ChannelSet3DAttributes(MP3Stream->audio, -1, 0.0f, 10.0f, 360, 360, boolvars.gamevolume);
							AudioLib.Apply3D();
							AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
						}
						else
						{
							AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
						}
					}
					//AudioLib.ChannelSetAttribute(MP3Stream->audio, 2, boolvars.gamevolume);
					return true;
				}
				else
				{
					//CFont::SetBackground(0, 0);
					//CFont::SetOrientation(ALIGN_CENTER);
					//CFont::SetProportional(true);
					//CFont::SetJustify(false);
					//CFont::SetColor(CRGBA(255, 255, 255, 255));
					//CFont::SetFontStyle(FONT_SUBTITLES);
					//CFont::SetEdge(2);
					//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
					//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
					//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
					//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "IMStream[ failed");
					return false;
				}
			/*}
			else
			{
				//CFont::SetBackground(0, 0);
				//CFont::SetOrientation(ALIGN_CENTER);
				//CFont::SetProportional(true);
				//CFont::SetJustify(false);
				//CFont::SetColor(CRGBA(255, 255, 255, 255));
				//CFont::SetFontStyle(FONT_SUBTITLES);
				//CFont::SetEdge(2);
				//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
				//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
				//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
				//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass init failed");
				return false;
			}*/
		}
		else
		{
			//CFont::SetBackground(0, 0);
			//CFont::SetOrientation(ALIGN_CENTER);
			//CFont::SetProportional(true);
			//CFont::SetJustify(false);
			//CFont::SetColor(CRGBA(255, 255, 255, 255));
			//CFont::SetFontStyle(FONT_SUBTITLES);
			//CFont::SetEdge(2);
			//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
			//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
			//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
			//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "error audio not null");
			return false;
		}
		return false;
	}

endgame:
	{
	return true;
}
}

void JarvisVoice::StopThrustersIDLE(Taudiofile *MP3Stream, Tthrusters* jets) {
	if (AudioLib.isset())
	{
		if (AudioLib.ChannelIsActive(MP3Stream->audio) == 1 && MP3Stream->audio != NULL) {
			if (AudioLib.ChannelStop != false) {
				AudioLib.ChannelStop(MP3Stream->audio);
				MP3Stream->audio = NULL;
			}
			else
			{
				//CFont::SetBackground(0, 0);
				//CFont::SetOrientation(ALIGN_CENTER);
				//CFont::SetProportional(true);
				//CFont::SetJustify(false);
				//CFont::SetColor(CRGBA(255, 255, 255, 255));
				//CFont::SetFontStyle(FONT_SUBTITLES);
				//CFont::SetEdge(2);
				//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
				//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
				//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
				//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass channel stop failed");

			}
		}
		else
		{

		}
	}
	else
	{
		//CFont::SetBackground(0, 0);
		//CFont::SetOrientation(ALIGN_CENTER);
		//CFont::SetProportional(true);
		//CFont::SetJustify(false);
		//CFont::SetColor(CRGBA(255, 255, 255, 255));
		//CFont::SetFontStyle(FONT_SUBTITLES);
		//CFont::SetEdge(2);
		//CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
		//CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
		//WritePrivateProfileString("CONFIG", "ERROR", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));  AQUI IBA
		//CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), "bass failed");

	}
	if (jets->manoizqfx != 0)
	{
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoizqfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoderfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->pieizqfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->piederfx);
		jets->manoizqfx = 0;
		jets->manoderfx = 0;
		jets->piederfx = 0;
		jets->pieizqfx = 0;
	}
}

void JarvisVoice::create_thrusters(int pedid, Ttraje *suit, int code, Tthrusters *jets) {
	CVector mano1;
	CVector mano2;
	CVector pie1;
	bool sepudo=false;
	CVector pie2;
	CVector anglefeet, anglelhand, anglerhand;
	if (suit->hasini == false)
	{

		plugin::config_file config1(GAME_PATH("modloader\\IronManMod\\Skins\\GenericArmor.dat"));
		suit->rightfoot.x = config1["IDLErightfootX"].asFloat(0.071f);
		suit->rightfoot.y = config1["IDLErightfootY"].asFloat(0.067f);
		suit->rightfoot.z = config1["IDLErightfootZ"].asFloat(-1.016f);
		suit->leftfoot.x = config1["IDLEleftfootX"].asFloat(-0.067f);
		suit->leftfoot.y = config1["IDLEleftfootY"].asFloat(0.067f);
		suit->leftfoot.z = config1["IDLEleftfootZ"].asFloat(-1.016f);
		suit->righthand.x = config1["IDLErighthandX"].asFloat(0.362f);
		suit->righthand.y = config1["IDLErighthandY"].asFloat(0.013f);
		suit->righthand.z = config1["IDLErighthandZ"].asFloat(-0.077f);
		suit->lefthand.x = config1["IDLElefthandX"].asFloat(-0.357f);
		suit->lefthand.y = config1["IDLElefthandY"].asFloat(0.013f);
		suit->lefthand.z = config1["IDLElefthandZ"].asFloat(-0.077f);

		suit->flyrightfoot.x = config1["FLYrightfootX"].asFloat(0.12f);
		suit->flyrightfoot.y = config1["FLYrightfootY"].asFloat(-1.05f);
		suit->flyrightfoot.z = config1["FLYrightfootZ"].asFloat(0.1549f);
		suit->flyleftfoot.x = config1["FLYleftfootX"].asFloat(-0.105f);
		suit->flyleftfoot.y = config1["FLYleftfootY"].asFloat(-1.05f);
		suit->flyleftfoot.z = config1["FLYleftfootZ"].asFloat(0.1549f);
		suit->flyrighthand.x = config1["FLYrighthandX"].asFloat(0.435f);
		suit->flyrighthand.y = config1["FLYrighthandY"].asFloat(-0.06f);
		suit->flyrighthand.z = config1["FLYrighthandZ"].asFloat(0.1399f);
		suit->flylefthand.x = config1["FLYlefthandX"].asFloat(-0.435f);
		suit->flylefthand.y = config1["FLYlefthandY"].asFloat(-0.015f);
		suit->flylefthand.z = config1["FLYlefthandZ"].asFloat(0.1549f);
		suit->hasini = true;
		
	}
	if (boolvars.has_a_car == true)
	{
		if (code == flotar)
		{
			pie2.x = suit->rightfoot.x;
			pie2.y = suit->rightfoot.y;
			pie2.z = suit->rightfoot.z;
			pie1.x = suit->leftfoot.x;
			pie1.y = suit->leftfoot.y;
			pie1.z = suit->leftfoot.z;
			mano2.x = suit->rightfoot.x;
			mano2.y = suit->rightfoot.y;
			mano2.z = suit->rightfoot.z;
			mano1.x = suit->leftfoot.x;
			mano1.y = suit->leftfoot.y;
			mano1.z = suit->leftfoot.z;
			anglefeet.x = 0.0f;
			anglefeet.y = -90.0f;
			anglefeet.z = -180.0f;
			anglerhand.x = 0.0f;
			anglerhand.y = -90.0f;
			anglerhand.z = -180.0f;
			anglelhand.x = 0.0f;
			anglelhand.y = -90.0f;
			anglelhand.z = -180.0f;
			sepudo = true;
		}
		else {
			if (code == adelante)
			{
				mano2.x = suit->rightfoot.x +0.099f - 0.016F;
				mano2.y = suit->rightfoot.y -0.193f - 0.515F;
				mano2.z = suit->rightfoot.z +0.127f + 0.307F;
				mano1.x = suit->leftfoot.x -0.065f + 0.018F;
				mano1.y = suit->leftfoot.y +0.051f - 0.9F;
				mano1.z = suit->leftfoot.z +0.127f + 0.317F;

				pie2.x = suit->rightfoot.x +0.099f - 0.016F;
				pie2.y = suit->rightfoot.y -0.193f - 0.515F;
				pie2.z = suit->rightfoot.z +0.127f + 0.307F;
				pie1.x = suit->leftfoot.x -0.065f + 0.018F;
				pie1.y = suit->leftfoot.y +0.051f - 0.9F;
				pie1.z = suit->leftfoot.z +0.127f + 0.317F;
				anglefeet.x = 0.0f;
				anglefeet.y = -90.0f;
				anglefeet.z = 0.0f;
				anglerhand.x = 0.0f;
				anglerhand.y = -90.0f;
				anglerhand.z = 0.0f;
				anglelhand.x = 0.0f;
				anglelhand.y = -90.0f;
				anglelhand.z = 0.0f;
				sepudo = true;
			}
			else
			{
				if (code == atras)
				{
					mano2.x = suit->rightfoot.x +0.099f - 0.057F;
					mano2.y = suit->rightfoot.y -0.193f + 0.778F;
					mano2.z = suit->rightfoot.z +0.127f + 0.125F;
					mano1.x = suit->leftfoot.x -0.065f + 0.044F;
					mano1.y = suit->leftfoot.y +0.051f + 0.482F;
					mano1.z = suit->leftfoot.z +0.127f + 0.423F;

					pie2.x = suit->rightfoot.x +0.099f - 0.057F;
					pie2.y = suit->rightfoot.y -0.193f + 0.778F;
					pie2.z = suit->rightfoot.z +0.127f + 0.125F;
					pie1.x = suit->leftfoot.x -0.065f + 0.044f;
					pie1.y = suit->leftfoot.y +0.051f + 0.482f;
					pie1.z = suit->leftfoot.z +0.127f + 0.423f;
					anglefeet.x = 0.0f;
					anglefeet.y = 90.0f;
					anglefeet.z = 0.0f;
					anglerhand.x = 0.0f;
					anglerhand.y = 90.0f;
					anglerhand.z = 0.0f;
					anglelhand.x = 0.0f;
					anglelhand.y = 90.0f;
					anglelhand.z = 0.0f;
					sepudo = true;
				}
				else
				{
					if (code == izquierda)
					{
						pie2.x = suit->rightfoot.x +0.099f + 0.236f;
						pie2.y = suit->rightfoot.y -0.193f + 0.233f;
						pie2.z = suit->rightfoot.z +0.127f + 0.028f;
						pie1.x = suit->leftfoot.x -0.065f + 0.257f;
						pie1.y = suit->leftfoot.y +0.051f - 0.163f;
						pie1.z = suit->leftfoot.z +0.127f - 0.096f;

						mano2.x = suit->rightfoot.x +0.099f + 0.236f;
						mano2.y = suit->rightfoot.y -0.193f + 0.233f;
						mano2.z = suit->rightfoot.z +0.127f + 0.028f;
						mano1.x = suit->leftfoot.x -0.065f + 0.257f;
						mano1.y = suit->leftfoot.y +0.051f - 0.163f;
						mano1.z = suit->leftfoot.z +0.127f - 0.096f;
						
						anglefeet.x = 180.0f;
						anglefeet.y = 0.0f;
						anglefeet.z = -45.0f;
						anglerhand.x = 180.0f;
						anglerhand.y = 0.0f;
						anglerhand.z = -45.0f;
						anglelhand.x = 180.0f;
						anglelhand.y = 0.0f;
						anglelhand.z = -45.0f;
						sepudo = true;
					}
					else
					{
						if (code == derecha)
						{
							pie2.x = suit->rightfoot.x +0.099f - 0.372f;
							pie2.y = suit->rightfoot.y -0.193f - 0.072f;
							pie2.z = suit->rightfoot.z +0.127f - 0.096f;
							pie1.x = suit->leftfoot.x -0.065f - 0.303f;
							pie1.y = suit->leftfoot.y +0.051f - 0.192f;
							pie1.z = suit->leftfoot.z +0.127f + 0.141f;

							mano2.x = suit->rightfoot.x +0.099f - 0.372f;
							mano2.y = suit->rightfoot.y -0.193f - 0.072f;
							mano2.z = suit->rightfoot.z +0.127f - 0.096f;
							mano1.x = suit->leftfoot.x -0.065f - 0.303f;
							mano1.y = suit->leftfoot.y +0.051f - 0.192f;
							mano1.z = suit->leftfoot.z +0.127f + 0.141f;
							
							anglefeet.x = -180.0f;
							anglefeet.y = 0.0f;
							anglefeet.z = -45.0f;
							anglerhand.x = -180.0f;
							anglerhand.y = 0.0f;
							anglerhand.z = -45.0f;
							anglelhand.x = -180.0f;
							anglelhand.y = 0.0f;
							anglelhand.z = -45.0f;
							sepudo = true;
						}
						else
						{
							if (code == arriba)
							{
								pie2.x = suit->rightfoot.x +0.099f - 0.069f;
								pie2.y = suit->rightfoot.y -0.193f - 0.044f;
								pie2.z = suit->rightfoot.z +0.127f - 0.035f;
								pie1.x = suit->leftfoot.x -0.065f + 0.025f;
								pie1.y = suit->leftfoot.y +0.051f - 0.288f;
								pie1.z = suit->leftfoot.z +0.127f + 0.035f;

								mano2.x = suit->rightfoot.x +0.099f - 0.069f;
								mano2.y = suit->rightfoot.y -0.193f - 0.044f;
								mano2.z = suit->rightfoot.z +0.127f - 0.035f;
								mano1.x = suit->leftfoot.x -0.065f + 0.025f;
								mano1.y = suit->leftfoot.y +0.051f - 0.288f;
								mano1.z = suit->leftfoot.z +0.127f + 0.035f;
								anglefeet.x = 0.0f;
								anglefeet.y = -90.0f;
								anglefeet.z = -180.0f;
								anglerhand.x = 0.0f;
								anglerhand.y = -90.0f;
								anglerhand.z = -180.0f;
								anglelhand.x = 0.0f;
								anglelhand.y = -90.0f;
								anglelhand.z = -180.0f;
								sepudo = true;
							}
							else
							{
								if (code == abajo)
								{
									pie2.x = suit->rightfoot.x +0.099f - 0.052f;
									pie2.y = suit->rightfoot.y -0.193f + 0.306f;
									pie2.z = suit->rightfoot.z +0.127f - 0.057f;
									pie1.x = suit->leftfoot.x -0.065f + 0.032f;
									pie1.y = suit->leftfoot.y +0.051f + 0.058f;
									pie1.z = suit->leftfoot.z +0.127f + 0.036f;

									mano2.x = suit->rightfoot.x +0.099f - 0.052f;
									mano2.y = suit->rightfoot.y -0.193f + 0.306f;
									mano2.z = suit->rightfoot.z +0.127f - 0.057f;
									mano1.x = suit->leftfoot.x -0.065f + 0.032f;
									mano1.y = suit->leftfoot.y +0.051f + 0.058f;
									mano1.z = suit->leftfoot.z +0.127f + 0.036f;

									anglefeet.x = 0.0f;
									anglefeet.y = -90.0f;
									anglefeet.z = -180.0f;

									anglerhand.x = 0.0f;
									anglerhand.y = -90.0f;
									anglerhand.z = -180.0f;
									anglelhand.x = 0.0f;
									anglelhand.y = -90.0f;
									anglelhand.z = -180.0f;

									sepudo = true;
								}
								else
								{
									if (code == im_flyfast)
									{
										pie2.x = suit->flyrightfoot.x;
										pie2.y = suit->flyrightfoot.y;
										pie2.z = suit->flyrightfoot.z;
										pie1.x = suit->flyleftfoot.x;
										pie1.y = suit->flyleftfoot.y;
										pie1.z = suit->flyleftfoot.z;
										mano2.x = suit->flyrighthand.x;
										mano2.y = suit->flyrighthand.y;
										mano2.z = suit->flyrighthand.z;
										mano1.x = suit->flylefthand.x;
										mano1.y = suit->flylefthand.y;
										mano1.z = suit->flylefthand.z;
										anglefeet.x = 0.0f;
										anglefeet.y = -180.0f;
										anglefeet.z = 180.0f;
										anglerhand.x = 0.0f;
										anglerhand.y = -180.0f;
										anglerhand.z = 180.0f;
										anglelhand.x = 0.0f;
										anglelhand.y = -180.0f;
										anglelhand.z = 180.0f;
										sepudo = true;
									}
									else
									{
										if (code == apuntar)
										{
											pie2.x = suit->rightfoot.x +0.099f + 0.042f + 1.1f;
											pie2.y = suit->rightfoot.y -0.193f + 0.051f + 0.007f;
											pie2.z = suit->rightfoot.z +0.127f - 0.05f + 0.017f;
											pie1.x = suit->leftfoot.x -0.065f - 0.06f + 1.1f;
											pie1.y = suit->leftfoot.y +0.051f - 0.28f + 0.007f;
											pie1.z = suit->leftfoot.z +0.127f - 0.087f + 0.017f;

											mano2.x = suit->rightfoot.x +0.099f + 0.042f + 1.1f;
											mano2.y = suit->rightfoot.y -0.193f + 0.051f + 0.007f;
											mano2.z = suit->rightfoot.z +0.127f - 0.05f + 0.017f;
											mano1.x = suit->leftfoot.x -0.065f - 0.06f + 1.1f;
											mano1.y = suit->leftfoot.y +0.051f - 0.28f + 0.007f;
											mano1.z = suit->leftfoot.z +0.127f - 0.087f + 0.017f;
											
											anglefeet.x = 0.0f;
											anglefeet.y = -90.0f;
											anglefeet.z = -180.0f;
											anglerhand.x = 0.0f;
											anglerhand.y = -90.0f;
											anglerhand.z = -180.0f;
											anglelhand.x = 0.0f;
											anglelhand.y = -90.0f;
											anglelhand.z = -180.0f;
											sepudo = true;
										}
										else
										{
											if (code == apuntarbot)
											{
												pie2.x = suit->rightfoot.x +0.099f - 0.009f;
												pie2.y = suit->rightfoot.y -0.193f - 0.09f;
												pie2.z = suit->rightfoot.z +0.127f - 0.004f;
												pie1.x = suit->leftfoot.x -0.065f - 0.108f;
												pie1.y = suit->leftfoot.y +0.051f - 0.139f;
												pie1.z = suit->leftfoot.z +0.127f - 0.104f;
												mano2.x = suit->righthand.x +0.063f - 0.014f;// + 1.061;
												mano2.y = suit->righthand.y + 0.024f;// + 0.094;
												mano2.z = suit->righthand.z +0.124f - 0.066f;// 0.132;
												mano1.x = suit->righthand.x +0.063f - 0.014f;// + 1.061;
												mano1.y = suit->righthand.y + 0.024f;// + 0.094;
												mano1.z = suit->righthand.z +0.124f - 0.066f;// 0.132;
												anglefeet.x = 0.0f;
												anglefeet.y = -90.0f;
												anglefeet.z = -180.0f;
												anglerhand.x = 0.0f;
												anglerhand.y = -90.0f;
												anglerhand.z = -180.0f;
												anglelhand.x = 0.0f;
												anglelhand.y = -90.0f;
												anglelhand.z = -180.0f;
												sepudo = true;
											}
											else
											{
												plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoizqfx);
												plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoderfx);
												plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->pieizqfx);
												plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->piederfx);
												jets->manoizqfx = 0;
												jets->manoderfx = 0;
												jets->pieizqfx = 0;
												jets->piederfx = 0;
												sepudo = false;
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
	else
	{
		if (code == flotar)
		{
			pie2.x = suit->rightfoot.x;
			pie2.y = suit->rightfoot.y;
			pie2.z = suit->rightfoot.z;
			pie1.x = suit->leftfoot.x;
			pie1.y = suit->leftfoot.y;
			pie1.z = suit->leftfoot.z;
			mano2.x = suit->righthand.x;
			mano2.y = suit->righthand.y;
			mano2.z = suit->righthand.z;
			mano1.x = suit->lefthand.x;
			mano1.y = suit->lefthand.y;
			mano1.z = suit->lefthand.z;
			anglefeet.x = 0.0f;
			anglefeet.y = -90.0f;
			anglefeet.z = -180.0f;
			anglerhand.x = 0.0f;
			anglerhand.y = -90.0f;
			anglerhand.z = -180.0f;
			anglelhand.x = 0.0f;
			anglelhand.y = -90.0f;
			anglelhand.z = -180.0f;
			sepudo = true;
		}
		else {
			if (code == adelante)
			{
				mano2.x = suit->rightfoot.x +0.099f - 0.016F;
				mano2.y = suit->rightfoot.y -0.193f - 0.515F;
				mano2.z = suit->rightfoot.z +0.127f + 0.307F;
				mano1.x = suit->leftfoot.x -0.065f + 0.018F;
				mano1.y = suit->leftfoot.y +0.051f - 0.9F;
				mano1.z = suit->leftfoot.z +0.127f + 0.317F;

				pie2.x = suit->rightfoot.x +0.099f - 0.016F;
				pie2.y = suit->rightfoot.y -0.193f - 0.515F;
				pie2.z = suit->rightfoot.z +0.127f + 0.307F;
				pie1.x = suit->leftfoot.x -0.065f + 0.018F;
				pie1.y = suit->leftfoot.y +0.051f - 0.9F;
				pie1.z = suit->leftfoot.z +0.127f + 0.317F;
				anglefeet.x = 0.0f;
				anglefeet.y = -90.0f;
				anglefeet.z = 0.0f;
				anglerhand.x = 0.0f;
				anglerhand.y = -90.0f;
				anglerhand.z = 0.0f;
				anglelhand.x = 0.0f;
				anglelhand.y = -90.0f;
				anglelhand.z = 0.0f;
				sepudo = true;
			}
			else
			{
				if (code == atras)
				{
					mano2.x = suit->rightfoot.x +0.099f - 0.057F;
					mano2.y = suit->rightfoot.y -0.193f + 0.778F;
					mano2.z = suit->rightfoot.z +0.127f + 0.125F;
					mano1.x = suit->leftfoot.x -0.065f + 0.044F;
					mano1.y = suit->leftfoot.y +0.051f + 0.482F;
					mano1.z = suit->leftfoot.z +0.127f + 0.423F;

					pie2.x = suit->rightfoot.x +0.099f - 0.057F;
					pie2.y = suit->rightfoot.y -0.193f + 0.778F;
					pie2.z = suit->rightfoot.z +0.127f + 0.125F;
					pie1.x = suit->leftfoot.x -0.065f + 0.044f;
					pie1.y = suit->leftfoot.y +0.051f + 0.482f;
					pie1.z = suit->leftfoot.z +0.127f + 0.423f;
					anglefeet.x = 0.0f;
					anglefeet.y = 90.0f;
					anglefeet.z = 0.0f;
					anglerhand.x = 0.0f;
					anglerhand.y = 90.0f;
					anglerhand.z = 0.0f;
					anglelhand.x = 0.0f;
					anglelhand.y = 90.0f;
					anglelhand.z = 0.0f;
					sepudo = true;
				}
				else
				{
					if (code == izquierda)
					{
						pie2.x = suit->rightfoot.x +0.099f + 0.236f;
						pie2.y = suit->rightfoot.y -0.193f + 0.233f;
						pie2.z = suit->rightfoot.z +0.127f + 0.028f;
						pie1.x = suit->leftfoot.x -0.065f + 0.257f;
						pie1.y = suit->leftfoot.y +0.051f - 0.163f;
						pie1.z = suit->leftfoot.z +0.127f - 0.096f;
						mano2.x = suit->righthand.x +0.063f - 0.019f;
						mano2.y = suit->righthand.y + 0.081f;
						mano2.z = suit->righthand.z +0.124f - 0.131f;
						mano1.x = suit->lefthand.x +0.049f - 0.12f;
						mano1.y = suit->lefthand.y +0.286f - 0.174f;
						mano1.z = suit->lefthand.z +0.124f - 0.145f;
						anglefeet.x = 180.0f;
						anglefeet.y = 0.0f;
						anglefeet.z = -45.0f;
						anglerhand.x = 180.0f;
						anglerhand.y = 0.0f;
						anglerhand.z = -45.0f;
						anglelhand.x = 180.0f;
						anglelhand.y = 0.0f;
						anglelhand.z = -45.0f;
						sepudo = true;
					}
					else
					{
						if (code == derecha)
						{
							pie2.x = suit->rightfoot.x +0.099f - 0.372f;
							pie2.y = suit->rightfoot.y -0.193f - 0.072f;
							pie2.z = suit->rightfoot.z +0.127f - 0.096f;
							pie1.x = suit->leftfoot.x -0.065f - 0.303f;
							pie1.y = suit->leftfoot.y +0.051f - 0.192f;
							pie1.z = suit->leftfoot.z +0.127f + 0.141f;
							mano2.x = suit->righthand.x +0.063f + 0.101f;
							mano2.y = suit->righthand.y + 0.112f;
							mano2.z = suit->righthand.z +0.124f - 0.145f;
							mano1.x = suit->lefthand.x +0.049f;
							mano1.y = suit->lefthand.y +0.286f - 0.205f;
							mano1.z = suit->lefthand.z +0.124f - 0.15f;
							anglefeet.x = -180.0f;
							anglefeet.y = 0.0f;
							anglefeet.z = -45.0f;
							anglerhand.x = -180.0f;
							anglerhand.y = 0.0f;
							anglerhand.z = -45.0f;
							anglelhand.x = -180.0f;
							anglelhand.y = 0.0f;
							anglelhand.z = -45.0f;
							sepudo = true;
						}
						else
						{
							if (code == arriba)
							{
								pie2.x = suit->rightfoot.x +0.099f - 0.069f;
								pie2.y = suit->rightfoot.y -0.193f - 0.044f;
								pie2.z = suit->rightfoot.z +0.127f - 0.035f;
								pie1.x = suit->leftfoot.x -0.065f + 0.025f;
								pie1.y = suit->leftfoot.y +0.051f - 0.288f;
								pie1.z = suit->leftfoot.z +0.127f + 0.035f;

								mano2.x = suit->rightfoot.x +0.099f - 0.069f;
								mano2.y = suit->rightfoot.y -0.193f - 0.044f;
								mano2.z = suit->rightfoot.z +0.127f - 0.035f;
								mano1.x = suit->leftfoot.x -0.065f + 0.025f;
								mano1.y = suit->leftfoot.y +0.051f - 0.288f;
								mano1.z = suit->leftfoot.z +0.127f + 0.035f;
								anglefeet.x = 0.0f;
								anglefeet.y = -90.0f;
								anglefeet.z = -180.0f;
								anglerhand.x = 0.0f;
								anglerhand.y = -90.0f;
								anglerhand.z = -180.0f;
								anglelhand.x = 0.0f;
								anglelhand.y = -90.0f;
								anglelhand.z = -180.0f;
								sepudo = true;
							}
							else
							{
								if (code == abajo)
								{
									pie2.x = suit->rightfoot.x +0.099f - 0.052f;
									pie2.y = suit->rightfoot.y -0.193f + 0.306f;
									pie2.z = suit->rightfoot.z +0.127f - 0.057f;
									pie1.x = suit->leftfoot.x -0.065f + 0.032f;
									pie1.y = suit->leftfoot.y +0.051f + 0.058f;
									pie1.z = suit->leftfoot.z +0.127f + 0.036f;
									mano2.x = suit->righthand.x +0.063f + 0.094f;
									mano2.y = suit->righthand.y + 0.225f;
									mano2.z = suit->righthand.z +0.124f;
									mano1.x = suit->lefthand.x +0.049f - 0.189f;
									mano1.y = suit->lefthand.y +0.286f - 0.328f;
									mano1.z = suit->lefthand.z +0.124f - 0.084f;
									anglefeet.x = 0.0f;
									anglefeet.y = -90.0f;
									anglefeet.z = -180.0f;
									anglerhand.x = 180.0f;
									anglerhand.y = 0.0f;
									anglerhand.z = -45.0f;
									anglelhand.x = -180.0f;
									anglelhand.y = 0.0f;
									anglelhand.z = -45.0f;
									sepudo = true;
								}
								else
								{
									if (code == im_flyfast)
									{
										pie2.x = suit->flyrightfoot.x;
										pie2.y = suit->flyrightfoot.y;
										pie2.z = suit->flyrightfoot.z;
										pie1.x = suit->flyleftfoot.x;
										pie1.y = suit->flyleftfoot.y;
										pie1.z = suit->flyleftfoot.z;
										mano2.x = suit->flyrighthand.x;
										mano2.y = suit->flyrighthand.y;
										mano2.z = suit->flyrighthand.z;
										mano1.x = suit->flylefthand.x;
										mano1.y = suit->flylefthand.y;
										mano1.z = suit->flylefthand.z;
										anglefeet.x = 0.0f;
										anglefeet.y = -180.0f;
										anglefeet.z = 180.0f;
										anglerhand.x = 0.0f;
										anglerhand.y = -180.0f;
										anglerhand.z = 180.0f;
										anglelhand.x = 0.0f;
										anglelhand.y = -180.0f;
										anglelhand.z = 180.0f;
										sepudo = true;
									}
									else
									{
										if (code == apuntar)
										{
											pie2.x = suit->rightfoot.x +0.099f + 0.042f + 1.1f;
											pie2.y = suit->rightfoot.y -0.193f + 0.051f + 0.007f;
											pie2.z = suit->rightfoot.z +0.127f - 0.05f + 0.017f;
											pie1.x = suit->leftfoot.x -0.065f - 0.06f + 1.1f;
											pie1.y = suit->leftfoot.y +0.051f - 0.28f + 0.007f;
											pie1.z = suit->leftfoot.z +0.127f - 0.087f + 0.017f;
											mano2.x = suit->righthand.x +0.063f - 0.042f + 1.1f;// + 1.061;
											mano2.y = suit->righthand.y + 0.096f + 0.007f;// + 0.094;
											mano2.z = suit->righthand.z +0.124f - 0.15f + 0.017f;// 0.132;
											mano1.x = suit->righthand.x +0.063f - 0.042f + 1.1f;// + 1.061;
											mano1.y = suit->righthand.y + 0.096f + 0.007f;// + 0.094;
											mano1.z = suit->righthand.z +0.124f - 0.15f + 0.017f;// 0.132;
											anglefeet.x = 0.0f;
											anglefeet.y = -90.0f;
											anglefeet.z = -180.0f;
											anglerhand.x = 0.0f;
											anglerhand.y = -90.0f;
											anglerhand.z = -180.0f;
											anglelhand.x = 0.0f;
											anglelhand.y = -90.0f;
											anglelhand.z = -180.0f;
											sepudo = true;
										}
										else
										{
											if (code == apuntarbot)
											{
												pie2.x = suit->rightfoot.x +0.099f - 0.009f;
												pie2.y = suit->rightfoot.y -0.193f - 0.09f;
												pie2.z = suit->rightfoot.z +0.127f - 0.004f;
												pie1.x = suit->leftfoot.x -0.065f - 0.108f;
												pie1.y = suit->leftfoot.y +0.051f - 0.139f;
												pie1.z = suit->leftfoot.z +0.127f - 0.104f;
												mano2.x = suit->righthand.x +0.063f - 0.014f;// + 1.061;
												mano2.y = suit->righthand.y + 0.024f;// + 0.094;
												mano2.z = suit->righthand.z +0.124f - 0.066f;// 0.132;
												mano1.x = suit->righthand.x +0.063f - 0.014f;// + 1.061;
												mano1.y = suit->righthand.y + 0.024f;// + 0.094;
												mano1.z = suit->righthand.z +0.124f - 0.066f;// 0.132;
												anglefeet.x = 0.0f;
												anglefeet.y = -90.0f;
												anglefeet.z = -180.0f;
												anglerhand.x = 0.0f;
												anglerhand.y = -90.0f;
												anglerhand.z = -180.0f;
												anglelhand.x = 0.0f;
												anglelhand.y = -90.0f;
												anglelhand.z = -180.0f;
												sepudo = true;
											}
											else
											{
												if (code == stopcaida)
												{
													pie2.x = 0.254f;
													pie2.y = 0.491f;
													pie2.z = -0.691f;
													pie1.x = -0.271f;
													pie1.y = 0.491f;
													pie1.z = -0.691f;
													mano2.x = 0.254f;
													mano2.y = 0.491f;
													mano2.z = -0.691f;
													mano1.x = -0.271f;
													mano1.y = 0.491f;
													mano1.z = -0.691f;
													anglefeet.x = 0.0f;
													anglefeet.y = -90.0f;
													anglefeet.z = -180.0f;
													anglerhand.x = 0.0f;
													anglerhand.y = -90.0f;
													anglerhand.z = -180.0f;
													anglelhand.x = 0.0f;
													anglelhand.y = -90.0f;
													anglelhand.z = -180.0f;
													sepudo = true;
												}
												else
												{
													plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoizqfx);
													plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoderfx);
													plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->pieizqfx);
													plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->piederfx);
													jets->manoizqfx = 0;
													jets->manoderfx = 0;
													jets->pieizqfx = 0;
													jets->piederfx = 0;
													sepudo = false;
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
	if (jets->manoizqfx == 0)
	{
		if (sepudo == true)
		{
			CPed *player = FindPlayerPed(0);
			if ((int)player != pedid)
			{
				player = CPools::GetPed(pedid);
			}
			if (jets->enviroment_ID == 0)
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thrusters", player, mano1.x, mano1.y, mano1.z, anglelhand.x, anglelhand.y, anglelhand.z, 15, &jets->manoizqfx);
			}
			else
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thruster_under", player, mano1.x, mano1.y, mano1.z, anglelhand.x, anglelhand.y, anglelhand.z, 15, &jets->manoizqfx);
			}
			plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, jets->manoizqfx);
			if (jets->enviroment_ID == 0)
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thrusters", player, mano2.x, mano2.y, mano2.z, anglerhand.x, anglerhand.y, anglerhand.z, 15, &jets->manoderfx);
			}
			else
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thruster_under", player, mano2.x, mano2.y, mano2.z, anglerhand.x, anglerhand.y, anglerhand.z, 15, &jets->manoderfx);
			}
			plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, jets->manoderfx);
			if (jets->enviroment_ID == 0)
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thrusters", player, pie1.x, pie1.y, pie1.z, anglefeet.x, anglefeet.y, anglefeet.z, 15, &jets->pieizqfx);
			}
			else
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thruster_under", player, pie1.x, pie1.y, pie1.z, anglefeet.x, anglefeet.y, anglefeet.z, 15, &jets->pieizqfx);
			}
			plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, jets->pieizqfx);
			if (jets->enviroment_ID == 0)
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thrusters", player, pie2.x, pie2.y, pie2.z, anglefeet.x, anglefeet.y, anglefeet.z, 15, &jets->piederfx);
			}
			else
			{
				plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION, "thruster_under", player, pie2.x, pie2.y, pie2.z, anglefeet.x, anglefeet.y, anglefeet.z, 15, &jets->piederfx);
			}
			plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, jets->piederfx);
		}
	}
	else
	{
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoizqfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoderfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->pieizqfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->piederfx);
		jets->manoizqfx = 0;
		jets->manoderfx = 0;
		jets->piederfx = 0;
		jets->pieizqfx = 0;
	}
}

void JarvisVoice::kill_thrusters(Tthrusters *jets) {
	if (jets->manoizqfx != 0)
	{
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoizqfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->manoderfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->pieizqfx);
		plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, jets->piederfx);
		jets->manoizqfx = 0;
		jets->manoderfx = 0;
		jets->piederfx = 0;
		jets->pieizqfx = 0;
	}
}

void JarvisVoice::aim(float *x, float *y, float *z) {
	CPed *player;
	player = FindPlayerPed(0);
	CVector campos;
	CVector playerpos;
	CVector dist;
	CVector pointat;
	CVector camvec, camaimpoint;

	for (float i = 1.0f; i <= 300.0f; i++) {
		plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.8f, 0.0f, &playerpos.x, &playerpos.y, &playerpos.z);
		TheCamera.Find3rdPersonCamTargetVector(i, playerpos, &camvec, &camaimpoint);
		RwV3d coords, camcoors;
		coords.x = 0.185f;
		coords.y = 0.08f;
		coords.z = 0.0f;
		player->GetTransformedBonePosition(coords, 3, false);
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &camcoors.x, &camcoors.y, &camcoors.z);
		CColPoint colpnt;
		CEntity *pedcolliding;
		pedcolliding = NULL;

		*x = camaimpoint.x;
		*y = camaimpoint.y;
		*z = camaimpoint.z;
		if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, coords.x, coords.y, coords.z, camaimpoint.x, camaimpoint.y, camaimpoint.z, 1, 1, 1, 1, 0) == false)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, coords.x, coords.y, coords.z, camaimpoint.x, camaimpoint.y, camaimpoint.z, 1, 1, 1, 1, 0) == false)
			{
				if (pedcolliding != (CEntity*)(player))
				{
					break;
				}
			}
		}
	}
}

void JarvisVoice::aimcar(int carmass, float *x, float *y, float *z, float *x1, float *y1, float *z1) {
	CPed *player;
	player = FindPlayerPed(0);
	DWORD* pTarget = (DWORD*)0xB6F3B8;//pointer of target object. 0xB6F028

	CVector playerpos;
	CVector carcor;
	CVector veloc;
	CVector pointat;
	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &pointat.x, &pointat.y, &pointat.z);




	CVector camvec, camaimpoint;
	TheCamera.Find3rdPersonCamTargetVector(100, pointat, &camvec, &camaimpoint);

	*x = (camaimpoint.x - pointat.x)*5.0f;
	*y = (camaimpoint.y - pointat.y)*5.0f;
	*z = (camaimpoint.z - pointat.z)*5.0f;


	float speedfactor= 35.0f + carmass/100.0f;

	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &veloc.x, &veloc.y, &veloc.z);
	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &playerpos.x, &playerpos.y, &playerpos.z);
	playerpos.x -= veloc.x;
	playerpos.y -= veloc.y;
	playerpos.z -= veloc.z;
	playerpos.x *= speedfactor;
	playerpos.y *= speedfactor;
	playerpos.z *= speedfactor;
	veloc.x += playerpos.x;
	veloc.y += playerpos.y;
	veloc.z += playerpos.z;
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &playerpos.x, &playerpos.y, &playerpos.z);
	veloc.x -= playerpos.x;
	veloc.y -= playerpos.y;
	veloc.z -= playerpos.z;
	*x1 = veloc.x * -speedfactor;
	*y1 = veloc.y * -speedfactor;
	*z1 = veloc.z * -speedfactor;
}

void JarvisVoice::coordstoscreen(float x, float y, float z, float *dx, float *dy, float *wid, float *hei) {
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

int JarvisVoice::activesuitwearing() {
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

bool JarvisVoice::is_wearing_suit(int bp) {
	CPed *player;
	player = FindPlayerPed(0);
	static int hulkbuster;
	static unsigned int armorsuit;
	static int error;
	static int mark;
	mark = boolvars.mark;
	error = 0;
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
						if(bp!=1)
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
//for regular superpowers without hulkbuster
bool JarvisVoice::is_wearing_suit_and_not_driving(int bp) {
	CPed *player;
	player = FindPlayerPed(0);
	static int error;
	static int mark;
	static unsigned int armorsuit;
	error = 0;
	mark = boolvars.mark;

	static int head, shades, reloj, zapas;
	head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
	reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
	shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
	zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
	if (error == 0)
	{
		if (player->m_nPedState != PEDSTATE_DRIVING) 
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
					if (armorsuit == -1137657761)
					{
						if (mark != 1)
						{
							WritePrivateProfileString("CONFIG", "MARK", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							
						}
						return false;
					}
					else
					{
						if (armorsuit == -192727393)
						{
							if (mark != 2)
							{
								WritePrivateProfileString("CONFIG", "MARK", "2", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
								
							}
							return true;
						}
						else
						{
							if (armorsuit == 2061229512)
							{
								if (mark != 3)
								{
									WritePrivateProfileString("CONFIG", "MARK", "3", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									
								}
								return true;
							}
							else
							{
								if (armorsuit == 1226861131)
								{
									if (mark != 4)
									{
										WritePrivateProfileString("CONFIG", "MARK", "4", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
										
									}
									return true;
								}
								else
								{
									if (armorsuit == 1817399610)
									{
										if (mark != 5)
										{
											WritePrivateProfileString("CONFIG", "MARK", "5", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
											
										}
										return true;
									}
									else
									{
										if (armorsuit == 900618200)
										{
											if (mark != 6)
											{
												WritePrivateProfileString("CONFIG", "MARK", "6", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
												
											}
											return true;
										}
										else
										{
											if (armorsuit == -559695285)
											{
												if (mark != 7)
												{
													WritePrivateProfileString("CONFIG", "MARK", "7", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
													
												}
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
																			if (head==1348958410 ||
																				shades==672552983)
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
								}
							}
						}
					}
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
	else
	{
		return false;
	}
}

bool JarvisVoice::is_on_air_or_water() {
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

bool JarvisVoice::is_on_foot() {
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
	if (actorstate == 3)
		return true;
	else
		return false;
}

void JarvisVoice::writeProtected(int addr, uchar value, int size)//, HANDLE pHandle)
{
	plugin::patch::SetUChar(addr, value, true);
}

void JarvisVoice::SetVelocityUnlimited(bool id) {
	if (id == true) {
		writeProtected(0x5E91CE, 0x90, 1);
		writeProtected(0x5E91CF, 0x90, 1);
		writeProtected(0x5E91D0, 0x90, 1);
		writeProtected(0x5E91D1, 0x90, 1);
		writeProtected(0x5E91D2, 0x90, 1);
		writeProtected(0x5E91D3, 0x90, 1);
		writeProtected(0x5E91D4, 0x90, 1);
	}
	else
	{
		writeProtected(0x5E91CE, 0xC7, 1);
		writeProtected(0x5E91CF, 0x46, 1);
		writeProtected(0x5E91D0, 0x4C, 1);
		writeProtected(0x5E91D1, 0x00, 1);
		writeProtected(0x5E91D2, 0x00, 1);
		writeProtected(0x5E91D3, 0x80, 1);
		writeProtected(0x5E91D4, 0x3E, 1);
	}
}

bool JarvisVoice::is_over_ground(float distance) {
	static float dist;
	CPed *player;
	player = FindPlayerPed(0);
	CVector coords;

	if (player)
	{
		coords = player->GetPosition();
	}

	if (is_on_air_or_water() == true) {
		plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, coords.x, coords.y, coords.z, &dist);
		if (dist != 0.0f) {
			if (coords.z - dist > distance) {
				if (jarvisvoice.has_obstacles(player, 0.0f, 0.0f, 0.0f - distance, 1, 1, 0, 1, 0) == false)
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

bool is_over_ground2(float distance);

bool is_over_ground2(float distance) {
	static float dist;
	CPed *player;
	player = FindPlayerPed(0);
	CVector coords;

	if (player)
	{
		coords = player->GetPosition();
	}

	if (jarvisvoice.is_on_air_or_water() == true) {
		plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, coords.x, coords.y, coords.z, &dist);
		if (dist != 0.0f) {
			if (coords.z - dist > distance) {
				return true;
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

bool JarvisVoice::nosemueve() {
	CPad *pad;
	pad = CPad::GetPad(0);
	CPed *player;
	player = FindPlayerPed(0);
	int x, y, specialx, specialy;

	plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
	if (x == 0 && y == 0 && !pad->GetSprint() && !pad->GetDuck())
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool JarvisVoice::nosemueve2() {
	CPad *pad;
	pad = CPad::GetPad(0);
	CPed *player;
	player = FindPlayerPed(0);
	int x, y, specialx, specialy;

	plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
	if (x == 0 && y == 0 && !pad->GetSprint() && !pad->GetDuck())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool JarvisVoice::is_aiming() {
	CPad *pad;
	pad = CPad::GetPad(0);
	CPed *player = FindPlayerPed(0);
	static int controller;

	//if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 16) == false)
	//{
		plugin::scripting::CallCommandById(COMMAND_GET_CONTROLLER_MODE, &controller);
		if (jarvisvoice.is_over_ground(2.0f) == false)
		{
			plugin::scripting::CallCommandById(COMMAND_GET_CONTROLLER_MODE, &controller);
			if (controller == 1) {
				if (CMouseControllerState().rmb == 1 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == true)
				{
					return true;
				}
				else {
					return false;
				}
			}
			else
			{
				if (CMouseControllerState().rmb == 1 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 6) == true)
				{
					return true;
				}
				else {
					return false;
				}
			}


		}
		else {
			if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 0) == false && plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 1) == false)// && !pad->GetSprint() && !pad->GetDuck())
			{
				if (controller == 1) {
					if (CMouseControllerState().rmb == 1 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == true)
					{
						return true;
					}
					else {
						return false;
					}
				}
				else
				{
					if (CMouseControllerState().rmb == 1 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 6) == true)
					{
						return true;
					}
					else {
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
	/*}
	else
	{
		return false;
	}*/
}

bool JarvisVoice::Has_not_switched_enviroment(Tthrusters *jets) {
	CPed *player;
	player = FindPlayerPed(0);
	static int envi;
	if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_IN_WATER, player))
		envi = 1;
	else
		envi = 0;
	if (envi == jets->enviroment_ID)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool was_player_damaged(float *damage, float *damage1) {
	static int contador;

	CPed *player;
	player = FindPlayerPed(0);

	if ((player->m_fHealth < *damage) || (player->m_fArmour < *damage1))
	{
		*damage = player->m_fHealth;
		*damage1 = player->m_fArmour;
		contador = CTimer::m_snTimeInMillisecondsNonClipped;
		if (jarvisvoice.IsAudioVoicePlayed(19) == true)
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

float JarvisVoice::newangleposforflight() {
	CPed *player = FindPlayerPed(0);
	if (!boolvars.vueloconmouse)
	{
		static int x, y, specialx, specialy;
		static float orz, xangle, orientz;
		plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &orientz);
		if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 0) == true)
		{
			plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
			x *= -1;
			orientz += (float)x / 2800.0f / 3.1415926535897932384f * 180.0f;
		}
		return orientz;
	}
	else
	{
		CVector campos, pointat;
		CVector2D aimcam;
		static float angle, dist;
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &campos.x, &campos.y, &campos.z);
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &pointat.x, &pointat.y, &pointat.z);

		aimcam.x = pointat.x - campos.x;
		aimcam.y = pointat.y - campos.y;

		if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 19) == true)
		{
			return (atan2(aimcam.y, aimcam.x) - 4.71238898038469f) / 3.1415926535897932384f * 180.0f;
		}
		else
		{
			return (atan2(aimcam.y, aimcam.x) + 4.71238898038469f) / 3.1415926535897932384f * 180.0f;
		}
	}
}

float JarvisVoice::newanglepos() {

	CVector campos, pointat;
	CVector2D aimcam;
	static float angle, dist;
	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &campos.x, &campos.y, &campos.z);
	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &pointat.x, &pointat.y, &pointat.z);

	aimcam.x = pointat.x - campos.x;
	aimcam.y = pointat.y - campos.y;
	if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 19) == true)
	{
		return atan2(aimcam.y, aimcam.x) - 4.71238898038469f;
	}
	else
	{
		return atan2(aimcam.y, aimcam.x) + 4.71238898038469f;
	}
}
static float newangle1();

float JarvisVoice::newangle2() {
	static float angle;

	CVector camvec, camaimpoint, pointat;
	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &pointat.x, &pointat.y, &pointat.z);

	TheCamera.Find3rdPersonCamTargetVector(100, pointat, &camvec, &camaimpoint);

	float hd1 = camaimpoint.x - pointat.x;
	float hd2 = camaimpoint.y - pointat.y;
	plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, hd1, hd2, &angle);
	
	return angle;
}

float  newangle1() {
	static float angle;

	CVector camvec, camaimpoint, pointat;
	plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &pointat.x, &pointat.y, &pointat.z);

	TheCamera.Find3rdPersonCamTargetVector(100, pointat, &camvec, &camaimpoint);

	float hd1 = camaimpoint.x - pointat.x;
	float hd2 = camaimpoint.y - pointat.y;
	plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, hd1, hd2, &angle);

	if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 19) == true)
	{
		return angle + 180.0f;
	}
	else
	{
		return angle;
	}
}

bool JarvisVoice::has_obstacles(CPed *player, float offsetx, float offsety, float offsetz, int solid, int car, int actor, int object, int particle) {
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

static bool obj_has_obstacles(int obj, float offsetx, float offsety, float offsetz, int solid, int car, int actor, int object, int particle);

bool obj_has_obstacles(int obj, float offsetx, float offsety, float offsetz, int solid, int car, int actor, int object, int particle) {
	float coordx, coordy, coordz;
	CVector position;
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS, obj, offsetx, offsety, offsetz, &coordx, &coordy, &coordz);
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS, obj, 0.0f, 0.0f, 0.0f, &position.x, &position.y, &position.z);
	if ((plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, position.x, position.y, position.z, coordx, coordy, coordz, solid, car, actor, object, particle)) == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void JarvisVoice::setvelocityindirection(CPed *player, float velocx, float velocy, float velocz)
{
	float coordx, coordy, coordz;
	CVector playpos;
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, velocx, velocy, velocz, &coordx, &coordy, &coordz);
	plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &playpos.x, &playpos.y, &playpos.z);
	coordx -= playpos.x;
	coordy -= playpos.y;
	coordz -= playpos.z;
	plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, coordx, coordy, coordz);
}

void JarvisVoice::Set_XZ_cam_angle_to_Actor(CPed *player) {
	static float x, y, z, newangle = TheCamera.GetHeading();
	CMatrixLink *matrix = TheCamera.m_matrix;
	plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, matrix->up.x, matrix->up.y, matrix->up.z);
}

bool JarvisVoice::has_not_switched_weapon(int wid)
{
	static int wpn1;
	wpn1 = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
	if (wid == wpn1) {
		return true;
	}
	else
	{
		return false;
	}
}

float JarvisVoice::GetXAngleforopcodes() {
	CPed *player = FindPlayerPed(0);
	if (!boolvars.vueloconmouse)
	{
		static float orx, ory, orz;
		player->GetOrientation(orx, ory, orz);
		
		if (orx <= 1.0)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 1) == true)
			{
				static int x, y, specialx, specialy;
				plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
				if (y > 0)
				{
					return 6.0f;
				}
				if (y < 0)
				{
					return 2.0f;
				}
				return 2.0f;
			}
			else
			{
				return 0.0f;
			}
		}
		else
		{
			if (orx >= 6.0f)
			{
				return 6.0f;
			}
			else
			{
				return 2.0f;
			}
		}
	}
	else
	{
		float camposx, camposy, camposz, playerposx, playerposy, playerposz, distx, disty;
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &camposx, &camposy, &camposz);
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &playerposx, &playerposy, &playerposz);
		plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, camposx, camposy, playerposx, playerposy, &distx);


		disty = camposz - playerposz;
		float xang = CGeneral::GetATanOfXY(distx, disty);
		return xang;
	}
}

float JarvisVoice::GetXAngle() {
	CPed *player = FindPlayerPed(0);
	if (!boolvars.vueloconmouse)
	{
		static int x, y, specialx, specialy;
		static float orx, ory, orz;

		player->GetOrientation(orx, ory, orz);

		if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 1) == true)
		{
			plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
			if (orx >= -1.5f && orx <= 1.5f)
			{
				orx -= (float)y / 2800.0f;
			}
			else
			{
				if (orx <= -1.5f)
				{
					if (y < 0)
					{
						orx += (float)y / 2800.0f;
					}
					else
					{
						orx = -1.5f;
					}
				}
				
				if (orx >= 1.5f)
				{
					if (y > 0)
					{
						orx += (float)y / 2800.0f;
					}
					else
					{
						orx = 1.5f;
					}
				}
			}
		}
		else
		{
			orx *= 0.75;
		}
		
		return orx;
	}
	else
	{
		static float camposx, camposy, camposz, playerposx, playerposy, playerposz, distx, disty;
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &camposx, &camposy, &camposz);
		plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &playerposx, &playerposy, &playerposz);
		plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, camposx, camposy, playerposx, playerposy, &distx);
		disty = -playerposz + camposz;
		CPad *pad = CPad::GetPad(0);
		if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 19) == true)
		{
			return (CGeneral::GetATanOfXY(distx, disty));
		}
		else
		{
			return -(CGeneral::GetATanOfXY(distx, disty));
		}
	}
}

float JarvisVoice::GetYAngle() {
	CPed *player = FindPlayerPed(0);
	static float turnrot;
	if (!boolvars.vueloconmouse)
	{
		static float orx, orz;

		if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 0) == true)
		{
			static int x, y, specialx, specialy;
			plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
			
			turnrot += (float)x / 2800.0f;
			if (turnrot <= -2.0)
			{
				turnrot = -2.0f;
			}
			if (turnrot >= 2.0)
			{
				turnrot = 2.0f;
			}
			return turnrot;
		}
		else
		{
			turnrot *= 0.75;
			return turnrot;
		}
	}
	else
	{
		float movspeed = 0.0008f;
		float movx, movy;
		plugin::scripting::CallCommandById(COMMAND_GET_PC_MOUSE_MOVEMENT, &movx, &movy);
		int movfactor = 0;

		movfactor = plugin::patch::GetInt(11987996, false);
		movfactor /= 10000000;
		movx *= (float)movfactor;
		movx *= movspeed;
		turnrot += movx;
		turnrot *= 0.75;
		if (turnrot <= -2.0)
		{
			turnrot = -2.0f;
		}
		if (turnrot >= 2.0)
		{
			turnrot = 2.0f;
		}
		return turnrot;
	}
}

bool JarvisVoice::Va_adelante() {
	int x, y, specialx, specialy;
	plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
	if (y < 0) {
		return true;
	}
	else {
		return false;
	}
}

bool JarvisVoice::Va_atras() {
	int x, y, specialx, specialy;
	plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
	if (y > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool JarvisVoice::Va_izquierda() {
	int x, y, specialx, specialy;
	plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
	if (x < 0) {
		return true;
	}
	else {
		return false;
	}
}

bool JarvisVoice::Va_derecha() {
	int x, y, specialx, specialy;
	plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
	if (x > 0) {
		return true;
	}
	else {
		return false;
	}
}

short zDelta2;
short zDelta = 0;
short zDelta1 = 0;

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_MOUSEWHEEL)
		{
			MSLLHOOKSTRUCT *pMhs = (MSLLHOOKSTRUCT *)lParam;
			zDelta += HIWORD(pMhs->mouseData);

		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}



void JarvisVoice::AimedPedReaction(CVector posn, int *vict) {}

void JarvisVoice::AimedPedReactiontwo(int *vict) {
}

static int weaponid;

void JarvisVoice::Display_targets_on_screen(int wid, int storedtargets[], CSprite2d *target, CSprite2d *blocked)
{

	CVector playercoords = FindPlayerCoors(0);
	RwV3d coords;
	CVector2D coord2d;
	float w, h;
	for (int i = 1; i < 5; i++)
	{
		if (storedtargets[i] != NULL)
		{
			if (wid == 5) {
				if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) == true) {
					CPed *objective = CPools::GetPed(storedtargets[i]);
					objective->GetBonePosition(coords, 5, false);
					coordstoscreen(coords.x, coords.y, coords.z, &coord2d.x, &coord2d.y, &w, &h);
					float j = (w);
					float k = (w);
					if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, playercoords.x, playercoords.y, playercoords.z, coords.x, coords.y, coords.z, 1, 1, 0, 1, 0) == true) {
						movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), target, i);
					}
					else {
						movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), blocked, i);
					}
				}
			}
			else
			{
				if (wid == 3) {
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) == true) {
						plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &coords.x, &coords.y, &coords.z);
						coordstoscreen(coords.x, coords.y, coords.z, &coord2d.x, &coord2d.y, &w, &h);
						float j = (w);
						float k = (w);
						if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, playercoords.x, playercoords.y, playercoords.z, coords.x, coords.y, coords.z, 1, 0, 1, 1, 0) == true) {
							movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), target, i);
						}
						else {
							movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), blocked, i);
						}
					}
				}
				else
				{
					if (wid == 0) {
						if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[1]) == true) {
							CPed *objective = CPools::GetPed(storedtargets[1]);
							objective->GetBonePosition(coords, 5, false);
							coordstoscreen(coords.x, coords.y, coords.z, &coord2d.x, &coord2d.y, &w, &h);
							float j = (w);
							float k = (w);
							if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, playercoords.x, playercoords.y, playercoords.z, coords.x, coords.y, coords.z, 1, 1, 0, 1, 0) == true) {
								movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), target, 1);
							}
							else {
								movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), blocked, 1);
							}
						}
						else
						{
							if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[1]) == true) {
								plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &coords.x, &coords.y, &coords.z);
								coordstoscreen(coords.x, coords.y, coords.z, &coord2d.x, &coord2d.y, &w, &h);
								float j = (w);
								float k = (w);
								if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, playercoords.x, playercoords.y, playercoords.z, coords.x, coords.y, coords.z, 1, 0, 1, 1, 0) == true) {
									movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), target, 1);
								}
								else {
									movtextures.drawsizedtexture(CRect((coord2d.x - SCREEN_COORD(j / 2)), (coord2d.y - SCREEN_COORD(k / 2)), (coord2d.x + SCREEN_COORD(j / 2)), (coord2d.y + SCREEN_COORD(k / 2))), blocked, 1);
								}
							}
						}
					}
				}
			}
		}
	}
}

int *JarvisVoice::Disable_targets(int storedtargets[])
{

	for (int i = 0; i < 5; i++)
	{
		storedtargets[i] = 0;
	}
	return storedtargets;
}

int *JarvisVoice::Mark_targets(int wid, int current, int storedtargets[])
{
	CPed *player = FindPlayerPed(0);
	bool isstored = false;
	static int salud;
	wid = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
	if (wid == 5) {
		CVector posent;
		for (int i = 1; i < 5; i++)
		{
			if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) == true)
			{
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, storedtargets[i], &salud);
				if (salud <= 0)
				{
					storedtargets[i] = 0;
				}
				else
				{
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, storedtargets[i], &posent.x, &posent.y, &posent.z);
					float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
					if (dist2 > 50.0f) {
						storedtargets[i] = 0;
					}
				}
			}
			if (current == storedtargets[i]) {
				isstored = true;
				break;
			}
		}
		if (isstored == false)
		{
			if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, current) == true)
			{
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, current, &posent.x, &posent.y, &posent.z);
				float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
				if (dist2 <= 50.0f) {
					for (int i = 1; i < 5; i++)
					{
						if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) != true) {
							JarvisVoice::PlayAudiostream(audiostream[targetSet], 0, &IMStream[0]);
							storedtargets[i] = current;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		if (wid == 3)
		{
			CVector posent;
			for (int i = 1; i < 5; i++)
			{
				if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) == true)
				{
					plugin::scripting::CallCommandById(COMMAND_GET_CAR_HEALTH, storedtargets[i], &salud);
					if (salud <= 0)
					{
						storedtargets[i] = 0;
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &posent.x, &posent.y, &posent.z);
						float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
						if (dist2 > 50.0f) {
							storedtargets[i] = 0;
						}
					}
				}
				if (current == storedtargets[i]) {
					isstored = true;
					break;
				}
			}
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, current) == true)
			{
				if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, current) == 0) {
					plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, current, &posent.x, &posent.y, &posent.z);
					float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
					if (dist2 <= 50.0f) {
						if (isstored == false)
						{
							for (int i = 1; i < 5; i++)
							{
								if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) != true) {
									JarvisVoice::PlayAudiostream(audiostream[targetSet], 0, &IMStream[0]);
									storedtargets[i] = current;
									break;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if (wid == 0)
			{
				int i = 1;
				CVector posent;
				if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) == true)
				{
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, storedtargets[i], &posent.x, &posent.y, &posent.z);
					float dist1 = DistanceBetweenPoints(posent, boolvars.target);
					float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
					if (dist2 > 50.0f || dist1 > 1.0f) {
						storedtargets[i] = 0;
					}
				}
				else
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &posent.x, &posent.y, &posent.z);
						float dist1 = DistanceBetweenPoints(posent, boolvars.target);
						float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
						if (dist2 > 50.0f || dist1 > 5.0f) {
							storedtargets[i] = 0;
						}
					}
				}
				if (current == storedtargets[i]) {
					isstored = true;
				}

				if (isstored == false)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, current) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, current, &posent.x, &posent.y, &posent.z);
						float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
						if (dist2 <= 50.0f) {
							if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) != true) {
								JarvisVoice::PlayAudiostream(audiostream[targetSet], 0, &IMStream[0]);
								storedtargets[i] = current;
							}
						}
					}
					else
					{
						if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, current) == true)
						{
							plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, current, &posent.x, &posent.y, &posent.z);
							float dist2 = DistanceBetweenPoints(posent, player->GetPosition());
							if (dist2 <= 50.0f) {
								if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) != true) {
									JarvisVoice::PlayAudiostream(audiostream[targetSet], 0, &IMStream[0]);
									storedtargets[i] = current;
								}
							}
						}
					}
				}
			}
		}
	}
	return storedtargets;
}


void botshootsmissile(Tbotrockets *rocket, bool *completed);

void botshootsmissile(Tbotrockets *rocket, bool *completed)
{
	CPed *player = FindPlayerPed(0);
	if ((int)rocket->objective == (int)player)
	{
		if (*completed == false && boolvars.suit[1])
		{
			static int timedam;
			if (CTimer::m_snTimeInMillisecondsNonClipped > timedam + 100)
			{
				if (CTimer::m_snTimeInMillisecondsNonClipped > timedam + 150)
				{
					timedam = CTimer::m_snTimeInMillisecondsNonClipped;
				}
				JarvisVoice::PlayBeep(&JarvisMalarm);
				malarm->Draw(CRect(SCREEN_WIDTH - SCREEN_HEIGHT / 2.0f,
					SCREEN_COORD(0.0f),
					SCREEN_WIDTH,
					SCREEN_HEIGHT / 2.0f), CRGBA(255, 255, 255, 255));
			}
		}
	}

	CVector misilpos;
	float misilangz;
	if (CTimer::m_snTimeInMillisecondsNonClipped < rocket->timecreated + 5000)
	{
		if (CTimer::m_snTimeInMillisecondsNonClipped < rocket->timecreated + 1000)
		{
			rocket->recordcol = 1;
			CVector misilvel;
			if (rocket->objective != NULL)
			{
				CVector direction = rocket->objective->GetPosition();
				if (rocket->misil != NULL)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->misil) == true)
					{
						if (rocket->recordcol != 0)
						{
							rocket->recordcol = 0;
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, rocket->misil, rocket->recordcol);
						}
						plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->misil, &misilpos.x, &misilpos.y, &misilpos.z);
						misilvel.x = (direction.x - misilpos.x)*2.0f;
						misilvel.y = (direction.y - misilpos.y)*2.0f;
						misilvel.z = (direction.z - misilpos.z)*2.0f;
						rocket->velprevx = misilvel.x;
						rocket->velprevy = misilvel.y;
						rocket->velprevz = misilvel.z;
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->misil, misilvel.x, misilvel.y, misilvel.z);
						*completed = false;
					}
				}
			}
		}
		else
		{
			CVector misilvel;
			if (rocket->objective != NULL)
			{
				CVector direction = rocket->objective->GetPosition();
				if (rocket->misil != NULL)
				{
					plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_HEADING, rocket->misil, &misilangz);
					if (rocket->recordcol != 1)
					{
						rocket->recordcol = 1;
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, rocket->misil, rocket->recordcol);
					}
					plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->misil, &misilpos.x, &misilpos.y, &misilpos.z);
					if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, misilpos.x, misilpos.y, misilpos.z, direction.x, direction.y, direction.z, 1, 1, 1, 1, 0) == true)
					{
						misilvel.x = (direction.x - misilpos.x)*2.0f;
						misilvel.y = (direction.y - misilpos.y)*2.0f;
						misilvel.z = (direction.z - misilpos.z)*2.0f;
						rocket->velprevx = misilvel.x;
						rocket->velprevy = misilvel.y;
						rocket->velprevz = misilvel.z;
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->misil, misilvel.x, misilvel.y, misilvel.z);
						*completed = false;
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, misilpos.x, misilpos.y, misilpos.z, 10);
						plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->misil);
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->misil);
						rocket->objective = NULL;
						rocket->misil = NULL;
						rocket->timecreated = 0;
						*completed = true;
						if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, misilpos.x, misilpos.y, misilpos.z, 10.0f, 10.0f, 10.0f, 0) == true)
						{
							boolvars.anglecohete = misilangz;
							boolvars.cohetebotexplotado = true;

							static float hadin;
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

							if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
								boolvars.vuelaatras = true;
							else
								boolvars.vuelaatras = false;

							boolvars.landgetup = 1;
						}
					}
				}
				else
				{
					rocket->objective = NULL;
					rocket->timecreated = 0;
					*completed = true;
				}
			}
			else
			{
				rocket->objective = NULL;
				CVector direction;
				if (rocket->misil != NULL)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->misil) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_HEADING, rocket->misil, &misilangz);

						plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->misil, &misilpos.x, &misilpos.y, &misilpos.z);
						plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, misilpos.x, misilpos.y, misilpos.z, &direction.z);
						direction.x = misilpos.x + rocket->velprevx * 0.5f;
						direction.y = misilpos.y + rocket->velprevy * 0.5f;
						float directionz = misilpos.z + (direction.z - misilpos.z) + rocket->velprevz * 0.5f;

						if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, misilpos.x, misilpos.y, misilpos.z, direction.x, direction.y, directionz, 1, 1, 1, 1, 0) == true)
						{
							misilvel.x = rocket->velprevx * 0.5f;
							misilvel.y = rocket->velprevy * 0.5f;
							misilvel.z = (direction.z - misilpos.z) + rocket->velprevz * 0.5f;
							rocket->velprevx = misilvel.x;
							rocket->velprevy = misilvel.y;
							rocket->velprevz = misilvel.z;
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->misil, misilvel.x, misilvel.y, misilvel.z);
							*completed = false;
						}
						else
						{
							plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, misilpos.x, misilpos.y, misilpos.z, 10);
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->misil);
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->misil);
							rocket->timecreated = 0;
							rocket->misil = NULL;
							*completed = true;
							if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, misilpos.x, misilpos.y, misilpos.z, 10.0f, 10.0f, 10.0f, 0) == true)
							{
								boolvars.anglecohete = misilangz;
								boolvars.cohetebotexplotado = true;

								static float hadin;
								plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

								if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
									boolvars.vuelaatras = true;
								else
									boolvars.vuelaatras = false;

								boolvars.landgetup = 1;

							}
						}
					}
					else
					{
						rocket->misil = NULL;
						*completed = true;
					}
				}
				else
				{
					*completed = true;
				}
			}
		}
	}
	else
	{
		if (rocket->misil != NULL)
		{
			if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->misil) == true)
			{
				plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_HEADING, rocket->misil, &misilangz);

				plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->misil, &misilpos.x, &misilpos.y, &misilpos.z);
				plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, misilpos.x, misilpos.y, misilpos.z, 10);
				plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->misil);
				plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->misil);
				
				*completed = true;
				rocket->misil = NULL;
			}
			else
			{
				rocket->misil = NULL;
				*completed = true;
			}
		}
		else
		{
			*completed = true;
		}
	}
}

int *JarvisVoice::Neutralize_targets(int wid, int storedtargets[], vector<int>*rocket, bool *completed) {
	CPed *player;
	player = FindPlayerPed(0);
	CVector playerpos;
	CVector veloc;
	CVector speeed;
	static bool fallos[5];
	static bool success[5];
	static bool c[5];
	static float head;
	static bool d[5];
	static bool created[5];
	static bool init;
	static unsigned int timestart;
	static unsigned int timeanim[5];
	static int taskstuff;
	float fx, fy;
	if (!init)
	{
		for (int i = 1; i < 5; i++) {
			rocket->at(i) = 0;
		}
		init = true;
	}

	if (wid == 5) {
		static int salud = 0;
		for (int i = 1; i < 5; i++) {
			if (rocket->at(i) == 0)
			{
				if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) != true || storedtargets[i] <= 0)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
						rocket->at(i) = 0;
					}
					storedtargets[i] = 0;
					c[i] = true;
					d[i] = true;
					fallos[i] = true;
				}
				else
				{
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, storedtargets[i], &salud);
					if (salud <= 0)
					{
						storedtargets[i] = 0;
					}
					else
					{
						RwV3d coords1;
						coords1.x = 0.185f;
						coords1.y = 0.08f;
						coords1.z = 0.0f;
						CColPoint colpnt1;
						CEntity *pedcolliding1 = NULL;
						player->GetTransformedBonePosition(coords1, 3, false);
						playerpos.x = coords1.x;
						playerpos.y = coords1.y;
						playerpos.z = coords1.z;

						*completed = true;
						JarvisVoice::PlayAudiostream(audiostream[darts], 0, &IMStream[0]);
						plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 345, playerpos.x, playerpos.y, playerpos.z, &rocket->at(i));

						objectlist[dim] = rocket->at(i);
						dim++;
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, rocket->at(i), 1);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, rocket->at(i), 1);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_TURN_MASS, rocket->at(i), 0.0f);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, rocket->at(i), 1);


						timestart = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.timetowait = 50;




						CVector dir;
						CVector origin;
						plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);

						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, storedtargets[i], &dir.x, &dir.y, &dir.z);

						plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);
						fy = dir.z - origin.z;

						float xangle = (CGeneral::GetATanOfXY(fy, fx));

						float headx = dir.x - origin.x;
						float heady = dir.y - origin.y;
						plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, rocket->at(i), xangle, 2, head);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, rocket->at(i), head);
						speeed.x = (dir.x - origin.x)*10.0f;
						speeed.y = (dir.y - origin.y)*10.0f;
						speeed.z = (dir.z - origin.z)*10.0f;
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->at(i), speeed.x, speeed.y, speeed.z);
						
					}
				}
			}
			else
			{

				if ((timestart + 2500) <= CTimer::m_snTimeInMillisecondsNonClipped)
				{
					for (i = 1; i < 5; i++) {
						if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)) == true)
						{
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
							rocket->at(i) = 0;
						}
						storedtargets[i] = 0;
						c[i] = true;
						d[i] = true;
						fallos[i] = true;
					}
					break;
				}
				else
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) != true || storedtargets[i] <= 0)
					{
						if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)) == true)
						{
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
							rocket->at(i) = 0;
						}
						storedtargets[i] = 0;
						c[i] = true;
						d[i] = true;
					}
					else
					{

						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, storedtargets[i], &salud);
						if (salud <= 0)
						{
							storedtargets[i] = 0;
						}
						else
						{
							CVector origin;
							plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);
							if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, storedtargets[i], origin.x, origin.y, origin.z, 1.5f, 1.5f, 1.5f, 0) == true)
							{
								if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, storedtargets[i], "KILL_KNIFE_PED_DAMAGE") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, storedtargets[i], "KILL_KNIFE_PED_DIE") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, storedtargets[i], "KO_SKID_BACK") == false)
								{
									plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, storedtargets[i]);
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEALTH, storedtargets[i], 0);
									timeanim[i] = CTimer::m_snTimeInMillisecondsNonClipped;
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_BLEEDING, storedtargets[i], 1);
									plugin::scripting::CallCommandById(COMMAND_TASK_SAY, storedtargets[i], 342);
									plugin::scripting::CallCommandById(COMMAND_EXPLODE_CHAR_HEAD, storedtargets[i]);
									success[i] = true;
								}
								c[i] = true;
							}
							else
							{
								if (plugin::scripting::CallCommandById(COMMAND_HAS_OBJECT_COLLIDED_WITH_ANYTHING, rocket->at(i)) == true)
								{
									fallos[i] = true; //missed
									c[i] = true;
									d[i] = true;
									plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
									plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
									rocket->at(i) = 0;
									c[i] = true;

								}
								else
								{
									CVector dir;
									CVector origin;
									plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);
									plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, storedtargets[i], &dir.x, &dir.y, &dir.z);
									
									plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);
									fy = dir.z - origin.z;

									float xangle = (CGeneral::GetATanOfXY(fy, fx));
									float headx = dir.x - origin.x;
									float heady = dir.y - origin.y;
									plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, rocket->at(i), xangle, 2, head);
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, rocket->at(i), head);
									speeed.x = (dir.x - origin.x)*10.0f;
									speeed.y = (dir.y - origin.y)*10.0f;
									speeed.z = (dir.z - origin.z)*10.0f;
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->at(i), speeed.x, speeed.y, speeed.z);
								}
							}
							if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) == false)
							{
								storedtargets[i] = 0;
								if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)) == true)
								{
									plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
									plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
									rocket->at(i) = 0;
								}
								storedtargets[i] = 0;
								c[i] = true;
								d[i] = true;
							}
						}
					}
				}
			}


			if (d[i] == false && c[i] == true && !fallos[i])
			{
				if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[i]) == true && storedtargets[i] > 0) {
					if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, storedtargets[i]) == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, storedtargets[i], "KILL_KNIFE_PED_DAMAGE") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, storedtargets[i], "KILL_KNIFE_PED_DIE") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, storedtargets[i], "KO_SKID_BACK") == false)
					{
						if ((timeanim[i] + 500) <= CTimer::m_snTimeInMillisecondsNonClipped)
						{
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, storedtargets[i]);
							int salud1;
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, storedtargets[i], &salud1);
							if (salud == salud1) {
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEALTH, storedtargets[i], 0);
							}
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_HEALTH_GREATER, storedtargets[i], 1000))
							{
								plugin::scripting::CallCommandById(COMMAND_DAMAGE_CHAR, storedtargets[i], 1000, 1);
							}
							plugin::scripting::CallCommandById(COMMAND_TASK_DIE_NAMED_ANIM, storedtargets[i], "KO_SKID_BACK", "PED", 4.0f, -1);
							plugin::scripting::CallCommandById(COMMAND_TASK_SAY, storedtargets[i], 342);
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_BLEEDING, storedtargets[i], 1);
							storedtargets[i] = 0;
							d[i] = true;
						}
					}
					else
					{
						storedtargets[i] = 0;
						d[i] = true;
					}
				}
				else
				{
					if (rocket->at(i) > 0) {
						if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)) == true)
						{
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
							rocket->at(i) = 0;
						}
					}
					c[i] = true;
					d[i] = true;
				}
			}
			else
			{
				if (fallos[i] == true)
				{
					c[i] = true;
					d[i] = true;
				}
			}
		}

		if (d[1] == true && d[2] == true && d[3] == true && d[4] == true)
		{
			for (int i = 1; i < 5; i++) {
				d[i] = false;
				c[i] = false;
				fallos[i] = false;
				created[i] = false;
				success[i] = false;
				if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)))
				{
					plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
					plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
					rocket->at(i) = 0;
				}
			}
			*completed = false;
		}
	}
	else
	{
		if (wid == 3)
		{
			CVehicle *car = NULL;
			for (int i = 1; i < 5; i++) {
				if (rocket->at(i) == 0)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) != true || storedtargets[i] <= 0)
					{
						storedtargets[i] = 0;
						d[i] = true;
						rocket->at(i) = 0;
					}
					else
					{
						car = CPools::GetVehicle(storedtargets[i]);
						if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, car) == false && car->m_fHealth > 0) {
							if (c[i] == false)
							{
								RwV3d coords1;
								coords1.x = 0.185f;
								coords1.y = 0.08f;
								coords1.z = 0.0f;
								CColPoint colpnt1;
								CEntity *pedcolliding1 = NULL;
								player->GetTransformedBonePosition(coords1, 3, false);
								playerpos.x = coords1.x;
								playerpos.y = coords1.y;
								playerpos.z = coords1.z;
								*completed = true;
								JarvisVoice::PlayAudiostream(audiostream[launchM], 0, &IMStream[0]);
								plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 345, playerpos.x, playerpos.y, playerpos.z, &rocket->at(i));
								objectlist[dim] = rocket->at(i);
								dim++;
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, rocket->at(i), 1);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, rocket->at(i), 1);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_TURN_MASS, rocket->at(i), 0.0f);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, rocket->at(i), 1);
								c[i] = true;

								timestart = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 50;

								plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &head);
								CVector vel;
								CVector dir;
								CVector origin;
								plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);

								plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &dir.x, &dir.y, &dir.z);

								plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);
								fy = dir.z - origin.z;

								float xangle = (CGeneral::GetATanOfXY(fy, fx));
								float headx = dir.x - origin.x;
								float heady = dir.y - origin.y;
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, rocket->at(i), xangle, 2, head);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, rocket->at(i), head);
								speeed.x = (dir.x - origin.x)*10.0f;
								speeed.y = (dir.y - origin.y)*10.0f;
								speeed.z = (dir.z - origin.z)*10.0f;
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->at(i), speeed.x, speeed.y, speeed.z);
								
							}
						}
						else
						{
							storedtargets[i] = 0;
							d[i] = true;
						}
					}
				}
				else
				{
					CVector origin;
					if ((timestart + 15000) <= CTimer::m_snTimeInMillisecondsNonClipped)
					{
						i = 5;
						*completed = false;
						int j = 1;
						while (j < 5) {
							if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(j)) == true)
							{
								plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(j), &origin.x, &origin.y, &origin.z);
								plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, origin.x, origin.y, origin.z, 10);
								plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(j));
								plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(j));
							}
							if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[j]) == true) {
								plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, storedtargets[j]);
							}
							c[j] = false;
							rocket->at(j) = 0;
							storedtargets[j] = 0;
							d[j] = true;
							j++;
						}
					}
					else
					{
						if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)) == true)
						{
							if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) != true || storedtargets[i] <= 0)
							{
								plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);
								plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, origin.x, origin.y, origin.z, 10);
								plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
								plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
								rocket->at(i) = 0;
								storedtargets[i] = 0;
								d[i] = true;
							}
							else
							{
								car = CPools::GetVehicle(storedtargets[i]);
								if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, car) == true || car->m_fHealth < 1)
								{
									if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)))
									{
										plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
										plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
										rocket->at(i) = 0;
									}
									rocket->at(i) = 0;
									storedtargets[i] = 0;
									d[i] = true;
								}
								else
								{
									CVector dir;
									CVector vel;
									plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);
									plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &dir.x, &dir.y, &dir.z);
									CVector charobstacle = player->GetPosition();
									float dist = DistanceBetweenPoints(charobstacle, origin);
									float distchar = DistanceBetweenPoints(player->GetPosition(), origin);
									if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, car) == false && distchar <= 50.f && car->m_fHealth > 0) {
										if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, origin.x, origin.y, origin.z, dir.x, dir.y, dir.z, 1, 1, 1, 1, 0) == true && dist <= 15.0 || plugin::scripting::CallCommandById(COMMAND_LOCATE_OBJECT_3D, rocket->at(i), dir.x, dir.y, dir.z, 3.0f, 3.0f, 3.0f, 0) == false) {


											plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);

											plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, storedtargets[i], &dir.x, &dir.y, &dir.z);

											plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);
											fy = dir.z - origin.z;

											float xangle = (CGeneral::GetATanOfXY(fy, fx));

											float headx = dir.x - origin.x;
											float heady = dir.y - origin.y;
											plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, rocket->at(i), xangle, 2, head);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, rocket->at(i), head);
											speeed.x = (dir.x - origin.x)*10.0f;
											speeed.y = (dir.y - origin.y)*10.0f;
											speeed.z = (dir.z - origin.z)*10.0f;
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, rocket->at(i), speeed.x, speeed.y, speeed.z);
											
										}
										else
										{
											plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, origin.x, origin.y, origin.z, 10);
											plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
											plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
											rocket->at(i) = 0;
											success[i] = true;
											plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, storedtargets[i]);
											storedtargets[i] = 0;
											d[i] = true;
										}
									}
									else
									{
										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, rocket->at(i), &origin.x, &origin.y, &origin.z);
										plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, origin.x, origin.y, origin.z, 10);
										plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
										plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
										rocket->at(i) = 0;
										storedtargets[i] = 0;
										d[i] = true;
									}
								}
							}
						}
						else
						{
							if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[i]) == true)
							{
								plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, storedtargets[i]);
							}
							storedtargets[i] = 0;
							rocket->at(i) = 0;
							d[i] = true;
						}
					}
				}
			}

			if (d[1] == true && d[2] == true && d[3] == true && d[4] == true)
			{
				for (int i = 1; i < 5; i++) {
					d[i] = false;
					c[i] = false;
					fallos[i] = false;
					created[i] = false;
					success[i] = false;
					if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, rocket->at(i)))
					{
						plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, rocket->at(i));
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
					}
					rocket->at(i) = 0;
					car = CPools::GetVehicle(storedtargets[i]);
					if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, car) == false && car->m_fHealth > 0) {
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, rocket->at(i));
					}
					storedtargets[i] = 0;
				}
				*completed = false;
			}
		}
		else
		{
			if (player && wid == 0)
			{
				static int auto1;
				storedtargets[0] = 0;
				storedtargets[2] = 0;
				storedtargets[3] = 0;
				storedtargets[4] = 0;
				static int salud = 0;
				static int nadapedauto;
				
				

				if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, storedtargets[1]) != 1)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, storedtargets[1]) != true || storedtargets[1] <= 0)
					{
						storedtargets[1] = 0;
						auto1 = storedtargets[1];
						*completed = false;
					}
					else
					{
						auto1 = storedtargets[1];
					}
				}
				else
				{
					auto1 = storedtargets[1];
				}

				if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, auto1) != 1)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, auto1) != 1 || auto1 <= 0)
					{
						auto1 = 0;
						*completed = false;
					}
					else
					{
						*completed = true;
						static float fz;
						static bool explo, done;
						CVector dir;
						plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, auto1, &dir.x, &dir.y, &dir.z);

						if (done == false
							&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_punchcar") == 0)//&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_uppercut") == 0)
						{
							CVector origin;
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, player, &origin.x, &origin.y, &origin.z);


							plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, origin.x, origin.y, origin.z, dir.x, dir.y, dir.z, &fz);

							if (fz < 100.0f)
							{
								CVehicle *car = CPools::GetVehicle(auto1);
								
								if (fz <= 3.0f && car->GetVehicleAppearance() != 1)
								{
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_punchcar", "ironman", 10.0f, 0, 0, 0, 0, -1);
									explo = true;
									done = true;
									*completed = true;
									boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
									boolvars.timetowait = 300;
									storedtargets[1] = auto1;
									return storedtargets;
								}

								for (int i = 0; i < 6; i++)
								{
									CEntity *entidad = player->m_apCollidedEntities[i];
									int entityfound = (int)entidad;
									if (entityfound == (int)car &&
										plugin::scripting::CallCommandById(COMMAND_IS_CHAR_TOUCHING_VEHICLE, player, auto1) == true)
									{										
										done = true;
										break;
									}
								}
								if (done == true)
								{
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_punchcar", "ironman", 10.0f, 0, 0, 0, 0, -1);
									explo = true;
									*completed = true;
									boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
									boolvars.timetowait = 300;
									return storedtargets;
								}

								if (jarvisvoice.has_obstacles(player, 0.0f, 3.0f, 0.0f, 1, 0, 1, 1, 0) == true)
								{
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Stop", "ironman", 10.0f, 0, 0, 0, 0, -1);
									done = true;
									explo = false;
									*completed = true;
									return storedtargets;
								}

								if (is_over_ground(2.0f))
								{
									plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);


									fy = dir.z - origin.z;

									float xangle = (CGeneral::GetATanOfXY(fy, fx));


									float headx = dir.x - origin.x;
									float heady = dir.y - origin.y;
									plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
									speeed.x = (dir.x - origin.x);
									speeed.y = (dir.y - origin.y);
									speeed.z = (dir.z - origin.z);

									veloc.x = speeed.x / speeed.Magnitude() * 50.0f;
									veloc.y = speeed.y / speeed.Magnitude() * 50.0f;
									veloc.z = speeed.z / speeed.Magnitude() * 50.0f;

									*completed = true;
									boolvars.iscjfrozen = false;
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, veloc.x, veloc.y, veloc.z);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_aimpunch", "ironman", 10.0f, 0, 1, 1, 0, -1);
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, head);
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, xangle, 0.0f, head);
									
								}
								else
								{
									plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);


									fy = dir.z - origin.z;

									float xangle = (CGeneral::GetATanOfXY(fy, fx));


									float headx = dir.x - origin.x;
									float heady = dir.y - origin.y;
									plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
									speeed.x = (dir.x - origin.x);
									speeed.y = (dir.y - origin.y);
									speeed.z = (dir.z - origin.z);

									veloc.x = speeed.x / speeed.Magnitude() * 50.0f;
									veloc.y = speeed.y / speeed.Magnitude() * 50.0f;
									veloc.z = speeed.z / speeed.Magnitude() * 50.0f;

									*completed = true;
									boolvars.iscjfrozen = false;
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_aimpunchSurf", "ironman", 10.0f, 0, 1, 1, 1, -1);
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, head);
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, veloc.x, veloc.y, veloc.z);
									
								}

							}
							else
							{
								explo = false;
								*completed = false;
								auto1 = 0;
							}
						}
						else
						{
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
							boolvars.timetowait = 20;
							if (explo == true || done == false)
							{
								explo = false;
								boolvars.punchedcar = auto1;
							}
							done = false;
							*completed = false;
							auto1 = 0;
						}
					}
				}
				else
				{
					*completed = true;
					static float fz;
					static bool explo, done, explo1;
					CVector dir;
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, auto1, &dir.x, &dir.y, &dir.z);
					
					if (done == false
						&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_punchped") == 0)
					{
						CVector origin;
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, player, &origin.x, &origin.y, &origin.z);

						
						plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, origin.x, origin.y, origin.z, dir.x, dir.y, dir.z, &fz);

						if (fz < 100.0f)
						{

							if (fz <= 3.0f)
							{
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_punchped", "ironman", 10.0f, 0, 0, 0, 0, -1);
								explo = true;
								done = true;
								*completed = true;
								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 100;
								return storedtargets;
							}
							
							if (jarvisvoice.has_obstacles(player, 0.0f, 3.0f, 0.0f, 1, 1, 0, 1, 0) == true)
							{
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Stop", "ironman", 10.0f, 0, 0, 0, 0, -1);
								done = true;
								*completed = true;
								return storedtargets;
							}

							if (is_over_ground(2.0f))
							{
								plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);


								fy = dir.z - origin.z;

								float xangle = (CGeneral::GetATanOfXY(fy, fx));


								float headx = dir.x - origin.x;
								float heady = dir.y - origin.y;
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
								speeed.x = (dir.x - origin.x);
								speeed.y = (dir.y - origin.y);
								speeed.z = (dir.z - origin.z);

								veloc.x = speeed.x / speeed.Magnitude() * 50.0f;
								veloc.y = speeed.y / speeed.Magnitude() * 50.0f;
								veloc.z = speeed.z / speeed.Magnitude() * 50.0f;

								*completed = true;
								boolvars.iscjfrozen = false;
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, veloc.x, veloc.y, veloc.z);
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_aimpunch", "ironman", 10.0f, 0, 1, 1, 0, -1);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, head);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, xangle, 0.0f, head);
								
							}
							else
							{
								plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);


								fy = dir.z - origin.z;

								float xangle = (CGeneral::GetATanOfXY(fy, fx));


								float headx = dir.x - origin.x;
								float heady = dir.y - origin.y;
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
								speeed.x = (dir.x - origin.x);
								speeed.y = (dir.y - origin.y);
								speeed.z = (dir.z - origin.z);

								veloc.x = speeed.x / speeed.Magnitude() * 50.0f;
								veloc.y = speeed.y / speeed.Magnitude() * 50.0f;
								veloc.z = speeed.z / speeed.Magnitude() * 50.0f;

								*completed = true;
								boolvars.iscjfrozen = false;
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_aimpunchSurf", "ironman", 10.0f, 0, 1, 1, 1, -1);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, head);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, veloc.x, veloc.y, veloc.z);
								
							}
							
						}
						else
						{
							plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, auto1);

							*completed = false;
							auto1 = 0;
						}
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						boolvars.timetowait = 20;
						if (explo == true || done==false)
						{
							explo = false;
							boolvars.punchedvictim = auto1;
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 1000;
						}
						done = false;
						*completed = false;
						plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, auto1);
						auto1 = 0;
					}
					storedtargets[1] = auto1;
				}
			}
		}
	}
	return storedtargets;
}

static int helper, helper12, vehic1;

bool JarvisVoice::got_car(int *car) {
	CPed *player;
	player = FindPlayerPed(0);
	static bool holdingcar;
	static int entityfound;
	static float dist;
	static int mdl;
	if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, *car))
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic1 != *car) {
			plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
			plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic1);
			vehic1 = NULL;
		}
		return true;
	}
	else {
		if (GetKeyState('E') & 0x8000)
		{
			CEntity *entidad = player->m_apCollidedEntities[1];
			entityfound = (int)entidad;
			CVector playerpos, carpos, carpos2;
			if (entityfound != 0) {
				if (entidad->m_nType == ENTITY_TYPE_VEHICLE) {
					CVehicle *aux = (CVehicle*)entidad;
					if (aux != NULL && CPools::ms_pVehiclePool->IsObjectValid(aux) == true) {
						if (aux->IsVisible() == true)
						{
							int vehiculo1 = CPools::GetVehicleRef(aux);
							if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic1 != vehiculo1) {
								plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
								plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic1);
								vehic1 = NULL;
							}
							if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, vehiculo1) == false)
							{
								if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_TOUCHING_VEHICLE, player, vehiculo1) == true) {
									*car = vehiculo1;
									return true;
								}
							}
							return false;
						}
						else
						{

							return false;
						}
					}
					else
					{
						*car = NULL;
						return false;
					}
				}
				else {
					if (entidad->m_nType == ENTITY_TYPE_PED) {
						CPed *nosirve = (CPed*)entidad;
						if (nosirve != NULL && CPools::ms_pPedPool->IsObjectValid(nosirve) == true) {
							plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, nosirve);
						}
					}
					else {
						if (entidad->m_nType == ENTITY_TYPE_OBJECT) {
							CObject *nosirve1 = (CObject*)entidad;
							if (nosirve1 != NULL && CPools::ms_pObjectPool->IsObjectValid(nosirve1) == true) {
								plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, nosirve1);
							}
						}
					}
					*car = NULL;
					return false;
				}
			}
			else {
				*car = NULL;
				return false;
			}
		}
		else {
			return false;
		}
	}
}

void Movtextures::Loadmenuicons()
{
	/*
	menu1 = new CSprite2d();
	menu1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\miniarrowh.png"));
	*/
	menu3 = new CSprite2d();
	menu3->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\nextarrows.png"));

	menu4 = new CSprite2d();
	menu4->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\nextarrowsh.png"));

	menu5 = new CSprite2d();
	menu5->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\board.png"));

	chosen = new CSprite2d();
	chosen->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\chosen.png"));

	notchosen = new CSprite2d();
	notchosen->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\chosenh.png"));

	closewdw = new CSprite2d();
	closewdw->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\miniarrow.png"));

	cursor = new CSprite2d();
	cursor->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\menuarrow.png"));

	signo = new CSprite2d();
	signo->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\unk1.png"));

	signo1 = new CSprite2d();
	signo1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\unk0.png"));

	marktony = new CSprite2d();
	marktony->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\Tony.png"));

	settingmenu = new CSprite2d();
	settingmenu->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\settings.png"));
	/*
	settingmenuh = new CSprite2d();
	settingmenuh->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\settingsh.png"));
	*/
	closebtn = new CSprite2d();
	closebtn->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\closebutn.png"));
	/*
	closebtnh = new CSprite2d();
	closebtnh->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\closebutnh.png"));
	*/

	imhud2 = new CSprite2d();
	imhud2->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Target\\Target01.png"));

	imhud3 = new CSprite2d();
	imhud3->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Target\\Target02.png"));

	malarm = new CSprite2d();
	malarm->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\PlayerInfo\\malarm.png"));

	
	fondo = new CSprite2d();
	fondo->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Background\\visor0.png"));

	fondo1 = new CSprite2d();
	fondo1->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Background\\visor1.png"));

	check = new CSprite2d();
	check->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\on.png"));
	uncheck = new CSprite2d();
	uncheck->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\off.png"));
	menusel = new CSprite2d();
	menusel->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\SuitMenu\\selector.png"));
	
}

bool JarvisVoice::aimironman() {
	static bool pact;
	static DWORD* pActor = (DWORD*)0xB6F5F0;
	static float i;
	static int indexfriends;
	if (!pact)
	{
		pact = true;
		i = 1.0f;
		indexfriends = 0;
		boolvars.timecreatedped = 0;
	}


	static bool justonce;
	if (!justonce)
	{
		

		JarvisVoice::storeenviroment(&propellers.enviroment_ID);
		
		justonce = true;
	}

	static bool langbol;

	int lang = 0;
	plugin::scripting::CallCommandById(COMMAND_GET_CURRENT_LANGUAGE, &lang);

	std::string lng = "IronMan\\text\\";
	
	if (langbol == false)
	{
		if (lang == 0)
		{
			lng += "american";
		}
		if (lang == 1)
		{
			lng += "french";
		}
		if (lang == 2)
		{
			lng += "german";
		}
		if (lang == 3)
		{
			lng += "italian";
		}
		if (lang == 4)
		{
			lng += "spanish";
		}
		
		lng += ".dat";
		plugin::config_file language(PLUGIN_PATH(lng.data()));
		
			boolvars.helptext1.clear();
			boolvars.helptext2.clear();
			boolvars.helptext3.clear();
			boolvars.helptext4.clear();
			boolvars.helptext5.clear();
			boolvars.helptext6.clear();
			boolvars.helptext7.clear();
			boolvars.helptext8.clear();
			boolvars.online.clear();
			boolvars.impulso.clear();
			boolvars.suitarmormenu.clear();
			boolvars.Clicoptions.clear();
			boolvars.Tonyplay.clear();
			boolvars.Notplay.clear();
			boolvars.Suitplay.clear();
			boolvars.Prevmenu.clear();
			boolvars.Nextmenu.clear();
			boolvars.Options.clear();
			boolvars.Closesets.clear();
			boolvars.menupag1.clear();
			boolvars.menupag2.clear();
			boolvars.distan.clear();
			boolvars.dronfar.clear();
			boolvars.helptext1 = language["imhelp1"].asString("Elegir arma");
			boolvars.helptext2 = language["imhelp2"].asString("Apuntar");
			boolvars.helptext3 = language["imhelp3"].asString("Menu armas");
			boolvars.helptext4 = language["imhelp4"].asString("Golpear");
			boolvars.helptext5 = language["imhelp5"].asString("Cambiar velocidad");
			boolvars.helptext6 = language["imhelp6"].asString("Lanzar vehiculo");
			boolvars.helptext7 = language["imhelp7"].asString("Disparar");
			boolvars.helptext8 = language["setsnav"].asString("Usa ARRIBA y ABAJO para navegar, ESPACIO para seleccionar");
			boolvars.online = language["online"].asString("En linea y listos");
			boolvars.impulso = language["impulse"].asString("Impulso");
			boolvars.suitarmormenu = language["menuironman"].asString("menu de skins");
			boolvars.Clicoptions = language["clicopt"].asString("Clic izquierdo para vestir armadura. Clic derecho para generar bot");
			boolvars.Tonyplay = language["tsplay"].asString("Clic para jugar como Tony");
			boolvars.Notplay = language["notply"].asString("Armadura no disponible");
			boolvars.Suitplay = language["armply"].asString("Clic para usar mark ");
			boolvars.Prevmenu = language["pmenu"].asString("Clic para retroceder de menu");
			boolvars.Nextmenu = language["nmenu"].asString("Clic para avanzar de menu");
			boolvars.Options = language["settings"].asString("Opciones");
			boolvars.Closesets = language["closets"].asString("Cerrar opciones");
			boolvars.Closemenu = language["closemenu"].asString("Cerrar menu");
			boolvars.menupag1 = language["nopag1"].asString("Clic para retroceder pagina");
			boolvars.menupag2 = language["nopag2"].asString("Clic para avanzar pagina");
			boolvars.distan = language["distan"].asString("Distancia critica");
			boolvars.dronfar = language["dronfar"].asString("Pulse 'y' para reagrupar los bots");
		langbol = true;
	}


	if (plugin::scripting::CallCommandById(COMMAND_HAS_LANGUAGE_CHANGED) == true)
	{
		langbol = false;
	}	
	return true;
}

void JarvisVoice::superpowersironman() {
	static bool pact;
	static float i;
	static int indexfriends;
	if (!pact)
	{
		pact = true;
		i = 1.0f;
		indexfriends = 0;
		boolvars.timecreatedped = 0;
	}
	while (true)
	{
		CPed *player = FindPlayerPed(0);
		if (boolvars.cannothitcar == false)
		{
			if ((DWORD*)0xB6F5F0 > 0)
			{
				if (player)
				{
					JarvisVoice::Updateaudiopositions();
					if (boolvars.menuisactive == false)
					{
						if (boolvars.indx == 22 &&
							boolvars.escudoactivo == false)
						{
							CVector camvec, camaimpoint, camaimpoint8;
							CVector campos;
							CVector playerpos;
							CVector dist;
							CVector pointat;
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.8f, 0.0f, &playerpos.x, &playerpos.y, &playerpos.z);
							CColPoint colpnt;
							CEntity *pedcolliding;
							pedcolliding = NULL;
							boolvars.iscalculating = false;
							boolvars.fartarget = true;
							TheCamera.Find3rdPersonCamTargetVector(300, playerpos, &camvec, &camaimpoint8);

							for (i = 1.0f; i < 300; i++)
							{
								if ((DWORD*)0xB6F5F0 > 0)
								{
									if (player)
									{
										TheCamera.Find3rdPersonCamTargetVector(i, playerpos, &camvec, &camaimpoint);
										if (CWorld::ProcessLineOfSight(CVector(playerpos.x, playerpos.y, playerpos.z), camaimpoint, colpnt, pedcolliding, 1, 1, 1, 1, 0, 1, 1, 0) == true)
										{
											if (pedcolliding != FindPlayerEntity(0))
											{
												boolvars.pricel.x = colpnt.m_vecPoint.x;
												boolvars.pricel.y = colpnt.m_vecPoint.y;
												boolvars.pricel.z = colpnt.m_vecPoint.z;
												boolvars.target.x = colpnt.m_vecPoint.x;
												boolvars.target.y = colpnt.m_vecPoint.y;
												boolvars.target.z = colpnt.m_vecPoint.z;
												boolvars.iscalculating = false;
												boolvars.fartarget = false;
												break;
											}
											else
											{
												boolvars.pricel.x = camaimpoint.x;
												boolvars.pricel.y = camaimpoint.y;
												boolvars.pricel.z = camaimpoint.z;
												boolvars.target.x = camaimpoint.x;
												boolvars.target.y = camaimpoint.y;
												boolvars.target.z = camaimpoint.z;
												boolvars.iscalculating = true;
												boolvars.fartarget = false;
												break;
											}
										}
										else
										{
											boolvars.pricel.x = camaimpoint8.x;
											boolvars.pricel.y = camaimpoint8.y;
											boolvars.pricel.z = camaimpoint8.z;
											boolvars.target.x = camaimpoint8.x;
											boolvars.target.y = camaimpoint8.y;
											boolvars.target.z = camaimpoint8.z;
											boolvars.iscalculating = false;
											boolvars.fartarget = true;
										}
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
						}



						if (dimfriends > 0)
						{
							for (int indexfriends = 0; indexfriends < dimfriends; indexfriends++)
							{
								if ((DWORD*)0xB6F5F0 > 0)
								{
									if (player)
									{
										if (int(jarvisfriend[indexfriends].actorchar) != NULL && int(jarvisfriend[indexfriends].actorchar) != (int)player)
										{
											if (jarvisfriend[indexfriends].wpn == 0)
											{
												if (int(jarvisfriend[indexfriends].target.actorchar) != NULL && int(jarvisfriend[indexfriends].target.actorchar) != (int)player)
												{
													CVector camaimpoint1 = jarvisfriend[indexfriends].target.actorchar->GetPosition();
													RwV3d coords1;
													coords1.x = 0.185f;
													coords1.y = 0.08f;
													coords1.z = 0.0f;
													CColPoint colpnt1;
													CEntity *pedcolliding1 = NULL;
													jarvisfriend[indexfriends].actorchar->GetTransformedBonePosition(coords1, 3, false);

													for (jarvisfriend[indexfriends].contadordist = 1.0f; jarvisfriend[indexfriends].contadordist < 100; jarvisfriend[indexfriends].contadordist++)
													{
														if ((DWORD*)0xB6F5F0 > 0)
														{
															if (player)
															{
																if (CWorld::ProcessLineOfSight(CVector(coords1.x, coords1.y, coords1.z), camaimpoint1, colpnt1, pedcolliding1, 1, 1, 1, 1, 0, 0, 1, 0))
																{
																	if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, coords1.x, coords1.y, coords1.z, camaimpoint1.x, camaimpoint1.y, camaimpoint1.z, 1, 1, 1, 1, 0) == false)
																	{
																		if (pedcolliding1 != (CEntity*)(jarvisfriend[indexfriends].actorchar))
																		{
																			break;
																		}
																	}
																}
															}
															else
															{
																break;
															}
														}
														else
														{
															break;
														}
													}
													jarvisfriend[indexfriends].obstaclex = camaimpoint1.x;
													jarvisfriend[indexfriends].obstacley = camaimpoint1.y;
													jarvisfriend[indexfriends].obstaclez = camaimpoint1.z;
												}
											}
										}
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
						}

						if (dimenemy > 0)
						{
							for (int indexenemys = 0; indexenemys < dimenemy; indexenemys++)
							{
								if ((DWORD*)0xB6F5F0 > 0)
								{
									if (player)
									{
										if (int(jarvisenemy[indexenemys].actorchar) != NULL && int(jarvisenemy[indexenemys].actorchar) != (int)player)
										{
											if (jarvisenemy[indexenemys].wpn == 0)
											{
												if (int(jarvisenemy[indexenemys].target.actorchar) != NULL && int(jarvisenemy[indexenemys].target.actorchar) != (int)player)
												{
													CVector camaimpoint2 = jarvisenemy[indexenemys].target.actorchar->GetPosition();
													RwV3d coords2;
													coords2.x = 0.185f;
													coords2.y = 0.08f;
													coords2.z = 0.0f;
													CColPoint colpnt2;
													CEntity *pedcolliding2 = NULL;
													jarvisenemy[indexenemys].actorchar->GetTransformedBonePosition(coords2, 3, false);

													for (jarvisenemy[indexenemys].contadordist = 1.0f; jarvisenemy[indexenemys].contadordist < 100; jarvisenemy[indexenemys].contadordist++)
													{
														if ((DWORD*)0xB6F5F0 > 0)
														{
															if (player)
															{
																if (CWorld::ProcessLineOfSight(CVector(coords2.x, coords2.y, coords2.z), camaimpoint2, colpnt2, pedcolliding2, 1, 1, 1, 1, 0, 0, 1, 0))
																{
																	if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, coords2.x, coords2.y, coords2.z, camaimpoint2.x, camaimpoint2.y, camaimpoint2.z, 1, 1, 1, 1, 0) == false)
																	{
																		if (pedcolliding2 != (CEntity*)(jarvisenemy[indexenemys].actorchar))
																		{
																			break;
																		}
																	}
																}
															}
															else
															{
																break;
															}
														}
														else
														{
															break;
														}
													}
													jarvisenemy[indexenemys].obstaclex = camaimpoint2.x;
													jarvisenemy[indexenemys].obstacley = camaimpoint2.y;
													jarvisenemy[indexenemys].obstaclez = camaimpoint2.z;

												}
											}
										}
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		break;
	}
	CGame::Process();
}

void JarvisVoice::changeoutfitonair() {
	CPed *player = FindPlayerPed(0);
	if (player)
	{
		if (plugin::scripting::CallCommandById(COMMAND_IS_PLAYER_PLAYING, 0) == true)
		{
			if (plugin::scripting::CallCommandById(COMMAND_HAS_ANIMATION_LOADED, "parachute") == 1) {
				static float alfafade;
				if (player->m_nModelIndex == MODEL_NULL
					&& boolvars.menuisactive == false)
				{
					if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] == 0)// != -559695285)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "PARA_open") == 1)
						{
							if (!boolvars.startfade)
							{
								boolvars.startfade = true;
								boolvars.unfadetime = CTimer::m_snTimeInMillisecondsNonClipped;
								alfafade = 0.0f;
							}
						}
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "PARA_Land") == 1 ||
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "PARA_Land_Water") == 1 ||
							player->m_fHealth <= 1
							)
						{
							boolvars.startfade = false;
							alfafade = 0.0f;
						}
						if (boolvars.startfade == true)
						{
							if (boolvars.unfadetime + 2650 > CTimer::m_snTimeInMillisecondsNonClipped)
							{
								boolvars.fadetime = CTimer::m_snTimeInMillisecondsNonClipped;
							}
							else
							{
								if (boolvars.fadetime + 400 > CTimer::m_snTimeInMillisecondsNonClipped)
								{
									if (alfafade <= 210.0f)
									{
										alfafade += 40.0f;

									}
									if (alfafade >= 255.0f)
									{
										alfafade = 255.0F;
									}
									CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)alfafade));
								}
							}
							if (alfafade <= 0.0f)
							{
								alfafade = 0.0f;
							}
							CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)alfafade));
						}
					}
				}
				else
				{
					if (boolvars.startfade == true) {
						if (boolvars.fadetime + 400 < CTimer::m_snTimeInMillisecondsNonClipped)
						{
							if (alfafade >= 15.0f)
							{
								alfafade -= 15.0f;
								if (alfafade <= 0.0f)
								{
									alfafade = 0.0f;
								}
								CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)alfafade));
							}
							else
							{
								if (boolvars.fadetime + 600 < CTimer::m_snTimeInMillisecondsNonClipped)
								{
									boolvars.startfade = false;
									//importing preferences from ini files
									AudioLib.PlayVoiceEvent(19);
									
								}
							}
						}
						else
						{
							if (alfafade <= 210.0f)
							{
								alfafade += 40.0f;

							}
							if (alfafade >= 255.0f)
							{
								alfafade = 255.0F;
							}
							CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)alfafade));
						}
					}
				}
			}
		}
	}
}

static std::vector<int> rockets(10);

bool JarvisVoice::ironmanpowers() {
	/*
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
	*/
	std::string keyp = PLUGIN_PATH("IronMan\\MaxirpHud.dat");
	plugin::config_file config(keyp);
	//LOAD THE MODELS AND ANIMATIONS WE NEED
	float sizex = SCREEN_COORD(120.0f); //((SCREEN_HEIGHT) / 7 * 1);

	static int iconsel, iconsel1;
	float sixe = (SCREEN_COORD_CENTER_X - SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT)+SCREEN_COORD(10.0f) + sizex;
	static int cfgindx, cfgnro;
	static float offsetx;
	static int timerforpages;
	static bool debevolar;
	static bool onxe;
	
	std::string nameanim;

	boolvars.mark = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

	CPed *player = FindPlayerPed(0);
	static bool star;
	static unsigned int clanktime;

	static CVector camposn;
	static CVector camrot;

	static int code22, code23, code24;
	static int lastused;
	static bool startin;

	static bool walkactive;
	static int modelplayr;
	static int ropa[18];
	bool pedmove = false;
	static bool cambio;
	if (!startin)
	{
		walkactive = true;
		boolvars.indx = 0;
		startin = true;
	}
	if (player)
	{
		if (!onxe)
		{
			if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 594) == true) {
				if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 407) == true) {
					if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 345) == true) {
						if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 1224) == true) {
							if (plugin::scripting::CallCommandById(COMMAND_HAS_ANIMATION_LOADED, "parachute") == 1) {
								if (plugin::scripting::CallCommandById(COMMAND_HAS_ANIMATION_LOADED, "ironman") == 1) {
									onxe = true;
								}
								else
								{
									onxe = false;
									plugin::scripting::CallCommandById(COMMAND_REQUEST_ANIMATION, "ironman");
								}
							}
							else
							{
								onxe = false;
								plugin::scripting::CallCommandById(COMMAND_REQUEST_ANIMATION, "parachute");
							}
						}
						else
						{
							onxe = false;
							plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, 1224);
						}
					}
					else
					{
						onxe = false;
						plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, 345);
					}
				}
				else
				{
					onxe = false;
					plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, 407);
				}
			}
			else
			{
				onxe = false;
				plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, 594);
			}
		}
		if (!onxe)
		{
			plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
			return true;
		}
		boolvars.activesuit = jarvisvoice.activesuitwearing();

		boolvars.suit[0] = jarvisvoice.is_wearing_suit(0);
		
		boolvars.suit[1] = jarvisvoice.is_wearing_suit(1);
		
		boolvars.suit[2] = jarvisvoice.is_wearing_suit(2);

		boolvars.suit[3] = jarvisvoice.is_wearing_suit(3);
		
		boolvars.suit[4] = jarvisvoice.is_wearing_suit(4);
				
		boolvars.suit[5] = jarvisvoice.is_wearing_suit(5);
		
		boolvars.suit[6] = jarvisvoice.is_wearing_suit(6);
		
		//WHEN THE PLAYER IS ACTIVE, WE UPDATE THE 3D AUDIO POSITIONS
		//JarvisVoice::Updateaudiopositions();

		//IF THE MENU IS NOT DISPLAYED, WE CHECK THE WALKSTYLE WHEN NOT AIMING
		if (boolvars.indx != 22 && boolvars.indx != 25)
		{
			if (settings.bWEnablealkstyle == true)
			{
				//WE MUST CHANGE THE WALKSTYLE ON EVERY OUTFIT CHANGE, BECAUSE THAT RESETS THE WALKSTYLE
				if (player->m_nModelIndex == 0)
				{
					for (int i = 0; i < 18; i++)
					{
						if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[i] != ropa[i])
						{
							walkactive = true;
							ropa[i] = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[i];
						}
					}
				}
				//WE MUST CHANGE THE WALKSTYLE ON EVERY SKIN CHANGE, BECAUSE THAT RESETS THE WALKSTYLE
				if (player->m_nModelIndex != modelplayr)
				{
					modelplayr = player->m_nModelIndex;
					walkactive = true;
				}
				if (walkactive == true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_CAN_PLAYER_START_MISSION, 0) == 1)
					{
						pedmove = true;
					}
					plugin::patch::SetInt(6330958, -1869574000, 1);
					plugin::patch::SetShort(6330962, 37008, 1);
					if (pedmove != false)
					{
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
					}
					plugin::scripting::CallCommandById(COMMAND_SET_ANIM_GROUP_FOR_CHAR, player, "ironman");
					walkactive = false;
					cambio = true;
					return true;
				}
			}
			else
			{
				if (cambio == true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_CAN_PLAYER_START_MISSION, 0) == 1)
					{
						pedmove = true;
					}
					plugin::patch::SetInt(6330958, 81036937, 1);
					plugin::patch::SetShort(6330962, 0, 1);
					if (pedmove != false)
					{
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
					}
					if (boolvars.indx == 22)
					{
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 0);
					}
					cambio = false;
					walkactive = true;
					return true;
				}
			}
		}

		if (boolvars.menuisactive == true)
		{
			//IF THE MENU IS DISPLAYED, WE HIDE THE HUD
			if (!settings.bDonewithradar)
			{
				if (!settings.bEnableRadar1)
				{
					plugin::patch::SetInt(0xBA676C, 2, false);
				}

				if (settings.bEnableRadar1)
				{
					plugin::patch::SetInt(0xBA676C, 0, false);
				}
				settings.Read();
				settings.bDonewithradar = true;
			}
		}
	}
	//CHECK THE LIGHTS CREATED FOR REPULSOR AND CHEST BEAMS
	static int ibeam[3];
	for (int i = 0; i <= 2; i++)
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_SEARCHLIGHT_EXIST, ibeam[i]) == true)
		{
			plugin::scripting::CallCommandById(COMMAND_DELETE_SEARCHLIGHT, ibeam[i]);
		}
	}

	if (boolvars.indx != 22 || boolvars.wpn != 4 ||
		plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) != true)
	{
		//IF IRON MAN IS NOT AIMIN WEAPON 4 (CHEST BEAM) WE STOP THE LOOP OF UNIBEAM SOUND
		Playuniloop(0);
	}

	static int ruidito;

	if(boolvars.indx != 22)
	{
		ruidito = 0;
	}
	
	static float hdn, armorperc, carposnx, carposny, carposnz, angle, opsx, opsy, opsz;
	static bool wasdressed;
	static bool ordendada;
	static bool ordenelegir;

	static DWORD* pActor;
	static DWORD* pCamera;
	static int objetohb;
	static int vehic, objeto1, cheatcode, clr1, clr2, passengers, liftedcarmodel;
	static unsigned int carinairtime, unholdtime;
	static CVector pricel2, pricell, pric1, pric2;
	static float x, y, z, vel1, vel2, vel3, dir1, dir2, dir3;
	float time1;
	static int pedindex;
	static CPed *player2;
	static int player22;
	CPad *pad;
	pad = CPad::GetPad(0);
	pActor = (DWORD*)0xB6F5F0;//pointer of player actor 
	pCamera = (DWORD*)0xB6F99C;//camera pointer

	//CHECK FOR ROCKETS ACTIVATION
	if (boolvars.killedtargetsexist == true)
	{
		//WE STORE THE PEDS THAT WE NEUTRALIZE
		*boolvars.aimedpeds = *jarvisvoice.Neutralize_targets(boolvars.wpn, boolvars.aimedpeds, &rockets, &boolvars.killedtargetsexist);
	}

	static bool gameisstopped, hastoloadskin, en, en2, iscliqued, isclicked;
	static int menupage, chosenmark, markk, sum, cutenabled, impulse;
	static float u, cursorx, cursory, spd;

	//MENU ACTIVATION THROUGH INI FILE
	int menuact = GetPrivateProfileInt("CONFIG", "MENU", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

	if (menuact == 1)
	{
		boolvars.menuisactive = true;
		boolvars.indx = 25;
		boolvars.alphafad = 50;

		WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
		if (!gameisstopped)
		{
			if (menupage != 0 && menupage != 1)
				menupage = 0;
			offsetx = sixe;
			iconsel = 0;
			cfgindx = 0;
			cfgnro = 0;
			iconsel1 = 0;
			timerforpages = 0;
			gameisstopped = true;
			hastoloadskin = false;
			hastoloadfriend = false;
			isentered = false;
			iscliqued = false;
			isclicked = false;
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 0);

			if (settings.radarmode != 2)
			{
				plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 0);
			}
		}
		goto suitmenu;
	}

	if (!en) {
		
		pedindex = 0;
		wasdressed = false;
		menupage = 0;
		boolvars.victim = 0;
		boolvars.victim1 = 0;
		boolvars.ped = 0;
		boolvars.pricel.x = 0.0f;
		boolvars.pricel.y = 0.0f;
		boolvars.pricel.z = 0.0f;
		boolvars.aimedpeds[0] = NULL;
		boolvars.aimedpeds[1] = NULL;
		boolvars.aimedpeds[2] = NULL;
		boolvars.aimedpeds[3] = NULL;
		boolvars.aimedpeds[4] = NULL;
		boolvars.range = 75.0f;
		spd = 200.0f;
		en = true;
		boolvars.waiter = 0;
		boolvars.timetowait = 0;
	}
	static HHOOK mousehook;
	static bool waswaiting;
	static int clktim;
	if (CTimer::m_snTimeInMillisecondsNonClipped < boolvars.waiter + boolvars.timetowait)
	{
		//THE WAIT FUNCTION
		//STRUCTURE:
		//boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
		//boolvars.timetowait = 200; //as example
		//using this, the script will automatically wait 200 milliseconds for next action
		if ((DWORD*)0xB6F5F0 > 0 && player)
		{
			if (boolvars.activesuit > 0)
			{
				if (player->m_fArmour <= 0.0f || player->m_fHealth <= 0.0f)
				{
					if (player->m_nModelIndex != 0)
					{

						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_NULL);
						plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
						boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.timetowait = 200;
						player = FindPlayerPed(0);
						boolvars.murio = true;
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
					}
					if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != 0)
					{
						plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);
						plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
						boolvars.hudactive = false;
						boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.timetowait = 150;

						boolvars.iscjfrozen = false;
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						boolvars.indx = 0;
						WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
					}
					if (player->m_fHealth <= 0.0)
					{
						plugin::scripting::CallCommandById(COMMAND_KILL_CHAR, player);
					}
				}
			}
			if (plugin::scripting::CallCommandById(COMMAND_CAN_PLAYER_START_MISSION, 0) == 1)
			{
				if (boolvars.indx != 21 && boolvars.indx != 23 && boolvars.indx != 13)
				{
					//WE ANALYZE THE CHEAT INPUT, FIRST 4 CHARS
					impulse = 0;
					cheatcode = plugin::patch::GetInt(9867536, false);

					if (cheatcode == 1279870533)
					{
						//IF INPUT IS LIFE, WE RESTORE CHAR HEALTH
						if (boolvars.suit[5] == true)
						{
							//WHEN WEARING ARMOR, WE SET 1000 ARMOR
							plugin::patch::SetInt(9867536, 0, false);
							player->m_fArmour = 1000;
							player->m_fHealth = player->m_fMaxHealth;
						}
						else
						{
							//WHEN NOT WEARING ARMOR, WE SET 100 ARMOR
							plugin::patch::SetInt(9867536, 0, false);
							player->m_fArmour = 100;
							player->m_fHealth = player->m_fMaxHealth;
						}
					}
					else
					{
						if (boolvars.indx != 22 && cheatcode == 1230131022 && boolvars.has_a_car == false && player->m_nPedState != PEDSTATE_DRIVING && player->m_nPedState != PEDSTATE_PASSENGER)
						{
							//IF INPUT IS IRON, WE ACTIVATE THE MENU
							beginend = false;
							plugin::patch::SetInt(9867536, 0, false);
							boolvars.menuisactive = true;
							boolvars.indx = 25;
							boolvars.alphafad = 50;
							if (!gameisstopped)
							{
								if (menupage != 0 && menupage != 1)
									menupage = 0;
								offsetx = sixe;
								iconsel = 0;
								cfgindx = 0;
								cfgnro = 0;
								iconsel1 = 0;
								timerforpages = 0;
								gameisstopped = true;
								hastoloadskin = false;
								hastoloadfriend = false;
								isentered = false;
								iscliqued = false;
								isclicked = false;
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 0);

								if (settings.radarmode != 2)
								{
									plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 0);
								}
							}

							return true;
						}
					}
				}
			}
			//Drawfondo(boolvars.alphafad);
			CVector ofstexplo1, ofstexplo2;
			CVector ofstexplo3, ofstexplo4;
			if (debevolar == true)
			{
				if (boolvars.cohetebotexplotado == true)
				{
					if (boolvars.has_a_car == true)
					{
						boolvars.indx = 16;
						goto abort_throw_car;
					}
					boolvars.cohetebotexplotado = false;
					if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") != 1 &&
						plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != 1
						&& boolvars.indx != 20
						&& boolvars.landgetup == 0 &&
						plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != 1 &&
						plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") != 1)
					{
						if (boolvars.mousewheelhacktrigger == true) {
							spd = 200.0f;
							UnhookWindowsHookEx(mousehook);
							jarvisvoice.SetVelocityUnlimited(false);
							plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
							boolvars.mousewheelhacktrigger = false;
						}

						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;

						JarvisVoice::storeenviroment(&propellers.enviroment_ID);

						if (boolvars.vuelaatras == true)
						{
							nameanim.assign("Fall_FrontExplosion");
						}
						else
						{
							nameanim.assign("Fall_BackExplosion");
						}
						boolvars.landgetup = 1;
						waswaiting = true;
						boolvars.iscjfrozen = false;
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, nameanim.data(), "ironman", 10.0f, 0, 1, 1, 0, -2); //"ironman", 10.0f, 0, 1, 1, 0, -2);
						if (boolvars.immune == true)
							plugin::scripting::CallCommandById(COMMAND_DAMAGE_CHAR, player, 75, 1);

						nameanim.clear();
						boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.timetowait = 800;

						if (boolvars.has_a_car == false)
						{
							boolvars.indx = 20;
						}
						else
						{
							boolvars.indx = 16;
							goto abort_throw_car;
						}
					}
				}
				else
				{
					CVector playpos = player->GetPosition();
					if (boolvars.stordexplpos.x != boolvars.explosionpos.x
						&& boolvars.stordexplpos.y != boolvars.explosionpos.y
						&& boolvars.stordexplpos.z != boolvars.explosionpos.z
						&& boolvars.expltype != 8
						&& boolvars.expltype != 9)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") != 1 &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != 1
							&& boolvars.indx != 20 &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != 1 &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") != 1)
						{
							if (boolvars.has_a_car == true)
							{
								boolvars.indx = 16;
								goto abort_throw_car;
							}
							boolvars.stordexplpos.x = boolvars.explosionpos.x;
							boolvars.stordexplpos.y = boolvars.explosionpos.y;
							boolvars.stordexplpos.z = boolvars.explosionpos.z;

							if (boolvars.mousewheelhacktrigger == true) {
								spd = 200.0f;
								UnhookWindowsHookEx(mousehook);
								jarvisvoice.SetVelocityUnlimited(false);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
								boolvars.mousewheelhacktrigger = false;
							}
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							boolvars.thrusterskilled = true;

							JarvisVoice::storeenviroment(&propellers.enviroment_ID);

							CVector2D explhead;

							boolvars.landgetup = 1;

							explhead.x = playpos.x - boolvars.stordexplpos.x;
							explhead.y = playpos.y - boolvars.stordexplpos.y;
							static float explhd;
							plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &explhd);

							float magnh = explhead.Magnitude();

							static float hadin;
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

							if (explhd > hadin - 90.0f && explhd < hadin + 90.0f)
							{
								nameanim.assign("Fall_FrontExplosion");
							}
							else
							{
								nameanim.assign("Fall_BackExplosion");
							}

							boolvars.landgetup = 1;
							waswaiting = true;

							boolvars.iscjfrozen = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, nameanim.data(), "ironman", 10.0f, 0, 1, 1, 0, -2); //"ironman", 10.0f, 0, 1, 1, 0, -2);
							if (boolvars.immune == true)
								plugin::scripting::CallCommandById(COMMAND_DAMAGE_CHAR, player, 75, 1);

							nameanim.clear();
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 800;
							if (boolvars.has_a_car == false)
							{
								boolvars.indx = 20;
							}
							else
							{
								boolvars.indx = 16;
								goto abort_throw_car;
							}
						}
					}
					return true;
				}
			}
			
		}
		else
		{
			settings.radarmode = plugin::patch::GetUChar(0xBA676C, false);
			pActor = (DWORD*)0xB6F5F0;
			player = FindPlayerPed(0);
			boolvars.landgetup = 0;
			if (boolvars.indx != 0)
			{
				boolvars.indx = 0;
				plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
			}
			WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			WritePrivateProfileString("CONFIG", "WEAPON", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			waswaiting = false;
			WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			boolvars.menuisactive = false;
			boolvars.alphafad = 0;
			boolvars.settingisactive = false;
			boolvars.flytime = 0;
			boolvars.landedpara = false;
			boolvars.paralineactive = false;
			boolvars.iscjfrozen = false;
			boolvars.isshootin = false;
			boolvars.startfade = false;
			boolvars.cannothitcar = false;
			boolvars.aims = false;
			boolvars.has_a_car = false;
			hastoloadskin = false;
			iscliqued = false;
			isclicked = false;
			chosenmark = -1;
			boolvars.radarisactive = true;
			boolvars.systemerror = false;
			boolvars.cohetebotexplotado = false;
			JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
			JarvisVoice::storeenviroment(&propellers.enviroment_ID);
			boolvars.thrusterskilled = true;
			if (boolvars.mousewheelhacktrigger == true) {
				spd = 200.0f;
				UnhookWindowsHookEx(mousehook);
				jarvisvoice.SetVelocityUnlimited(false);
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
				boolvars.mousewheelhacktrigger = false;
			}

			pedindex = 0;
			wasdressed = false;
			menupage = 0;
			boolvars.victim = 0;
			boolvars.victim1 = 0;
			boolvars.ped = 0;
			boolvars.pricel.x = 0.0f;
			boolvars.pricel.y = 0.0f;
			boolvars.pricel.z = 0.0f;
			boolvars.aimedpeds[0] = NULL;
			boolvars.aimedpeds[1] = NULL;
			boolvars.aimedpeds[2] = NULL;
			boolvars.aimedpeds[3] = NULL;
			boolvars.aimedpeds[4] = NULL;
			boolvars.range = 75.0f;
			spd = 200.0f;
			boolvars.waiter = 0;
			boolvars.timetowait = 0;
			cursorx = SCREEN_COORD_CENTER_X;
			cursory = SCREEN_COORD_CENTER_Y;
			ordendada = false;
			ordenelegir = false;
			boolvars.alphafad = 0;
			boolvars.timrmenu = 0;
			gameisstopped = false;
		}
		return true;
		goto endin;
	}
	else
	{
		if (player)
		{
			if (boolvars.indx != 21 && boolvars.indx != 23 && boolvars.indx != 13)
			{
				impulse = 0;
				cheatcode = plugin::patch::GetInt(9867536, false);

				if (cheatcode == 1279870533)
				{
					if (boolvars.suit[5] == true)
					{
						plugin::patch::SetInt(9867536, 0, false);
						player->m_fArmour = 1000;
						player->m_fHealth = player->m_fMaxHealth;
					}
					else
					{
						plugin::patch::SetInt(9867536, 0, false);
						player->m_fArmour = 100;
						player->m_fHealth = player->m_fMaxHealth;
					}
				}
				else
				{
					if (boolvars.indx != 22 && cheatcode == 1230131022 && boolvars.has_a_car == false && player->m_nPedState != PEDSTATE_DRIVING && player->m_nPedState != PEDSTATE_PASSENGER)
					{
						plugin::patch::SetInt(9867536, 0, false);
						boolvars.menuisactive = true;
						boolvars.indx = 25;
						boolvars.alphafad = 50;

						if (!gameisstopped)
						{
							if (menupage != 0 && menupage != 1)
								menupage = 0;
							offsetx = sixe;
							iconsel = 0;
							cfgindx = 0;
							cfgnro = 0;
							iconsel1 = 0;
							timerforpages = 0;
							gameisstopped = true;
							hastoloadskin = false;
							hastoloadfriend = false;
							isentered = false;
							iscliqued = false;
							isclicked = false;
							plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 0);

							if (settings.radarmode != 2)
							{
								plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 0);
							}
						}
					}
				}
			}
			if (is_on_foot() == true && waswaiting == true)
			{
				if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != true
					&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == true)
					{
						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						JarvisVoice::storeenviroment(&propellers.enviroment_ID);
						boolvars.thrusterskilled = true;
						if (boolvars.mousewheelhacktrigger == true) {
							spd = 200.0f;
							UnhookWindowsHookEx(mousehook);
							jarvisvoice.SetVelocityUnlimited(false);
							plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
							boolvars.mousewheelhacktrigger = false;
						}
						boolvars.landgetup = 1;
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getupfront", "ironman", 4.0f, 0, 1, 1, 0, -1);
					}
					else
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == true)
						{
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							boolvars.thrusterskilled = true;
							if (boolvars.mousewheelhacktrigger == true) {
								spd = 200.0f;
								UnhookWindowsHookEx(mousehook);
								jarvisvoice.SetVelocityUnlimited(false);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
								boolvars.mousewheelhacktrigger = false;
							}
							boolvars.landgetup = 1;
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getup", "ironman", 4.0f, 0, 1, 1, 0, -1);
						}
					}
				}
				else
				{
					boolvars.landgetup = 1;
				}
				waswaiting = false;
				if (boolvars.has_a_car == true)
				{
					boolvars.indx = 16;
					goto abort_throw_car;
				}
				else
				{
					boolvars.indx = 20;
					boolvars.landgetup = 1;
					goto make_a_clank;
				}
			}
			if (boolvars.landgetup == 1 && debevolar==true)// || 
			{
				goto make_a_clank;
			}
			else
			{
				boolvars.landgetup = 0;
			}
			if (boolvars.indx != 22)
			{
				boolvars.isshootin = false;
				if (boolvars.aims == true)
				{
					if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true) {
						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;
						JarvisVoice::storeenviroment(&propellers.enviroment_ID);

					}
					if (jarvisvoice.is_over_ground(2.0f) == true
						|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "aim_ArmIDLE") == 1
						|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "aim_ChestIDLE") == 1)
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
					}
					boolvars.aims = false;
					boolvars.murio = true;
				}
			}
		}
		else
		{
			settings.radarmode = plugin::patch::GetInt(0xBA676C, false);
			pActor = (DWORD*)0xB6F5F0;
			player = FindPlayerPed(0);
			boolvars.landgetup = 0;
			if (boolvars.indx != 0)
			{
				boolvars.indx = 0;
				plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
			}
			WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			WritePrivateProfileString("CONFIG", "WEAPON", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			waswaiting = false;
			WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			boolvars.menuisactive = false;
			boolvars.alphafad = 0;
			boolvars.settingisactive = false;
			boolvars.flytime = 0;
			boolvars.landedpara = false;
			boolvars.paralineactive = false;
			boolvars.iscjfrozen = false;
			boolvars.isshootin = false;
			boolvars.startfade = false;
			boolvars.cannothitcar = false;
			boolvars.aims = false;
			boolvars.has_a_car = false;
			hastoloadskin = false;
			iscliqued = false;
			isclicked = false;
			chosenmark = -1;
			boolvars.radarisactive = true;
			boolvars.systemerror = false;
			boolvars.cohetebotexplotado = false;
			JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
			JarvisVoice::storeenviroment(&propellers.enviroment_ID);
			boolvars.thrusterskilled = true;
			if (boolvars.mousewheelhacktrigger == true) {
				spd = 200.0f;
				UnhookWindowsHookEx(mousehook);
				jarvisvoice.SetVelocityUnlimited(false);
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
				boolvars.mousewheelhacktrigger = false;
			}

			pedindex = 0;
			wasdressed = false;
			menupage = 0;
			boolvars.victim = 0;
			boolvars.victim1 = 0;
			boolvars.ped = 0;
			boolvars.pricel.x = 0.0f;
			boolvars.pricel.y = 0.0f;
			boolvars.pricel.z = 0.0f;
			boolvars.aimedpeds[0] = NULL;
			boolvars.aimedpeds[1] = NULL;
			boolvars.aimedpeds[2] = NULL;
			boolvars.aimedpeds[3] = NULL;
			boolvars.aimedpeds[4] = NULL;
			boolvars.range = 75.0f;
			spd = 200.0f;
			boolvars.waiter = 0;
			boolvars.timetowait = 0;
			cursorx = SCREEN_COORD_CENTER_X;
			cursory = SCREEN_COORD_CENTER_Y;
			ordendada = false;
			ordenelegir = false;
			boolvars.alphafad = 0;
			boolvars.timrmenu = 0;
			gameisstopped = false;
		}
	}
	waswaiting = false;
	static int hoveredmark;

	static bool frizado;

	/*
	if (boolvars.indx != 24 && boolvars.wpmenuisactive == false)
	{
		boolvars.wpmenuisactive = false;
	}*/
	if (player)
	{
		if (player->m_nModelIndex == MODEL_NULL && player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] == 0)
		{
			if (boolvars.startfade == true)
			{
				plugin::scripting::CallCommandById(COMMAND_LOAD_SPECIAL_CHARACTER, 7, "mark7");
				plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
				if (boolvars.unfadetime + 2500 < CTimer::m_snTimeInMillisecondsNonClipped)
				{
					if (boolvars.fadetime + 1000 > CTimer::m_snTimeInMillisecondsNonClipped)
					{
						if (boolvars.fadetime + 180 < CTimer::m_snTimeInMillisecondsNonClipped && plugin::scripting::CallCommandById(COMMAND_HAS_SPECIAL_CHARACTER_LOADED, 7)==1)
						{
							plugin::scripting::CallCommandById(COMMAND_REMOVE_WEAPON_FROM_CHAR, player, 46);
							
							markk = 7;
							
							if (!wasdressed)
							{
								wasdressed = true;
								armorperc = player->m_fArmour;
							}
							player->m_fArmour = 1000;

							boolvars.hudactive = true;

							plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_SPECIAL07);
							plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 200;

							player = FindPlayerPed(0);
							boolvars.murio = true;
							plugin::scripting::CallCommandById(COMMAND_UNLOAD_SPECIAL_CHARACTER, 7);
							plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, MODEL_SPECIAL07);
							
							WritePrivateProfileString("CONFIG", "MARK", "7", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_PLAYING_FLAG, player, "PARA_open", 1);
							boolvars.landedpara = true;
						}
						else
						{
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_PLAYING_FLAG, player, "PARA_open", 0);
						}
					}
				}
			}
		}
	}


	if (!en2) {
		WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
		boolvars.menuisactive = false;
		hastoloadskin = false;
		iscliqued = false;
		isclicked = false;
		chosenmark = -1;
		boolvars.radarisactive = true;

		en2 = true;		
	}
	player = FindPlayerPed(0);
	
	if (*pActor > 0 && player)
	{
		if (boolvars.alphafad > 200)
		{
			Drawfondo(boolvars.alphafad);
			if (isclicked == true)
			{
				if (strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark01") == 0 ||
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark02") == 0 ||
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark03") == 0 ||
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark04") == 0 ||
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark05") == 0 ||
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark06") == 0 ||
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark07") == 0)
				{
					if (chosenmark == 0)
					{
						boolvars.pageofsuit = boolvars.yndex;
						if (player->m_nModelIndex != 0)
						{

							plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_NULL);

							plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 150;

						}
					}
					else
					{
						if (chosenmark == 1)
						{
							markk = chosenmark;
							if (!wasdressed)
							{
								wasdressed = true;
								armorperc = player->m_fArmour;
							}
							player->m_fArmour = 1000;

							plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "gimpleg", "gimpleg", 17);
							plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							boolvars.hudactive = true;
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 150;
							isclicked = false;
							chosenmark = -1;
							boolvars.iscjfrozen = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							boolvars.indx = 0;
							WritePrivateProfileString("CONFIG", "MARK", "1", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							return true;
						}
						else
						{
							if (chosenmark == 2)
							{
								markk = chosenmark;
								if (!wasdressed)
								{
									wasdressed = true;
									armorperc = player->m_fArmour;
								}
								player->m_fArmour = 1000;

								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "policetr", "policetr", 17);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
								boolvars.hudactive = true;

								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 150;
								isclicked = false;
								chosenmark = -1;
								boolvars.iscjfrozen = false;
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
								boolvars.indx = 0;
								WritePrivateProfileString("CONFIG", "MARK", "2", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
								return true;
							}
							else
							{
								if (chosenmark == 3)
								{
									markk = chosenmark;
									if (!wasdressed)
									{
										wasdressed = true;
										armorperc = player->m_fArmour;
									}
									player->m_fArmour = 1000;

									plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "medictr", "medictr", 17);
									plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
									boolvars.hudactive = true;
									boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
									boolvars.timetowait = 150;
									isclicked = false;
									chosenmark = -1;
									boolvars.iscjfrozen = false;
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
									plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
									boolvars.indx = 0;
									WritePrivateProfileString("CONFIG", "MARK", "3", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									return true;
								}
								else
								{
									if (chosenmark == 4)
									{
										markk = chosenmark;
										if (!wasdressed)
										{
											wasdressed = true;
											armorperc = player->m_fArmour;
										}
										player->m_fArmour = 1000;

										plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "pimptr", "pimptr", 17);
										plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
										boolvars.hudactive = true;
										boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
										boolvars.timetowait = 150;
										isclicked = false;
										chosenmark = -1;
										boolvars.iscjfrozen = false;
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
										plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
										boolvars.indx = 0;
										WritePrivateProfileString("CONFIG", "MARK", "4", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
										return true;
									}
									else
									{
										if (chosenmark == 5)
										{
											markk = chosenmark;
											if (!wasdressed)
											{
												wasdressed = true;
												armorperc = player->m_fArmour;
											}
											player->m_fArmour = 1000;

											plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "GARAGELEG", "garagetr", 17);
											plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
											boolvars.hudactive = true;
											boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
											boolvars.timetowait = 150;
											isclicked = false;
											chosenmark = -1;
											boolvars.iscjfrozen = false;
											plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
											plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
											boolvars.indx = 0;
											WritePrivateProfileString("CONFIG", "MARK", "5", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
											return true;
										}
										else
										{
											if (chosenmark == 6)
											{
												markk = chosenmark;
												if (!wasdressed)
												{
													wasdressed = true;
													armorperc = player->m_fArmour;
												}
												player->m_fArmour = 1000;

												plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "valet", "valet", 17);
												plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
												boolvars.hudactive = true;
												boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
												boolvars.timetowait = 150;
												isclicked = false;
												chosenmark = -1;
												boolvars.iscjfrozen = false;
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
												boolvars.indx = 0;
												WritePrivateProfileString("CONFIG", "MARK", "6", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
												return true;
											}
											else
											{
												if (chosenmark == 7)
												{
													markk = chosenmark;
													if (!wasdressed)
													{
														wasdressed = true;
														armorperc = player->m_fArmour;
													}
													player->m_fArmour = 1000;
													plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "countrytr", "countrytr", 17);
													plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
													boolvars.hudactive = true;
													boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
													boolvars.timetowait = 150;
													isclicked = false;
													chosenmark = -1;
													boolvars.iscjfrozen = false;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
													boolvars.indx = 0;

													WritePrivateProfileString("CONFIG", "MARK", "7", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
													return true;
												}
												else
												{
													if (chosenmark == 0)
													{
														plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);
														plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
														boolvars.hudactive = false;
														boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
														boolvars.timetowait = 150;
														isclicked = false;
														chosenmark = -1;

														boolvars.iscjfrozen = false;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
														boolvars.indx = 0;
														WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
														return true;
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
				else
				{
					isclicked = false;
				}
			}
			else
			{
				isclicked = false;
			}
		
			
			isclicked = false;

			if (iscliqued == true )
			{
				if (chosenmark != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark01") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark02") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark03") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark04") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark05") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark06") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark07") != 0 &&
					settings.folderdirs[boolvars.yndex].suits[chosenmark].texexists == true && settings.folderdirs[boolvars.yndex].suits[chosenmark].mdlexists == true)
				{
					boolvars.pageofsuit = boolvars.yndex;
					plugin::scripting::CallCommandById(COMMAND_LOAD_SPECIAL_CHARACTER, 7, settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname);
					plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);

					iscliqued = false;
					hastoloadskin = true;
					goto endin;
				}
				else
				{
					iscliqued = false;
					hastoloadskin = false;
					chosenmark = -1;
				}
			}

			if (hastoloadskin == true)
			{
				
				if (chosenmark != 0 &&
					settings.folderdirs[boolvars.pageofsuit].suits[chosenmark].texexists == true && settings.folderdirs[boolvars.pageofsuit].suits[chosenmark].mdlexists == true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_HAS_SPECIAL_CHARACTER_LOADED, 7) != 1)
					{
						plugin::scripting::CallCommandById(COMMAND_LOAD_SPECIAL_CHARACTER, 7, settings.folderdirs[boolvars.pageofsuit].suits[chosenmark].pngname);
						plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
						goto endin;
					}
					else
					{
						hastoloadskin = false;
						if (!wasdressed)
						{
							wasdressed = true;
							armorperc = player->m_fArmour;
						}
						player->m_fArmour = 1000;
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_SPECIAL07);
						plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

						boolvars.hudactive = true;
						boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.timetowait = 200;

						player = FindPlayerPed(0);
						boolvars.murio = true;
						plugin::scripting::CallCommandById(COMMAND_UNLOAD_SPECIAL_CHARACTER, 7);
						plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, MODEL_SPECIAL07);

						markk = boolvars.mark;
						if (markk != chosenmark)
						{
							char *numberito;
							numberito = new char[4];
							sprintf(numberito, "%d", chosenmark);
							WritePrivateProfileString("CONFIG", "MARK", numberito, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							delete[] numberito;
							markk = chosenmark;
						}

						chosenmark = -1;
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						fflush(stdin);
						goto endin;
					}
				}
				else
				{
					iscliqued = false;
					hastoloadskin = false;
					chosenmark = -1;
					goto endin;
				}
			}

			if (isentered == true)
			{
				if (settings.folderdirs[boolvars.yndex].suits[chosenmark].texexists == true && settings.folderdirs[boolvars.yndex].suits[chosenmark].mdlexists == true)
				{
					plugin::scripting::CallCommandById(COMMAND_LOAD_SPECIAL_CHARACTER, 9, settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname);
					plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
					//jarvisvoice.PlayShot(&jarvisfriend[pedindex].botstream[1]);
					isentered = false;
					hastoloadfriend = true;
					goto endin;
				}
				else
				{
					isentered = false;
					hastoloadfriend = false;
					chosenmark = -1;
					goto endin;
				}
			}


			if (hastoloadfriend == true)
			{
				/*
				*/
				if (chosenmark != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark01") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark02") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark03") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark04") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark05") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark06") != 0 &&
					strcmp(settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname, "mark07") != 0 &&
					settings.folderdirs[boolvars.yndex].suits[chosenmark].texexists == true &&
					settings.folderdirs[boolvars.yndex].suits[chosenmark].mdlexists == true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_HAS_SPECIAL_CHARACTER_LOADED, 9) != 1)
					{
						//jarvisvoice.PlayShot(&jarvisfriend[pedindex].botstream[1]);
						plugin::scripting::CallCommandById(COMMAND_LOAD_SPECIAL_CHARACTER, 9, settings.folderdirs[boolvars.yndex].suits[chosenmark].pngname);
						plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
						goto endin;
					}
					else
					{
						/*if(pedindex<contador1 && contador1<20)
						{
							pedindex=contador1;
						}*/
						CVector posplayer;
						static float ang;
						ang = -90.0f + 18.0f *(float)pedindex;
						CVector2D pedcoords;
						//for(float ang=90.0f; ang<=450.0f; ang += 72){
						plugin::scripting::CallCommandById(COMMAND_SIN, ang, &pedcoords.y);
						plugin::scripting::CallCommandById(COMMAND_COS, ang, &pedcoords.x);
						pedcoords.y *= 2.0f;
						pedcoords.x *= 2.0f;

						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, pedcoords.x, pedcoords.y, 2.0f, &posplayer.x, &posplayer.y, &posplayer.z);
						//player->SetModelIndex(MODEL_SPECIAL10);

						static float direction;
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &direction);
						//1224 - caja
						//1598 - pelota playera
						plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 1224, posplayer.x, posplayer.y, posplayer.z, &jarvisfriend[pedindex].baseobj);
						pagebelong[pedindex] = boolvars.yndex;
						objectlist[dim] = jarvisfriend[pedindex].baseobj;
						dim++;
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[pedindex].baseobj, direction);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, jarvisfriend[pedindex].baseobj, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ONLY_DAMAGED_BY_PLAYER, jarvisfriend[pedindex].baseobj, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_PROOFS, jarvisfriend[pedindex].baseobj, 1, 1, 1, 1, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, jarvisfriend[pedindex].baseobj, 0);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, jarvisfriend[pedindex].baseobj, 0.001f);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, jarvisfriend[pedindex].baseobj, 0);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[pedindex].baseobj, 0);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisfriend[pedindex].baseobj, 0.0f, 0.0f, 0.0);
						plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[pedindex].baseobj, 1);
						jarvisfriend[pedindex].isfrozen = true;

						plugin::scripting::CallCommandById(COMMAND_CREATE_CHAR, 8, MODEL_SPECIAL09, -100.0f, 100.0f, -40.0f, &jarvisfriend[pedindex].actorid);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, jarvisfriend[pedindex].actorid, 0, 0, 0, 1, 0);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEALTH, jarvisfriend[pedindex].actorid, 10000);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COLLISION, jarvisfriend[pedindex].actorid, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ONLY_DAMAGED_BY_PLAYER, jarvisfriend[pedindex].actorid, 0);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, jarvisfriend[pedindex].actorid, direction);

						plugin::scripting::CallCommandById(COMMAND_REMOVE_ALL_CHAR_WEAPONS, jarvisfriend[pedindex].actorid);

						jarvisfriend[pedindex].actorchar = CPools::GetPed(jarvisfriend[pedindex].actorid);
						jarvisfriend[pedindex].mark = chosenmark;
						jarvisfriend[pedindex].raysactive = false;

						jarvisfriend[pedindex].taskindex = 0;
						jarvisfriend[pedindex].thrusterscode = 0;
						jarvisfriend[pedindex].velz = -1.0f;
						jarvisfriend[pedindex].raystime = 0;
						jarvisfriend[pedindex].hasacar = false;
						jarvisfriend[pedindex].blip = 0;
						jarvisfriend[pedindex].hp = 10000;
						jarvisfriend[pedindex].wpn = 0;
						jarvisfriend[pedindex].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
						jarvisfriend[pedindex].jarvisprop.manoizqfx = 0;
						jarvisfriend[pedindex].jarvisprop.manoderfx = 0;
						jarvisfriend[pedindex].jarvisprop.pieizqfx = 0;
						jarvisfriend[pedindex].jarvisprop.piederfx = 0;
						//Sleep(200);

						plugin::scripting::CallCommandById(COMMAND_UNLOAD_SPECIAL_CHARACTER, 9);
						plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, MODEL_SPECIAL09);

						plugin::scripting::CallCommandById(COMMAND_ATTACH_CHAR_TO_OBJECT, jarvisfriend[pedindex].actorchar, jarvisfriend[pedindex].baseobj, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[pedindex].actorchar, "fly_Hover", "ironman", 10.0f, 1, 1, 1, 0, -2);
						plugin::scripting::CallCommandById(COMMAND_ADD_BLIP_FOR_CHAR, jarvisfriend[pedindex].actorchar, &jarvisfriend[pedindex].blip);
						plugin::scripting::CallCommandById(COMMAND_CHANGE_BLIP_COLOUR, jarvisfriend[pedindex].blip, 2);
						jarvisfriend[pedindex].thrusterscode = flotar;
						PlayThrustersIDLE(1, pedindex, &jarvisfriend[pedindex].botstream[0], (int)jarvisfriend[pedindex].actorid, settings.folderdirs[pagebelong[pedindex]].suits[jarvisfriend[pedindex].mark], 0, jarvisfriend[pedindex].thrusterscode, &jarvisfriend[pedindex].jarvisprop);
						chosenmark = -1;


						for (int u = 0; u <= 2; u++)
						{
							jarvisfriend[pedindex].ibeam[u] = NULL;
						}
						dimfriends += 1;
						hastoloadfriend = false;
						fflush(stdin);
						goto endin;
					}
				}
				else
				{
					isentered = false;
					hastoloadfriend = false;
					chosenmark = -1;
					goto endin;
				}
			}
			isentered = false;
			hastoloadfriend = false;
			iscliqued = false;
			hastoloadskin = false;
			chosenmark = -1;
		}
		if (player->m_fArmour <= 0.0f && boolvars.activesuit>0)
		{
			if (player->m_nModelIndex != 0)
			{

				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_NULL);
				plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
				//Sleep(200);
				boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
				boolvars.timetowait = 200;
				player = FindPlayerPed(0);
				boolvars.murio = true;
				plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
			}
			if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != 0)
			{
				plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);
				plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
				boolvars.hudactive = false;
				boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
				boolvars.timetowait = 150;
				boolvars.murio = true;

				boolvars.iscjfrozen = false;
				plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
				plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
				boolvars.indx = 0;
				WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			}
		}

		if (player->m_fHealth <= 0.0f)
		{
			WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			boolvars.menuisactive = false;
			boolvars.indx = 0;

			if (player->m_nModelIndex != 0)
			{

				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_NULL);
				plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
				//Sleep(200);
				boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
				boolvars.timetowait = 200;
				player = FindPlayerPed(0);
				boolvars.murio = true;
				plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
			}
			else
			{
				if (boolvars.activesuit != 0)
				{
					if (boolvars.activesuit > 0)
					{
						if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != 0)
						{
							//plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP,
							plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);

							plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							//Sleep(200);
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 200;
							player = FindPlayerPed(0);
							boolvars.murio = true;
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						}
						else
						{
							static int head, shades, reloj, zapas;
							head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
							reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
							shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
							zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
							if (reloj == -1155750708 ||
								reloj == 1142040938 ||
								zapas == -592930753 ||
								head == 1348958410 ||
								shades == 672552983)
							{
								if (reloj == -1155750708 ||
									reloj == 1142040938)
								{
									plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_torso", "torso", 0);
									plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

								}
								if (zapas == -592930753)
								{
									plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_feet", "feet", 3);
									plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
								}
								if (head == 1348958410)
								{
									plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_face", "head", 1);
									plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
								}

								if (shades == 672552983) {
									plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 15);
									plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
								}

								//Sleep(200);
								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 200;
								player = FindPlayerPed(0);
								boolvars.murio = true;
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							}
						}
					}
					else
					{
						static int head, shades, reloj, zapas;
						head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
						reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
						shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
						zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
						if (reloj == -1155750708 ||
							reloj == 1142040938 ||
							zapas == -592930753 ||
							head == 1348958410 ||
							shades == 672552983)
						{
							if (reloj == -1155750708 ||
								reloj == 1142040938)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_torso", "torso", 0);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

							}
							if (zapas == -592930753)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_feet", "feet", 3);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							}
							if (head == 1348958410)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_face", "head", 1);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							}

							if (shades == 672552983) {
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 15);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							}

							//Sleep(200);
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 200;
							player = FindPlayerPed(0);
							boolvars.murio = true;
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						}
					}
				}
			}
			static int mark;// , error1;
			
			mark = boolvars.mark;
			//mark = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			if (mark != 0)
			{
				WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			}

			//error1 = GetPrivateProfileInt("CONFIG", "ERROR", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			//if (error1 != 0)
			//{
				//WritePrivateProfileString("CONFIG", "ERROR", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			//}
			if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true || isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
				JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
				boolvars.thrusterskilled = true;

				JarvisVoice::storeenviroment(&propellers.enviroment_ID);
			}
			if (boolvars.iscjfrozen == true)
			{
				boolvars.iscjfrozen = false;
				plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
			}
			if (boolvars.indx == 13 || boolvars.has_a_car == true)
			{
				boolvars.indx = 16;
				goto abort_throw_car;
			}
			else
			{
				boolvars.indx = 0;
				return true;
			}
		}

		if (boolvars.murio == true)
		{
			plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
			walkactive = true;
			//Sleep(200);
			boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
			boolvars.timetowait = 200;
			player = FindPlayerPed(0);
			boolvars.murio = false;
		}


		if (plugin::scripting::CallCommandById(COMMAND_IS_PLAYER_PLAYING, 0) == true)
		{
			if (player->m_nPedState != PEDSTATE_DRIVING && player->m_nPedState != PEDSTATE_PASSENGER)
			{

				CVector ofstexplo1, ofstexplo2;
				CVector ofstexplo3, ofstexplo4;
				if (boolvars.cohetebotexplotado == true)
				{
					//IF PLAYER IS HIT BY ENEMY ROCKETS
					boolvars.cohetebotexplotado = false;
					if (!boolvars.escudoactivo)
					{
						debevolar = true;
					}
					else
					{
						if (boolvars.vuelaatras == true)
						{
							debevolar = true;
						}
						else
						{
							boolvars.landgetup = 0;
							debevolar = false;
						}
					}
					if (debevolar == true)
					{
						if (boolvars.has_a_car == true)
						{
							boolvars.indx = 16;
							goto abort_throw_car;
						}
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") != 1
							&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != 1
							&& boolvars.indx != 20
							&& boolvars.landgetup == 0
							&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != 1
							&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") != 1)
						{
							boolvars.landgetup = 1;
							if (boolvars.mousewheelhacktrigger == true) {
								spd = 200.0f;
								UnhookWindowsHookEx(mousehook);
								jarvisvoice.SetVelocityUnlimited(false);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
								boolvars.mousewheelhacktrigger = false;
							}

							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							boolvars.thrusterskilled = true;

							JarvisVoice::storeenviroment(&propellers.enviroment_ID);

							if (boolvars.vuelaatras == true)
							{
								nameanim.assign("Fall_FrontExplosion");
							}
							else
							{
								nameanim.assign("Fall_BackExplosion");
							}

							boolvars.iscjfrozen = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);

							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, nameanim.data(), "ironman", 10.0f, 0, 1, 1, 0, -2); //"ironman", 10.0f, 0, 1, 1, 0, -2);
							if (boolvars.immune == true)
								plugin::scripting::CallCommandById(COMMAND_DAMAGE_CHAR, player, 75, 1);

							nameanim.clear();

							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 800;

							if (boolvars.has_a_car == false)
							{
								boolvars.indx = 19;
							}
							else
							{
								boolvars.indx = 16;
								goto abort_throw_car;
							}
						}
						else
						{
							goto make_a_clank;
						}
						return true;
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_SHAKE_CAM, 20);
					}
				}
				else
				{
					if (debevolar == true)
					{
						CVector playpos = player->GetPosition();
						if (boolvars.stordexplpos.x != boolvars.explosionpos.x
							&& boolvars.stordexplpos.y != boolvars.explosionpos.y
							&& boolvars.stordexplpos.z != boolvars.explosionpos.z
							&& boolvars.expltype != 8
							&& boolvars.expltype != 9)
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") != 1 &&
								plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != 1
								&& boolvars.indx != 20
								&& boolvars.landgetup == 0 &&
								//lastexpl.m_bIsActive == true &&
								//DistanceBetweenPoints(playpos, boolvars.explosionpos) < 15.0f &&
								plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != 1 &&
								plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") != 1)
							{
								if (boolvars.has_a_car == true)
								{
									boolvars.indx = 16;
									goto abort_throw_car;
								}
								boolvars.stordexplpos.x = boolvars.explosionpos.x;
								boolvars.stordexplpos.y = boolvars.explosionpos.y;
								boolvars.stordexplpos.z = boolvars.explosionpos.z;

								boolvars.landgetup = 1;
								if (boolvars.mousewheelhacktrigger == true) {
									spd = 200.0f;
									UnhookWindowsHookEx(mousehook);
									jarvisvoice.SetVelocityUnlimited(false);
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
									boolvars.mousewheelhacktrigger = false;
								}

								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);


								CVector2D explhead;

								explhead.x = playpos.x - boolvars.stordexplpos.x;
								explhead.y = playpos.y - boolvars.stordexplpos.y;

								static float explhd;
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &explhd);

								float magnh = explhead.Magnitude();

								static float hadin;// newpedangle1;
								plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

								if (explhd > hadin - 90.0f && explhd < hadin + 90.0f)
								{
									/*
									CVector2D charhd, explhd;
									charhd.x = cosf(hadin);
									charhd.y = sinf(hadin);

									explhd.x = explhead.x / magnh;
									explhd.y = explhead.y / magnh;
									newpedangle1 = acosf((charhd.x * explhd.x + charhd.y * explhd.y));
									float newpedangle = newpedangle1 * 180.0f / 3.1415926535897932384f;
									if (newpedangle < 0.0f)
									{
										newpedangle = 360.0f + newpedangle;
									}
									if (newpedangle >= 90.0f && 270.0f >= newpedangle)
									{*/
									nameanim.assign("Fall_FrontExplosion");
								}
								else
								{
									nameanim.assign("Fall_BackExplosion");
								}

								boolvars.landgetup = 1;
								waswaiting = true;

								boolvars.iscjfrozen = false;
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								//if (jarvisvoice.is_over_ground(2.0f))
								//{
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, nameanim.data(), "ironman", 10.0f, 0, 1, 1, 0, -2); //"ironman", 10.0f, 0, 1, 1, 0, -2);
								if (boolvars.immune == true)
									plugin::scripting::CallCommandById(COMMAND_DAMAGE_CHAR, player, 75, 1);
								/*if (player->m_fArmour > 75.0f)
									player->m_fArmour = player->m_fArmour - 75.0f;
								else
									player->m_fArmour = 0.0f;

							}
							else
							{
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, nameanim.data(), "ironman", 10.0f, 0, 1, 1, 0, -2);
								//plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, nameanim.data(), "ironman", 4.0f, 0, 1, 1, 1, -2);
								if (player->m_fArmour > 75.0f)
									player->m_fArmour = player->m_fArmour - 75.0f;
								else
									player->m_fArmour = 0.0f;

							}*/

								nameanim.clear();
								//if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") != 1) {

								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 800;
								//}
								if (boolvars.has_a_car == false)
								{
									boolvars.indx = 20;
								}
								else
								{
									boolvars.indx = 16;
									goto abort_throw_car;
								}
							}
						}
					}
				}
			}
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, player) != 1)
			{
				if (plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_ARRESTED, player) == false)
				{
					if (boolvars.menuisactive == true)
					{
						goto suitmenu;
					}
					else
					{
						if (boolvars.indx == 25)
						{
							goto suitmenu;
						}
						else
						{
							WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							boolvars.menuisactive = false;


							/*if (ordenelegir == true)
							{*/
							if (!iscliqued && !isclicked && !hastoloadfriend && !hastoloadskin && !isentered)
							{
								if (boolvars.alphafad >= 40)
								{
									boolvars.alphafad -= 40;
									Drawfondo(boolvars.alphafad);
									//CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)boolvars.alphafad));
									return true;
								}
								else
								{
									boolvars.alphafad = 0;
									Drawfondo(boolvars.alphafad);
									ordenelegir = false;
									ordendada = false;
								}
							}
							//}
							cutenabled = plugin::patch::GetUChar(11989093, false);
							hoveredmark = -1;
							if (gameisstopped == true)
							{
								//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
								//plugin::scripting::CallCommandById(COMMAND_SET_TIME_SCALE, 1.0);
								//plugin::scripting::CallCommandById(COMMAND_SET_EVERYONE_IGNORE_PLAYER, 0, 0);
								gameisstopped = false;
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
								if (settings.radarmode != 2)
								{
									plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 1);
								}
							}
							/*
							if (boolvars.radarisactive == false && cutenabled != 1 && settings.radarmode != 2)
							{
								if (plugin::scripting::CallCommandById(COMMAND_HAS_CUTSCENE_LOADED) == true)
								{
									if (plugin::scripting::CallCommandById(COMMAND_HAS_CUTSCENE_FINISHED) == true)
									{
										plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 1);
										boolvars.radarisactive = true;
									}
								}
								else
								{
									plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 1);
									boolvars.radarisactive = true;
								}
							}*/
							cursorx = SCREEN_COORD_CENTER_X;
							cursory = SCREEN_COORD_CENTER_Y;

							if (IsAudioVoicePlayed(19) == true)
							{
								CFont::SetBackground(0, 0);
								CFont::SetColor(CRGBA(255, 255, 255, 255));
								CFont::SetOrientation(ALIGN_CENTER);
								CFont::SetProportional(true);
								CFont::SetJustify(false);
								CFont::SetFontStyle(FONT_MENU);
								CFont::SetEdge(2);
								CFont::SetScale(SCREEN_MULTIPLIER(settings.vecVehicleNameScale.x), SCREEN_MULTIPLIER(settings.vecVehicleNameScale.y));
								CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesPosnY), boolvars.online.data());
							}
							
							markk = boolvars.mark;
							//markk = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							boolvars.maxarmor = static_cast<float>(FindPlayerPed(0)->GetPlayerInfoForThisPlayerPed()->m_nMaxArmour);

							if (player->m_fHealth <= 0) {
								//if (ischoosingwp == true) {
									//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
									//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
									//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
									//ischoosingwp = false;
								//}
								WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
								boolvars.menuisactive = false;
								if (player->m_nModelIndex != 0)
								{

									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_NULL);

									plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
									//Sleep(200);
									boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
									boolvars.timetowait = 200;
									player = FindPlayerPed(0);
									if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != 0)
									{
										//plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP,
										plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);

										plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
										//Sleep(200);
										boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
										boolvars.timetowait = 200;
										player = FindPlayerPed(0);
										plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
									}
									boolvars.murio = true;
								}
								else
								{
									if (boolvars.activesuit != 0) {
										if (boolvars.activesuit > 0)
										{
											if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != 0)
											{
												//plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP,
												plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);

												plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
												//Sleep(200);
												boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
												boolvars.timetowait = 200;
												player = FindPlayerPed(0);
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
												boolvars.murio = true;
											}
										}
										else
										{
											static int head, shades, reloj, zapas;
											head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
											reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
											shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
											zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
											if (reloj == -1155750708 ||
												reloj == 1142040938 ||
												zapas == -592930753 ||
												head == 1348958410 ||
												shades == 672552983)
											{
												if (reloj == -1155750708 ||
													reloj == 1142040938)
												{
													plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_torso", "torso", 0);
													plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

												}
												if (zapas == -592930753)
												{
													plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_feet", "feet", 3);
													plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
												}
												if (head == 1348958410)
												{
													plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_face", "head", 1);
													plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
												}

												if (shades == 672552983) {
													plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 15);
													plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
												}

												//Sleep(200);
												boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
												boolvars.timetowait = 200;
												player = FindPlayerPed(0);
												boolvars.murio = true;
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
											}
										}
									}
								}
								if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true || isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
									JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
									boolvars.thrusterskilled = true;

									JarvisVoice::storeenviroment(&propellers.enviroment_ID);
								}
								WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

								//markk = boolvars.mark;
								markk = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

								if (boolvars.iscjfrozen == true)
								{
									boolvars.iscjfrozen = false;
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								}
								if (boolvars.indx == 13 || boolvars.has_a_car == true)
								{
									boolvars.indx = 16;
									goto abort_throw_car;
								}
								else
								{
									boolvars.indx = 0;
									return true;
								}
							}
							if (boolvars.suit[0] == true)
							{
								if (boolvars.suit[5] == true)
								{
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 1, 1, 0);
									boolvars.immune = true;

									boolvars.maxarmor = 1000;
									if (!wasdressed)
									{
										wasdressed = true;
										armorperc = player->m_fArmour;
										player->m_fArmour = 1000;

										if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != -1137657761)
											//AudioLib.PlayVoiceEvent(19);

											return true;
									}
									//importing preferences from ini files
								}
								else
								{
									if (wasdressed == true) {
										wasdressed = false;
										if (armorperc > boolvars.maxarmor)
										{
											armorperc = boolvars.maxarmor;
										}
										if (player->m_fArmour != armorperc)
										{
											player->m_fArmour = armorperc;
										}
									}
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 1, 1, 0);
									boolvars.immune = true;
									if (player->m_fArmour > boolvars.maxarmor)
									{
										player->m_fArmour = boolvars.maxarmor;
									}

									if (markk != 0)
									{
										WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									}

									if (boolvars.iscjfrozen == true) {
										boolvars.iscjfrozen = false;
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
									}
								}
							}
							else
							{
								if (boolvars.immune == true)
								{
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 0, 0, 0);
									boolvars.immune = false;
								}

								//if (ischoosingwp == true) {
									//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
									//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
									//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
								//}
								if (boolvars.iscjfrozen == true) {
									boolvars.iscjfrozen = false;
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								}

								if (markk != 0)
								{
									WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
								}

								if (wasdressed == true) {
									wasdressed = false;
									if (armorperc > boolvars.maxarmor)
									{
										armorperc = boolvars.maxarmor;
									}
									if (player->m_fArmour != armorperc)
									{
										player->m_fArmour = armorperc;
									}
								}

								if (boolvars.mousewheelhacktrigger == true) {
									spd = 200.0f;
									UnhookWindowsHookEx(mousehook);
									jarvisvoice.SetVelocityUnlimited(false);
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
									boolvars.mousewheelhacktrigger = false;
								}
							}



							/*
							if (currsuit != 44 && currsuit != 48)
							{
							}

						code22 = plugin::patch::GetInt(12010652, false);
						code22 += 4;
						code22 = plugin::patch::GetInt(code22, false);
						code23 = 0;*/
						//ciclos:

						//flyground
						//flyfast
						//clank
						//



							lastused = boolvars.activesuit;

							boolvars.activesuit = jarvisvoice.activesuitwearing();
							settings.marktraje = boolvars.activesuit;
							//if (lastused != boolvars.activesuit)
							//{

							//}


							static bool markloaded, botenemycreated, disawea;
							static int pressed;
							if (boolvars.activesuit != 0)
							{

								disawea = true;
								plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);

								if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == true && pressed != 5)
								{
									pressed = 5;
									//no usar armas con vest y manos de ironman
									plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
									boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									if (boolvars.wpn >= 1)
									{
										boolvars.wpn -= 1;
										char *numb;
										numb = new char[1];
										sprintf(numb, "%d", boolvars.wpn);
										WritePrivateProfileString("CONFIG", "WEAPON", numb, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
										delete[] numb;

									}
									else
									{
										boolvars.wpn = 5;
										WritePrivateProfileString("CONFIG", "WEAPON", "5", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									}
								}
								if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 7) == true && pressed != 7)
								{
									pressed = 7;
									//no usar armas con vest y manos de ironman
									plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
									boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									if (boolvars.wpn < 5)
									{
										boolvars.wpn += 1;
										char *numb;
										numb = new char[1];
										sprintf(numb, "%d", boolvars.wpn);
										WritePrivateProfileString("CONFIG", "WEAPON", numb, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
										delete[] numb;

									}
									else
									{
										boolvars.wpn = 0;
										WritePrivateProfileString("CONFIG", "WEAPON", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									}
								}
								if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == false
									&& plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 7) == false)
								{
									pressed = 0;
								}

								int level1;
								plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level1);
								if (level1 > 3) {
									if ((int)jarvisenemy[0].actorchar == 0)
									{
										int mark;
										mark = 17;
										if (settings.folderdirs[0].suits[2].texexists == true && settings.folderdirs[0].suits[2].mdlexists == true &&
											mark > 0)
										{
											if (boolvars.activesuit != 1)
												AudioLib.PlayVoiceEvent(21);

											if (plugin::scripting::CallCommandById(COMMAND_HAS_SPECIAL_CHARACTER_LOADED, 8) != 1)
											{
												//jarvisvoice.PlayShot(&jarvisenemy[enemyindex].botstream[1]);
												boolvars.timecreatedped = CTimer::m_snTimeInMillisecondsNonClipped;
												plugin::scripting::CallCommandById(COMMAND_LOAD_SPECIAL_CHARACTER, 8, "ultdron");
												plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
												goto endin;
											}
											else
											{
												int enemyindex = 0;
												pageenemy[enemyindex] = 2;
												CVector posplayer;
												posplayer.x = 1553.9343f;
												posplayer.y = -1675.4996f;
												posplayer.z = 78.3716f;

												static float direction;
												plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &direction);
												plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 1224, posplayer.x, posplayer.y, posplayer.z, &jarvisenemy[enemyindex].baseobj);
												objectlist[dim] = jarvisenemy[enemyindex].baseobj;
												dim++;
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisenemy[enemyindex].baseobj, direction);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, jarvisenemy[enemyindex].baseobj, 1);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ONLY_DAMAGED_BY_PLAYER, jarvisenemy[enemyindex].baseobj, 1);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_PROOFS, jarvisenemy[enemyindex].baseobj, 1, 1, 1, 1, 1);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, jarvisenemy[enemyindex].baseobj, 0);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, jarvisenemy[enemyindex].baseobj, 0.001f);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, jarvisenemy[enemyindex].baseobj, 0);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[enemyindex].baseobj, 0);
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisenemy[enemyindex].baseobj, 0.0f, 0.0f, 0.0);
												plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[enemyindex].baseobj, 1);
												jarvisenemy[enemyindex].isfrozen = true;

												plugin::scripting::CallCommandById(COMMAND_CREATE_CHAR, 6, MODEL_SPECIAL08, -100.0f, 100.0f, -40.0f, &jarvisenemy[enemyindex].actorid);
												plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, jarvisenemy[enemyindex].actorid, 0, 0, 0, 1, 0);
												plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEALTH, jarvisenemy[enemyindex].actorid, 10000);
												plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COLLISION, jarvisenemy[enemyindex].actorid, 1);
												plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ONLY_DAMAGED_BY_PLAYER, jarvisenemy[enemyindex].actorid, 0);
												plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, jarvisenemy[enemyindex].actorid, direction);

												plugin::scripting::CallCommandById(COMMAND_REMOVE_ALL_CHAR_WEAPONS, jarvisenemy[enemyindex].actorid);

												jarvisenemy[enemyindex].actorchar = CPools::GetPed(jarvisenemy[enemyindex].actorid);
												jarvisenemy[enemyindex].mark = mark;
												jarvisenemy[enemyindex].raysactive = false;
												jarvisenemy[enemyindex].damagetimer = NULL;
												jarvisenemy[enemyindex].taskindex = 0;
												jarvisenemy[enemyindex].thrusterscode = 0;
												jarvisenemy[enemyindex].velz = -1.0f;
												jarvisenemy[enemyindex].raystime = 0;
												jarvisenemy[enemyindex].hasacar = false;
												jarvisenemy[enemyindex].blip = 0;
												jarvisenemy[enemyindex].hp = 10000;
												jarvisenemy[enemyindex].wpn = 0;
												jarvisenemy[enemyindex].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
												jarvisenemy[enemyindex].contadorinicial = true;

												//Sleep(200);

												plugin::scripting::CallCommandById(COMMAND_UNLOAD_SPECIAL_CHARACTER, 8);
												plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, MODEL_SPECIAL08);

												plugin::scripting::CallCommandById(COMMAND_ATTACH_CHAR_TO_OBJECT, jarvisenemy[enemyindex].actorchar, jarvisenemy[enemyindex].baseobj, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0);
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[enemyindex].actorchar, "fly_botAimIdle", "ironman", 10.0f, 1, 1, 1, 0, -2);
												plugin::scripting::CallCommandById(COMMAND_ADD_BLIP_FOR_CHAR, jarvisenemy[enemyindex].actorchar, &jarvisenemy[enemyindex].blip);
												jarvisenemy[enemyindex].thrusterscode = apuntarbot;
												PlayThrustersIDLE(2, enemyindex, &jarvisenemy[enemyindex].botstream[0], (int)jarvisenemy[enemyindex].actorid, settings.folderdirs[enemyindex].suits[jarvisenemy[enemyindex].mark], 0, jarvisenemy[enemyindex].thrusterscode, &jarvisenemy[enemyindex].jarvisprop);
												mark = -1;

												for (int u = 0; u <= 2; u++)
												{
													jarvisenemy[enemyindex].ibeam[u] = NULL;
												}
												dimenemy++;
												botenemycreated = true;
												fflush(stdin);
												boolvars.timecreatedped = CTimer::m_snTimeInMillisecondsNonClipped;
												goto endin;
											}
										}
									}
								}
								else
								{
									botenemycreated = false;
								}
							}
							else
							{
								pressed = 0;
								if (disawea == true)
								{
									disawea = false;
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
								}
							}
							int flymode = GetPrivateProfileInt("CONFIG", "FLYMOUSE", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

							if (flymode == 0)
							{
								boolvars.vueloconmouse = false;
							}
							else
							{
								boolvars.vueloconmouse = true;
							}

							settings.hud = GetPrivateProfileInt("CONFIG", "HUD", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

							fflush(stdin);
							if (boolvars.indx == 0)
							{
								goto ciclobasico;
							}
							else
							{
								if (boolvars.indx == 7)
								{
									if (boolvars.activesuit != 0 &&
										boolvars.activesuit != -5 &&
										boolvars.activesuit != -4)
									{
										goto levantarauto;
									}
									else
									{
										boolvars.indx = 0;
										goto ciclobasico;
									}
								}
								else
								{
									if (boolvars.indx == 8)
									{
										if (boolvars.activesuit != 0 &&
											boolvars.activesuit != -5 &&
											boolvars.activesuit != -4)
										{
											goto origcareliminate;
										}
										else
										{
											boolvars.indx = 0;
											goto ciclobasico;
										}
									}
									else
									{
										if (boolvars.indx == 9)
										{
											if (boolvars.activesuit != 0 &&
												boolvars.activesuit != -5 &&
												boolvars.activesuit != -4)
											{
												goto loadmodels;
											}
											else
											{
												boolvars.indx = 0;
												goto ciclobasico;
											}
										}
										else
										{
											if (boolvars.indx == 10)
											{
												if (boolvars.activesuit != 0 &&
													boolvars.activesuit != -5 &&
													boolvars.activesuit != -4)
												{
													goto createandlift;
												}
												else
												{
													boolvars.indx = 0;
													goto ciclobasico;
												}
											}
											else
											{
												if (boolvars.indx == 11)
												{
													if (boolvars.activesuit != 0 &&
														boolvars.activesuit != -5 &&
														boolvars.activesuit != -4)
													{
														goto animate;
													}
													else
													{
														boolvars.indx = 0;
														goto ciclobasico;
													}
												}
												else
												{
													if (boolvars.indx == 12)
													{
														if (boolvars.activesuit != 0 &&
															boolvars.activesuit != -5 &&
															boolvars.activesuit != -4)
														{
															goto attachnewcar;
														}
														else
														{
															boolvars.indx = 0;
															goto ciclobasico;
														}
													}
													else
													{
														if (boolvars.indx == 13)
														{
															goto throwcar;
														}
														else
														{
															if (boolvars.indx == 14)
															{
																goto animthrowcar;
															}
															else
															{
																if (boolvars.indx == 15)
																{
																	goto carfly;
																}
																else
																{
																	if (boolvars.indx == 16)
																	{
																		boolvars.has_a_car = false;
																		goto abort_throw_car;
																	}
																	else
																	{
																		if (boolvars.indx == 17)
																		{
																			goto unload_car;
																		}
																		else
																		{
																			if (boolvars.indx == 18)
																			{
																				if (boolvars.activesuit != -4 && boolvars.activesuit != -5 && boolvars.activesuit != 0)
																				{
																					goto volarbajo;
																				}
																				else
																				{
																					if (boolvars.landgetup == 1)
																					{
																						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != true
																							&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != true)
																						{
																							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == true)
																							{
																								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
																								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
																								boolvars.thrusterskilled = true;
																								if (boolvars.mousewheelhacktrigger == true) {
																									spd = 200.0f;
																									UnhookWindowsHookEx(mousehook);
																									jarvisvoice.SetVelocityUnlimited(false);
																									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
																									boolvars.mousewheelhacktrigger = false;
																								}
																								boolvars.landgetup = 1;
																								//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
																								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getupfront", "ironman", 4.0f, 0, 1, 1, 0, -1);
																								//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
																								//player->SetHeading(jarvisvoice.newanglepos());
																								//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
																							}
																							else
																							{
																								if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == true)
																								{
																									JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
																									JarvisVoice::storeenviroment(&propellers.enviroment_ID);
																									boolvars.thrusterskilled = true;
																									if (boolvars.mousewheelhacktrigger == true) {
																										spd = 200.0f;
																										UnhookWindowsHookEx(mousehook);
																										jarvisvoice.SetVelocityUnlimited(false);
																										plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
																										boolvars.mousewheelhacktrigger = false;
																									}
																									boolvars.landgetup = 1;
																									//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
																									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getup", "ironman", 4.0f, 0, 1, 1, 0, -1);
																									//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
																									//player->SetHeading(jarvisvoice.newanglepos());
																									//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
																								}
																							}
																						}
																						else
																						{
																							boolvars.landgetup = 1;

																						}
																					}
																					if (boolvars.has_a_car == false)
																					{
																						boolvars.indx = 20;
																						goto make_a_clank;
																					}
																					else
																					{
																						boolvars.indx = 16;
																						goto abort_throw_car;
																					}
																				}
																			}
																			else
																			{
																				if (boolvars.indx == 19)
																				{
																					if (boolvars.activesuit != -4 && boolvars.activesuit != -5 && boolvars.activesuit != 0)
																					{
																						if (is_on_foot() == true && boolvars.has_a_car == false)
																						{
																							boolvars.indx = 20;
																							goto make_a_clank;
																						}
																						else
																						{
																							goto controlesdevuelo;
																						}
																					}
																					else
																					{
																						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
																						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
																						//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
																						//player->SetHeading(jarvisvoice.newanglepos());
																						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());
																						boolvars.indx = 0;
																						goto ciclobasico;
																					}
																				}
																				else
																				{
																					if (boolvars.indx == 20)
																					{
																						if (debevolar == true)
																						{
																							goto make_a_clank;
																						}
																						else
																						{
																							boolvars.indx = 22;
																							goto aimin;
																						}
																					}
																					else
																					{
																						if (boolvars.indx == 21)
																						{
																							if (boolvars.activesuit != -4 && boolvars.activesuit != -5 && boolvars.activesuit != 0)
																							{
																								goto impulsarse;
																							}
																							else
																							{
																								if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != true
																									&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != true)
																								{
																									if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == true)
																									{
																										JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
																										JarvisVoice::storeenviroment(&propellers.enviroment_ID);
																										boolvars.thrusterskilled = true;
																										if (boolvars.mousewheelhacktrigger == true) {
																											spd = 200.0f;
																											UnhookWindowsHookEx(mousehook);
																											jarvisvoice.SetVelocityUnlimited(false);
																											plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
																											boolvars.mousewheelhacktrigger = false;
																										}
																										boolvars.landgetup = 1;
																										//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
																										plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getupfront", "ironman", 4.0f, 0, 1, 1, 0, -1);
																										//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
																										//player->SetHeading(jarvisvoice.newanglepos());
																										//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
																									}
																									else
																									{
																										if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == true)
																										{
																											JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
																											JarvisVoice::storeenviroment(&propellers.enviroment_ID);
																											boolvars.thrusterskilled = true;
																											if (boolvars.mousewheelhacktrigger == true) {
																												spd = 200.0f;
																												UnhookWindowsHookEx(mousehook);
																												jarvisvoice.SetVelocityUnlimited(false);
																												plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
																												boolvars.mousewheelhacktrigger = false;
																											}
																											boolvars.landgetup = 1;
																											//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
																											plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getup", "ironman", 4.0f, 0, 1, 1, 0, -1);
																											//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
																											//player->SetHeading(jarvisvoice.newanglepos());
																											//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
																										}
																									}
																								}
																								else
																								{
																									boolvars.landgetup = 1;
																								}
																								goto make_a_clank;
																							}
																						}
																						else
																						{
																							if (boolvars.indx == 22)
																							{
																								goto aimin;
																							}
																							else
																							{
																								if (boolvars.indx == 23)
																								{
																									if (boolvars.activesuit != -4 && boolvars.activesuit != -5 && boolvars.activesuit != 0)
																									{
																										goto volarsolo;
																									}
																									else
																									{
																										plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
																										plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
																										//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
																										//player->SetHeading(jarvisvoice.newanglepos());
																										plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());
																										boolvars.indx = 0;
																										goto ciclobasico;
																									}
																								}
																								else
																								{
																									boolvars.indx = 0;
																									goto ciclobasico;
																									
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
												}
											}
										}
									}
								}
							}
							/*
							case 24:
							{
								goto wpicons;
								break;
							}*/
						}
					}
				}
				else
				{
					if (wasdressed == true) {
						boolvars.aimedpeds[0] = 0;
						boolvars.aimedpeds[1] = 0;
						boolvars.aimedpeds[2] = 0;
						boolvars.aimedpeds[3] = 0;
						boolvars.aimedpeds[4] = 0;
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, player) == true)
						{
							boolvars.landgetup = 0;
							player->m_fArmour = 0;
						}
						else
						{
							player->m_fArmour = armorperc;
						}
						if (boolvars.iscjfrozen == true) {
							boolvars.iscjfrozen = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						}
						wasdressed = false;
					}
					boolvars.indx = 0;
					return true;
				}
			}
			else
			{
				boolvars.landgetup = 0;
				player->m_fArmour = 0;
			}
		}
		else
		{
			boolvars.indx = 0;
			return true;
		}
	}
	else
	{
		pActor = (DWORD*)0xB6F5F0;
		if (onxe == true)
		{
			if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 407) != true) { onxe = false; }
			
			if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 594) == true) { onxe = false; }

			if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 345) != true) { onxe = false; }

			if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, 1224) != true) { onxe = false; }

			//if (plugin::scripting::CallCommandById(COMMAND_HAS_ANIMATION_LOADED, "ironman") != true) { onxe = false; }

			if (plugin::scripting::CallCommandById(COMMAND_HAS_ANIMATION_LOADED, "parachute") != true) { onxe = false; }

			if (plugin::scripting::CallCommandById(COMMAND_HAS_ANIMATION_LOADED, "ironman") != true) { onxe = false; }

		}
		player = FindPlayerPed(0);
		if (player->m_fHealth < 1)
		{
			WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			boolvars.menuisactive = false;


			if (player->m_nModelIndex != 0)
			{
				 
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_MODEL, 0, MODEL_NULL);
				plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
				//Sleep(200);
				boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
				boolvars.timetowait = 200;
				player = FindPlayerPed(0);
				boolvars.murio = true;
			}
			else
			{
				if (boolvars.activesuit != 0)
				{
					if (boolvars.activesuit > 0)
					{
						if (player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[17] != 0)
						{
							//plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP,
							plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 17);

							plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							//Sleep(200);
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 200;
							player = FindPlayerPed(0);
							boolvars.murio = true;
						}
					}
					else
					{
						static int head, shades, reloj, zapas;
						head = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[1];
						reloj = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[0];
						shades = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[15];
						zapas = player->m_pPlayerData->m_pPedClothesDesc->m_anTextureKeys[3];
						if (reloj == -1155750708 ||
							reloj == 1142040938 ||
							zapas == -592930753 ||
							head == 1348958410 ||
							shades == 672552983)
						{
							if (reloj == -1155750708 ||
								reloj == 1142040938)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_torso", "torso", 0);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);

							}
							if (zapas == -592930753)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_feet", "feet", 3);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							}
							if (head == 1348958410)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES_OUTSIDE_SHOP, 0, "player_face", "head", 1);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							}

							if (shades == 672552983) {
								plugin::scripting::CallCommandById(COMMAND_GIVE_PLAYER_CLOTHES, 0, 0, 0, 15);
								plugin::scripting::CallCommandById(COMMAND_BUILD_PLAYER_MODEL, 0);
							}

							//Sleep(200);
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 200;
							player = FindPlayerPed(0);
							boolvars.murio = true;
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						}
					}
				}
			}
			//static int mark;// , error1;
			//boolvars.mark = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

			if (boolvars.mark != 0)
			{
				WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			}

			if (boolvars.iscjfrozen == true) {
				boolvars.iscjfrozen = false;
				plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
			}
			//error1 = GetPrivateProfileInt("CONFIG", "ERROR", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			//if (error1 != 0)
			//{
			//	WritePrivateProfileString("CONFIG", "ERROR", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			//}
		}

		return true;
	}

	//0
ciclobasico:
	{
	/*
	CFont::SetBackground(0, 0);
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetProportional(true);
	CFont::SetJustify(false);
	CFont::SetColor(CRGBA(255, 255, 255, 255));
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetEdge(2);
	CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
	CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
	CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), lng1.data());
	*/
	if (*pActor > 0)
	{
		if (boolvars.wpmenuisactive == false && boolvars.indx!=24)
		{

			if (boolvars.activesuit != 0 && player->m_nPedState != PEDSTATE_DRIVING)
			{

				if (boolvars.punchedtargetsexist == true &&
					boolvars.suit[3] == true)
				{
					jarvisvoice.Display_targets_on_screen(boolvars.wpn, boolvars.aimedpeds, imhud2, imhud3);
					boolvars.indx = 0;
					*boolvars.aimedpeds = *jarvisvoice.Neutralize_targets(boolvars.wpn, boolvars.aimedpeds, &rockets, &boolvars.punchedtargetsexist);
					return true;
				}
				if (jarvisvoice.is_aiming() == true)
				{
					boolvars.indx = 22;
					return true;
				}
				else
				{
					if (boolvars.activesuit != -4 && boolvars.activesuit != -5)
					{
						if (jarvisvoice.is_over_ground(2.0f) == true)
						{
							if (!boolvars.mousewheelhacktrigger) {
								jarvisvoice.SetVelocityUnlimited(true);
								mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
								boolvars.mousewheelhacktrigger = true;
							}

							boolvars.indx = 19;
							goto controlesdevuelo;
						}
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_glide") == true ||
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_launch") == true ||
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_launch_R") == true)
						{
							float altura = 0.0f;
							//super saltos
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEIGHT_ABOVE_GROUND, player, &altura);
							if (altura <= 1.0f)
							{
								boolvars.indx = 0;
							}
							else
							{
								float angleplayer = player->GetHeading();
								CVector vel = { sinf(angleplayer) * -10.0f, cosf(angleplayer) * 10.0f, 20.0f };
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, vel.x, vel.y, vel.z);
							}
							boolvars.indx = 19;
							goto controlesdevuelo;
						}
						
						if (boolvars.mousewheelhacktrigger == true) {
							spd = 200.0f;
							UnhookWindowsHookEx(mousehook);
							jarvisvoice.SetVelocityUnlimited(false);
							plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
							boolvars.mousewheelhacktrigger = false;
						}
						if (GetKeyState('E') & 0x8000)
						{
							boolvars.indx = 7;
							goto endin;
						}
						else
						{
							AimedPedReactiontwo(&boolvars.ped);

							if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true || isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							}
							if (GetKeyState('X') & 0x8000)
							{
								boolvars.indx = 21;
								goto impulsarse;
							}
							else
							{
								/*if (GetKeyState(VK_MBUTTON) & 0x8000)
								{
									boolvars.wpmenuisactive = true;
									boolvars.indx = 24;
									goto wpicons;
								}
								else
								{*/
									if (boolvars.iscjfrozen == true)
									{
										boolvars.iscjfrozen = false;
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
									}
									return true;
								//}
							}

						}
					}
					else
					{
						//nousatraje

						if (boolvars.iscjfrozen == true)
						{
							boolvars.iscjfrozen = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						}
						return true;
					}
				}
			}
			else
			{

				if (boolvars.mousewheelhacktrigger == true) {
					spd = 200.0f;
					UnhookWindowsHookEx(mousehook);
					jarvisvoice.SetVelocityUnlimited(false);
					plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
					boolvars.mousewheelhacktrigger = false;
				}
				if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true || isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
					boolvars.thrusterskilled = true;

					JarvisVoice::storeenviroment(&propellers.enviroment_ID);
				}
				if (boolvars.iscjfrozen == true)
				{
					boolvars.iscjfrozen = false;
					plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
				}
				return true;
			}
		}
		else
		{
			boolvars.wpmenuisactive = false;
			boolvars.indx = 0;
			return true;
		}
	}
	else
	{
		pActor = (DWORD*)0xB6F5F0;
		return true;
	}
	return true;
	}
	//7
levantarauto:
	{
		if (*pActor > 0)
		{
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, boolvars.car1) == false)
			{
				if (jarvisvoice.got_car(&boolvars.car1) == true) {
					if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "lift_car") == false) {
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "lift_car", "ironman", 4.0f, 1, 1, 1, 0, 1000);
						boolvars.indx = 8;
						return true;
					}
					else {
						boolvars.indx = 8;
						return true;
					}
				}
				else
				{
					boolvars.indx = 0;
					goto cleartask;
				}
			}
			else
			{
				boolvars.indx = 8;
				return true;
			}
		}
		else
		{
			pActor = (DWORD*)0xB6F5F0;
			return true;
		}
	}

	//8
origcareliminate:
	{
		if (*pActor > 0)
		{
			static float time;
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "lift_car") != false) {
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_ANIM_CURRENT_TIME, player, "lift_car", &time);
				if (time >= 0.5f) {
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, boolvars.car1) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);

						if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper12) == true) {
							plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, helper12);
							helper12 = NULL;
						}
						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CAR_IN_WORLD_COORDS, boolvars.car1, 0.0f, 0.0f, 0.0f, &carposnx, &carposny, &carposnz);
						plugin::scripting::CallCommandById(COMMAND_GET_CAR_COLOURS, boolvars.car1, &clr1, &clr2);

						plugin::scripting::CallCommandById(COMMAND_GET_NUMBER_OF_PASSENGERS, boolvars.car1, &passengers);

						CVehicle *aux = CPools::GetVehicle(boolvars.car1);
						liftedcarmodel = aux->m_nModelIndex;
						/*
						if (passengers > 0) {
						CPed *actor = aux->PickRandomPassenger();
						int pactor = (int)actor;
						if (pactor > 0) {
						if (aux->IsDriver(actor) == false) {
						mdlindex[1] = actor->m_nModelIndex;
						}
						else {
						mdlindex[0] = actor->m_nModelIndex;
						}
						}

						plugin::scripting::CallCommandById(COMMAND_DELETE_CHAR, actor);
						plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, actor);
						if (passengers > 1) {
						CPed *actor1 = aux->PickRandomPassenger();
						int pactor1 = (int)actor1;
						if (pactor1 > 0) {
						if (aux->IsDriver(actor1) == false) {
						if (pactor != pactor1) {
						mdlindex[2] = actor1->m_nModelIndex;
						}
						}
						else {
						mdlindex[0] = actor1->m_nModelIndex;
						}
						}
						plugin::scripting::CallCommandById(COMMAND_DELETE_CHAR, actor1);
						plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, actor1);
						if (passengers > 2) {
						CPed *actor2 = aux->PickRandomPassenger();
						int pactor2 = (int)actor2;
						if (pactor2 > 0) {
						if (aux->IsDriver(actor2) == false) {
						if ((pactor2 != pactor1) && (pactor2 != pactor))
						mdlindex[3] = actor2->m_nModelIndex;
						}
						else {
						mdlindex[0] = actor2->m_nModelIndex;
						}
						}
						plugin::scripting::CallCommandById(COMMAND_DELETE_CHAR, actor2);
						plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, actor2);
						}
						}
						}*/
						angle = aux->GetHeading();
						plugin::scripting::CallCommandById(COMMAND_GET_CAR_COORDINATES, boolvars.car1, &opsx, &opsy, &opsz);
						plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, boolvars.car1);
						plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, boolvars.car1);
						boolvars.car1 = NULL;
						boolvars.indx = 9;
						goto loadmodels;
					}
					else
					{
						boolvars.indx = 0;
						goto cleartask;
					}
				}
				else {
					//i keep on waiting
					goto endin;
				}
			}
			else
			{
				//boolvars.indx = 0;//not sure
				return true;
			}
		}
		else {
			pActor = (DWORD*)0xB6F5F0;
			return true;
		}
}
	//9
loadmodels:
	{
		if (*pActor > 0)
		{
			if (plugin::scripting::CallCommandById(COMMAND_HAS_MODEL_LOADED, liftedcarmodel) == true) {
				if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == false || plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objeto1) == false)
				{
					boolvars.indx = 10;
					goto createandlift;
				}
				else {
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic1 != vehic) {
						plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
						vehic1 = NULL;

					}
					boolvars.indx = 11;
					goto animate;
				}
			}
			else {
				/*if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true && plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objeto1) == true) {
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic1 != vehic) {
						plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
						vehic1 = NULL;

					}
					boolvars.indx = 11;
					goto animate;
				}
				else
				{*/
				plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, liftedcarmodel);
				plugin::scripting::CallCommandById(COMMAND_LOAD_ALL_MODELS_NOW);
				goto endin;
				//}
			}
		}
		else {
			pActor = (DWORD*)0xB6F5F0;
			return true;
		}
}
	//10
createandlift:
	{
		if (*pActor > 0)
	{

		plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 1224, carposnx, carposny, carposnz, &objeto1);
		objectlist[dim] = objeto1;
		dim++;
		plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, objeto1, 0);
		plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, objeto1, 0);
		plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, objeto1, 0.001f);
		plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, objeto1, 0);

		//Sleep(20);
		boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
		boolvars.timetowait = 150;

		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper12) == true) {
			plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, helper12);
			helper12 = NULL;
		}

		plugin::scripting::CallCommandById(COMMAND_CREATE_CAR, liftedcarmodel, carposnx, carposny, carposnz, &vehic);

		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic1 != vehic) {
			plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
			vehic1 = NULL;

		}
		if (liftedcarmodel == 599 || liftedcarmodel == 523 || liftedcarmodel == 596 || liftedcarmodel == 597 || liftedcarmodel == 598) {
			int level = 0;
			plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level);
			if (level < 3) {
				level += 1;
				plugin::scripting::CallCommandById(COMMAND_ALTER_WANTED_LEVEL_NO_DROP, 0, level);
			}
		}
		/*if (passengers > 0) {
		if (mdlindex[0] > 0) {
		CPed *driver;
		plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, mdlindex[0]);
		plugin::scripting::CallCommandById(COMMAND_CREATE_CHAR_INSIDE_CAR, 4, mdlindex[0], vehi, &driver);
		plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, mdlindex[0]);
		plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, driver);
		driver = NULL;
		}
		if (mdlindex[1] > 0) {
		CPed *pass1;
		plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, mdlindex[1]);
		plugin::scripting::CallCommandById(COMMAND_CREATE_CHAR_AS_PASSENGER, 4, mdlindex[1], vehi, 0, &pass1);
		plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, mdlindex[1]);
		plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, pass1);
		pass1 = NULL;
		}
		if (mdlindex[2] > 0) {
		CPed *pass2;
		plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, mdlindex[2]);
		plugin::scripting::CallCommandById(COMMAND_CREATE_CHAR_AS_PASSENGER, 4, mdlindex[2], vehi, 1, &pass2);
		plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, mdlindex[2]);
		plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, pass2);
		pass2 = NULL;
		}
		if (mdlindex[3] > 0) {
		CPed *pass3;
		plugin::scripting::CallCommandById(COMMAND_REQUEST_MODEL, mdlindex[3]);
		plugin::scripting::CallCommandById(COMMAND_CREATE_CHAR_AS_PASSENGER, 4, mdlindex[3], vehi, 2, &pass3);
		plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, mdlindex[3]);
		plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, pass3);
		pass3 = NULL;
		}

		}*/
		plugin::scripting::CallCommandById(COMMAND_CHANGE_CAR_COLOUR, vehic, clr1, clr2);
		plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 0);


		plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEALTH, vehic, 1000);
		plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, vehic, 1, 1, 1, 1, 1);
		plugin::scripting::CallCommandById(COMMAND_SET_CAR_COORDINATES, vehic, carposnx, carposny, carposnz);
		plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEADING, vehic, angle);
		
		if (liftedcarmodel == 537 || liftedcarmodel == 538 || liftedcarmodel == 569 || liftedcarmodel == 570) {
			u = 2.05f;
		}
		else
		{
			if (liftedcarmodel == 449)
			{
				u = 1.25f;
			}
			else
			{
				u = 0.7898f;
			}
		}

		plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_OBJECT, vehic, objeto1, u, 0.65f, 0.0f, 0.0f, 90.0f, 0.0f);
		CVehicle *auxil = CPools::GetVehicle(vehic);
		//auxil->SetRwObjectAlpha(150);

		boolvars.indx = 11;
		goto animate;
	}
		else {
			pActor = (DWORD*)0xB6F5F0;
			return true;
		}
}
//11
animate:
{
if (*pActor > 0)
	{
		if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "hold_car") == false)
		{
			plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_SECONDARY, player, "hold_car", "ironman", 18.0f, 1, 1, 1, 0, -1);
			boolvars.indx = 12;
			goto endin;
		}
		else
		{
			boolvars.indx = 12;
			goto attachnewcar;
		}
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}
	//12
attachnewcar:
{
	if (*pActor > 0)
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic != vehic1) {
			plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
			vehic1 = NULL;
		}
		plugin::scripting::CallCommandById(COMMAND_GET_CAR_MODEL, vehic, &liftedcarmodel);
		if (liftedcarmodel == 537 || liftedcarmodel == 538 || liftedcarmodel == 449 || liftedcarmodel == 569 || liftedcarmodel == 570)
		{
			//plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, objeto1, 0.01f);
			//plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, objeto1, 0);
			//plugin::scripting::CallCommandById(COMMAND_TASK_PICK_UP_OBJECT, player, objeto1, 0.0f, 0.0f, 0.0f, 6, 16, "NULL", "NULL", 1);
			plugin::scripting::CallCommandById(COMMAND_ATTACH_OBJECT_TO_CHAR, objeto1, player, 0.5f, 0.0959f, 0.879f, -12.0f, -90.0f, 50.0f);
			plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, objeto1, 0.01f);
			plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, objeto1, 0);
		}
		else
		{
			//plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, objeto1, 0.01f);
			//plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, objeto1, 0);
			//plugin::scripting::CallCommandById(COMMAND_TASK_PICK_UP_OBJECT, player, objeto1, 0.0f, 0.0f, 0.0f, 6, 16, "NULL", "NULL", 1);
			plugin::scripting::CallCommandById(COMMAND_ATTACH_OBJECT_TO_CHAR, objeto1, player, 0.5f, 0.0959f, 0.879f, -12.0f, -90.0f, 50.0f);
			plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, objeto1, 0.01f);
			plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, objeto1, 0);
		}
		//Sleep(200);

		//plugin::scripting::CallCommandById(COMMAND_ATTACH_CAMERA_TO_CHAR, player, 0.0f, -7.0f, 5.0f, 0.0f, 0.0f, 2.0f, 0.0f, 2);
		//plugin::patch::SetUChar(11988014, 0, 0);
		plugin::scripting::CallCommandById(COMMAND_POINT_CAMERA_AT_CAR, vehic, 18, 1);
		plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_IN_CAR_CAMERA_MODE, 2);

		//Sleep(20);
		plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, 1224);
		plugin::scripting::CallCommandById(COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED, liftedcarmodel);
		boolvars.indx = 13;
		boolvars.has_a_car = true;
		goto throwcar;
	}
	else {
		pActor = (DWORD*)0xB6F5F0;
		return true;
	}
}
	//13
throwcar:
{
	if (*pActor > 0)
	{
		if (boolvars.activesuit!=0 && boolvars.activesuit != -4 && boolvars.activesuit != -5 && boolvars.activesuit != -6)
		{
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true && vehic1 != vehic) {
				plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
				vehic1 = NULL;
			}
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true) {
				float pesoauto = 0.0f;
				int classauto;
				
				plugin::scripting::CallCommandById(COMMAND_GET_VEHICLE_CLASS, vehic, &classauto);
				
				plugin::scripting::CallCommandById(COMMAND_GET_CAR_MASS, vehic, &pesoauto);
				//char *shades;
				//std::string shad;
				//shades = new char[20];
				// 
				//sprintf(shades, "%.0f", pesoauto);
				//"%s", settings.folderdirs[boolvars.yndex].suits[0].pngname); //shades, settings.folderdirs[4].name);
				/*usas la %d con %d trajes. ", boolvars.yndex, settings.folderdirs[boolvars.yndex].subcontentamnt*/
				//shad += shades;
				//shad = ;
				/*
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
				if (classauto == 11)
				{
					pesoauto = -2000;
				}

				jarvisvoice.aimcar((int)pesoauto, &pricell.x, &pricell.y, &pricell.z, &pricel2.x, &pricel2.y, &pricel2.z);
				if (pad->GetCarGunFired() != 0 || CMouseControllerState().lmb == 1 || GetKeyState(VK_LBUTTON) & 0x8000 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) == true) {

					pad->bDisablePlayerEnterCar = 0;
					//pad->bDisablePlayerJump = 0;
					pad->bDisablePlayerFireWeaponWithL1 = 0;
					plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_NEVER_GETS_TIRED, 0, 0);

					boolvars.has_a_car = false;

					CVehicle *auxil = CPools::GetVehicle(vehic);

					//auxil->SetRwObjectAlpha(255);
					plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

					plugin::scripting::CallCommandById(COMMAND_GET_CAR_MODEL, vehic, &liftedcarmodel);
					if (liftedcarmodel == 537 || liftedcarmodel == 538 || liftedcarmodel == 449 || liftedcarmodel == 569 || liftedcarmodel == 570)
					{
						CVector poss;
						if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objeto1) == true && objeto1 != NULL) {
							plugin::scripting::CallCommandById(COMMAND_DETACH_OBJECT, objeto1, 0.0f, 0.0f, 0.0f, 0);
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, objeto1);
							objeto1 = NULL;
							plugin::scripting::CallCommandById(COMMAND_DETACH_CAR, vehic, 0.0f, 0.0f, 0.0f, 0);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 0);
						}
						if (liftedcarmodel == 449) {
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 1.5f, 1.879f, &poss.x, &poss.y, &poss.z);
						}
						else {
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 2.5f, 1.879f, &poss.x, &poss.y, &poss.z);
						}

						if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper) == false) {
							plugin::scripting::CallCommandById(COMMAND_GET_CAR_HEADING, vehic, &hdn);
							plugin::scripting::CallCommandById(COMMAND_CREATE_CAR, 407, poss.x, poss.y, poss.z, &helper);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, helper, 1);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEADING, helper, hdn);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_VISIBLE, helper, 0);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, helper, 1, 1, 1, 1, 1);
							if (liftedcarmodel == 449) {
								plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, hdn);
							}
							else {
								plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, hdn);
							}
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);

							plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, helper, 1);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, helper, 1);
							plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, helper, 1, 1, 1, 1, 1);
						}
					}
					else
					{
						if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objeto1) == true) {
							plugin::scripting::CallCommandById(COMMAND_DETACH_OBJECT, objeto1, 0.0f, 0.0f, 0.0f, 0);
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, objeto1);
							objeto1 = NULL;
							plugin::scripting::CallCommandById(COMMAND_DETACH_CAR, vehic, 0.0f, 0.0f, 0.0f, 0);
						}
						plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, vehic, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, vehic, 0, 0, 0, 0, 0);

					}
					plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangleposforflight());
					if (jarvisvoice.is_over_ground(2.0f) == false)
					{
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM, player, "throw_car", "ironman", 20.0f, 0, 0, 0, 0, -1);
					}
					else {
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "throw_car", "ironman", 10.0f, 0, 0, 0, 0, -1);
					}
					boolvars.indx = 14;
					return true;

				}
				else
				{
					if (GetKeyState(VK_RETURN) & 0x8000 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 15) == true)
					{
						boolvars.has_a_car = false;
						boolvars.indx = 16;
						goto abort_throw_car;
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_NEVER_GETS_TIRED, 0, 1);
						pad->bDisablePlayerEnterCar = 1;
						//pad->bDisablePlayerJump = 1;
						pad->bDisablePlayerFireWeaponWithL1 = 1;

						plugin::patch::SetUChar(12006502, 0, 0);
						plugin::patch::SetUChar(12006498, 0, 0);
						//float coord2dx, coord2dy, w, h;
						//jarvisvoice.coordstoscreen(pricell.x, pricell.y, pricell.z, &coord2dx, &coord2dy, &w, &h);
						movtextures.drawindexedtexture(12);
						
						if (jarvisvoice.is_over_ground(2.0f)==true)
						{
							if (!boolvars.mousewheelhacktrigger) {
								jarvisvoice.SetVelocityUnlimited(true);
								mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
								boolvars.mousewheelhacktrigger = true;
							}
							if (!pad->GetSprint())
							{
								boolvars.spdmouse = false;
								//plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_OBJECT, vehic, objeto1, u, 0.65f, 0.0f, 0.0f, 90.0f, 0.0f);
								goto controlesdevuelo;
							}
							else
							{
								boolvars.indx = 16;
								goto abort_throw_car;
							}
						}
						else
						{
							if ((isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL)==true) || (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL)==true)) {
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							}
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "hold_car") == false)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_SECONDARY, player, "hold_car", "ironman", 18.0f, 1, 1, 1, 0, -1);
							}
							if (boolvars.mousewheelhacktrigger == true) {
								spd = 200.0f;
								UnhookWindowsHookEx(mousehook);
								jarvisvoice.SetVelocityUnlimited(false);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
								boolvars.mousewheelhacktrigger = false;
							}
							goto impulsarse;
						}
					}
				}

				//}
				//else
				//{
					//goto abort_throw_car;
				//}
			}
			else
			{
				boolvars.indx = 16;
				goto abort_throw_car;
			}
		}
		else
		{
			if (boolvars.mousewheelhacktrigger == true) {
				spd = 200.0f;
				UnhookWindowsHookEx(mousehook);
				jarvisvoice.SetVelocityUnlimited(false);
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
				boolvars.mousewheelhacktrigger = false;
			}
			boolvars.has_a_car = false;
			boolvars.indx = 16;
			goto abort_throw_car;
		}
		return true;
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}
	//14
animthrowcar:
{
if (*pActor > 0)
	{

	if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "throw_car") != false) {
		boolvars.has_a_car = false;
		plugin::scripting::CallCommandById(COMMAND_GET_CHAR_ANIM_CURRENT_TIME, player, "throw_car", &time1);
		if (time1 >= 0.285f) {
			dir1 = pricel2.x;
			dir2 = pricel2.y;
			dir3 = pricel2.z;
			vel1 = pricell.x;
			vel2 = pricell.y;
			vel3 = pricell.z;
			plugin::scripting::CallCommandById(COMMAND_GET_CAR_HEADING, vehic, &hdn);
			//plugin::patch::SetUChar(11988014, 1, 0);
			plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
			plugin::scripting::CallCommandById(COMMAND_SET_CAMERA_BEHIND_PLAYER);
			plugin::scripting::CallCommandById(COMMAND_GET_CAR_MODEL, vehic, &liftedcarmodel);
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper) == true)
			{
				if (liftedcarmodel == 449) {
					plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, hdn);
				}
				else {
					if (liftedcarmodel == 537 || liftedcarmodel == 538 || liftedcarmodel == 569 || liftedcarmodel == 570) {
						plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, hdn);
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, helper);
						helper = NULL;
						CVehicle *auxil = CPools::GetVehicle(vehic);
						auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(0.0f, 0.0f, 0.5f), false);
						//Sleep(50);
						carinairtime = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.indx = 15;
						goto endin;
					}
				}
				plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);

				plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, helper, 1);
				plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, helper, 1);
				plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, helper, 1, 1, 1, 1, 1);
				CVehicle *auxil = CPools::GetVehicle(helper);
				auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(vel1, vel2, vel3), false);
				//Sleep(50);
			}
			else
			{
				CVehicle *auxil = CPools::GetVehicle(vehic);
				auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(0.0f, 0.0f, 0.5f), false);
				//Sleep(50);
			}
			carinairtime = CTimer::m_snTimeInMillisecondsNonClipped;
			boolvars.indx = 15;
			return true;
		}
		else
		{
			return true;
		}
	}
	else
	{
		if (boolvars.has_a_car == true)
		{
			plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "throw_car", "ironman", 10.0f, 0, 0, 0, 0, -1);
		}
		else {
			boolvars.indx = 15;
		}
		return true;
	}
}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}
	//15
carfly:
{
	if (*pActor > 0)
	{

		boolvars.has_a_car = false;
	if (CTimer::m_snTimeInMillisecondsNonClipped <= (carinairtime + 500))
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true) {
			plugin::scripting::CallCommandById(COMMAND_GET_CAR_MODEL, vehic, &liftedcarmodel);
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper) == true)
			{
				plugin::scripting::CallCommandById(COMMAND_GET_CAR_HEADING, vehic, &hdn);
				if (liftedcarmodel == 449) {
					plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, hdn);
					plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);

					plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, helper, 1);
					plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, helper, 1);
					plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, helper, 1, 1, 1, 1, 1);
					CVehicle *auxil = CPools::GetVehicle(helper);
					auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(0.0f, 0.0f, 0.5f), false);
					return true;
				}
				else {
					if (liftedcarmodel == 537 || liftedcarmodel == 538 || liftedcarmodel == 569 || liftedcarmodel == 570) {
						plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, hdn);
						plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);

						plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, helper, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, helper, 1);
						plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, helper, 1, 1, 1, 1, 1);
						CVehicle *auxil = CPools::GetVehicle(helper);
						auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(0.0f, 0.0f, 0.5f), false);
						return true;
					}
					else {
						plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, helper);
						helper = NULL;
						CVehicle *auxil = CPools::GetVehicle(vehic);
						auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(0.0f, 0.0f, 0.5f), false);
						return true;
					}
				}

			}
			else
			{
				CVehicle *auxil = CPools::GetVehicle(vehic);
				auxil->ApplyForce(CVector(dir1, dir2, dir3), CVector(0.0f, 0.0f, 0.5f), false);
				boolvars.indx = 16;
				goto abort_throw_car; //not sure
			}
		}
		else
		{
			boolvars.indx = 16;
			goto abort_throw_car;
		}
	}
	else
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper) == true) {
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true) {
				plugin::scripting::CallCommandById(COMMAND_GET_CAR_HEADING, vehic, &hdn);
				plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_CAR, vehic, helper, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, hdn);
				plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);

				plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, helper, 1);
				plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, helper, 1);
				plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, helper, 1, 1, 1, 1, 1);
				if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true)
				{
					plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
				}
				plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
				vehic1 = vehic;
				vehic = NULL;
			}
			plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, helper);
			helper12 = helper;
			helper = NULL;
		}
		else
		{
			helper = NULL;

			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic1) == true)
			{
				plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic1);
			}
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true)
			{
				plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
				vehic = NULL;
			}
			else
			{
				vehic = NULL;
			}
		}
		boolvars.indx = 16;
		goto abort_throw_car;
	}
}
	else {
		pActor = (DWORD*)0xB6F5F0;
		return true;
	}
}
	//16
abort_throw_car:
{
if (*pActor > 0)
	{

	boolvars.has_a_car = false;
		if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objeto1) == true)
		{
			//plugin::scripting::CallCommandById(COMMAND_DETACH_OBJECT, objeto1, 0.0f, 0.0f, 0.0f, 0);
			plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, objeto1);
			//plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, objeto1);
			//Sleep(20);
			objeto1 = NULL;
		}
		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true) {
			//plugin::scripting::CallCommandById(COMMAND_DETACH_CAR, vehic, 0.0f, 0.0f, 0.0f, 1);
			plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 1);
			plugin::scripting::CallCommandById(COMMAND_SET_VEHICLE_RECORDS_COLLISIONS, vehic, 0);
			plugin::scripting::CallCommandById(COMMAND_SET_CAR_HEAVY, vehic, 1);
			plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, vehic, 0, 0, 0, 0, 0);
			CVehicle *auxil = CPools::GetVehicle(vehic);
			//auxil->SetRwObjectAlpha(255);
			auxil->ApplyGravity();
			unholdtime = CTimer::m_snTimeInMillisecondsNonClipped;
			//plugin::patch::SetUChar(11988014, 1, 0);
			plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
			plugin::scripting::CallCommandById(COMMAND_SET_CAMERA_BEHIND_PLAYER);
			pad->bDisablePlayerEnterCar = 0;
			//pad->bDisablePlayerJump = 0;
			pad->bDisablePlayerFireWeaponWithL1 = 0;
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_NEVER_GETS_TIRED, 0, 0);
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
			boolvars.indx = 17;
			goto unload_car;
		}
		else {
			boolvars.indx = 0;
			pad->bDisablePlayerEnterCar = 0;
			//pad->bDisablePlayerJump = 0;
			pad->bDisablePlayerFireWeaponWithL1 = 0;
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_NEVER_GETS_TIRED, 0, 0);
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
			return true;
		}
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}
	//17
unload_car: 
{
if (*pActor > 0)
	{
	boolvars.has_a_car = false;
		if (CTimer::m_snTimeInMillisecondsNonClipped > (unholdtime + 500)) {
			if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, vehic) == true) {
				CVehicle *auxil = CPools::GetVehicle(vehic);

				//auxil->SetRwObjectAlpha(255);
				if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_STOPPED, vehic) && plugin::scripting::CallCommandById(COMMAND_IS_CAR_IN_AIR_PROPER, vehic)) {
					plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic);
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper) == true) {
						plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, helper);
						helper = NULL;
					}
				}
				else {
					plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
					if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, helper) == true) {
						plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, helper);
						helper = NULL;
					}
				}
				if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objeto1) == true) {
					plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, objeto1);
					objeto1 = NULL;
				}
				vehic = NULL;
				boolvars.indx = 0;
			}
			boolvars.indx = 0;
			return true;
		}
		else
		{
			//i keep on waiting
			goto endin;
		}
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}
	
cleartask:
{
	if (*pActor > 0)
	{
		if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "lift_car") != false) {
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
		}
		boolvars.indx = 0;
		return true;
	}
	else {
		pActor = (DWORD*)0xB6F5F0;
		return true;
	}
}

volaralto:
{
	if (*pActor > 0)
	{
		if (!boolvars.mousewheelhacktrigger) {
			jarvisvoice.SetVelocityUnlimited(true);
			mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
			plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
			boolvars.mousewheelhacktrigger = true;
		}
		markk = boolvars.mark;
		//markk = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
		if (jarvisvoice.Has_not_switched_enviroment(&propellers))
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Fast") == false)
			{
				if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Start") == false
					&& boolvars.landgetup == 0
					&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == false
					&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == false)
				{
					boolvars.flytime = CTimer::m_snTimeInMillisecondsNonClipped;
				}
				else
				{
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
					boolvars.thrusterskilled = true;
					JarvisVoice::storeenviroment(&propellers.enviroment_ID);
				}
				
			}
		}
		else
		{
			JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
				boolvars.thrusterskilled = true;
				JarvisVoice::storeenviroment(&propellers.enviroment_ID);
			

		}
		if (boolvars.flytime + 500 < CTimer::m_snTimeInMillisecondsNonClipped 
			|| boolvars.landgetup == 1
			|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == true
			|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == true)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Fast") == false)
			{
				JarvisVoice::PlayAudiostream(audiostream[flyfast], 0, &IMStream[0]);

			}

			if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
				JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, im_flyfast, &propellers);

			}
			float angx = jarvisvoice.GetXAngle();
			float angy = jarvisvoice.GetYAngle();
			float angz = jarvisvoice.newangleposforflight();
			if (jarvisvoice.has_obstacles(player, 0.0f, 3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
				boolvars.iscjfrozen = true;				
				plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
				plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Fast", "ironman", 10.0f, 0, 0, 0, 0, -1);
				plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, angz);
				//player->SetHeading(angz * 3.1415926535897932384 / 180.0f);
				plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, angx * 180.0f / 3.1415926535897932384f, angy * 180.0f / 3.1415926535897932384f, angz);
				//player->SetOrientation(angx, angy, angz * 3.1415926535897932384 / 180.0f);

				return true;
			}
			else
			{
				boolvars.iscjfrozen = false;
				plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
				plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Fast", "ironman", 10.0f, 0, 0, 0, 0, -1);
				plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, angz);
				//player->SetHeading(angz * 3.1415926535897932384 / 180.0f);
				plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, angx * 180.0f / 3.1415926535897932384f, angy * 180.0f / 3.1415926535897932384f, angz);
				//player->SetOrientation(angx, angy, angz * 3.1415926535897932384 / 180.0f);

				if (!boolvars.vueloconmouse)
				{
					boolvars.spdmouse = true;
					if (zDelta2 != 0) {
						zDelta1 = zDelta;
						if (zDelta2 > 0)
							sum = 1;
						else {
							if (spd == 200.0f)
							{
								sum = 0;
							}
							else
							{
								if (zDelta2 < 0 && spd > 199.9f)
									sum = -1;
								else {
									sum = 0;
									if (spd <= 200.0f)
									{
										spd = 200.0f;
									}
								}
							}
						}
						spd += sum;
					}
					zDelta2 = zDelta - zDelta1;
					jarvisvoice.setvelocityindirection(player, 0.0f, spd, 0.0f);
					//player->SetOrientation(jarvisvoice.GetXAngle(), cursorxxx, jarvisvoice.newanglepos());
					return true;
				}
				else
				{
					if (jarvisvoice.Va_adelante()) {
						boolvars.spdmouse = true;
						if (zDelta2 != 0) {
							zDelta1 = zDelta;
							if (zDelta2 > 0)
								sum = 1;
							else {
								if (spd == 200.0f)
								{
									sum = 0;
								}
								else
								{
									if (zDelta2 < 0 && spd > 199.9f)
										sum = -1;
									else {
										sum = 0;
										if (spd <= 200.0f)
										{
											spd = 200.0f;
										}
									}
								}
							}
							spd += sum;
						}
						zDelta2 = zDelta - zDelta1;
						jarvisvoice.setvelocityindirection(player, 0.0f, spd, 0.0f);
						//player->SetOrientation(jarvisvoice.GetXAngle(), cursorxxx, jarvisvoice.newanglepos());
						return true;
					}
					else {
						boolvars.spdmouse = false;
						jarvisvoice.setvelocityindirection(player, 0.0f, 100.0f, 0.0f);
						//player->SetOrientation(jarvisvoice.GetXAngle(), cursorxxx, jarvisvoice.newanglepos());
						return true;
					}
				}
			}
		}
		else
		{
			boolvars.iscjfrozen = true;
			plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
			plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Start", "ironman", 10.0f, 0, 0, 0, 0, -1);
			plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangleposforflight());
			plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newangleposforflight());
			//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newangleposforflight());
			return true;
		}
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}
//18
volarbajo:
{
if (*pActor > 0)
	{
	//si mantiene presionado vuelo
	//volar cerca del suelo hasta que suelte las teclas de volar
	//if (pad->GetSprint())
	//{
		if (boolvars.indx == 13)
		{
			boolvars.indx = 16; //abort throw car
			return true;
		}
		else
		{
			boolvars.indx = 18;
			if (!boolvars.mousewheelhacktrigger) {
				jarvisvoice.SetVelocityUnlimited(true);
				mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
				plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
				boolvars.mousewheelhacktrigger = true;
			}
			markk = boolvars.mark;
			//markk = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

			static float xangk;
			xangk = jarvisvoice.GetXAngleforopcodes();
			
			if (jarvisvoice.is_over_ground(2.0f) == false)
			{
				if (pad->GetSprint() == true)
				{

					if (xangk <= 1.0f)
					{

						if (jarvisvoice.Has_not_switched_enviroment(&propellers))
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_FastSurf") == false)
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);

							}

							if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL)==false) {
								JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, im_flyfast, &propellers);

							}
						}
						else
						{
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							

						}

						boolvars.iscjfrozen = false;
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_FastSurf", "ironman", 1.0f, 0, 1, 1, 1, -1);
						//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newanglepos());				
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangleposforflight());// jarvisvoice.newangle2());
						//player->SetHeading(jarvisvoice.newanglepos());
						if (!boolvars.vueloconmouse)
						{
							boolvars.spdmouse = true;
							if (zDelta2 != 0) {
								zDelta1 = zDelta;
								if (zDelta2 > 0)
									sum = 1;
								else {
									if (spd == 200.0f)
									{
										sum = 0;
									}
									else
									{
										if (zDelta2 < 0 && spd > 199.9f)
											sum = -1;
										else {
											sum = 0;
											if (spd <= 199.9f)
											{
												spd = 200.0f;
											}
										}
									}
								}
								spd += sum;
							}
							zDelta2 = zDelta - zDelta1;

							jarvisvoice.setvelocityindirection(player, 0.0f, spd, 0.0f);
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_SPEED, player, "fly_FastSurf", 2.0f);
							return true;
						}
						else
						{
							if (jarvisvoice.Va_adelante()) {
								boolvars.spdmouse = true;
								if (zDelta2 != 0) {
									zDelta1 = zDelta;
									if (zDelta2 > 0)
										sum = 1;
									else {
										if (spd == 200.0f)
										{
											sum = 0;
										}
										else
										{
											if (zDelta2 < 0 && spd > 199.9f)
												sum = -1;
											else {
												sum = 0;
												if (spd <= 199.9f)
												{
													spd = 200.0f;
												}
											}
										}
									}
									spd += sum;
								}
								zDelta2 = zDelta - zDelta1;

								jarvisvoice.setvelocityindirection(player, 0.0f, spd, 0.0f);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_SPEED, player, "fly_FastSurf", 2.0f);
								return true;
							}
							else {
								boolvars.spdmouse = false;
								jarvisvoice.setvelocityindirection(player, 0.0f, 100.0f, 0.0f);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_SPEED, player, "fly_FastSurf", 1.0f);
								return true;
							}
						}
					}
					else {
						if (xangk >= 6.0f)
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Fast") == false)
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);

							}
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Fast", "ironman", 10.0f, 0, 0, 0, 0, -1);
							//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangleposforflight());
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newangleposforflight());
							//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newangleposforflight());
							//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newangle2());
							jarvisvoice.setvelocityindirection(player, 0.0f, 100.0f, 0.0f);
							boolvars.indx = 19;
							boolvars.flytime = 0;
							goto volaralto;
						}
						else
						{

							if (boolvars.landgetup != 1 && !boolvars.has_a_car)
							{

								boolvars.indx = 20;
								goto make_a_clank;
								/*
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_land", "ironman", 10.0f, 0, 0, 0, 0, 1000);
								//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
								//player->SetHeading(jarvisvoice.newanglepos());
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());*/
							}
							if (!boolvars.has_a_car)
							{
								boolvars.indx = 20;
								goto make_a_clank;
							}
							else
							{
								boolvars.indx = 16;
								goto abort_throw_car;
							}
						}
					}

				}
				else
				{
					boolvars.spdmouse = false;
					if (boolvars.landgetup != 1 && !boolvars.has_a_car)
					{

						boolvars.indx = 20;
						goto make_a_clank;
						/*
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_land", "ironman", 10.0f, 0, 0, 0, 0, 1000);
						//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
						//player->SetHeading(jarvisvoice.newanglepos());
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());*/
					}
					if (!boolvars.has_a_car)
					{
						boolvars.indx = 20;
						goto make_a_clank;
					}
					else
					{
						boolvars.indx = 16;
						goto abort_throw_car;
					}
				}
			}
			else
			{
				boolvars.indx = 19;
				return true;
			}
		}
		return true;
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}

//24
wpicons:
{
	if (*pActor > 0)
	{
		cutenabled = plugin::patch::GetUChar(11989093, false);
		if (cutenabled != 1 && settings.radarmode != 2)
		{
			if (plugin::scripting::CallCommandById(COMMAND_HAS_CUTSCENE_LOADED) == true)
			{
				if (plugin::scripting::CallCommandById(COMMAND_HAS_CUTSCENE_FINISHED) == false)
				{
					boolvars.wpmenuisactive = false;
					//if (ischoosingwp == true) {
						//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
						//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
						//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
						//ischoosingwp = false;
					//}
					boolvars.indx = 0;
					return true;
				}
			}
		}
		else {
			boolvars.wpmenuisactive = false;
			//if (ischoosingwp == true) {
				//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
				//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
				//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
				//ischoosingwp = false;
			//}
			boolvars.indx = 0;
			return true;
		}
		if (cutenabled != 1 &&
			boolvars.wpmenuisactive == true &&
			boolvars.menuisactive == false &&
			plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_ARRESTED, player) == false &&
			plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, player) == false &&
			plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SWIMMING, player) == false &&
			boolvars.has_a_car == false &&
			boolvars.aims == false)
		{
			boolvars.indx = 24;
			static int iconsel;
			//if (!ischoosingwp)
			//{
				//plugin::scripting::CallCommandById(COMMAND_SET_TIME_SCALE, 0.0);
				//plugin::scripting::CallCommandById(COMMAND_SET_EVERYONE_IGNORE_PLAYER, 0, 1);
				//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 0);
				


				//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);




				//plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_COORDINATES, &posn.x, &posn.y, &posn.z);

				//CVector point = player->GetPosition();
				//plugin::scripting::CallCommandById(COMMAND_GET_ACTIVE_CAMERA_POINT_AT, &point.x, &point.y, &point.z);


				//plugin::scripting::CallCommandById(COMMAND_SET_CAMERA_BEHIND_PLAYER);
				//plugin::scripting::CallCommandById(COMMAND_CAMERA_PERSIST_POS, 1);

				//TheCamera.GetOrientation(camrot.x, camrot.y, camrot.z);
				//camposn = TheCamera.GetMatrix()->pos;
				//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 0);
				//plugin::scripting::CallCommandById(COMMAND_SET_FIXED_CAMERA_POSITION, camposn.x, camposn.y, camposn.z, camrot.x, camrot.y, camrot.z);
				//TheCamera.SetCamPositionForFixedMode(&camposn, &camrot);
				//TheCamera.UpdateAimingCoors(&point);
				//TheCamera.SetOrientation(camrot.x, camrot.y, camrot.z);
				//plugin::scripting::CallCommandById(COMMAND_POINT_CAMERA_AT_POINT, point.x, point.y, point.z, 2);
				//plugin::scripting::CallCommandById(COMMAND_CAMERA_PERSIST_FOV, 1);
				//TheCamera.m_bCameraPersistPosition = true;
				//TheCamera.m_bCameraPersistTrack = true;
				//iconsel = 0;
				//ischoosingwp = true;
			//}

			float movspeed = 0.05f;
			float movx, movy;
			plugin::scripting::CallCommandById(COMMAND_GET_PC_MOUSE_MOVEMENT, &movx, &movy);
			if (plugin::scripting::CallCommandById(COMMAND_IS_MOUSE_USING_VERTICAL_INVERSION) != NULL)
			{
				movy *= -1;
			}
			int movfactor = 0;

			movfactor = plugin::patch::GetInt(11987996, false) / 100000000;
			movx *= movfactor;
			movy *= movfactor;
			movx *= movspeed;
			movy *= movspeed;
			cursorx += movx;
			cursory += movy;
			if (cursorx < SCREEN_COORD(0.0f)) {
				cursorx = SCREEN_COORD(0.0f);
			}

			if (cursorx > SCREEN_WIDTH) {
				cursorx = SCREEN_WIDTH;
			}

			if (cursory < SCREEN_COORD(0.0f)) {
				cursory = SCREEN_COORD(0.0f);
			}

			if (cursory > SCREEN_HEIGHT) {
				cursory = SCREEN_HEIGHT;
			}
			//update mouse position end
			static bool hoverclose, hoverclose1, hoverclose2, hoverclose3, hoverclose4;

			CSprite2d::DrawRect(CRect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(11, 26, 59, 100));
			wpmenu->Draw(CRect(SCREEN_COORD_CENTER_LEFT(200.0f), SCREEN_COORD_CENTER_UP(200.0f), SCREEN_COORD_CENTER_RIGHT(200.0f), SCREEN_COORD_CENTER_DOWN(200.0f)), CRGBA(255, 255, 255, 255));
			static float ang;
			ang = -90.0f;
			CVector2D wpcoords;


			//for(float ang=90.0f; ang<=450.0f; ang += 72){
			plugin::scripting::CallCommandById(COMMAND_SIN, ang, &wpcoords.y);
			plugin::scripting::CallCommandById(COMMAND_COS, ang, &wpcoords.x);
			wpcoords.y *= 152.0f;
			wpcoords.x *= 152.0f;
			/*
			if(DrawClickIconAtCoords(true, wp1, hwp1, SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) + SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) + SCREEN_COORD(50.0f), cursorx, cursory)==true)
			{
				if (iconsel != 1)
				{
					iconsel = 1;
				}
			}*/
			ang += 72.0f;
			plugin::scripting::CallCommandById(COMMAND_SIN, ang, &wpcoords.y);
			plugin::scripting::CallCommandById(COMMAND_COS, ang, &wpcoords.x);
			wpcoords.y *= 152.0f;
			wpcoords.x *= 152.0f;

			//DrawClickIconAtCoords(true, wp2, hwp2, SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) + SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) + SCREEN_COORD(50.0f), cursorx, cursory, &hoverclose1);
			if (hoverclose1 == true) {
				if (iconsel != 2)
				{
					iconsel = 2;
				}
			}
			ang += 72.0f;
			plugin::scripting::CallCommandById(COMMAND_SIN, ang, &wpcoords.y);
			plugin::scripting::CallCommandById(COMMAND_COS, ang, &wpcoords.x);
			wpcoords.y *= 152.0f;
			wpcoords.x *= 152.0f;

			//DrawClickIconAtCoords(true, wp3, hwp3, SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) + SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) + SCREEN_COORD(50.0f), cursorx, cursory, &hoverclose2);
			if (hoverclose2 == true) {
				if (iconsel != 3)
				{
					iconsel = 3;
				}
			}
			ang += 72.0f;
			plugin::scripting::CallCommandById(COMMAND_SIN, ang, &wpcoords.y);
			plugin::scripting::CallCommandById(COMMAND_COS, ang, &wpcoords.x);
			wpcoords.y *= 152.0f;
			wpcoords.x *= 152.0f;

			//DrawClickIconAtCoords(true, wp4, hwp4, SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) + SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) + SCREEN_COORD(50.0f), cursorx, cursory, &hoverclose3);
			if (hoverclose3 == true) {
				if (iconsel != 4)
				{
					iconsel = 4;
				}
			}
			ang += 72.0f;
			plugin::scripting::CallCommandById(COMMAND_SIN, ang, &wpcoords.y);
			plugin::scripting::CallCommandById(COMMAND_COS, ang, &wpcoords.x);
			wpcoords.y *= 152.0f;
			wpcoords.x *= 152.0f;

			//DrawClickIconAtCoords(true, wp5, hwp5, SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) - SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_X + SCREEN_COORD(wpcoords.x) + SCREEN_COORD(50.0f), SCREEN_COORD_CENTER_Y + SCREEN_COORD(wpcoords.y) + SCREEN_COORD(50.0f), cursorx, cursory, &hoverclose4);
			if (hoverclose4 == true) {
				if (iconsel != 5)
				{
					iconsel = 5;
				}
			}
			cursor->Draw(CRect(cursorx, cursory, cursorx + SCREEN_COORD(20.0f), cursory + SCREEN_COORD(20.0f)), CRGBA(255, 255, 255, 255));

			if (iconsel <= 0 || iconsel > 5)
			{
				iconsel = 0;
			}

			if (jarvisvoice.is_over_ground(2.0f))
			{
				if (!boolvars.mousewheelhacktrigger) {
					jarvisvoice.SetVelocityUnlimited(true);
					mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
					plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
					plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
					boolvars.mousewheelhacktrigger = true;
				}
				//CVector playpos = player->GetPosition();
				CVector waterpos, playerpos;
				int waterfx;
				float waterh;

				//plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, playpos.x, playpos.y, playpos.z, &waterh);
				
				float waterhei = waterh;
				waterh += 15.0f;
				playerpos = player->GetPosition();
				plugin::scripting::CallCommandById(COMMAND_GET_WATER_HEIGHT_AT_COORDS, playerpos.x, playerpos.y, 0, &waterh);

				if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, playerpos.x, playerpos.y, playerpos.z, playerpos.x, playerpos.y, waterhei, 1, 1, 0, 1, 0) == true) {
					if (waterh >= playerpos.z && playerpos.z >= 0.0f && is_on_foot() == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SWIMMING, player) == false)
					{
						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &waterpos.x, &waterpos.y, &waterpos.z);
						waterh += -17.5;
						plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM, "WATER_SB", waterpos.x, waterpos.y, waterh, 1, &waterfx);
						//plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, waterfx);
						plugin::scripting::CallCommandById(COMMAND_PLAY_AND_KILL_FX_SYSTEM, waterfx);
					}
				}

				spd = 200.0F;
				if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
					JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					boolvars.thrusterskilled = true;

				}


				//AimedPedReactiontwo(&boolvars.ped);
				if (boolvars.iscjfrozen == false)
				{
					boolvars.iscjfrozen = true;

					plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
				}
				//if(plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Hover") == false)
				//{
				plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Hover", "ironman", 10.0f, 0, 0, 0, 0, -1);
				//}
				//player->SetHeading(jarvisvoice.newanglepos());
				if (jarvisvoice.Has_not_switched_enviroment(&propellers))
				{
					if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == false) {
						JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 0, flotar, &propellers);


					}
				}
				else
				{
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;

						JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					

				}
			}


			if (CMouseControllerState().lmb != 1 && (GetKeyState(VK_LBUTTON) & 0x8000) == false)
			{
				return true;
			}
			else
			{
				if (iconsel > 0 && iconsel < 6)
				{
					char *numb;
					numb = new char[4];
					sprintf(numb, "%d", iconsel);
					WritePrivateProfileString("CONFIG", "WEAPON", numb, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
					delete[] numb;
				}
				//escribir en ini
				boolvars.indx = 0;
				boolvars.wpmenuisactive = false;
				//if (ischoosingwp == true) {
					//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
					//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
					//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
					//ischoosingwp = false;
				//}
				return true;
			}
		}
		else
		{
			boolvars.indx = 0;
			boolvars.wpmenuisactive = false;
			boolvars.landgetup = 0;
			//if (ischoosingwp == true) {
				//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
				//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
				//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
				//ischoosingwp = false;
			//}
			return true;
		}
	}
	else
	{
		boolvars.indx = 0;
		boolvars.wpmenuisactive = false;
		//if (ischoosingwp == true) {
			//plugin::patch::SetUChar(0x6FF410, 0x8B, 1);
			//plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
			//plugin::scripting::CallCommandById(COMMAND_RESTORE_CAMERA_JUMPCUT);
			//ischoosingwp = false;
		//}
		return true;
	}
}
//19

controlesdevuelo:
{
if (debevolar == false && boolvars.escudoactivo == true)
{
	boolvars.indx = 22;
	goto aimin;
}
if (CTimer::m_snTimeInMillisecondsNonClipped > boolvars.waiter + boolvars.timetowait)
{
	/*
	if (boolvars.indx == 24 || boolvars.wpmenuisactive == true)
	{
		boolvars.indx = 24;
		boolvars.wpmenuisactive = true;
		return true;
	}
	else
	{*/
		if (*pActor > 0)
		{
			static int timefalling;
			if (boolvars.landedpara == true)
			{
				if (boolvars.fadetime + 1000 < CTimer::m_snTimeInMillisecondsNonClipped)
				{
					boolvars.landedpara = false;
					boolvars.flytime = CTimer::m_snTimeInMillisecondsNonClipped;
					plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "skydive_FallStop", "ironman", 2.5, 0, 0, 0, 0, -1);
					//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newanglepos());
					if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;

						JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					}
					JarvisVoice::PlayAudiostream(audiostream[23], 0, &IMStream[2]);
					return true;
				}
				else
				{
					if (jarvisvoice.is_over_ground(2.0f))
					{
						boolvars.flytime = CTimer::m_snTimeInMillisecondsNonClipped;
						boolvars.iscjfrozen = false;
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "FALL_skyDive", "parachute", 10.0f, 0, 0, 0, 0, -1);
						if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							boolvars.thrusterskilled = true;

							JarvisVoice::storeenviroment(&propellers.enviroment_ID);
						}
						return true;
					}
					else
					{
						if (!boolvars.has_a_car)
						{
							boolvars.indx = 20;
							boolvars.landedpara = false;
							goto make_a_clank;
						}
						else
						{
							boolvars.indx = 16;
							boolvars.landedpara = false;
							goto abort_throw_car;
						}
					}
				}
			}
			if (boolvars.landgetup == 0 &&
				(plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Stop") == 1 ||
					plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Start") == 1 ||
					plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_hover_start") == 1))
			{
				if (CTimer::m_snTimeInMillisecondsNonClipped < boolvars.flytime + 200)
				{
					if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;

						JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					}
					return true;
				}
			}
			static int dust;
			static bool duststart;
			if (!duststart)
			{
				dust = 0;
				duststart = true;
			}
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "skydive_FallStop") == true)
			{
				if (jarvisvoice.is_over_ground(2.0f))
				{
					if (CTimer::m_snTimeInMillisecondsNonClipped < boolvars.flytime + 1200)
					{
						if (CTimer::m_snTimeInMillisecondsNonClipped > boolvars.flytime + 400)
						{
							boolvars.iscjfrozen = true;
							//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
								//JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, stopcaida, &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);

								if (dust == 0)
								{
									CVector posfordust = player->GetPosition();
									plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, posfordust.x, posfordust.y, posfordust.z, &posfordust.z);
									plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM, "heli_dust", posfordust.x, posfordust.y, posfordust.z, 1, &dust);
									plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, dust);
								}
							}
						}
						return true;
					}
				}
				else
				{
					if (!boolvars.has_a_car)
					{
						boolvars.indx = 20;
						goto make_a_clank;
					}
					else
					{
						boolvars.indx = 16;
						goto abort_throw_car;
					}
				}
			}
			else
			{
				if (dust != 0) {
					plugin::scripting::CallCommandById(COMMAND_KILL_FX_SYSTEM, dust);
					dust = 0;
				}
			}
			/*
			if ((GetKeyState(VK_MBUTTON) & 0x8000) && boolvars.has_a_car == false && boolvars.wpmenuisactive == false)
			{
			boolvars.wpmenuisactive = true;
			boolvars.indx = 24;
			return true;
			}*/
			if (boolvars.has_a_car == false)
			{
				boolvars.indx = 19;
			}
			if (jarvisvoice.is_over_ground(2.0f))
			{
				if (!boolvars.mousewheelhacktrigger) {
					jarvisvoice.SetVelocityUnlimited(true);
					mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
					plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
					plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
					boolvars.mousewheelhacktrigger = true;
				}
				CVector playpos = player->GetPosition();
				CVector waterpos, playerpos;
				int waterfx;
				float waterh;
				static int groundfx;
				static float groundat;
				plugin::scripting::CallCommandById(COMMAND_GET_WATER_HEIGHT_AT_COORDS, playpos.x, playpos.y, 0, &waterh);
				plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, playpos.x, playpos.y, playpos.z, &groundat);

				float waterhei = waterh;

				waterh += 15.0f;
				playerpos = player->GetPosition();

				if (is_on_foot() == false && boolvars.altitude >= 0.0f && boolvars.altitude < 10.0f)
				{
					if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, playerpos.x, playerpos.y, playerpos.z, playerpos.x, playerpos.y, waterhei, 1, 1, 0, 1, 0) == true)
					{
						if (waterh >= playpos.z && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SWIMMING, player) == false)
						{
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &waterpos.x, &waterpos.y, &waterpos.z);
							waterh += -17.5;
							plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM, "im_watersprk", waterpos.x, waterpos.y, waterh, 1, &waterfx);
							//plugin::scripting::CallCommandById(COMMAND_PLAY_FX_SYSTEM, waterfx);
							plugin::scripting::CallCommandById(COMMAND_PLAY_AND_KILL_FX_SYSTEM, waterfx);
						}
						else
						{
							plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM, "im_dust", playpos.x, playpos.y, groundat, 1, &groundfx);
							plugin::scripting::CallCommandById(COMMAND_PLAY_AND_KILL_FX_SYSTEM, groundfx);
						}
					}
					else
					{
						CVector posfordust = player->GetPosition();
						plugin::scripting::CallCommandById(COMMAND_GET_GROUND_Z_FOR_3D_COORD, posfordust.x, posfordust.y, posfordust.z, &posfordust.z);
						plugin::scripting::CallCommandById(COMMAND_CREATE_FX_SYSTEM, "im_dust", posfordust.x, posfordust.y, posfordust.z, 1, &groundfx);
						plugin::scripting::CallCommandById(COMMAND_PLAY_AND_KILL_FX_SYSTEM, groundfx);
					}
				}

				if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_glide") == true || 
					plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_launch") == true ||
					plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_launch_R") == true)
				{
					plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
					plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_hover_start", "ironman", 10.0f, 0, 0, 0, 0, 1000);

					JarvisVoice::PlayAudiostream(audiostream[takeoff], 0, &IMStream[2]);
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
					JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					boolvars.thrusterskilled = true;
				}
			}

			AimedPedReactiontwo(&boolvars.ped);
			if (!pad->GetJump())
			{
				if (jarvisvoice.nosemueve() == true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Fast") == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Stop") == false)
						{
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							boolvars.thrusterskilled = true;
						}
						boolvars.flytime = CTimer::m_snTimeInMillisecondsNonClipped;
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
						boolvars.iscjfrozen = true;
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Stop", "ironman", 10.0f, 0, 0, 0, 0, -1);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangleposforflight());
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newangleposforflight());
						//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newangleposforflight());
						JarvisVoice::PlayAudiostream(audiostream[flystop], 0, &IMStream[2]);
						return true;
					}
					//NO SE MUEVE TRUE
					/*spd = 200.0F;
					if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL)==true) {
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
					boolvars.thrusterskilled = true;

					}
					JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					*/

					if (jarvisvoice.is_aiming() == false) {
						//AimedPedReactiontwo(&boolvars.ped);

						if (jarvisvoice.is_over_ground(2.0f) == true)
						{
							if (!boolvars.mousewheelhacktrigger) {
								jarvisvoice.SetVelocityUnlimited(true);
								mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
								boolvars.mousewheelhacktrigger = true;
							}
							/*if (boolvars.has_a_car == false)
							{
							if (boolvars.action == 1 || boolvars.action == 2 || boolvars.action == 3 || boolvars.action == 4 || boolvars.action == 5)
							{
							boolvars.action = 0;
							}
							}*/
							/*
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carHover") == false &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Hover") == false)
							{
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							boolvars.thrusterskilled = true;
							//JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.yndex].suits[markk], 0, flotar, &propellers);

							}*/
							boolvars.iscjfrozen = true;

							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							if (boolvars.has_a_car == true)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carHover", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							else {
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Hover", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							//player->SetHeading(jarvisvoice.newanglepos());
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							if (jarvisvoice.Has_not_switched_enviroment(&propellers))
							{
								if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
									JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
									boolvars.thrusterskilled = true;

									JarvisVoice::storeenviroment(&propellers.enviroment_ID);
								}

								if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == false) {
									JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 0, flotar, &propellers);


								}
							}
							else
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);


							}
						}
						return true;
					}
					else
					{
						if (boolvars.has_a_car == false)
						{
							boolvars.indx = 22;
							return true;
						}
						else {
							return true;
						}
					}

				}
				else
				{
					if ((isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true) || (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true)) {
						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;

						JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					}
					if (pad->GetSprint())
					{
						boolvars.flytime = 0;
						if (boolvars.indx == 13)
						{
							boolvars.indx = 16; //abort throw car
							return true;
						}
						else
						{
							if (jarvisvoice.is_over_ground(2.0f)) {

								goto volaralto;
							}
							else
							{
								boolvars.indx = 18;
								goto volarbajo;
							}
						}
					}
					else
					{
						boolvars.spdmouse = false;
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Fast") == true)
						{
							boolvars.flytime = CTimer::m_snTimeInMillisecondsNonClipped;
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							boolvars.iscjfrozen = true;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_Stop", "ironman", 10.0f, 0, 0, 0, 0, -1);
							player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
							if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							}
							JarvisVoice::PlayAudiostream(audiostream[flystop], 0, &IMStream[2]);
							return true;
						}
						if (pad->GetDuck())
						{
							if (jarvisvoice.Has_not_switched_enviroment(&propellers))
							{

								if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
									JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, abajo, &propellers);

								}
							}
							else
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
								boolvars.thrusterskilled = true;




							}
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveDown") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carDown") == false) {
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
							}

							if (jarvisvoice.has_obstacles(player, 0.0f, 0.0f, -3.0f, 1, 1, 0, 1, 0) == false) {
								if (!boolvars.mousewheelhacktrigger) {
									jarvisvoice.SetVelocityUnlimited(true);
									mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
									plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
									boolvars.mousewheelhacktrigger = true;
								}

								if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carDown") == false &&
									plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveDown") == false)
								{
									JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
									JarvisVoice::storeenviroment(&propellers.enviroment_ID);
									boolvars.thrusterskilled = true;
								}
								boolvars.iscjfrozen = false;
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
								if (boolvars.has_a_car == true)
								{
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carDown", "ironman", 10.0f, 0, 0, 0, 0, -1);
								}
								else {
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveDown", "ironman", 10.0f, 0, 0, 0, 0, -1);
								}
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
								//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
								//player->SetHeading(jarvisvoice.newanglepos());
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
								jarvisvoice.setvelocityindirection(player, 0.0f, 0.0f, -30.0);
								return true;
							}
							else
							{
								boolvars.indx = 20;
								goto make_a_clank;
							}
						}
						else {
							if (jarvisvoice.is_over_ground(2.0f) == true)
							{
								if (!boolvars.mousewheelhacktrigger) {
									jarvisvoice.SetVelocityUnlimited(true);
									mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
									plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
									boolvars.mousewheelhacktrigger = true;
								}

								int x, y, specialx, specialy;
								plugin::scripting::CallCommandById(COMMAND_GET_POSITION_OF_ANALOGUE_STICKS, 0, &x, &y, &specialx, &specialy);
								if (y < 0)
								{
									if (x > 0) {
										if (jarvisvoice.Has_not_switched_enviroment(&propellers))
										{

											if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
												JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, adelante, &propellers);



											}
										}
										else
										{
											JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
											boolvars.thrusterskilled = true;

											JarvisVoice::storeenviroment(&propellers.enviroment_ID);



										}

										if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveFWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carFWD") == false) {
											plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
											JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
											JarvisVoice::storeenviroment(&propellers.enviroment_ID);
											boolvars.thrusterskilled = true;
										}
										if (jarvisvoice.has_obstacles(player, 3.0f, 3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
											boolvars.iscjfrozen = true;
											plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
											if (boolvars.has_a_car == true)
											{
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
											}
											else {
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
											}
											player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
											return true;
										}
										else {
											boolvars.iscjfrozen = false;
											plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
											if (boolvars.has_a_car == true)
											{
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
											}
											else {
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
											}
											player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
											jarvisvoice.setvelocityindirection(player, 0.0f, 30.0f, 0.0);
											return true;
										}
									}
									else
									{
										if (x < 0)
										{
											if (jarvisvoice.Has_not_switched_enviroment(&propellers))
											{

												if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
													JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, adelante, &propellers);



												}
											}
											else
											{
												JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
												boolvars.thrusterskilled = true;
												JarvisVoice::storeenviroment(&propellers.enviroment_ID);



											}
											if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveFWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carFWD") == false) {
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
												JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
												JarvisVoice::storeenviroment(&propellers.enviroment_ID);
												boolvars.thrusterskilled = true;
											}
											if (jarvisvoice.has_obstacles(player, -3.0f, 3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
												boolvars.iscjfrozen = true;
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
												if (boolvars.has_a_car == true)
												{
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												else {
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
												return true;
											}
											else
											{
												boolvars.iscjfrozen = false;
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
												if (boolvars.has_a_car == true)
												{
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												else {
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
												jarvisvoice.setvelocityindirection(player, 0.0f, 30.0f, 0.0);
												return true;
											}
										}
										else
										{
											if (jarvisvoice.has_obstacles(player, 0.0f, 3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
												boolvars.iscjfrozen = true;
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
												if (pad->GetSprint()) {
													if (boolvars.indx == 13)
													{
														boolvars.indx = 16; //abort throw car
														return true;
													}
													else
													{
														goto volaralto;
													}
												}
												else
												{
													boolvars.spdmouse = false;
													if (jarvisvoice.Has_not_switched_enviroment(&propellers))
													{

														if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
															JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, adelante, &propellers);



														}
													}
													else
													{
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														boolvars.thrusterskilled = true;
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);



													}
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveFWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carFWD") == false) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);
														boolvars.thrusterskilled = true;
													}

													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													//player->SetHeading(jarvisvoice.newanglepos());
													//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													return true;
												}

											}
											else
											{
												if (pad->GetSprint()) {
													if (boolvars.indx == 13)
													{
														boolvars.indx = 16; //abort throw car
														return true;
													}
													else
													{
														if (jarvisvoice.is_over_ground(2.0f)) {
															goto volaralto;
														}
														else
														{
															boolvars.indx = 18;
															goto volarbajo;
														}
													}
												}
												else {
													boolvars.spdmouse = false;
													if (jarvisvoice.Has_not_switched_enviroment(&propellers))
													{

														if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
															JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, adelante, &propellers);


														}
													}
													else
													{
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														boolvars.thrusterskilled = true;
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);



													}
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveFWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carFWD") == false) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);
														boolvars.thrusterskilled = true;
													}
													boolvars.iscjfrozen = false;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													//player->SetHeading(jarvisvoice.newanglepos());
													//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

													jarvisvoice.setvelocityindirection(player, 0.0f, 30.0f, 0.0);
													return true;
												}
											}
										}
									}

								}
								else {
									if (y > 0)
									{
										if (x > 0) {
											if (jarvisvoice.Has_not_switched_enviroment(&propellers))
											{

												if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
													JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, atras, &propellers);

												}
											}
											else
											{
												JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
												boolvars.thrusterskilled = true;
												JarvisVoice::storeenviroment(&propellers.enviroment_ID);



											}
											if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveBWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carBWD") == false) {
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

												JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
												JarvisVoice::storeenviroment(&propellers.enviroment_ID);
												boolvars.thrusterskilled = true;
											}
											if (jarvisvoice.has_obstacles(player, 3.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
												boolvars.iscjfrozen = true;
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
												if (boolvars.has_a_car == true)
												{
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												else {
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
												return true;
											}
											else {
												boolvars.iscjfrozen = false;
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
												if (boolvars.has_a_car == true)
												{
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												else {
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
												}
												player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
												jarvisvoice.setvelocityindirection(player, 0.0f, -30.0f, 0.0);
												return true;
											}
										}
										else
										{
											if (x < 0)
											{
												if (jarvisvoice.Has_not_switched_enviroment(&propellers))
												{
													if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
														JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, atras, &propellers);

													}
												}
												else
												{
													JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
													boolvars.thrusterskilled = true;
													JarvisVoice::storeenviroment(&propellers.enviroment_ID);



												}
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveBWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carBWD") == false) {
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

													JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
													JarvisVoice::storeenviroment(&propellers.enviroment_ID);
													boolvars.thrusterskilled = true;
												}
												if (jarvisvoice.has_obstacles(player, -3.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
													boolvars.iscjfrozen = true;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
													return true;
												}
												else
												{
													boolvars.iscjfrozen = false;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
													jarvisvoice.setvelocityindirection(player, 0.0f, -30.0f, 0.0);
													return true;
												}
											}
											else
											{
												if (jarvisvoice.Has_not_switched_enviroment(&propellers))
												{
													if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
														JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, atras, &propellers);

													}
												}
												else
												{
													JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
													boolvars.thrusterskilled = true;
													JarvisVoice::storeenviroment(&propellers.enviroment_ID);



												}
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveBWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carBWD") == false) {
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
													JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
													JarvisVoice::storeenviroment(&propellers.enviroment_ID);
													boolvars.thrusterskilled = true;

												}
												if (jarvisvoice.has_obstacles(player, 0.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
													boolvars.iscjfrozen = true;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													//player->SetHeading(jarvisvoice.newanglepos());
													//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

													return true;
												}
												else
												{
													boolvars.iscjfrozen = false;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													//player->SetHeading(jarvisvoice.newanglepos());
													//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
													plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

													jarvisvoice.setvelocityindirection(player, 0.0f, -30.0f, 0.0);
													return true;
												}
											}
										}
									}
									else
									{
										if (x > 0)
										{
											if (y > 0) {
												if (jarvisvoice.Has_not_switched_enviroment(&propellers))
												{
													if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
														JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, atras, &propellers);

													}
												}
												else
												{
													JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
													boolvars.thrusterskilled = true;
													JarvisVoice::storeenviroment(&propellers.enviroment_ID);



												}
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveBWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carBWD") == false) {
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

													JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
													JarvisVoice::storeenviroment(&propellers.enviroment_ID);
													boolvars.thrusterskilled = true;

												}
												if (jarvisvoice.has_obstacles(player, -3.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
													boolvars.iscjfrozen = true;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
													return true;
												}
												else {
													boolvars.iscjfrozen = false;
													plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
													if (boolvars.has_a_car == true)
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
													}
													player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
													jarvisvoice.setvelocityindirection(player, 0.0f, -30.0f, 0.0);
													return true;
												}
											}
											else
											{
												if (y < 0) {
													if (jarvisvoice.Has_not_switched_enviroment(&propellers))
													{
														if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
															JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, adelante, &propellers);

														}
													}
													else
													{
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														boolvars.thrusterskilled = true;
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);



													}
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveFWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carFWD") == false) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);
														boolvars.thrusterskilled = true;

													}
													if (jarvisvoice.has_obstacles(player, 3.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
														boolvars.iscjfrozen = true;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
														if (boolvars.has_a_car == true)
														{
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														else {
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
														return true;
													}
													else {
														boolvars.iscjfrozen = false;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
														if (boolvars.has_a_car == true)
														{
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														else {
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
														jarvisvoice.setvelocityindirection(player, 0.0f, 30.0f, 0.0);
														return true;
													}
												}
												else
												{
													if (jarvisvoice.Has_not_switched_enviroment(&propellers))
													{
														if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
															JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, derecha, &propellers);

														}
													}
													else
													{
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														boolvars.thrusterskilled = true;
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);


													}
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveR") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carR") == false) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);
														boolvars.thrusterskilled = true;
													}
													if (jarvisvoice.has_obstacles(player, 3.0f, 0.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
														boolvars.iscjfrozen = true;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
														if (boolvars.has_a_car == true)
														{
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carR", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														else {
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveR", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														//player->SetHeading(jarvisvoice.newanglepos());
														//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
														//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
														plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
														plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

														return true;
													}
													else
													{
														boolvars.iscjfrozen = false;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
														if (boolvars.has_a_car == true)
														{
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carR", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														else {
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveR", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														//player->SetHeading(jarvisvoice.newanglepos());
														//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
														//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
														plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
														plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

														jarvisvoice.setvelocityindirection(player, 30.0f, 0.0f, 0.0);
														return true;
													}
												}
											}
										}
										else
										{
											if (x < 0)
											{
												if (y > 0) {
													if (jarvisvoice.Has_not_switched_enviroment(&propellers))
													{
														if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
															JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, atras, &propellers);

														}
													}
													else
													{
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														boolvars.thrusterskilled = true;
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);



													}

													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveBWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carBWD") == false) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
														JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
														JarvisVoice::storeenviroment(&propellers.enviroment_ID);
														boolvars.thrusterskilled = true;
													}
													if (jarvisvoice.has_obstacles(player, -3.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
														boolvars.iscjfrozen = true;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
														if (boolvars.has_a_car == true)
														{
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														else {
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
														return true;
													}
													else {
														boolvars.iscjfrozen = false;
														plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
														if (boolvars.has_a_car == true)
														{
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														else {
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveBWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
														}
														player->SetHeading(jarvisvoice.newanglepos() - 45.0f);
														jarvisvoice.setvelocityindirection(player, 0.0f, -30.0f, 0.0);
														return true;
													}
												}
												else
												{
													if (y < 0) {
														if (jarvisvoice.Has_not_switched_enviroment(&propellers))
														{
															if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
																JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, adelante, &propellers);

															}
														}
														else
														{
															JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
															boolvars.thrusterskilled = true;
															JarvisVoice::storeenviroment(&propellers.enviroment_ID);



														}

														if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carFWD") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveFWD") == false) {
															plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
															JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
															JarvisVoice::storeenviroment(&propellers.enviroment_ID);
															boolvars.thrusterskilled = true;
														}
														if (jarvisvoice.has_obstacles(player, 3.0f, -3.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
															boolvars.iscjfrozen = true;
															plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
															if (boolvars.has_a_car == true)
															{
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															else {
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
															return true;
														}
														else {
															boolvars.iscjfrozen = false;
															plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
															if (boolvars.has_a_car == true)
															{
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															else {
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveFWD", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															player->SetHeading(jarvisvoice.newanglepos() + 45.0f);
															jarvisvoice.setvelocityindirection(player, 0.0f, 30.0f, 0.0);
															return true;
														}
													}
													else
													{
														if (jarvisvoice.Has_not_switched_enviroment(&propellers))
														{
															if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
																JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, izquierda, &propellers);

															}
														}
														else
														{
															JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
															boolvars.thrusterskilled = true;
															JarvisVoice::storeenviroment(&propellers.enviroment_ID);



														}

														if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carL") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveL") == false) {
															plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
															JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
															JarvisVoice::storeenviroment(&propellers.enviroment_ID);
															boolvars.thrusterskilled = true;
														}
														if (jarvisvoice.has_obstacles(player, -3.0f, 0.0f, 0.0f, 1, 1, 0, 1, 0) == true) {
															boolvars.iscjfrozen = true;
															plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
															if (boolvars.has_a_car == true)
															{
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carL", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															else {
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveL", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															//player->SetHeading(jarvisvoice.newanglepos());
															//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
															//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
															plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
															plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

															return true;
														}
														else
														{
															boolvars.iscjfrozen = false;
															plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
															if (boolvars.has_a_car == true)
															{
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carL", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															else {
																plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveL", "ironman", 10.0f, 0, 0, 0, 0, -1);
															}
															//player->SetHeading(jarvisvoice.newanglepos());
															//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
															//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
															plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
															plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

															jarvisvoice.setvelocityindirection(player, -30.0f, 0.0f, 0.0);
															return true;
														}
													}
												}
											}
										}
									}
								}

								return true;
							}
							else
							{
								if (pad->GetSprint())
								{
									if (boolvars.indx == 13)
									{
										boolvars.indx = 16; //abort throw car
										return true;
									}
									else
									{
										if (!boolvars.mousewheelhacktrigger) {
											jarvisvoice.SetVelocityUnlimited(true);
											mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
											plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
											plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
											boolvars.mousewheelhacktrigger = true;
										}
										if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Fast") == false) {

											boolvars.indx = 18;
											goto volarbajo;
										}
										else
										{
											boolvars.indx = 0;
											return true;
										}
									}
								}
								else {
									boolvars.spdmouse = false;
									/*
									if (boolvars.landgetup != 1)
									{
									if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_land") == false) {
									plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
									JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
									JarvisVoice::storeenviroment(&propellers.enviroment_ID);
									boolvars.thrusterskilled = true;
									}
									plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_land", "ironman", 10.0f, 0, 1, 1, 0, -1);
									//player->SetHeading(jarvisvoice.newanglepos());
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
									//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());
									}*/
									boolvars.indx = 20;
									goto make_a_clank;
								}
							}
						}
					}
				}
			}
			else
			{
				if (jarvisvoice.is_aiming() == true)
				{
					if (boolvars.has_a_car == false)
					{
						boolvars.indx = 22;
						return true;
					}
					else {
						return true;
					}
				}
				else
				{
					if (jarvisvoice.Has_not_switched_enviroment(&propellers))
					{

						if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == false) {
							JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, arriba, &propellers);



						}
					}
					else
					{
						JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
						boolvars.thrusterskilled = true;

						JarvisVoice::storeenviroment(&propellers.enviroment_ID);



					}
					if (jarvisvoice.is_over_ground(2.0f) == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveUp") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carUp") == false) {
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							JarvisVoice::storeenviroment(&propellers.enviroment_ID);
							boolvars.thrusterskilled = true;
						}
						if (jarvisvoice.has_obstacles(player, 0.0f, 0.0f, 3.0f, 1, 1, 0, 1, 0) == true) {

							boolvars.iscjfrozen = true;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							if (boolvars.has_a_car == true)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carUp", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							else {
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveUp", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							//player->SetHeading(jarvisvoice.newanglepos());
							//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

							return true;
						}
						else
						{
							boolvars.iscjfrozen = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
							if (boolvars.has_a_car == true)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carUp", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							else {
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveUp", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							//player->SetHeading(jarvisvoice.newanglepos());
							//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, 0.0f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);

							jarvisvoice.setvelocityindirection(player, 0.0f, 0.0f, 30.0);
							return true;
						}
					}
					else
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_glide") == true ||
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_launch") == true ||
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "JUMP_launch_R") == true)
						{
							float altura = 0.0f;
							//super saltos
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEIGHT_ABOVE_GROUND, player, &altura);
							if (altura <= 1.0f)
							{
								boolvars.indx = 0;
							}
							else
							{
								float angleplayer = player->GetHeading();
								CVector vel = { sinf(angleplayer) * -10.0f, cosf(angleplayer) * 10.0f, 20.0f };
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, player, vel.x, vel.y, vel.z);
							}
						}
						else
						{
							boolvars.indx = 0;
						}
						return true;
					}
				}
			}
		}

	//}
}
else
{
	if (is_on_foot() == true)
	{
		if (boolvars.has_a_car == false)
		{
			boolvars.indx = 20;
		}
		else
		{
			boolvars.indx = 16;
			goto abort_throw_car;
		}
		boolvars.landgetup = 1;
		if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != true
			&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != true)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == true)
			{
				JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
				JarvisVoice::storeenviroment(&propellers.enviroment_ID);
				boolvars.thrusterskilled = true;
				if (boolvars.mousewheelhacktrigger == true) {
					spd = 200.0f;
					UnhookWindowsHookEx(mousehook);
					jarvisvoice.SetVelocityUnlimited(false);
					plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
					boolvars.mousewheelhacktrigger = false;
				}
				//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
				plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getupfront", "ironman", 4.0f, 0, 1, 1, 0, -1);
				//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
				//player->SetHeading(jarvisvoice.newanglepos());
				//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
			}
			else
			{
				if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == true)
				{
					JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
					JarvisVoice::storeenviroment(&propellers.enviroment_ID);
					boolvars.thrusterskilled = true;
					if (boolvars.mousewheelhacktrigger == true) {
						spd = 200.0f;
						UnhookWindowsHookEx(mousehook);
						jarvisvoice.SetVelocityUnlimited(false);
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
						boolvars.mousewheelhacktrigger = false;
					}
					//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
					plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getup", "ironman", 4.0f, 0, 1, 1, 0, -1);
					//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
					//player->SetHeading(jarvisvoice.newanglepos());
					//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
				}
			}
		}
		else
		{
			boolvars.landgetup = 1;
		}
		goto make_a_clank;
	}
}

return true;
}
//20
make_a_clank:
{
static int timeclankin;
if (*pActor > 0)
	{
		JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
		JarvisVoice::storeenviroment(&propellers.enviroment_ID);
		boolvars.thrusterskilled = true;

	if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SWIMMING, player) == false)// && boolvars.has_a_car==false)
	{
		if ((boolvars.landgetup == 1 || boolvars.has_a_car == true)
			&& CTimer::m_snTimeInMillisecondsNonClipped > clanktime + timeclankin)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_land") == true
				|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") == true
				|| plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") == true
				|| jarvisvoice.is_over_ground(2.0f) == true)
			{
				if (jarvisvoice.is_over_ground(1.5f) == false)
				{
					//if (CTimer::m_snTimeInMillisecondsNonClipped > (m_nLastTimeWhenAnyActionWasEnabled + 200)) {
					if (boolvars.mousewheelhacktrigger == true) {
						spd = 200.0f;
						UnhookWindowsHookEx(mousehook);
						jarvisvoice.SetVelocityUnlimited(false);
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
						boolvars.mousewheelhacktrigger = false;
					}
					if (boolvars.systemerror == true) {
						AudioLib.PlayVoiceEvent(25);
						boolvars.systemerror = false;
					}

					boolvars.landgetup = 0;
					plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
					//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newanglepos());
					//player->SetHeading(jarvisvoice.newanglepos());
					//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
					boolvars.indx = 0;

					return true;
				}
				else {/*
					  if (jarvisvoice.is_over_ground(2.0f) == true)
					  {*/

					if (boolvars.systemerror == true) {
						AudioLib.PlayVoiceEvent(25);
						boolvars.systemerror = false;
					}

						boolvars.landgetup = 0;
						boolvars.indx = 19;
						return true;
					
					/*
					}
					else {
					if (boolvars.mousewheelhacktrigger == true) {
					spd = 200.0f;
					UnhookWindowsHookEx(mousehook);
					jarvisvoice.SetVelocityUnlimited(false);
					plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
					boolvars.mousewheelhacktrigger = false;
					}
					if (boolvars.iscjfrozen == true)
					{
					boolvars.iscjfrozen = false;
					plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
					}
					jarvisvoice.setvelocityindirection(player, 0.0f, 0.0f, -10.0);
					//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newanglepos());
					//player->SetHeading(jarvisvoice.newanglepos());
					//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
					return true;
					}*/
				}
			}
			else {
				boolvars.indx = 0;

				if (boolvars.systemerror == true) {
					AudioLib.PlayVoiceEvent(25);
					boolvars.systemerror = false;
				}

				boolvars.landgetup = 0;
				return true;
			}
		}
		else {
			if (debevolar == true)
			{
				if (boolvars.landgetup == 1)
				{
					boolvars.landgetup = 1;
					if (jarvisvoice.is_over_ground(1.5f) == false)
					{
						clanktime = CTimer::m_snTimeInMillisecondsNonClipped;
						timeclankin = 1500;
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getup") != true
							&& plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "getupfront") != true)
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_FrontExplosion") == true)
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
								boolvars.thrusterskilled = true;
								if (boolvars.mousewheelhacktrigger == true) {
									spd = 200.0f;
									UnhookWindowsHookEx(mousehook);
									jarvisvoice.SetVelocityUnlimited(false);
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
									boolvars.mousewheelhacktrigger = false;
								}
								//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getupfront", "ironman", 4.0f, 0, 1, 1, 0, -1);
								//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
								//player->SetHeading(jarvisvoice.newanglepos());
								//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
								return true;
							}
							else
							{
								if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "Fall_BackExplosion") == true)
								{
									JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
									JarvisVoice::storeenviroment(&propellers.enviroment_ID);
									boolvars.thrusterskilled = true;
									if (boolvars.mousewheelhacktrigger == true) {
										spd = 200.0f;
										UnhookWindowsHookEx(mousehook);
										jarvisvoice.SetVelocityUnlimited(false);
										plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
										boolvars.mousewheelhacktrigger = false;
									}
									boolvars.landgetup = 1;
									//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "getup", "ironman", 4.0f, 0, 1, 1, 0, -1);
									//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
									//player->SetHeading(jarvisvoice.newanglepos());
									//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
									return true;
								}
								return true;
							}
						}
					}
					else
					{
						if (debevolar==true)
						{
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
							boolvars.landgetup = 0;
							boolvars.indx = 19;
						}
						else
						{
							boolvars.indx = 22;
						}
						return true;
					}
					return true;
				}
				else
				{
					if (boolvars.has_a_car == true)
					{
						clanktime = CTimer::m_snTimeInMillisecondsNonClipped;
						timeclankin = 500;
						return true;
					}
					else
					{
						static bool clanktrigger;
						//plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_land", "ironman", 20.0f, 0, 0, 0, 1, 1000);
						//plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_land", "ironman", 10.0f, 0, 0, 0, 0, 500);
						//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newanglepos());
						if (jarvisvoice.is_over_ground(2.0f) == false)
						{
							if (clanktrigger == false)
							{
								clanktrigger = true;
								if (boolvars.activesuit != -4 && boolvars.activesuit != -5 && boolvars.activesuit != -6 && boolvars.activesuit != 0)
								{
									if (IsAudioStreamPlayed(IMStream[2], audiostream[clank]) == false)
									{

										PlayAudiostream(audiostream[clank], 0, &IMStream[2]);
										clanktime = CTimer::m_snTimeInMillisecondsNonClipped;
										plugin::scripting::CallCommandById(COMMAND_SHAKE_CAM, 100);
									}
								}
								else
								{
									clanktime = CTimer::m_snTimeInMillisecondsNonClipped;
								}
								//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_land", "ironman", 10.0f, 0, 1, 1, 0, -1);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
								//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
								//player->SetHeading(jarvisvoice.newanglepos());

								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());
								if (boolvars.mousewheelhacktrigger == true) {
									spd = 200.0f;
									UnhookWindowsHookEx(mousehook);
									jarvisvoice.SetVelocityUnlimited(false);
									plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
									boolvars.mousewheelhacktrigger = false;
								}
								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 500;
							}
							else
							{
								clanktrigger = false;
								boolvars.indx = 0;
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 200;
								return true;
							}
						}
						else
						{
							clanktrigger = false;
							boolvars.indx = 19;
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
							return true;
						}
					}
					//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);
					jarvisvoice.setvelocityindirection(player, 0.0f, 0.0f, -10.0);
					//player->SetOrientation(0.0f, 0.0f, jarvisvoice.newanglepos());
					//player->SetHeading(jarvisvoice.newanglepos());
					//m_nLastTimeWhenAnyActionWasEnabled = CTimer::m_snTimeInMillisecondsNonClipped;

					plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, newangle1());


					return true;
				}
			}
			else
			{
				boolvars.indx = 22;
				goto aimin;
			}
		}
	}
	else {
		
		if (boolvars.mousewheelhacktrigger == true) {
			spd = 200.0f;
			UnhookWindowsHookEx(mousehook);
			jarvisvoice.SetVelocityUnlimited(false);
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
			boolvars.mousewheelhacktrigger = false;
		}
		boolvars.landgetup = 0;
		if (boolvars.has_a_car == false)
		{
			boolvars.indx = 0;
		}
		else
		{
			boolvars.indx = 13;
		}
		return true;
	}
	}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
return true;
}
//21
impulsarse:
{
if (*pActor > 0)
{
	if (GetKeyState('X') & 0x8000)
	{
		if (impulse < 100)
		{
			impulse++;
		}
		if (boolvars.indx != 13 && boolvars.has_a_car == false)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_impulse") == false)
			{
				plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM, player, "fly_impulse", "ironman", 2.5, 0, 0, 0, 1, 2);
			}
			else
			{
				plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_PLAYING_FLAG, player, "fly_impulse", 0);
				plugin::scripting::CallCommandById(COMMAND_SHAKE_CAM, impulse);

			}
		}
		else
		{
			plugin::scripting::CallCommandById(COMMAND_SHAKE_CAM, impulse);
		}
		std::string impul = boolvars.impulso.c_str();
		float pR, pG;
		int R, G;
		char *numb1;
		numb1 = new char[17];
		sprintf(numb1, ": %d", impulse);
		impul += numb1;

		pR = (float)(255 * impulse / 50);
		pG = (float)(255 * (100 - impulse) / 50);
		if (pR > 255.0f) {
			pR = 255.0f;
		}
		if (pG > 255.0f) {
			pG = 255.0f;
		}
		R = (int)pR;
		G = (int)pG;
		CFont::SetBackground(0, 0);
		CFont::SetColor(CRGBA(R, G, 0, 255));
		CFont::SetOrientation(ALIGN_CENTER);
		CFont::SetProportional(true);
		CFont::SetJustify(false);
		CFont::SetFontStyle(FONT_MENU);
		CFont::SetScale(SCREEN_MULTIPLIER(settings.vecVehicleNameScale.x), SCREEN_MULTIPLIER(settings.vecVehicleNameScale.y));
		CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesPosnY), impul.data());
		delete[] numb1;
		impul.clear();
		return true;
	}
	else {
		if (impulse > 0 && impulse <= 100)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SWIMMING, player) == false && (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_impulse") == true || (boolvars.indx == 13)))
			{
				if (boolvars.indx == 13)
				{
					boolvars.has_a_car = true;
				}
				else
				{
					plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ANIM_PLAYING_FLAG, player, "fly_impulse", 1);
					boolvars.iscjfrozen = false;
					plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
				}
				boolvars.indx = 23;
				return true;
			}
			else
			{
				if (boolvars.indx != 13)
				{
					boolvars.indx = 0;
				}
				return true;
			}
		}
		else
		{
			if (boolvars.indx == 13)
			{
				boolvars.has_a_car = true;
			}
			else {
				boolvars.indx = 0;
			}
			return true;
		}
	}
}
else {
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}

//23
volarsolo:
{
if (*pActor > 0)
{
if (jarvisvoice.Has_not_switched_enviroment(&propellers))
{

	if (isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL)==false) {
		JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 1, arriba, &propellers);
	}
}
else
{
		JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
		JarvisVoice::storeenviroment(&propellers.enviroment_ID);
		boolvars.thrusterskilled = true;
	
}

if (jarvisvoice.has_obstacles(player, 0.0f, 0.0f, 3.0f, 1, 1, 0, 1, 0) == true) {
	impulse = 0;
	if (boolvars.has_a_car == true)
	{
		boolvars.indx = 13;
	}
	else
	{
		boolvars.indx = 19;
	}
	return true;
}
else
{
	if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_carUp") == false && plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_MoveUp") == false)
	{
		JarvisVoice::PlayAudiostream(audiostream[takeoff], 0, &IMStream[2]);
		JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
		JarvisVoice::storeenviroment(&propellers.enviroment_ID);
		boolvars.thrusterskilled = true;
	}
	boolvars.iscjfrozen = false;
	plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
	if (boolvars.has_a_car == true)
	{
		plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_carUp", "ironman", 10.0f, 0, 0, 0, 0, -1);
	}
	else
	{
		plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_MoveUp", "ironman", 10.0f, 0, 0, 0, 0, -1);
	}
	jarvisvoice.setvelocityindirection(player, 0.0f, 0.0f, impulse + 10.0f);
	if (!boolvars.mousewheelhacktrigger) {
		jarvisvoice.SetVelocityUnlimited(true);
		mousehook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);
		plugin::scripting::CallCommandById(COMMAND_SET_CURRENT_CHAR_WEAPON, player, 0);
		plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 0);
		boolvars.mousewheelhacktrigger = true;
	}
	if (jarvisvoice.is_over_ground((float)impulse))//se mueve o llego al limite
	{
		impulse = 0;
		if (boolvars.has_a_car == true)
		{
			boolvars.indx = 13;
		}
		else
		{
			boolvars.indx = 19;
		}
	}
	else
	{
		if (jarvisvoice.nosemueve() == false)
		{
			impulse = 0;
			if (boolvars.has_a_car == true)
			{
				boolvars.indx = 13;
			}
			else
			{
				boolvars.indx = 19;
			}
		}
	}
	return true;
}
}
else{
	pActor = (DWORD*)0xB6F5F0;
	return true;
}
}

suitmenu:
{
	CVector2D screensize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	CVector2D screenmiddle = { SCREEN_COORD_CENTER_X ,SCREEN_COORD_CENTER_Y };

	/*
	if (marktony->m_pTexture == NULL ||
		chosen->m_pTexture == NULL ||
		notchosen->m_pTexture == NULL ||
		signo->m_pTexture == NULL ||
		signo1->m_pTexture == NULL ||
		closewdw->m_pTexture == NULL ||
		menu3->m_pTexture == NULL ||
		menu4->m_pTexture == NULL ||
		menu5->m_pTexture == NULL ||
		cursor->m_pTexture == NULL ||
		settingmenu->m_pTexture == NULL ||
		closebtn->m_pTexture == NULL ||
		check->m_pTexture == NULL ||
		uncheck->m_pTexture == NULL ||
		menusel->m_pTexture == NULL)
	{
		movtextures.Loadmenuicons();
	}*/
		cutenabled = plugin::patch::GetUChar(11989093, false);
		if (cutenabled != 1)
		{
			if (settings.radarmode != 2)
			{
				if (plugin::scripting::CallCommandById(COMMAND_HAS_CUTSCENE_LOADED) == true)
				{
					if (plugin::scripting::CallCommandById(COMMAND_HAS_CUTSCENE_FINISHED) == true)
					{
						//plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 0);
						//boolvars.radarisactive = false;
					}
					else
					{
						boolvars.indx = 0;
						boolvars.settingisactive = false;
						WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
						boolvars.menuisactive = false;
						return true;
					}
				}
				else
				{
					//boolvars.radarisactive = false;
					//plugin::scripting::CallCommandById(COMMAND_DISPLAY_RADAR, 0);
				}
			}
		}
		else {
			boolvars.indx = 0;
			boolvars.settingisactive = false;
			WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			boolvars.menuisactive = false;
			return true;
		}

		if (boolvars.mousewheelhacktrigger == true) {
			spd = 200.0f;
			UnhookWindowsHookEx(mousehook);
			jarvisvoice.SetVelocityUnlimited(false);
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
			boolvars.mousewheelhacktrigger = false;
		}
		
		//boolvars.wpmenuisactive == false &&
		if (cutenabled != 1 && is_on_foot() == true &&
			boolvars.menuisactive == true &&
			plugin::scripting::CallCommandById(COMMAND_GET_FADING_STATUS) == false &&
			plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_ARRESTED, player) == false &&
			plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, player) == false &&
			plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SWIMMING, player) == false &&
			boolvars.has_a_car == false &&
			boolvars.aims == false)
		{
			if (!ordendada)
			{
				beginend = false;
				if (boolvars.alphafad > 200)
				{
					boolvars.alphafad = 250;
					ordendada = true;
					boolvars.timrmenu = CTimer::m_snTimeInMillisecondsNonClipped;
				}
				else
				{
					boolvars.alphafad += 50;
				}
				//CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 208, 225, (int)boolvars.alphafad));
				return true;
			}
			else
			{
				if (ordenelegir == true)
				{
					beginend = true;
					if (boolvars.alphafad > 200)
					{
						boolvars.alphafad = 250;
						WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
						boolvars.indx = 0;
						boolvars.menuisactive = false;
						boolvars.timrmenu = CTimer::m_snTimeInMillisecondsNonClipped;
						//   CLAVE!!!!!!!!!!!
					}
					else
					{
						boolvars.alphafad += 50;
					}
				}
				else
				{
					beginend = false;
					if (CTimer::m_snTimeInMillisecondsNonClipped > boolvars.timrmenu + 100)
					{
						if (boolvars.alphafad >= 40)
						{
							boolvars.alphafad -= 40;
						}
						else
						{
							boolvars.alphafad = 0;
						}
					}
				}
				/*
				static bool hoverprevfolder;
				static bool hovernextfolder;*/
				bool eligioalgo = false;
				/*static bool hoverprev;
				static bool hovernext;
				static bool hoversets;
				static bool hoverclosets; */



				static int timedoption;



				CSprite2d::DrawRect(CRect(0.0f, 0.0f, screensize.x, screensize.y), CRGBA(0, 0, 0, 170));
				//update mouse position begin
				float movspeed = 0.1f;
				float movx, movy;
				plugin::scripting::CallCommandById(COMMAND_GET_PC_MOUSE_MOVEMENT, &movx, &movy);
				if (plugin::scripting::CallCommandById(COMMAND_IS_MOUSE_USING_VERTICAL_INVERSION) != NULL)
				{
					movy *= -1;
				}
				int movfactor = 0;

				movfactor = plugin::patch::GetInt(11987996, false) / 100000000;
				movx *= movfactor;
				movy *= movfactor;
				movx *= movspeed;
				movy *= movspeed;
				cursorx += movx;
				cursory += movy;
				if (cursorx < SCREEN_COORD(0.0f)) {
					cursorx = SCREEN_COORD(0.0f);
				}

				if (cursorx > screensize.x) {
					cursorx = screensize.x;
				}

				if (cursory < SCREEN_COORD(0.0f)) {
					cursory = SCREEN_COORD(0.0f);
				}

				if (cursory > screensize.y) {
					cursory = screensize.y;
				}


				float margin = SCREEN_COORD(120.0f);
				float margin2 = SCREEN_COORD(160.0f);

				if (DrawClickIconAtCoords(0, true, closewdw, closewdw, (screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f) - sizex, SCREEN_COORD(10.0f), (screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f), margin - SCREEN_COORD(10.0f), cursorx, cursory) == true && boolvars.settingisactive == false)
				{
					iconsel = 1;
					eligioalgo = true;
				}



				static int elemento;
				CRect poselemento;
				if (settings.folderdirs[boolvars.yndex].subcontentamnt > 41)
				{
					if (DrawClickIconAtCoords(1, true, menu3, menu4, (screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f) - sizex, margin - SCREEN_COORD(10.0f), (screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f), margin + (screensize.y - margin2) + SCREEN_COORD(10.0f), cursorx, cursory) == true && boolvars.settingisactive == false)
					{
						iconsel = 2;
						eligioalgo = true;
					}

					if (DrawClickIconAtCoords(1, true, menu3, menu4, (screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f) + sizex, margin - SCREEN_COORD(10.0f), (screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f), margin + (screensize.y - margin2) + SCREEN_COORD(10.0f), cursorx, cursory) == true && boolvars.settingisactive == false)
					{
						iconsel = 3;
						eligioalgo = true;
					}
				}
				else
				{
					//hoverprev = false;
					//hovernext = false;
					menupage = 0;
				}
				if (DrawClickIconAtCoords(0, true, closewdw, closewdw, (screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f) + sizex, SCREEN_COORD(10.0f), (screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f), margin - SCREEN_COORD(10.0f), cursorx, cursory) == true && boolvars.settingisactive == false)
				{
					iconsel = 5;
					eligioalgo = true;
				}


				static bool noicon;
				static bool exist;
				static bool inexistente;
				static int chosen2;
				chosen2 = -1;
				static bool ishovered;
				static bool thereisasuit;

				menu5->Draw(CRect((screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f), margin - SCREEN_COORD(10.0f), (screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f), margin + (screensize.y - margin2) + SCREEN_COORD(10.0f)), CRGBA(255, 255, 255, 255));

				
				for (int filas=0;filas < 6;filas++)
				{
					for (int columnas = 0; columnas < 7; columnas++)
					{
						int armormenu = menupage*42;
						int filasx7 = filas * 7;
						elemento = columnas + filasx7 + armormenu;
						poselemento.left = (screenmiddle.x - screensize.y / 2) + (screensize.y) / 7 * (float)columnas;
						poselemento.top = margin + ((screensize.y - margin2) / 6 * (float)filas);
						poselemento.right = (screenmiddle.x - screensize.y / 2) + (screensize.y) / 7 * ((float)(columnas) + 1.0f);
						poselemento.bottom = margin + ((screensize.y - margin2) / 6 * ((float)(filas) + 1.0f));

						bool exost = false;
						if (strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark01") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark02") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark03") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark04") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark05") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark06") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[elemento].pngname, "mark07") != 0 &&
							elemento != 0)
						{
							if (settings.folderdirs[boolvars.yndex].suits[elemento].texexists == false || settings.folderdirs[boolvars.yndex].suits[elemento].mdlexists == false)
							{
								exost = false;
								exist = false;
							}
							else
							{
								exost = true;
								if (settings.folderdirs[boolvars.yndex].suits[elemento].iconexists == false)
								{
									noicon = true;
								}
								else
								{
									noicon = false;
								}
								exist = true;
							}
						}
						else
						{
							exost = true;
							
							if (settings.folderdirs[boolvars.yndex].suits[elemento].iconexists == false)
							{
								noicon = true;
							}
							else
							{
								noicon = false;
							}
							exist = true;
						}
						DrawSuitClickIconAtCoords(exist, noicon, elemento, poselemento.left, poselemento.top, poselemento.right, poselemento.bottom, cursorx, cursory, &ishovered);
						if (ishovered == true && boolvars.settingisactive == false)
						{
							eligioalgo = true;
							chosen2 = elemento;
							if (hoveredmark != chosen2)
							{
								hoveredmark = chosen2;
								if (
									iconsel!=0)
								{
									if (exost==true)
									{
										AudioLib.PlayMenuSFX(0);
									}
									else
									{
										AudioLib.PlayMenuSFX(6);
									}
								}
								iconsel = 4;
							}
							else
							{
								iconsel = 4;
								/**/
								if (strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark01") != 0 &&
									strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark02") != 0 &&
									strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark03") != 0 &&
									strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark04") != 0 &&
									strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark05") != 0 &&
									strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark06") != 0 &&
									strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark07") != 0 &&
									chosen2 != 0)
								{
									if (settings.folderdirs[boolvars.yndex].suits[chosen2].texexists == true || settings.folderdirs[boolvars.yndex].suits[chosen2].mdlexists == true)
									{
										inexistente = false;
									}
									else
									{
										inexistente = true;
									}
								}
								else
								{
									inexistente = false;
								}
							}
						}
						//}

						fflush(stdin);

					}
					
					fflush(stdin);
				}
				std::string suitarmormenu1;
				CFont::SetBackground(0, 0);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetColor(CRGBA(255, 255, 255, 255));
				CFont::SetFontStyle(FONT_MENU);
				CFont::SetEdge(2);
				CFont::SetCentreSize(screensize.x + SCREEN_COORD(-350.0f));
				CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
				
				if (offsetx >= sixe)
				{
					offsetx = sixe;
				}

				if (offsetx <= SCREEN_COORD(0.0f))
				{
					offsetx = SCREEN_COORD(0.0f);
				}


				

				if (boolvars.settingisactive == false)
				{
					if (offsetx <= sixe - SCREEN_COORD(50.0f))
					{
						offsetx += SCREEN_COORD(50.0f);
					}
					else
					{
						offsetx = sixe;
					}
					CSprite2d::DrawRect(CRect(SCREEN_COORD(0.0f) - offsetx,
						margin - SCREEN_COORD(10.0f),
						sixe - offsetx,
						margin + (screensize.y - margin2) + SCREEN_COORD(10.0f)), CRGBA(128, 128, 128, 255));

					CSprite2d::DrawRect(CRect(SCREEN_COORD(0.0f) - offsetx,
						margin + SCREEN_COORD(10.0f),
						sixe - SCREEN_COORD(20.0f) - offsetx,
						margin + (screensize.y - margin2) - SCREEN_COORD(10.0f)), CRGBA(50, 50, 50, 255));

					suitarmormenu1 = settings.folderdirs[boolvars.yndex].name;
					suitarmormenu1 += " ";
					suitarmormenu1 += boolvars.suitarmormenu;

					CFont::PrintString(screenmiddle.x, SCREEN_COORD_TOP(settings.fRadioNameTopPosnY), suitarmormenu1.data());
					suitarmormenu1.clear();

					CFont::SetFontStyle(FONT_SUBTITLES);
					CFont::SetScale(SCREEN_MULTIPLIER(0.7f), SCREEN_MULTIPLIER(1.4f));
					CFont::PrintString(screenmiddle.x, SCREEN_COORD_TOP(50.0f), boolvars.Clicoptions.data());
				
					if(DrawClickIconAtCoords(0, false,
						settingmenu, 
						settingmenu, 
						(screenmiddle.x - screensize.y / 2)-SCREEN_COORD(10.0f),
						SCREEN_COORD(10.0f),
						(screenmiddle.x - screensize.y / 2)-SCREEN_COORD(10.0f) + sizex,
						margin - SCREEN_COORD(10.0f), cursorx, cursory) == true) {
						iconsel = 6;
						eligioalgo = true;
					}

					if(DrawClickIconAtCoords(0, false,
						closebtn,
						closebtn,
						(screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f) - sizex,
						SCREEN_COORD(10.0f),
						(screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f),
						margin - SCREEN_COORD(10.0f), cursorx, cursory)== true) {
						iconsel = 7;
						eligioalgo = true;
					}
					
				}
				else
				{
					
					if (offsetx >= SCREEN_COORD(50.0f))
					{
						offsetx -= SCREEN_COORD(50.0f);
					}
					else
					{
						offsetx = SCREEN_COORD(0.0f);
					}
					CSprite2d::DrawRect(CRect(0.0f, 0.0f, screensize.x, screensize.y), CRGBA(0, 0, 0, 170));

					CSprite2d::DrawRect(CRect(SCREEN_COORD(0.0f) - offsetx,
						margin - SCREEN_COORD(10.0f),
						sixe - offsetx,
						margin + (screensize.y - margin2) + SCREEN_COORD(10.0f)), CRGBA(128, 128, 128, 255));
					
					CSprite2d::DrawRect(CRect(SCREEN_COORD(0.0f) - offsetx,
						margin + SCREEN_COORD(10.0f),
						sixe - SCREEN_COORD(20.0f) - offsetx,
						margin + (screensize.y - margin2) - SCREEN_COORD(10.0f)), CRGBA(50, 50, 50, 255));

					if(DrawClickIconAtCoords(0, false, 
						settingmenu,
						settingmenu,
						(screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f),
						SCREEN_COORD(10.0f),
						(screenmiddle.x - screensize.y / 2) - SCREEN_COORD(10.0f) + sizex,
						margin - SCREEN_COORD(10.0f), cursorx, cursory)== true) {
						iconsel = 6;
						eligioalgo = true;
					}

					if(DrawClickIconAtCoords(0, false,
						closebtn,
						closebtn,
						(screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f) - sizex,
						SCREEN_COORD(10.0f),
						(screenmiddle.x - screensize.y / 2) + (screensize.y)+SCREEN_COORD(10.0f),
						margin - SCREEN_COORD(10.0f), cursorx, cursory) == true) {
						iconsel = 7;
						eligioalgo = true;
					}
					suitarmormenu1 = boolvars.Options.data();
					CFont::PrintString(screenmiddle.x, SCREEN_COORD_TOP(settings.fRadioNameTopPosnY), suitarmormenu1.data());
					suitarmormenu1.clear();

					CFont::SetFontStyle(FONT_SUBTITLES);
					CFont::SetScale(SCREEN_MULTIPLIER(0.7f), SCREEN_MULTIPLIER(1.4f));
					CFont::PrintString(screenmiddle.x, SCREEN_COORD_TOP(50.0f), boolvars.helptext8.data());

					static int status;

					if (jarvisvoice.Va_adelante() && CTimer::m_snTimeInMillisecondsNonClipped > timedoption + 200)
					{
						timedoption = CTimer::m_snTimeInMillisecondsNonClipped;
						status = 1;
					}

					if (jarvisvoice.Va_atras() && CTimer::m_snTimeInMillisecondsNonClipped > timedoption + 200)
					{
						timedoption = CTimer::m_snTimeInMillisecondsNonClipped;
						status = 2;
					}
					
					if (status == 1)
					{
						while (cfgnro >= 0)
						{
							cfgnro -= 1; 
							if (cfgnro < 0)
							{
								break;
							}
							if (strcmp(config.paramLines.at(cfgnro)._value.c_str(), "NO") == 0
								|| strcmp(config.paramLines.at(cfgnro)._value.c_str(), "YES") == 0)
							{
								break;
							}
						}

						if (cfgnro < 0)
						{
							cfgnro = config.paramLines.size() - 1;

							while (cfgnro >= 0)
							{
								if (cfgnro < 0)
								{
									break;
								}
								if (strcmp(config.paramLines.at(cfgnro)._value.c_str(), "NO") == 0
									|| strcmp(config.paramLines.at(cfgnro)._value.c_str(), "YES") == 0)
								{
									break;
								}
								cfgnro -= 1;
							}
							if (cfgnro < 0)
							{
								cfgnro = config.paramLines.size() - 1;
							}
						}
						status = 0;
					}
					if (status == 2)
					{
						while (cfgnro <= config.paramLines.size() - 1)
						{
							cfgnro += 1;
							if (cfgnro > config.paramLines.size() - 1)
							{
								break;
							}
							if (strcmp(config.paramLines.at(cfgnro)._value.c_str(), "NO") == 0
								|| strcmp(config.paramLines.at(cfgnro)._value.c_str(), "YES") == 0)
							{
								break;
							}
						}

						if (cfgnro > config.paramLines.size() - 1)
						{
							cfgnro = 0;
							while (cfgnro < config.paramLines.size() - 1)
							{
								if (cfgnro > config.paramLines.size() - 1)
								{
									break;
								}
								if (strcmp(config.paramLines.at(cfgnro)._value.c_str(), "NO") == 0
									|| strcmp(config.paramLines.at(cfgnro)._value.c_str(), "YES") == 0)
								{
									break;
								}
								cfgnro += 1;
							}

							if (cfgnro > config.paramLines.size() - 1)
							{
								cfgnro = 0;
							}
						}
						status = 0;
					}
				}
				if (eligioalgo == true)
				{
					if (iconsel != iconsel1)
					{
						AudioLib.PlayMenuSFX(0);
					}
					iconsel1 = iconsel;
				}
				else
				{
					iconsel = 0;
					iconsel1 = 0;
				}
				
				CSprite2d::DrawRect(CRect(SCREEN_COORD_CENTER_LEFT(350.0f) - offsetx,
					SCREEN_COORD_CENTER_UP(50.0f), 
					SCREEN_COORD_CENTER_RIGHT(350.0f) - offsetx,
					SCREEN_COORD_CENTER_DOWN(10.0f)), CRGBA(95, 151, 208, 255));



				menusel->Draw(SCREEN_COORD_CENTER_RIGHT(350.0f) - SCREEN_COORD(80.0f) - offsetx,
					SCREEN_COORD_CENTER_UP(135.0f), 
					SCREEN_COORD(30.0f),
					SCREEN_COORD(30.0f), CRGBA(255, 255, 255, 255));
				
				CFont::SetFontStyle(FONT_SUBTITLES);
				CFont::SetEdge(1);
//		
				//menusel->Draw(SCREEN_COORD_CENTER_RIGHT(450.0f) - SCREEN_COORD(80.0f), SCREEN_COORD_CENTER_UP(135.0f), SCREEN_COORD(30.0f), SCREEN_COORD(30.0f), CRGBA(255, 255, 255, 255));

				int ant = cfgnro-1, post = cfgnro + 1;
				
				while (ant >= 0)
				{
					if (ant <= config.paramLines.size() - 1)
					{
						if (strcmp(config.paramLines.at(ant)._value.c_str(), "NO") == 0
							|| strcmp(config.paramLines.at(ant)._value.c_str(), "YES") == 0)
						{
							CFont::SetBackground(0, 0);
							CFont::SetColor(CRGBA(255, 255, 255, 150));
							CFont::SetOrientation(ALIGN_LEFT);
							CFont::SetProportional(true);
							CFont::SetJustify(false);
							CFont::SetFontStyle(FONT_SUBTITLES);
							CFont::SetEdge(0);
							//CFont::SetDropColor(CRGBA(0, 0, 0, 255));
							CFont::SetScale(SCREEN_MULTIPLIER(0.75f), SCREEN_MULTIPLIER(1.5f));
							CFont::PrintString(SCREEN_COORD_CENTER_LEFT(300.0f) - offsetx, SCREEN_COORD_CENTER_UP(90.0f), config.paramLines.at(ant).name.data());
							break;
						}
						else
						{
							ant -= 1;
						}
					}
					else
					{
						break;
					}
				}
				if (ant < 0)
				{
					ant = config.paramLines.size() - 1;
					while (ant >= 0)
					{
						if (ant <= config.paramLines.size() - 1)
						{
							if (strcmp(config.paramLines.at(ant)._value.c_str(), "NO") == 0
								|| strcmp(config.paramLines.at(ant)._value.c_str(), "YES") == 0)
							{
								CFont::SetBackground(0, 0);
								CFont::SetColor(CRGBA(255, 255, 255, 150));
								CFont::SetOrientation(ALIGN_LEFT);
								CFont::SetProportional(true);
								CFont::SetJustify(false);
								CFont::SetFontStyle(FONT_SUBTITLES);
								CFont::SetEdge(0);
								//CFont::SetDropColor(CRGBA(0, 0, 0, 255));
								CFont::SetScale(SCREEN_MULTIPLIER(0.75f), SCREEN_MULTIPLIER(1.5f));
								CFont::PrintString(SCREEN_COORD_CENTER_LEFT(300.0f) - offsetx, SCREEN_COORD_CENTER_UP(90.0f), config.paramLines.at(ant).name.data());
								break;
							}
							else
							{
								ant -= 1;
							}
						}
						else
						{
							break;
						}
					}
					if (ant < 0)
					{
						ant = config.paramLines.size() - 1;
					}
				}
				
				while (post <= config.paramLines.size() - 1)
				{
					if (post >= 0)
					{
						if (strcmp(config.paramLines.at(post)._value.c_str(), "NO") == 0
							|| strcmp(config.paramLines.at(post)._value.c_str(), "YES") == 0)
						{
							CFont::SetBackground(0, 0);
							CFont::SetColor(CRGBA(255, 255, 255, 150));
							CFont::SetOrientation(ALIGN_LEFT);
							CFont::SetProportional(true);
							CFont::SetJustify(false);
							CFont::SetFontStyle(FONT_SUBTITLES);
							CFont::SetEdge(0);
							//CFont::SetDropColor(CRGBA(0, 0, 0, 255));
							CFont::SetScale(SCREEN_MULTIPLIER(0.75f), SCREEN_MULTIPLIER(1.5f));
							CFont::PrintString(SCREEN_COORD_CENTER_LEFT(300.0f) - offsetx, SCREEN_COORD_CENTER_DOWN(15.0f), config.paramLines.at(post).name.data());
							break;
						}
						else
						{
							post += 1;
						}
					}
					else
					{
						break;
					}
				}
				
				if (post > config.paramLines.size() - 1)
				{
					post = 0;
					while (post <= config.paramLines.size() - 1)
					{
						if (post >= 0)
						{
							if (strcmp(config.paramLines.at(post)._value.c_str(), "NO") == 0
								|| strcmp(config.paramLines.at(post)._value.c_str(), "YES") == 0)
							{
								CFont::SetBackground(0, 0);
								CFont::SetColor(CRGBA(255, 255, 255, 150));
								CFont::SetOrientation(ALIGN_LEFT);
								CFont::SetProportional(true);
								CFont::SetJustify(false);
								CFont::SetFontStyle(FONT_SUBTITLES);
								CFont::SetEdge(0);
								//CFont::SetDropColor(CRGBA(0, 0, 0, 255));
								CFont::SetScale(SCREEN_MULTIPLIER(0.75f), SCREEN_MULTIPLIER(1.5f));
								CFont::PrintString(SCREEN_COORD_CENTER_LEFT(300.0f) - offsetx, SCREEN_COORD_CENTER_DOWN(15.0f), config.paramLines.at(post).name.data());
								break;
							}
							else
							{
								post += 1;
							}
						}
						else
						{
							break;
						}
					}
					if (post > config.paramLines.size() - 1)
					{
						post = 0;
					}
				}
				CFont::SetBackground(0, 0);
				CFont::SetColor(CRGBA(255, 255, 255, 255));
				CFont::SetOrientation(ALIGN_LEFT);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetFontStyle(FONT_SUBTITLES);
				CFont::SetEdge(0);
				//CFont::SetDropColor(CRGBA(0, 0, 0, 255));
				CFont::SetScale(SCREEN_MULTIPLIER(0.75f), SCREEN_MULTIPLIER(1.5f));
				CFont::PrintString(SCREEN_COORD_CENTER_LEFT(300.0f) - offsetx, SCREEN_COORD_CENTER_UP(40.0f), config.paramLines.at(cfgnro).name.data());
				
				if (strcmp(config.paramLines.at(cfgnro)._value.c_str(), "YES") == 0)
				{
					check->Draw(SCREEN_COORD_CENTER_RIGHT(350.0f) - SCREEN_COORD(80.0f) - offsetx,
						SCREEN_COORD_CENTER_UP(40.0f),
						SCREEN_COORD(60.0f),
						SCREEN_COORD(30.0f), CRGBA(255, 255, 255, 255));
					if (boolvars.settingisactive)
					{
						if (CTimer::m_snTimeInMillisecondsNonClipped > timedoption + 200)
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 16) == 1)
							{
								timedoption = CTimer::m_snTimeInMillisecondsNonClipped;
								config.paramLines.at(cfgnro)._value.clear();
								config.paramLines.at(cfgnro)._value.assign("NO");
								config.save();
								settings.Read();
							}
						}
					}
				}
				
				if (strcmp(config.paramLines.at(cfgnro)._value.c_str(), "NO") == 0)
				{
					uncheck->Draw(SCREEN_COORD_CENTER_RIGHT(350.0f) - SCREEN_COORD(80.0f) - offsetx,
						SCREEN_COORD_CENTER_UP(40.0f),
						SCREEN_COORD(60.0f),
						SCREEN_COORD(30.0f), CRGBA(255, 255, 255, 255));
					if (boolvars.settingisactive)
					{
						if (CTimer::m_snTimeInMillisecondsNonClipped > timedoption + 200)
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 16) == 1)
							{
								timedoption = CTimer::m_snTimeInMillisecondsNonClipped;
								config.paramLines.at(cfgnro)._value.clear();
								config.paramLines.at(cfgnro)._value.assign("YES");
								config.save();
								settings.Read();
							}
						}
					}
				}
				


















				cursor->Draw(CRect(cursorx, cursory, cursorx + SCREEN_COORD(20.0f), cursory + SCREEN_COORD(20.0f)), CRGBA(255, 255, 255, 255));
				
				if (boolvars.alphafad < 1)
				{
					if (CMouseControllerState().lmb != 1 && (GetKeyState(VK_LBUTTON) & 0x8000) == false) {
						if (CMouseControllerState().rmb != 1 && (GetKeyState(VK_RBUTTON) & 0x8000) == false)
						{
							if (iconsel > 0)
							{
								CFont::SetBackground(0, 0);
								CFont::SetOrientation(ALIGN_CENTER);
								CFont::SetProportional(true);
								CFont::SetJustify(false);
								CFont::SetColor(CRGBA(0, 0, 0, 255));
								CFont::SetFontStyle(FONT_SUBTITLES);
								CFont::SetEdge(0);
								CFont::SetCentreSize(screensize.x + SCREEN_COORD(-350));
								CFont::SetScale(SCREEN_MULTIPLIER(0.5f), SCREEN_MULTIPLIER(1.0f));
								if (iconsel == 4)
								{
									if (settings.folderdirs[boolvars.yndex].suits[chosen2].iconexists == true)
									{
										if (inexistente == true)
										{
											CFont::SetColor(CRGBA(255, 0, 0, 255));
											CSprite2d::DrawRect(CRect(cursorx + SCREEN_COORD(14.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(210.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
											CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Notplay.data());
										}
										else
										{
											if (chosen2 == 0)
											{
												CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
												CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Tonyplay.data());
											}
											else
											{
												CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
												CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), settings.folderdirs[boolvars.yndex].suits[chosen2].name);
											}
										}
									}
								}
								else
								{
									if (iconsel == 5)
									{
										CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
										CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Nextmenu.data());
									}
									else
									{
										if (iconsel == 2)
										{
											CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
											CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.menupag1.data());
										}
										else
										{
											if (iconsel == 3)
											{
												CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
												CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.menupag2.data());
											}
											else
											{
												if (iconsel == 1)
												{
													CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
													CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Prevmenu.data());
												}
												else
												{
													if (iconsel == 6)
													{														
														CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
														if (!boolvars.settingisactive)
															CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Options.data());
														else
															CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Closesets.data());
													}
													else
													{
														if (iconsel == 7)
														{
															CSprite2d::DrawRect(CRect(cursorx - SCREEN_COORD(3.0f), cursory + SCREEN_COORD(22.0f), cursorx + SCREEN_COORD(227.0f), cursory + SCREEN_COORD(46.0f)), CRGBA(239, 228, 176, 255));
															CFont::PrintString(cursorx + SCREEN_COORD(112.0f), cursory + SCREEN_COORD(22.0f), boolvars.Closemenu.data());
														}
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
							if (iconsel == 4)
							{
								AudioLib.PlayMenuSFX(1);
								if (chosen2 != 0)
								{
									chosenmark = chosen2;
									if (settings.folderdirs[boolvars.yndex].suits[chosenmark].texexists == true && settings.folderdirs[boolvars.yndex].suits[chosenmark].mdlexists == true)// && chosenmark >= 8)
									{
										bool banderilla = false;
										int pind = 0;
										while (pind <= dimfriends && banderilla == false)
										{
											if ((int)jarvisfriend[pind].actorchar == 0)
											{
												pedindex = pind;
												banderilla = true;
												break;
											}
											pind++;
										}
										if (banderilla == true && pind <= dimfriends)
										{
											isentered = true;
										}
										ordenelegir = true;
										boolvars.alphafad += 50;
									}
									else
									{
										ordenelegir = true;
										boolvars.alphafad += 50;
									}
									ordenelegir = true;
									boolvars.alphafad += 50;
									iconsel = 0;
								}
							}
							else
							{
								ordenelegir = true;
								boolvars.alphafad += 50;
							}
							iconsel = 0;
						}
					}
					else {
						if (iconsel == 4)
						{
							AudioLib.PlayMenuSFX(1);
							if (strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark01") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark02") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark03") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark04") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark05") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark06") != 0 &&
								strcmp(settings.folderdirs[boolvars.yndex].suits[chosen2].pngname, "mark07") != 0 &&
								chosen2 != 0)
							{
								chosenmark = chosen2;
								if (settings.folderdirs[boolvars.yndex].suits[chosen2].texexists == true &&
									settings.folderdirs[boolvars.yndex].suits[chosen2].mdlexists == true)
								{
									char *numb;
									numb = new char[4];
									sprintf(numb, "%d", chosenmark);
									WritePrivateProfileString("CONFIG", "MARK", numb, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									//Sleep(30);
									iscliqued = true;
									ordenelegir = true;
									boolvars.alphafad += 50;
									markk = GetPrivateProfileInt("CONFIG", "MARK", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
									delete[] numb;
									iconsel = 0;
								}
							}
							else
							{
								chosenmark = chosen2;
								isclicked = true;
								ordenelegir = true;
								boolvars.alphafad += 50;
								iconsel = 0;
							}
						}
						else
						{
							if (iconsel == 5)
							{
								if (CTimer::m_snTimeInMillisecondsNonClipped > (timerforpages + 1000))
								{
									AudioLib.PlayMenuSFX(2);
									timerforpages = CTimer::m_snTimeInMillisecondsNonClipped;
									if (boolvars.yndex >= settings.amount - 1)
									{
										boolvars.yndex = 0;
									}
									else
									{
										boolvars.yndex++;
									}
									JarvisVoice::Loadsuiticons();
								}
								iconsel = 0;
							}
							else
							{
								if (iconsel == 2)
								{
									if (CTimer::m_snTimeInMillisecondsNonClipped > (timerforpages + 1000))
									{
										AudioLib.PlayMenuSFX(5);
										timerforpages = CTimer::m_snTimeInMillisecondsNonClipped;
										if (menupage == 0)
										{
											menupage = 1;
										}
										else
										{
											menupage = 0;
										}
									}
									iconsel = 0;
								}
								else
								{
									if (iconsel == 3)
									{
										if (CTimer::m_snTimeInMillisecondsNonClipped > (timerforpages + 1000))
										{
											AudioLib.PlayMenuSFX(5);
											timerforpages = CTimer::m_snTimeInMillisecondsNonClipped;
											if (menupage == 1)
											{
												menupage = 0;
											}
											else
											{
												menupage = 1;
											}
										}
										iconsel = 0;
									}
									else
									{
										if (iconsel == 1)
										{
											if (CTimer::m_snTimeInMillisecondsNonClipped > (timerforpages + 1000))
											{
												AudioLib.PlayMenuSFX(2);
												timerforpages = CTimer::m_snTimeInMillisecondsNonClipped;
												if (boolvars.yndex <= 0)
												{
													boolvars.yndex = settings.amount - 1;
												}
												else
												{
													boolvars.yndex--;
												}
												JarvisVoice::Loadsuiticons();
											}
											iconsel = 0;
										}
										else
										{
											if (iconsel == 6)
											{
												if (CTimer::m_snTimeInMillisecondsNonClipped > (timerforpages + 1000))
												{
													if (!boolvars.settingisactive)
													{
														AudioLib.PlayMenuSFX(3);
														boolvars.settingisactive = true;
													}
													else
													{
														AudioLib.PlayMenuSFX(4);
														boolvars.settingisactive = false;
													}
													timerforpages = CTimer::m_snTimeInMillisecondsNonClipped;
												}
												iconsel = 0;
											}
											else
											{
												if (iconsel == 7)
												{
													AudioLib.PlayMenuSFX(4);
													boolvars.menuisactive = false;
													WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
													iconsel = 0;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				if (isclicked == true || iscliqued == true)
				{
					Drawfondo(boolvars.alphafad);
				}
				
				return true;
			}
		}
		else
		{
			boolvars.settingisactive = false;
			WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			boolvars.menuisactive = false;
			boolvars.indx = 0;
			boolvars.alphafad = 0;
			
			plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CONTROL, 0, 1);
			return true;
		}
	/*}
	else
	{
		boolvars.settingisactive = false;
		pActor = (DWORD*)0xB6F5F0;
		
		WritePrivateProfileString("CONFIG", "MENU", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
		boolvars.menuisactive = false;
		boolvars.indx = 0;
		return true;
	}*/
	
	return true;
}


//22
aimin:
{
if (*pActor > 0)
{
	if (boolvars.activesuit != 0 && boolvars.activesuit != -5)
	{
		static int escudo;
		RwV3d playerpos;
		if (jarvisvoice.is_aiming() == true)
		{
			if (boolvars.has_a_car == false)
			{
				boolvars.indx = 22;
				//aim(&boolvars.target.x, &boolvars.target.y, &boolvars.target.z);
				/*if (boolvars.wpn == 2 || boolvars.wpn == 4)
				{

					if (isvulnerablecharinarea(player, boolvars.target.x, boolvars.target.y, boolvars.target.z, 10.0f) == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "aim_MoveFWD") == false &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "aim_MoveBWD") == false &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "aim_MoveL") == false &&
							plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "aim_MoveR") == false

							)
						{
							CFont::SetBackground(0, 0);
							CFont::SetOrientation(ALIGN_CENTER);
							CFont::SetProportional(true);
							CFont::SetJustify(false);
							CFont::SetFontStyle(FONT_MENU);
							CFont::SetColor(CRGBA(255, 0, 0, 255));
							CFont::SetScale(SCREEN_MULTIPLIER(0.8f), SCREEN_MULTIPLIER(1.5f));
							CFont::PrintString(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_CENTER_UP(0.0f), boolvars.distan.data());
						}
					}
				}*/

				if (jarvisvoice.is_over_ground(2.0f) == true)
				{
					if (boolvars.activesuit == -4)
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						boolvars.indx = 0;
						return true;
					}

					boolvars.range = 150.0f;

					if (jarvisvoice.nosemueve2() == true)
					{
						if (boolvars.aims == false && boolvars.punchedtargetsexist == false)
						{
							boolvars.aimedpeds[0] = 0;
							boolvars.aimedpeds[1] = 0;
							boolvars.aimedpeds[2] = 0;
							boolvars.aimedpeds[3] = 0;
							boolvars.aimedpeds[4] = 0;
							if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true || isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								JarvisVoice::storeenviroment(&propellers.enviroment_ID);
								boolvars.thrusterskilled = true;

							}
							boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
							boolvars.aims = true;
						}
						if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 14) == false)
						{
							
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							boolvars.iscjfrozen = true;
							movtextures.drawindexedtexture(10);
							if (boolvars.escudoactivo == true && escudo != 0)
							{
								//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
								plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, escudo);
								plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, escudo);
								plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic);
								plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 1, 1, 0);
								vehic = 0; 
								escudo = 0;
								boolvars.escudoactivo = false;
								return true;
							}
							if (boolvars.wpn == 4)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ChestIDLE", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							else
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ArmIDLE", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}

							//player->SetHeading(jarvisvoice.newanglepos());
							//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_ROTATION, player, jarvisvoice.GetXAngle() * 180.0f / 3.1415926535897932384f, 0.0f, jarvisvoice.newanglepos() * 180.0f / 3.1415926535897932384f);
							//player->SetOrientation(jarvisvoice.GetXAngle(), 0.0f, jarvisvoice.newanglepos());
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());


							if (boolvars.wpn == 4 && ruidito != 1) {
								JarvisVoice::PlayAudiostream(audiostream[15], 0, &IMStream[2]);
								ruidito = 1;
							}
							if (boolvars.wpn != 4 && ruidito != 2) {
								JarvisVoice::PlayAudiostream(audiostream[repulsorstart], 0, &IMStream[2]);
								ruidito = 2;
							}

							if (jarvisvoice.Has_not_switched_enviroment(&propellers))
							{
								if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == false) {
									JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 0, apuntar, &propellers);

								}
							}
							else
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);


							}
							goto shootin;
						}
						else
						{
							if (jarvisvoice.Has_not_switched_enviroment(&propellers))
							{
								if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == false) {
									JarvisVoice::PlayThrustersIDLE(NULL, NULL, &IMStream[1], (int)player, settings.folderdirs[boolvars.pageofsuit].suits[markk], 0, apuntar, &propellers);

								}
							}
							else
							{
								JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
								boolvars.thrusterskilled = true;

								JarvisVoice::storeenviroment(&propellers.enviroment_ID);


							}
							static bool atached;
							if (!boolvars.escudoactivo)
							{
								//player->GetBonePosition(playerpos, 34, false);
								boolvars.escudoactivo = true;
								atached = false;
								boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
								boolvars.timetowait = 500;
								plugin::scripting::CallCommandById(COMMAND_CREATE_CAR, 594, 0.0f, 0.0f, 0.0f, &vehic);
								plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 1224, 0.0f, 0.0f, 0.0f, &escudo);
								
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, escudo, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, escudo, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, escudo, 0.001f);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, escudo, 0);
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);// 4.0f, 0, 0, 0, 1, -1);

								return true;
							}
							if (!atached)
							{
								plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_OBJECT, vehic, escudo, 0.0f, 0.35f, 0.0f, 80.0f, 0.0f, 180.0f);

								plugin::scripting::CallCommandById(COMMAND_ATTACH_OBJECT_TO_CHAR, escudo, player, 0.9f, 0.0f, 0.0f, 0.0f, 0.0f, 12.5f);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, escudo, 0.01f);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, escudo, 0);
								//plugin::scripting::CallCommandById(COMMAND_TASK_PICK_UP_OBJECT, player, escudo, 0.0f, 0.0f, 0.0f, 5, 17, "NULL", "NULL", 1);
								plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, vehic, 1, 1, 1, 1, 1);
								
								plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 0);
								plugin::scripting::CallCommandById(COMMAND_SET_VEHICLE_RECORDS_COLLISIONS, vehic, 0);
								atached = true;
								plugin::scripting::CallCommandById(COMMAND_GIVE_VEHICLE_PAINTJOB, vehic, 0);
							}
							if (boolvars.updateshieldnow == true)
							{
								plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, escudo);
								plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, escudo);
								plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic);
								plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
								vehic = 0;
								escudo = 0;
								boolvars.escudoactivo = false;
								boolvars.updateshieldnow = false;
								plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);// 4.0f, 0, 0, 0, 1, -1);

								return true;
							}
							if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == true ||
								plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 7) == true)
							{
								if (boolvars.shield12 != 1) {
									boolvars.shield12 = 1;
								}
								else
								{
									boolvars.shield12 = 0;
								}
							}
							if (boolvars.shield12 == 0)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_VEHICLE_PAINTJOB, vehic, 0);
							}
							if (boolvars.shield12 == 1)
							{
								plugin::scripting::CallCommandById(COMMAND_GIVE_VEHICLE_PAINTJOB, vehic, -1);
								//boolvars.updateshieldnow = true;
							}
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1); //4.0f, 0, 0, 0, 1, -1);
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS, escudo, 0.06f, 0.0f, 0.0f, &playerpos.x, &playerpos.y, &playerpos.z);
							plugin::scripting::CallCommandById(COMMAND_DRAW_WEAPONSHOP_CORONA, playerpos.x, playerpos.y, playerpos.z, 1.0f, 3, 1.5f, 0, 100, 255);
							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 1, 1, 1, 1, 1);
							return true;
						}
					}
					else
					{
						//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);

						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
						boolvars.indx = 19;
						return true;
					}
					return true;
				}
				else
				{

					boolvars.range = 75.0f;
					if (boolvars.mousewheelhacktrigger == true) {
						spd = 200.0f;
						UnhookWindowsHookEx(mousehook);
						jarvisvoice.SetVelocityUnlimited(false);
						plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
						boolvars.mousewheelhacktrigger = false;
					}
					if (boolvars.aims == false && boolvars.punchedtargetsexist == false)
					{
						boolvars.aimedpeds[0] = 0;
						boolvars.aimedpeds[1] = 0;
						boolvars.aimedpeds[2] = 0;
						boolvars.aimedpeds[3] = 0;
						boolvars.aimedpeds[4] = 0;
						if (isplayingfly(true, IMStream[1], 0, propellers.enviroment_ID, NULL, NULL) == true || isplayingfly(true, IMStream[1], 1, propellers.enviroment_ID, NULL, NULL) == true) {
							JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
							boolvars.thrusterskilled = true;
							JarvisVoice::storeenviroment(&propellers.enviroment_ID);
						}
						boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
						boolvars.aims = true;
					}
					if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 14) == false)
					{						
						movtextures.drawindexedtexture(10);
						if (boolvars.escudoactivo == true && escudo != 0)
						{
							//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, escudo);
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, escudo);
							plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic);
							plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);

							plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 1, 1, 0);
							vehic = 0;
							escudo = 0;
							boolvars.escudoactivo = false;
							
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "Aim_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);// 4.0f, 0, 0, 0, 1, -1);

							return true;
						}
						if (CMouseControllerState().lmb != 1 && (GetKeyState(VK_LBUTTON) & 0x8000) == false && plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) == false)
						{
							boolvars.isshootin = false;
							if (jarvisvoice.Va_adelante() == true)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_MoveFWD", "ironman", 5.0f, 0, 1, 1, 0, -1);
								//player->SetHeading(jarvisvoice.newanglepos());
								plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
							}
							else
							{
								if (jarvisvoice.Va_atras() == true)
								{
									plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_MoveBWD", "ironman", 5.0f, 0, 1, 1, 0, -1);
									//player->SetHeading(jarvisvoice.newanglepos());
									plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
								}
								else {
									if (jarvisvoice.Va_izquierda() == true)
									{
										plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_MoveL", "ironman", 5.0f, 0, 1, 1, 0, -1);
										//player->SetHeading(jarvisvoice.newanglepos());
										plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
									}
									else {
										if (jarvisvoice.Va_derecha() == true)
										{
											plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_MoveR", "ironman", 5.0f, 0, 1, 1, 0, -1);
											//player->SetHeading(jarvisvoice.newanglepos());
											plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());
										}
										else {
											if (boolvars.wpn != 4)
											{
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_ArmIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);
											}
											else
											{
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_ChestIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);
											}
										}
									}
								}
							}
						}
						else {
							if (boolvars.wpn != 4)
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_ArmIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);
							}
							else
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_ChestIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);
							}
						}
						//player->SetHeading(jarvisvoice.newanglepos());
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_HEADING, player, jarvisvoice.newangle2());

						if (boolvars.wpn == 4 && ruidito != 1) {
							JarvisVoice::PlayAudiostream(audiostream[15], 0, &IMStream[2]);
							ruidito = 1;
						}
						if (boolvars.wpn != 4 && ruidito != 2) {
							JarvisVoice::PlayAudiostream(audiostream[repulsorstart], 0, &IMStream[2]);
							ruidito = 2;
						}
						goto shootin;
					}
					else
					{
						
						static bool atached;
						if (!boolvars.escudoactivo)
						{
							//player->GetBonePosition(playerpos, 34, false);
							boolvars.escudoactivo = true;
							atached = false;
							boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
							boolvars.timetowait = 500;
							plugin::scripting::CallCommandById(COMMAND_CREATE_CAR, 594, 0.0f, 0.0f, 0.0f, &vehic);
							plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 1224, 0.0f, 0.0f, 0.0f, &escudo);

							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, escudo, 0);
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, escudo, 0);
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, escudo, 0.001f);
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, escudo, 0);

							plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 0);
							plugin::scripting::CallCommandById(COMMAND_SET_VEHICLE_RECORDS_COLLISIONS, vehic, 0);
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "Aim_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);// 4.0f, 0, 0, 0, 1, -1);

							return true;
						}
						if (!atached)
						{
							plugin::scripting::CallCommandById(COMMAND_ATTACH_CAR_TO_OBJECT, vehic, escudo, 0.0f, 0.55f, 0.0f, 80.0f, 0.0f, 180.0f);

							//plugin::scripting::CallCommandById(COMMAND_TASK_PICK_UP_OBJECT, player, escudo, 0.0f, 0.0f, 0.0f, 5, 17, "NULL", "NULL", 1);
							plugin::scripting::CallCommandById(COMMAND_ATTACH_OBJECT_TO_CHAR, escudo, player, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_SCALE, escudo, 0.01f);
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, escudo, 0);

							plugin::scripting::CallCommandById(COMMAND_SET_CAR_PROOFS, vehic, 1, 1, 1, 1, 1);

							plugin::scripting::CallCommandById(COMMAND_SET_CAR_COLLISION, vehic, 0);
							plugin::scripting::CallCommandById(COMMAND_SET_VEHICLE_RECORDS_COLLISIONS, vehic, 0);
							plugin::scripting::CallCommandById(COMMAND_GIVE_VEHICLE_PAINTJOB, vehic, 0);
							atached = true;
						}
						if (boolvars.updateshieldnow == true)
						{
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, escudo);
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, escudo);
							plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic);
							plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
							
							vehic = 0; 
							escudo = 0;
							boolvars.escudoactivo = false;
							boolvars.updateshieldnow = false;
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "Aim_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);// 4.0f, 0, 0, 0, 1, -1);

							return true;
						}
						if (plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5) == true ||
							plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 7) == true)
						{
							if (boolvars.shield12 != 1) {
								boolvars.shield12 = 1;
							}
							else
							{
								boolvars.shield12 = 0;
							}
						}

						if (boolvars.shield12 == 0)
						{
							plugin::scripting::CallCommandById(COMMAND_GIVE_VEHICLE_PAINTJOB, vehic, 0);							
						}
						if (boolvars.shield12 == 1)
						{
							plugin::scripting::CallCommandById(COMMAND_GIVE_VEHICLE_PAINTJOB, vehic, -1);
							//boolvars.updateshieldnow = true;
						}
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "Aim_ShieldIDLE", "ironman", 5.0f, 1, 1, 1, 0, -1);// 4.0f, 0, 0, 0, 1, -1);
						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS, escudo, 0.06f, 0.0f, 0.0f, &playerpos.x, &playerpos.y, &playerpos.z);
						plugin::scripting::CallCommandById(COMMAND_DRAW_WEAPONSHOP_CORONA, playerpos.x, playerpos.y, playerpos.z, 1.0f, 3, 1.5f, 0, 100, 255);
						plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 1, 1, 1, 1, 1);
						return true;
					}
				}
			}
			else
			{
				star = false;
				return true;
			}
		}
		else {
			if (boolvars.escudoactivo == true && escudo != 0)
			{
				//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
				plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, escudo);
				plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, escudo);
				plugin::scripting::CallCommandById(COMMAND_DELETE_CAR, vehic);
				plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, vehic);
				plugin::scripting::CallCommandById(COMMAND_SET_CHAR_PROOFS, player, 0, 0, 1, 1, 0);
				vehic = 0;
				escudo = 0;
				boolvars.escudoactivo = false;
			}
			star = false;
			AimedPedReactiontwo(&boolvars.ped);
			if (jarvisvoice.is_over_ground(2.0f) == true)
			{
				boolvars.indx = 19;

				return true;
			}
			else {
				boolvars.indx = 0;
				plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
				goto notaiminonground;
			}
		}
	}
	else
	{
		boolvars.indx = 0;
		plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
		if (jarvisvoice.is_over_ground(2.0f) == true)
		{
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
		}		
		return true;
	}
}
else
{
	pActor = (DWORD*)0xB6F5F0;
	boolvars.indx = 0;
	plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
	return true;
}
return true;
}

shootin:
{
	if (jarvisvoice.is_over_ground(2.0f) == true)
	{
		if (boolvars.activesuit == 0 || boolvars.activesuit == -4)
		{
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS, player);
			plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
			boolvars.indx = 0;
			return true;
		}
	}
	
	if (CMouseControllerState().lmb != 1 && (GetKeyState(VK_LBUTTON) & 0x8000) == false && plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) == false)
	{
		boolvars.isshootin = false;
		if (jarvisvoice.is_over_ground(2.0f) == false && boolvars.iscjfrozen == true) {
			plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 0);
			boolvars.iscjfrozen = false;
		}
		star = false;
		boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
		if (boolvars.wpn == 5 || boolvars.wpn == 3)
		{
			if (has_not_switched_weapon(boolvars.wpn))
			{
				*boolvars.aimedpeds = *jarvisvoice.Mark_targets(boolvars.wpn, boolvars.currenttarg, boolvars.aimedpeds);
				boolvars.currenttarg = 0;
			}
			else
			{
				boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
				boolvars.aimedpeds[0] = 0;
				boolvars.aimedpeds[1] = 0;
				boolvars.aimedpeds[2] = 0;
				boolvars.aimedpeds[3] = 0;
				boolvars.aimedpeds[4] = 0;
			}
			jarvisvoice.Display_targets_on_screen(boolvars.wpn, boolvars.aimedpeds, imhud2, imhud3);

		}

		if (boolvars.wpn == 0)
		{
			if (has_not_switched_weapon(boolvars.wpn))
			{
				*boolvars.aimedpeds = *jarvisvoice.Mark_targets(boolvars.wpn, boolvars.currenttarg, boolvars.aimedpeds);
				boolvars.currenttarg = 0;
			}
			else
			{
				boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
				boolvars.aimedpeds[0] = 0;
				boolvars.aimedpeds[1] = 0;
				boolvars.aimedpeds[2] = 0;
				boolvars.aimedpeds[3] = 0;
				boolvars.aimedpeds[4] = 0;
			}
			jarvisvoice.Display_targets_on_screen(boolvars.wpn, boolvars.aimedpeds, imhud2, imhud3);

		}
		return true;
	}
	else
	{


		boolvars.isshootin = true;
		if (boolvars.iscjfrozen == false) {
			plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, player, 1);
			boolvars.iscjfrozen = true;
		}
		if (boolvars.wpn == 5 || boolvars.wpn == 3)
		{
			star = false;
			if (boolvars.wpn == 5) {
				AimedPedReactiontwo(&boolvars.ped);
			}
			boolvars.killedtargetsexist = true;
			return true;
		}
		else
		{
			if (boolvars.wpn == 1)
			{
				star = false;
				JarvisVoice::PlayShot(&IMStream[2]);
				RwV3d coords;
				player->GetBonePosition(coords, 34, false);
				plugin::scripting::CallCommandById(COMMAND_ADD_BIG_GUN_FLASH, coords.x, coords.y, coords.z, boolvars.target.x, boolvars.target.y, boolvars.target.z);
				for (int i = 0; i <= 2; i++)
				{
					plugin::scripting::CallCommandById(COMMAND_FIRE_SINGLE_BULLET, coords.x, coords.y, coords.z, boolvars.target.x, boolvars.target.y, boolvars.target.z, 17);
				}
				//Sleep(30);
				boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
				boolvars.timetowait = 30;
				return true;
			}
			else {
				if (boolvars.wpn == 2)
				{
					boolvars.action = 4;
					static int time;
					RwV3d coords;
					player->GetBonePosition(coords, 34, false);
					if (!star)
					{
						if (JarvisVoice::IsAudioStreamPlayed(IMStream[2], audiostream[repulsorblast]) == false)
						{
							JarvisVoice::PlayAudiostream(audiostream[repulsorblast], 0, &IMStream[2]);
							time = 0;
						}

						for (int i = 0; i <= 2; i++)
						{
							plugin::scripting::CallCommandById(COMMAND_CREATE_SEARCHLIGHT, coords.x, coords.y, coords.z, boolvars.target.x, boolvars.target.y, boolvars.target.z, 0.045, 0.045, &ibeam[i]);
						}
						star = true;
						time = CTimer::m_snTimeInMillisecondsNonClipped;

					}
					//for (int i = 0; i <= 2; i++)
					//{
					plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION_NO_SOUND, boolvars.target.x, boolvars.target.y, boolvars.target.z, 8);

					//}
					if (CTimer::m_snTimeInMillisecondsNonClipped >= (time + 35))
					{
						for (int i = 0; i <= 2; i++)
						{
							plugin::scripting::CallCommandById(COMMAND_CREATE_SEARCHLIGHT, coords.x, coords.y, coords.z, boolvars.target.x, boolvars.target.y, boolvars.target.z, 0.045, 0.045, &ibeam[i]);

						}
					}

					return true;
				}
				else
				{
					if (boolvars.wpn == 4)
					{
						boolvars.action = 4;
						static int time;
						RwV3d coords;
						coords.x = 0.185f;
						coords.y = 0.08f;
						coords.z = 0.0f;
						player->GetTransformedBonePosition(coords, 3, false);
						if (!star)
						{
							if (JarvisVoice::IsAudioStreamPlayed(IMStream[0], audiostream[14]) == false)
							{
								JarvisVoice::PlayAudiostream(audiostream[14], 0, &IMStream[0]);
								time = 0;
							}
							for (int i = 0; i <= 2; i++)
							{
								plugin::scripting::CallCommandById(COMMAND_CREATE_SEARCHLIGHT, coords.x, coords.y, coords.z, boolvars.target.x, boolvars.target.y, boolvars.target.z, 0.120, 0.120, &ibeam[i]);
							}

							star = true;
							time = CTimer::m_snTimeInMillisecondsNonClipped;

						}
						//for (int i = 0; i <= 2; i++)
						//{
						plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION_NO_SOUND, boolvars.target.x, boolvars.target.y, boolvars.target.z, 8);

						//IF IRON MAN IS AIMIN WEAPON 4 (CHEST BEAM) WE START THE LOOP OF UNIBEAM SOUND
						Playuniloop(1);
						//}
						if (CTimer::m_snTimeInMillisecondsNonClipped >= (time + 35))
						{
							if (jarvisvoice.is_over_ground(2.0f) == true) {
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ChestIDLE", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							else
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_ChestIDLE", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							for (int i = 0; i <= 2; i++)
							{
								plugin::scripting::CallCommandById(COMMAND_CREATE_SEARCHLIGHT, coords.x, coords.y, coords.z, boolvars.target.x, boolvars.target.y, boolvars.target.z, 0.120, 0.120, &ibeam[i]);

							}
						}
						else {
							if (jarvisvoice.is_over_ground(2.0f) == true) {
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "fly_ChestIDLE", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
							else
							{
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, player, "aim_ChestIDLE", "ironman", 10.0f, 0, 0, 0, 0, -1);
							}
						}
					}
					else
					{
						if (boolvars.wpn == 0 &&
							boolvars.suit[3] == true)
						{
							star = false;
							boolvars.punchedtargetsexist = true;
							boolvars.indx = 0;
						}
					}
					return true;
				}
			}
		}

	}
	return true;
}

notaiminonground:
{
static int controller;
	plugin::scripting::CallCommandById(COMMAND_GET_CONTROLLER_MODE, &controller);
	if (controller == 1) {

		if (CMouseControllerState().rmb == 1 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 5))
		{
				boolvars.indx = 22;
				return true;
			
		}
		else {
			if (boolvars.mousewheelhacktrigger == true) {
				spd = 200.0f;
				UnhookWindowsHookEx(mousehook);
				jarvisvoice.SetVelocityUnlimited(false);
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
				boolvars.mousewheelhacktrigger = false;
			}
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

			boolvars.indx = 0;
			return true;
		}
	}
	else
	{
		if (CMouseControllerState().rmb == 1 || plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 6))
		{
				boolvars.indx = 22;
				return true;
			
		}
		else {
			if (boolvars.mousewheelhacktrigger == true) {
				spd = 200.0f;
				UnhookWindowsHookEx(mousehook);
				jarvisvoice.SetVelocityUnlimited(false);
				plugin::scripting::CallCommandById(COMMAND_SET_PLAYER_CYCLE_WEAPON_BUTTON, 0, 1);
				boolvars.mousewheelhacktrigger = false;
			}
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, player);

			boolvars.indx = 0;
			return true;
		}
	}
}

endin:
	{

	}
	return true;
}

static void useweaponwithtarget(Tjarvisbot *random);

void useweaponwithtarget(Tjarvisbot *random)
{
	CPed *player = FindPlayerPed(0);
	if ((int)random->actorchar != NULL && (int)random->target.actorchar != NULL)
	{
		CVector targetpos;
		plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, random->target.actorchar, &targetpos.x, &targetpos.y, &targetpos.z);

		if (random->wpn == 2)
		{
			for (int y = 0; y < dimenemy; y++)
			{
				if (jarvisenemy[y].actorchar == random->target.actorchar)
				{
					jarvisenemy[y].damagetimer = CTimer::m_snTimeInMillisecondsNonClipped;
					break;
				}
			}
			for (int j = 0; j < dimfriends; j++)
			{
				if (jarvisfriend[j].actorchar == random->target.actorchar)
				{
					jarvisfriend[j].damagetimer = CTimer::m_snTimeInMillisecondsNonClipped;
					break;
				}
			}
		}

		if (random->wpn == 1)
		{
			if (random->target.timer + 20 > CTimer::m_snTimeInMillisecondsNonClipped)
			{
				random->target.timer = CTimer::m_snTimeInMillisecondsNonClipped;
				random->raysactive = false;
				JarvisVoice::PlayShot(&random->botstream[1]);
				RwV3d coords;
				random->actorchar->GetBonePosition(coords, 34, false);
				plugin::scripting::CallCommandById(COMMAND_ADD_BIG_GUN_FLASH, coords.x, coords.y, coords.z, targetpos.x, targetpos.y, targetpos.z);
				for (int i = 0; i <= 2; i++)
				{
					plugin::scripting::CallCommandById(COMMAND_FIRE_SINGLE_BULLET, coords.x, coords.y, coords.z, targetpos.x, targetpos.y, targetpos.z, 17);
				}
				//Sleep(30);

				boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
				boolvars.timetowait = 30;
			}
		}
		else
		{
			if (random->wpn == 2)
			{
				if (random->target.timer + 1000 < CTimer::m_snTimeInMillisecondsNonClipped)
				{
					random->target.timer = CTimer::m_snTimeInMillisecondsNonClipped;
					boolvars.action = 5;
					random->yolohice = true;
					RwV3d coords;
					random->actorchar->GetBonePosition(coords, 34, false);
					if (!random->raysactive)
					{
						if (JarvisVoice::IsAudioStreamPlayed(random->botstream[1], audiostream[3]) == false)
						{
							JarvisVoice::PlayAudiostream(random->audios[3], 1, &random->botstream[1]);
							random->raystime = 0;
						}

						for (int i = 0; i <= 2; i++)
						{
							plugin::scripting::CallCommandById(COMMAND_CREATE_SEARCHLIGHT, coords.x, coords.y, coords.z, targetpos.x, targetpos.y, targetpos.z, 0.045, 0.045, &random->ibeam[i]);
						}
						random->raysactive = true;
						random->raystime = CTimer::m_snTimeInMillisecondsNonClipped;

					}
					for (int i = 0; i <= 2; i++)
					{
						plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION_NO_SOUND, targetpos.x, targetpos.y, targetpos.z, 8);
					}
					if (CTimer::m_snTimeInMillisecondsNonClipped >= (random->raystime + 35))
					{
						for (int i = 0; i <= 2; i++)
						{
							if (random->ibeam[i] == NULL)
							{
								plugin::scripting::CallCommandById(COMMAND_CREATE_SEARCHLIGHT, coords.x, coords.y, coords.z, targetpos.x, targetpos.y, targetpos.z, 0.045, 0.045, &random->ibeam[i]);
							}
						}
					}

					if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, targetpos.x, targetpos.y, targetpos.z, 10.0f, 10.0f, 10.0f, 0) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, random->actorchar, &boolvars.anglecohete);
						boolvars.cohetebotexplotado = true;

						static float hadin;
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

						if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
							boolvars.vuelaatras = true;
						else
							boolvars.vuelaatras = false;

						boolvars.landgetup = 1;
					}
				}
			}
			else
			{
				if (random->cohete.misil == NULL || random->complete == true)
				{
					JarvisVoice::PlayAudiostream(random->audios[4], 1, &random->botstream[1]);
					RwV3d playerpos;
					random->actorchar->GetBonePosition(playerpos, 34, false);
					plugin::scripting::CallCommandById(COMMAND_CREATE_OBJECT, 345, playerpos.x, playerpos.y, playerpos.z, &random->cohete.misil);
					
					objectlist[dim] = random->cohete.misil;
					dim++;

					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COLLISION, random->cohete.misil, 1);
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_RECORDS_COLLISIONS, random->cohete.misil, 0);
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_TURN_MASS, random->cohete.misil, 0.0f);
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, random->cohete.misil, 1);


					random->cohete.timecreated = CTimer::m_snTimeInMillisecondsNonClipped;
					//Sleep(50);
					boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
					boolvars.timetowait = 50;
					CVector dir, speeed;
					CVector origin;
					random->cohete.objective = random->target.actorchar;
					plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, random->cohete.misil, &origin.x, &origin.y, &origin.z);

					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, random->target.actorchar, &dir.x, &dir.y, &dir.z);
					float fx = 0.0f;
					plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, origin.x, origin.y, dir.x, dir.y, &fx);
					float fy = dir.z - origin.z;

					float xangle = (CGeneral::GetATanOfXY(fy, fx));

					float headx = dir.x - origin.x;
					float heady = dir.y - origin.y;
					float head;
					plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, headx, heady, &head);
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, random->cohete.misil, xangle, 2, head);
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, random->cohete.misil, head);
					speeed.x = (dir.x - origin.x)*10.0f;
					speeed.y = (dir.y - origin.y)*10.0f;
					speeed.z = (dir.z - origin.z)*10.0f;
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, random->cohete.misil, speeed.x, speeed.y, speeed.z);
					random->cohete.velprevx = speeed.x;
					random->cohete.velprevy = speeed.y;
					random->cohete.velprevz = speeed.z;
					random->complete = false;
				}
			}
		}
	}
}

static float mod(float base);

float mod(float base)
{
	if (base < 0.0f)
	{
		return -base;
	}
	else
	{
		return base;
	}
}

bool JarvisVoice::jarvisbotpowers() {
	/*if (CTimer::m_snTimeInMillisecondsNonClipped < boolvars.waiter + boolvars.timetowait)
	{
		return true;
	}*/
	static bool once1;
	static int level1=0;
	static bool isfaraway;
	static DWORD *pActor;
	static DWORD *pCamera;
	static int codigopropulsor;
	int i = 0;
	static bool repulsor = false;
	CPed *player = FindPlayerPed(0);
	if (!once1)
	{
		pActor = (DWORD*)0xB6F5F0;
		pCamera = (DWORD*)0xB6F99C;
		once1 = true;
	}

	if (*pActor > 0 && player)
	{
		plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level1);
		i = 0;
		//aqui hay error o justo despues de un STORE_WANTED_LEVEL
		while (i < dimenemy)
		{
			for (int u = 0; u <= 2; u++)
			{
				if (jarvisenemy[i].ibeam[u] != NULL)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_SEARCHLIGHT_EXIST, jarvisenemy[i].ibeam[u]) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_DELETE_SEARCHLIGHT, jarvisenemy[i].ibeam[u]);
						jarvisenemy[i].ibeam[u] = NULL;
					}
				}
			}
			if (jarvisenemy[i].cohete.misil != NULL && jarvisenemy[i].complete == false)
			{
				botshootsmissile(&jarvisenemy[i].cohete, &jarvisenemy[i].complete);
			}
			if ((int)jarvisenemy[i].actorchar != NULL && jarvisenemy[i].actorchar != player)
			{
				if (jarvisenemy[i].damagetimer + 500 < CTimer::m_snTimeInMillisecondsNonClipped)
				{
					if (CTimer::m_snTimeInMillisecondsNonClipped > jarvisenemy[i].timetoact + 500)
					{
						jarvisenemy[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
						jarvisenemy[i].wpn = 1 + rand() % 3;
						if (dimfriends > 0)
						{
							int allyindex = rand() % dimfriends;
							int helper = jarvisfriend[allyindex].actorid;
							if (helper > NULL && (int)jarvisenemy[i].target.actorchar != helper)
							{
								CPed *helpw;
								helpw = CPools::GetPed(helper);
								if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, helpw) == true && CPools::ms_pPedPool->IsObjectValid(helpw))
								{
									jarvisenemy[i].target.actorid = helper;
									jarvisenemy[i].target.actorchar = helpw;
									jarvisenemy[i].target.timer = NULL;
									jarvisfriend[allyindex].attacker.actorchar = jarvisenemy[i].actorchar;
									jarvisfriend[allyindex].attacker.actorid = jarvisenemy[i].actorid;
									CVector poss, poss1, poss2;
									plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[allyindex].actorchar, 0.0f, 0.0f, 0.0f, &poss.x, &poss.y, &poss.z);
									plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[i].actorchar, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);

									poss2.x = (poss.x - poss1.x)*2.1f;
									poss2.y = (poss.y - poss1.y)*2.1f;
									poss2.z = (poss.z - poss1.z)*2.1f;
									jarvisfriend[allyindex].velx = poss2.x;
									jarvisfriend[allyindex].vely = poss2.y;
									jarvisfriend[allyindex].velz = poss2.z;
								}
							}
							else
							{
								jarvisenemy[i].target.actorchar = FindPlayerPed(0);
								jarvisenemy[i].target.actorid = int(player);
								jarvisenemy[i].target.timer = NULL;
							}
						}
						else
						{
							jarvisenemy[i].target.actorchar = FindPlayerPed(0);
							jarvisenemy[i].target.actorid = int(player);
							jarvisenemy[i].target.timer = NULL;
						}
					}
					CVector dist, dist2, dist3, origin;
					int actorhp;
					if ((int)jarvisenemy[i].target.actorchar == NULL)
					{
						jarvisenemy[i].target.actorchar = FindPlayerPed(0);
						jarvisenemy[i].target.actorid = int(player);
						jarvisenemy[i].target.timer = NULL;
					}
					plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].baseobj, &dist.x, &dist.y, &dist.z);
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, jarvisenemy[i].target.actorchar, &dist2.x, &dist2.y, &dist2.z);
					plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[i].target.actorchar, 0.0f, 10.0f, 0.0f, &origin.x, &origin.y, &origin.z);

					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, jarvisenemy[i].actorchar, &actorhp);
					if (actorhp <= 1 || jarvisenemy[i].actorchar->m_fHealth <= 1)
					{
						static float angleobj;

						if (jarvisenemy[i].cohete.misil != NULL)
						{
							plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_HEADING, jarvisenemy[i].cohete.misil, &angleobj);
							CVector misilpos;
							plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].cohete.misil, &misilpos.x, &misilpos.y, &misilpos.z);
							plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, misilpos.x, misilpos.y, misilpos.z, 10);
							
							if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, misilpos.x, misilpos.y, misilpos.z, 10.0f, 10.0f, 10.0f, 0) == true)
							{
								boolvars.anglecohete = angleobj;
								boolvars.cohetebotexplotado = true;

								static float hadin;
								plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

								if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
									boolvars.vuelaatras = true;
								else
									boolvars.vuelaatras = false;

								boolvars.landgetup = 1;
							}
							plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, jarvisenemy[i].cohete.misil);
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, jarvisenemy[i].cohete.misil);
							jarvisenemy[i].cohete.objective = NULL;
							jarvisenemy[i].cohete.timecreated = 0;
							jarvisenemy[i].cohete.misil = NULL;

						}
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, jarvisenemy[i].actorchar, &angleobj);
						plugin::scripting::CallCommandById(COMMAND_REMOVE_BLIP, jarvisenemy[i].blip);
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, jarvisenemy[i].actorchar, &dist.x, &dist.y, &dist.z);
						plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, dist.x, dist.y, dist.z, 10);
						if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, dist.x, dist.y, dist.z, 10.0f, 10.0f, 10.0f, 0) == true)
						{
							CVector2D explhead;

							explhead.x = player->GetPosition().x - dist.x;
							explhead.y = player->GetPosition().y - dist.y;

							plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &boolvars.anglecohete);


							boolvars.cohetebotexplotado = true;

							static float hadin;
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

							if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
								boolvars.vuelaatras = true;
							else
								boolvars.vuelaatras = false;

							boolvars.landgetup = 1;
						}
						//enemigo explotado
						plugin::scripting::CallCommandById(COMMAND_DELETE_CHAR, jarvisenemy[i].actorchar);
						plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, jarvisenemy[i].baseobj);
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, jarvisenemy[i].baseobj);
						plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisenemy[i].actorchar);
						jarvisenemy[i].actorid = 0;
						jarvisenemy[i].target.actorchar = NULL;
						jarvisenemy[i].actorchar = NULL;
						jarvisenemy[i].isfrozen = false;

						int b = i;
						while (b < dimenemy - 1)
						{
							pageenemy[b] = pageenemy[b + 1];
							jarvisenemy[b].baseobj = jarvisenemy[b + 1].baseobj;
							jarvisenemy[b].thrusterscode = jarvisenemy[b + 1].thrusterscode;
							jarvisenemy[b].velx = jarvisenemy[b + 1].velx;
							jarvisenemy[b].vely = jarvisenemy[b + 1].vely;
							jarvisenemy[b].velz = jarvisenemy[b + 1].velz;
							jarvisenemy[b].target.actorchar = jarvisenemy[b + 1].target.actorchar;
							jarvisenemy[b].target.actorid = jarvisenemy[b + 1].target.actorid;
							jarvisenemy[b].target.timer = jarvisenemy[b + 1].target.timer;
							jarvisenemy[b].audios[0] = jarvisenemy[b + 1].audios[0];
							jarvisenemy[b].audios[1] = jarvisenemy[b + 1].audios[1];
							jarvisenemy[b].audios[2] = jarvisenemy[b + 1].audios[2];
							jarvisenemy[b].audios[3] = jarvisenemy[b + 1].audios[3];
							jarvisenemy[b].audios[4] = jarvisenemy[b + 1].audios[4];
							jarvisenemy[b].audios[5] = jarvisenemy[b + 1].audios[5];
							jarvisenemy[b].audios[6] = jarvisenemy[b + 1].audios[6];
							jarvisenemy[b].botstream[0] = jarvisenemy[b + 1].botstream[0];
							jarvisenemy[b].botstream[1] = jarvisenemy[b + 1].botstream[1];
							jarvisenemy[b].botstream[2] = jarvisenemy[b + 1].botstream[2];
							jarvisenemy[b].mark = jarvisenemy[b + 1].mark;
							jarvisenemy[b].raystime = jarvisenemy[b + 1].raystime;
							jarvisenemy[b].yolohice = jarvisenemy[b + 1].yolohice;
							jarvisenemy[b].ibeam[0] = jarvisenemy[b + 1].ibeam[0];
							jarvisenemy[b].ibeam[1] = jarvisenemy[b + 1].ibeam[1];
							jarvisenemy[b].ibeam[2] = jarvisenemy[b + 1].ibeam[2];
							jarvisenemy[b].hasacar = jarvisenemy[b + 1].hasacar;
							jarvisenemy[b].blip = jarvisenemy[b + 1].blip;
							jarvisenemy[b].raysactive = jarvisenemy[b + 1].raysactive;
							jarvisenemy[b].taskindex = jarvisenemy[b + 1].taskindex;
							jarvisenemy[b].hp = jarvisenemy[b + 1].hp;
							jarvisenemy[b].wpn = jarvisenemy[b + 1].wpn;
							jarvisenemy[b].actorid = jarvisenemy[b + 1].actorid;
							jarvisenemy[b].actorchar = jarvisenemy[b + 1].actorchar;
							jarvisenemy[b].timetoact = jarvisenemy[b + 1].timetoact;
							jarvisenemy[b].jarvisprop.manoizqfx = jarvisenemy[b + 1].jarvisprop.manoizqfx;
							jarvisenemy[b].jarvisprop.manoderfx = jarvisenemy[b + 1].jarvisprop.manoderfx;
							jarvisenemy[b].jarvisprop.pieizqfx = jarvisenemy[b + 1].jarvisprop.pieizqfx;
							jarvisenemy[b].jarvisprop.piederfx = jarvisenemy[b + 1].jarvisprop.piederfx;
							jarvisenemy[b].jarvisprop.enviroment_ID = jarvisenemy[b + 1].jarvisprop.enviroment_ID;
							jarvisenemy[b].isfrozen = jarvisenemy[b + 1].isfrozen;
							jarvisenemy[b].cohete = jarvisenemy[b + 1].cohete;
							jarvisenemy[b].complete = jarvisenemy[b + 1].complete;
							jarvisenemy[b].obstaclex = jarvisenemy[b + 1].obstaclex;
							jarvisenemy[b].obstacley = jarvisenemy[b + 1].obstacley;
							jarvisenemy[b].obstaclez = jarvisenemy[b + 1].obstaclez;
							jarvisenemy[b].contadordist = jarvisenemy[b + 1].contadordist;
							jarvisenemy[b].contadorinicial = jarvisenemy[b + 1].contadorinicial;
							jarvisenemy[b].damagetimer = jarvisenemy[b + 1].damagetimer;
							b++;
						}
						dimenemy -= 1;
					}
					else
					{
						if (level1 > 3 && player->m_fHealth > 0)
						{
							if (boolvars.menuisactive == true
								|| CWanted().m_bEverybodyBackOff == 1
								|| CWanted().m_bPoliceBackOff == 1
								|| CWanted().m_bPoliceBackOffGarage == 1
								|| CWanted().m_bLeavePlayerAlone == true)
							{
								if (jarvisenemy[i].isfrozen == true)
								{
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
									jarvisenemy[i].isfrozen = false;
								}
								codigopropulsor = flotar;
								if (jarvisenemy[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisenemy[i].jarvisprop) == false)
								{
									jarvisenemy[i].thrusterscode = codigopropulsor;
									if (isplayingfly(true, jarvisenemy[i].botstream[0], 1, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == true || jarvisenemy[i].jarvisprop.manoizqfx != 0) {
										JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
										JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
									}
									PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 0, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);

								}
								else
								{
									if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_Hover") == false)
									{
										JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
										JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
										plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
										plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_Hover", "ironman", 10.0f, 1, 1, 1, 0, -2);

									}

									if (isplayingfly(true, jarvisenemy[i].botstream[0], 0, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == false && jarvisenemy[i].jarvisprop.manoizqfx == 0) {
										PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 0, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);
									}
								}
								float angle;
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - dist2.x, dist.y - dist2.y, &angle);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, 0.0f, 0.0f, dist2.z - dist.z);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisenemy[i].baseobj, 0.0f, 0.0f, angle - 180.0f);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisenemy[i].baseobj, angle - 180.0f);
							}
							else
							{
								dist3.x = dist2.x - dist.x;
								dist3.y = dist2.y - dist.y;
								dist3.z = dist2.z - dist.z;
								if (dist3.Magnitude() < 50.0f && plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, origin.x, origin.y, origin.z, dist.x, dist.y, dist.z, 1, 1, 0, 0, 0) == true)
								{
									if (dist3.Magnitude() < 20.0f && plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, origin.x, origin.y, origin.z, dist.x, dist.y, dist.z, 1, 1, 0, 0, 0) == true)
									{
										if (jarvisenemy[i].isfrozen == true)
										{
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
											plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
											plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
											jarvisenemy[i].isfrozen = false;
										}
										codigopropulsor = apuntarbot;
										if (jarvisenemy[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisenemy[i].jarvisprop) == false)
										{
											jarvisenemy[i].thrusterscode = codigopropulsor;
											if (isplayingfly(true, jarvisenemy[i].botstream[0], 1, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == true || jarvisenemy[i].jarvisprop.manoizqfx != 0) {
												JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
												JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
											}
											PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 0, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);

										}
										else
										{
											if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_botAimIdle") == false)
											{
												JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
												JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_botAimIdle", "ironman", 10.0f, 1, 1, 1, 0, -2);

											}
											else
											{
												useweaponwithtarget(&jarvisenemy[i]);
											}
											if (isplayingfly(true, jarvisenemy[i].botstream[0], 0, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == false && jarvisenemy[i].jarvisprop.manoizqfx == 0) {
												JarvisVoice::PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 0, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);
											}
										}
										float angle;
										plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - dist2.x, dist.y - dist2.y, &angle);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, 0.0f, 0.0f, dist2.z - dist.z);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisenemy[i].baseobj, 0.0f, 0.0f, angle - 180.0f);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisenemy[i].baseobj, angle - 180.0f);
									}
									else
									{
										//moverse hacia ally con anim adelante (o autoadelante si lleva un auto)
										if (jarvisenemy[i].isfrozen == true)
										{
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
											plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
											plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
											jarvisenemy[i].isfrozen = false;
										}
										CVector velocidad;
										float angle;
										velocidad.x = (origin.x - dist.x)*1.0f;
										velocidad.y = (origin.y - dist.y)*1.0f;
										velocidad.z = (origin.z - dist.z)*1.0f;

										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, velocidad.x, velocidad.y, velocidad.z);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisenemy[i].baseobj, 0.0f, 0.0f, 0.0);
										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].baseobj, &dist.x, &dist.y, &dist.z);

										plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - dist2.x, dist.y - dist2.y, &angle);


										codigopropulsor = apuntarbot;
										if (jarvisenemy[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisenemy[i].jarvisprop) == false)
										{
											jarvisenemy[i].thrusterscode = codigopropulsor;
											if (isplayingfly(true, jarvisenemy[i].botstream[0], 1, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == true || jarvisenemy[i].jarvisprop.manoizqfx != 0) {
												JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
												JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
											}
											PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 0, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);

										}
										else
										{
											if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_botAimIdle") == false)
											{
												JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
												JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_botAimIdle", "ironman", 10.0f, 1, 1, 1, 0, -2);

											}
											else
											{
												useweaponwithtarget(&jarvisenemy[i]);
											}
											if (isplayingfly(true, jarvisenemy[i].botstream[0], 0, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == false && jarvisenemy[i].jarvisprop.manoizqfx == 0) {
												JarvisVoice::PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 0, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);
											}
										}
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisenemy[i].baseobj, angle - 180.0f);
									}
								}
								else
								{
									//moverse hacia ally con anim adelante (o autoadelante si lleva un auto)
									if (jarvisenemy[i].isfrozen == true)
									{
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
										jarvisenemy[i].isfrozen = false;
									}
									CVector velocidad;
									float angle;
									velocidad.x = (origin.x - dist.x)*1.0f;
									velocidad.y = (origin.y - dist.y)*1.0f;
									velocidad.z = (origin.z - dist.z)*1.0f;

									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, velocidad.x, velocidad.y, velocidad.z);
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisenemy[i].baseobj, 0.0f, 0.0f, 0.0);
									plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].baseobj, &dist.x, &dist.y, &dist.z);

									plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - dist2.x, dist.y - dist2.y, &angle);


									codigopropulsor = adelante;
									if (jarvisenemy[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisenemy[i].jarvisprop) == false)
									{
										jarvisenemy[i].thrusterscode = codigopropulsor;
										if (isplayingfly(true, jarvisenemy[i].botstream[0], 0, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == true || jarvisenemy[i].jarvisprop.manoizqfx != 0) {
											JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
											JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
										}
										PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 1, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);

									}
									else
									{
										if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_MoveFWD") == false)
										{
											JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
											JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
											plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
											plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_MoveFWD", "ironman", 10.0f, 1, 1, 1, 0, -2);

										}
										if (isplayingfly(true, jarvisenemy[i].botstream[0], 1, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == false && jarvisenemy[i].jarvisprop.manoizqfx == 0) {
											PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 1, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);


										}
									}
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisenemy[i].baseobj, angle - 180.0f);
								}
							}
						}
						else
						{
							CVector vel, vel2;
							float distance, angle;
							//go away
							vel2.x = 1553.9343f - dist.x;
							vel2.y = -1675.4996f - dist.y;
							vel2.z = 16.1953f - dist.z;
							plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, 1553.9343f, -1675.4996f, 78.3716f, dist.x, dist.y, dist.z, &distance);
							if (plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, 1553.9343f, -1675.4996f, 16.1953f, dist.x, dist.y, dist.z, 1, 1, 0, 1, 0) == false)
							{
								vel.x = 1553.9343f - dist.x;
								vel.y = -1675.4996f - dist.y;
								vel.z = 78.3716f - dist.z;
							}
							else
							{
								vel.x = 1553.9343f - dist.x;
								vel.y = -1675.4996f - dist.y;
								vel.z = 16.1953f - dist.z;
							}
							//if (vel2.Magnitude() < 60.0f)
							if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_2D, jarvisenemy[i].actorchar, 1553.9343f, -1675.4996f, 2.0f, 2.0f, 0) == true)
							{
								if (dist.z < 17.0f)
								{
									if (jarvisenemy[i].cohete.misil != NULL)
									{
										CVector misilpos;
										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_HEADING, jarvisenemy[i].cohete.misil, &boolvars.anglecohete);

										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].cohete.misil, &misilpos.x, &misilpos.y, &misilpos.z);
										plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, misilpos.x, misilpos.y, misilpos.z, 10);
										plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, jarvisenemy[i].cohete.misil);
										plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, jarvisenemy[i].cohete.misil);
										jarvisenemy[i].cohete.objective = NULL;
										jarvisenemy[i].cohete.timecreated = 0;
										jarvisenemy[i].cohete.misil = NULL;
										if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, misilpos.x, misilpos.y, misilpos.z, 10.0f, 10.0f, 10.0f, 0) == true)
										{
											CVector2D explhead;

											explhead.x = player->GetPosition().x - misilpos.x;
											explhead.y = player->GetPosition().y - misilpos.y;

											plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &boolvars.anglecohete);


											boolvars.cohetebotexplotado = true;

											static float hadin;
											plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

											if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
												boolvars.vuelaatras = true;
											else
												boolvars.vuelaatras = false;

											boolvars.landgetup = 1;
										}
									}
									plugin::scripting::CallCommandById(COMMAND_REMOVE_BLIP, jarvisenemy[i].blip);
									StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
									plugin::scripting::CallCommandById(COMMAND_REMOVE_CHAR_ELEGANTLY, jarvisenemy[i].actorchar);
									plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, jarvisenemy[i].baseobj);
									plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, jarvisenemy[i].baseobj);
									plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisenemy[i].actorchar);
									jarvisenemy[i].actorchar = NULL;
									jarvisenemy[i].target.actorchar = NULL;
									jarvisenemy[i].actorid = 0;
									jarvisenemy[i].isfrozen = false;

									int b = i;
									while (b < dimenemy - 1)
									{
										pageenemy[b] = pageenemy[b + 1];
										jarvisenemy[b].baseobj = jarvisenemy[b + 1].baseobj;
										jarvisenemy[b].thrusterscode = jarvisenemy[b + 1].thrusterscode;
										jarvisenemy[b].velx = jarvisenemy[b + 1].velx;
										jarvisenemy[b].vely = jarvisenemy[b + 1].vely;
										jarvisenemy[b].velz = jarvisenemy[b + 1].velz;
										jarvisenemy[b].target.actorchar = jarvisenemy[b + 1].target.actorchar;
										jarvisenemy[b].target.actorid = jarvisenemy[b + 1].target.actorid;
										jarvisenemy[b].target.timer = jarvisenemy[b + 1].target.timer;
										jarvisenemy[b].audios[0] = jarvisenemy[b + 1].audios[0];
										jarvisenemy[b].audios[1] = jarvisenemy[b + 1].audios[1];
										jarvisenemy[b].audios[2] = jarvisenemy[b + 1].audios[2];
										jarvisenemy[b].audios[3] = jarvisenemy[b + 1].audios[3];
										jarvisenemy[b].audios[4] = jarvisenemy[b + 1].audios[4];
										jarvisenemy[b].audios[5] = jarvisenemy[b + 1].audios[5];
										jarvisenemy[b].audios[6] = jarvisenemy[b + 1].audios[6];
										jarvisenemy[b].botstream[0] = jarvisenemy[b + 1].botstream[0];
										jarvisenemy[b].botstream[1] = jarvisenemy[b + 1].botstream[1];
										jarvisenemy[b].botstream[2] = jarvisenemy[b + 1].botstream[2];
										jarvisenemy[b].mark = jarvisenemy[b + 1].mark;
										jarvisenemy[b].raystime = jarvisenemy[b + 1].raystime;
										jarvisenemy[b].yolohice = jarvisenemy[b + 1].yolohice;
										jarvisenemy[b].ibeam[0] = jarvisenemy[b + 1].ibeam[0];
										jarvisenemy[b].ibeam[1] = jarvisenemy[b + 1].ibeam[1];
										jarvisenemy[b].ibeam[2] = jarvisenemy[b + 1].ibeam[2];
										jarvisenemy[b].hasacar = jarvisenemy[b + 1].hasacar;
										jarvisenemy[b].blip = jarvisenemy[b + 1].blip;
										jarvisenemy[b].raysactive = jarvisenemy[b + 1].raysactive;
										jarvisenemy[b].taskindex = jarvisenemy[b + 1].taskindex;
										jarvisenemy[b].hp = jarvisenemy[b + 1].hp;
										jarvisenemy[b].wpn = jarvisenemy[b + 1].wpn;
										jarvisenemy[b].actorid = jarvisenemy[b + 1].actorid;
										jarvisenemy[b].actorchar = jarvisenemy[b + 1].actorchar;
										jarvisenemy[b].timetoact = jarvisenemy[b + 1].timetoact;
										jarvisenemy[b].jarvisprop.manoizqfx = jarvisenemy[b + 1].jarvisprop.manoizqfx;
										jarvisenemy[b].jarvisprop.manoderfx = jarvisenemy[b + 1].jarvisprop.manoderfx;
										jarvisenemy[b].jarvisprop.pieizqfx = jarvisenemy[b + 1].jarvisprop.pieizqfx;
										jarvisenemy[b].jarvisprop.piederfx = jarvisenemy[b + 1].jarvisprop.piederfx;
										jarvisenemy[b].jarvisprop.enviroment_ID = jarvisenemy[b + 1].jarvisprop.enviroment_ID;
										jarvisenemy[b].isfrozen = jarvisenemy[b + 1].isfrozen;
										jarvisenemy[b].cohete = jarvisenemy[b + 1].cohete;
										jarvisenemy[b].complete = jarvisenemy[b + 1].complete;
										jarvisenemy[b].obstaclex = jarvisenemy[b + 1].obstaclex;
										jarvisenemy[b].obstacley = jarvisenemy[b + 1].obstacley;
										jarvisenemy[b].obstaclez = jarvisenemy[b + 1].obstaclez;
										jarvisenemy[b].contadordist = jarvisenemy[b + 1].contadordist;
										jarvisenemy[b].contadorinicial = jarvisenemy[b + 1].contadorinicial;
										jarvisenemy[b].damagetimer = jarvisenemy[b + 1].damagetimer;
										b++;
									}
									dimenemy -= 1;
								}
								else
								{
									if (jarvisenemy[i].isfrozen == true)
									{
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
										jarvisenemy[i].isfrozen = false;
									}

									codigopropulsor = abajo;
									if (jarvisenemy[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisenemy[i].jarvisprop) == false)
									{
										jarvisenemy[i].thrusterscode = codigopropulsor;
										if (isplayingfly(true, jarvisenemy[i].botstream[0], 0, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == true || jarvisenemy[i].jarvisprop.manoizqfx != 0) {
											JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
											JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
										}
										PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 1, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);

									}
									else
									{
										if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_MoveDown") == false)
										{
											JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
											JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
											plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
											plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_MoveDown", "ironman", 10.0f, 1, 1, 1, 0, -2);

										}
										if (isplayingfly(true, jarvisenemy[i].botstream[0], 1, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == false && jarvisenemy[i].jarvisprop.manoizqfx == 0) {
											PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 1, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);


										}
									}
									float magnitvel = vel2.Magnitude();
									vel2.x *= 20.0f / magnitvel;
									vel2.y *= 20.0f / magnitvel;
									vel2.z *= 20.0f / magnitvel;
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, vel2.x, vel2.y, vel2.z);
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisenemy[i].baseobj, 0.0f, 0.0f, 0.0);
									plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].baseobj, &dist.x, &dist.y, &dist.z);
								}
							}
							else
							{
								float magnitvel = vel.Magnitude();
								vel.x *= 100.0f / magnitvel;
								vel.y *= 100.0f / magnitvel;
								vel.z *= 100.0f / magnitvel;
								if (jarvisenemy[i].isfrozen == true)
								{
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
									jarvisenemy[i].isfrozen = false;
								}

								codigopropulsor = adelante;
								if (jarvisenemy[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisenemy[i].jarvisprop) == false)
								{
									jarvisenemy[i].thrusterscode = codigopropulsor;
									if (isplayingfly(true, jarvisenemy[i].botstream[0], 0, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == true || jarvisenemy[i].jarvisprop.manoizqfx != 0) {
										JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
										JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
									}
									PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 1, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);

								}
								else
								{
									if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_MoveFWD") == false)
									{
										JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
										JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
										plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
										plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_MoveFWD", "ironman", 10.0f, 1, 1, 1, 0, -2);

									}
									if (isplayingfly(true, jarvisenemy[i].botstream[0], 1, jarvisenemy[i].jarvisprop.enviroment_ID, 2, i) == false && jarvisenemy[i].jarvisprop.manoizqfx == 0) {
										PlayThrustersIDLE(2, i, &jarvisenemy[i].botstream[0], (int)jarvisenemy[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisenemy[i].mark], 1, jarvisenemy[i].thrusterscode, &jarvisenemy[i].jarvisprop);


									}
								}
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, -dist.x + 1553.9343f, -dist.y + -1675.4996f, &angle);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, vel.x, vel.y, vel.z);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisenemy[i].baseobj, 0.0f, 0.0f, 0.0);
								plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisenemy[i].baseobj, &dist.x, &dist.y, &dist.z);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisenemy[i].baseobj, angle);
							}
						}
					}
				}
				else
				{

					if (jarvisenemy[i].jarvisprop.manoizqfx != 0) {
						JarvisVoice::StopThrustersIDLE(&jarvisenemy[i].botstream[0], &jarvisenemy[i].jarvisprop);
						JarvisVoice::storeenviroment(&jarvisenemy[i].jarvisprop.enviroment_ID);
						kill_thrusters(&jarvisenemy[i].jarvisprop);
					}
					if (jarvisenemy[i].isfrozen == true)
					{
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisenemy[i].baseobj, 1);
						plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisenemy[i].baseobj, 0);
						plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisenemy[i].actorchar, 0);
						jarvisenemy[i].isfrozen = false;
					}
					if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisenemy[i].actorchar, "fly_Stop") == false) {
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisenemy[i].actorchar);
						plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisenemy[i].actorchar, "fly_Stop", "ironman", 10.0f, 0, 0, 0, 0, 500);
					}
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisenemy[i].baseobj, jarvisenemy[i].velx, jarvisenemy[i].vely, jarvisenemy[i].velz);

				}
			}
			else
			{
				int b = i;
				while (b < dimenemy - 1)
				{
					pageenemy[b] = pageenemy[b + 1];
					jarvisenemy[b].baseobj = jarvisenemy[b + 1].baseobj;
					jarvisenemy[b].thrusterscode = jarvisenemy[b + 1].thrusterscode;
					jarvisenemy[b].velx = jarvisenemy[b + 1].velx;
					jarvisenemy[b].vely = jarvisenemy[b + 1].vely;
					jarvisenemy[b].velz = jarvisenemy[b + 1].velz;
					jarvisenemy[b].target.actorchar = jarvisenemy[b + 1].target.actorchar;
					jarvisenemy[b].target.actorid = jarvisenemy[b + 1].target.actorid;
					jarvisenemy[b].target.timer = jarvisenemy[b + 1].target.timer;
					jarvisenemy[b].audios[0] = jarvisenemy[b + 1].audios[0];
					jarvisenemy[b].audios[1] = jarvisenemy[b + 1].audios[1];
					jarvisenemy[b].audios[2] = jarvisenemy[b + 1].audios[2];
					jarvisenemy[b].audios[3] = jarvisenemy[b + 1].audios[3];
					jarvisenemy[b].audios[4] = jarvisenemy[b + 1].audios[4];
					jarvisenemy[b].audios[5] = jarvisenemy[b + 1].audios[5];
					jarvisenemy[b].audios[6] = jarvisenemy[b + 1].audios[6];
					jarvisenemy[b].botstream[0] = jarvisenemy[b + 1].botstream[0];
					jarvisenemy[b].botstream[1] = jarvisenemy[b + 1].botstream[1];
					jarvisenemy[b].botstream[2] = jarvisenemy[b + 1].botstream[2];
					jarvisenemy[b].mark = jarvisenemy[b + 1].mark;
					jarvisenemy[b].raystime = jarvisenemy[b + 1].raystime;
					jarvisenemy[b].yolohice = jarvisenemy[b + 1].yolohice;
					jarvisenemy[b].ibeam[0] = jarvisenemy[b + 1].ibeam[0];
					jarvisenemy[b].ibeam[1] = jarvisenemy[b + 1].ibeam[1];
					jarvisenemy[b].ibeam[2] = jarvisenemy[b + 1].ibeam[2];
					jarvisenemy[b].hasacar = jarvisenemy[b + 1].hasacar;
					jarvisenemy[b].blip = jarvisenemy[b + 1].blip;
					jarvisenemy[b].raysactive = jarvisenemy[b + 1].raysactive;
					jarvisenemy[b].taskindex = jarvisenemy[b + 1].taskindex;
					jarvisenemy[b].hp = jarvisenemy[b + 1].hp;
					jarvisenemy[b].wpn = jarvisenemy[b + 1].wpn;
					jarvisenemy[b].actorid = jarvisenemy[b + 1].actorid;
					jarvisenemy[b].actorchar = jarvisenemy[b + 1].actorchar;
					jarvisenemy[b].timetoact = jarvisenemy[b + 1].timetoact;
					jarvisenemy[b].jarvisprop.manoizqfx = jarvisenemy[b + 1].jarvisprop.manoizqfx;
					jarvisenemy[b].jarvisprop.manoderfx = jarvisenemy[b + 1].jarvisprop.manoderfx;
					jarvisenemy[b].jarvisprop.pieizqfx = jarvisenemy[b + 1].jarvisprop.pieizqfx;
					jarvisenemy[b].jarvisprop.piederfx = jarvisenemy[b + 1].jarvisprop.piederfx;
					jarvisenemy[b].jarvisprop.enviroment_ID = jarvisenemy[b + 1].jarvisprop.enviroment_ID;
					jarvisenemy[b].isfrozen = jarvisenemy[b + 1].isfrozen;
					jarvisenemy[b].cohete = jarvisenemy[b + 1].cohete;
					jarvisenemy[b].complete = jarvisenemy[b + 1].complete;
					jarvisenemy[b].obstaclex = jarvisenemy[b + 1].obstaclex;
					jarvisenemy[b].obstacley = jarvisenemy[b + 1].obstacley;
					jarvisenemy[b].obstaclez = jarvisenemy[b + 1].obstaclez;
					jarvisenemy[b].contadordist = jarvisenemy[b + 1].contadordist;
					jarvisenemy[b].contadorinicial = jarvisenemy[b + 1].contadorinicial;
					jarvisenemy[b].damagetimer = jarvisenemy[b + 1].damagetimer;
					b++;
				}
				dimenemy -= 1;
			}
			i++;
		}

		if (isfaraway == true)
		{			
			i = 0;
			CFont::SetBackground(0, 0);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetFontStyle(FONT_MENU);
			CFont::SetScale(SCREEN_MULTIPLIER(settings.vecVehicleNameScale.x), SCREEN_MULTIPLIER(settings.vecVehicleNameScale.y));
			CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesPosnY), boolvars.dronfar.data());
			
			if (GetKeyState('Y') & 0x8000)
			{
				while (i < dimfriends)
				{
					for (int u = 0; u <= 2; u++)
					{
						if (jarvisfriend[i].ibeam[u] != NULL)
						{
							if (plugin::scripting::CallCommandById(COMMAND_DOES_SEARCHLIGHT_EXIST, jarvisfriend[i].ibeam[u]) == true)
							{
								plugin::scripting::CallCommandById(COMMAND_DELETE_SEARCHLIGHT, jarvisfriend[i].ibeam[u]);
								jarvisfriend[i].ibeam[u] = NULL;
							}
						}
					}
					if (jarvisfriend[i].cohete.misil != NULL && jarvisfriend[i].complete == false)
					{
						botshootsmissile(&jarvisfriend[i].cohete, &jarvisfriend[i].complete);
					}
					if ((int)jarvisfriend[i].actorchar != NULL && jarvisfriend[i].actorchar != player)
					{
						jarvisfriend[i].target.actorchar = NULL;
						jarvisfriend[i].target.actorid = NULL;
						jarvisfriend[i].target.timer = NULL;
						CVector origin, origin1, dist, distch;
						static float distance, angle;
						plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, &dist.x, &dist.y, &dist.z);

						static float ang;
						ang = -90.0f + 18.0f *(float)i;
						CVector2D pedcoords;
						plugin::scripting::CallCommandById(COMMAND_SIN, ang, &pedcoords.y);
						plugin::scripting::CallCommandById(COMMAND_COS, ang, &pedcoords.x);
						pedcoords.y *= 2.0f;
						pedcoords.x *= 2.0f;

						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, pedcoords.x, pedcoords.y, 2.0f, &origin.x, &origin.y, &origin.z);
						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &origin1.x, &origin1.y, &origin1.z);

						plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, origin1.x, origin1.y, origin1.z, dist.x, dist.y, dist.z, &distance);

						plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);


						if (jarvisfriend[i].isfrozen == true)
						{
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
							plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
							jarvisfriend[i].isfrozen = false;
						}
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, origin.x, origin.y, origin.z);
						plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, 0.0f, 0.0f, angle - 180.0f);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, angle - 180.0f);

						codigopropulsor = flotar;
						if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
						{
							jarvisfriend[i].thrusterscode = codigopropulsor;
							if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx!=0) {
								JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
								
								JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
							}
							JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pageenemy[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);
						}
						else
						{
							if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_Hover") == false)
							{
								JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
								
								JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
								plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
								plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_Hover", "ironman", 10.0f, 1, 1, 1, 0, -2);

							}
							if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
								JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


							}
						}
					}
					else
					{
						int b = i;
						while (b < dimfriends - 1)
						{
							pagebelong[b] = pagebelong[b + 1];
							jarvisfriend[b].baseobj = jarvisfriend[b + 1].baseobj;
							jarvisfriend[b].thrusterscode = jarvisfriend[b + 1].thrusterscode; 
							jarvisfriend[b].velx = jarvisfriend[b + 1].velx;
							jarvisfriend[b].vely = jarvisfriend[b + 1].vely;
							jarvisfriend[b].velz = jarvisfriend[b + 1].velz;
							jarvisfriend[b].audios[0] = jarvisfriend[b + 1].audios[0];
							jarvisfriend[b].audios[1] = jarvisfriend[b + 1].audios[1];
							jarvisfriend[b].audios[2] = jarvisfriend[b + 1].audios[2];
							jarvisfriend[b].audios[3] = jarvisfriend[b + 1].audios[3];
							jarvisfriend[b].audios[4] = jarvisfriend[b + 1].audios[4];
							jarvisfriend[b].audios[5] = jarvisfriend[b + 1].audios[5];
							jarvisfriend[b].audios[6] = jarvisfriend[b + 1].audios[6];
							jarvisfriend[b].botstream[0] = jarvisfriend[b + 1].botstream[0];
							jarvisfriend[b].botstream[1] = jarvisfriend[b + 1].botstream[1];
							jarvisfriend[b].botstream[2] = jarvisfriend[b + 1].botstream[2];
							jarvisfriend[b].mark = jarvisfriend[b + 1].mark;
							jarvisfriend[b].raystime = jarvisfriend[b + 1].raystime;
							jarvisfriend[b].yolohice = jarvisfriend[b + 1].yolohice;
							jarvisfriend[b].ibeam[0] = jarvisfriend[b + 1].ibeam[0];
							jarvisfriend[b].ibeam[1] = jarvisfriend[b + 1].ibeam[1];
							jarvisfriend[b].ibeam[2] = jarvisfriend[b + 1].ibeam[2];
							jarvisfriend[b].hasacar = jarvisfriend[b + 1].hasacar;
							jarvisfriend[b].blip = jarvisfriend[b + 1].blip;
							jarvisfriend[b].raysactive = jarvisfriend[b + 1].raysactive;
							jarvisfriend[b].taskindex = jarvisfriend[b + 1].taskindex;
							jarvisfriend[b].hp = jarvisfriend[b + 1].hp;
							jarvisfriend[b].wpn = jarvisfriend[b + 1].wpn;
							jarvisfriend[b].actorid = jarvisfriend[b + 1].actorid;
							jarvisfriend[b].actorchar = jarvisfriend[b + 1].actorchar;
							jarvisfriend[b].timetoact = jarvisfriend[b + 1].timetoact;
							jarvisfriend[b].jarvisprop.manoizqfx = jarvisfriend[b + 1].jarvisprop.manoizqfx;
							jarvisfriend[b].jarvisprop.manoderfx = jarvisfriend[b + 1].jarvisprop.manoderfx;
							jarvisfriend[b].jarvisprop.pieizqfx = jarvisfriend[b + 1].jarvisprop.pieizqfx;
							jarvisfriend[b].jarvisprop.piederfx = jarvisfriend[b + 1].jarvisprop.piederfx;
							jarvisfriend[b].jarvisprop.enviroment_ID = jarvisfriend[b + 1].jarvisprop.enviroment_ID;
							jarvisfriend[b].isfrozen = jarvisfriend[b + 1].isfrozen;
							jarvisfriend[b].target.actorchar = jarvisfriend[b + 1].target.actorchar;
							jarvisfriend[b].target.actorid = jarvisfriend[b + 1].target.actorid;
							jarvisfriend[b].target.timer = jarvisfriend[b + 1].target.timer;
							jarvisfriend[b].attacker.actorchar = jarvisfriend[b + 1].attacker.actorchar;
							jarvisfriend[b].attacker.actorid = jarvisfriend[b + 1].attacker.actorid;
							jarvisfriend[b].attacker.timer = jarvisfriend[b + 1].attacker.timer;
							jarvisfriend[b].cohete = jarvisfriend[b + 1].cohete;
							jarvisfriend[b].complete = jarvisfriend[b + 1].complete;
							jarvisfriend[b].obstaclex = jarvisfriend[b + 1].obstaclex;
							jarvisfriend[b].obstacley = jarvisfriend[b + 1].obstacley;
							jarvisfriend[b].obstaclez = jarvisfriend[b + 1].obstaclez;
							jarvisfriend[b].contadordist = jarvisfriend[b + 1].contadordist;
							jarvisfriend[b].contadorinicial = jarvisfriend[b + 1].contadorinicial;
							jarvisfriend[b].damagetimer = jarvisfriend[b + 1].damagetimer;
							b++;
						}
						dimfriends -= 1;
					}
					i++;
				}
				isfaraway = false;
				goto ending;
			}
			else
			{
				goto regular_routine;
			}
		}
		else
		{
			goto regular_routine;
		}
	}
	else
	{
		once1 = false;
	}
	goto ending;

regular_routine:
	{
		i = 0;
		while (i < dimfriends)
		{
			if (jarvisfriend[i].cohete.misil != NULL && jarvisfriend[i].complete == false)
			{
				botshootsmissile(&jarvisfriend[i].cohete, &jarvisfriend[i].complete);
			}
			for (int u = 0; u <= 2; u++)
			{
				if (jarvisfriend[i].ibeam[u] != NULL)
				{
					if (plugin::scripting::CallCommandById(COMMAND_DOES_SEARCHLIGHT_EXIST, jarvisfriend[i].ibeam[u]) == true)
					{
						plugin::scripting::CallCommandById(COMMAND_DELETE_SEARCHLIGHT, jarvisfriend[i].ibeam[u]);
						jarvisfriend[i].ibeam[u] = NULL;
					}
				}
			}
			if ((int)jarvisfriend[i].actorchar != NULL && jarvisfriend[i].actorchar != player)
			{
				int actorhp;
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, jarvisfriend[i].actorchar, &actorhp);
				if (actorhp <= 1)
				{
					if (jarvisfriend[i].cohete.misil != NULL)
					{
						CVector misilpos;
						plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].cohete.misil, &misilpos.x, &misilpos.y, &misilpos.z);
						plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, misilpos.x, misilpos.y, misilpos.z, 10);
						plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, jarvisfriend[i].cohete.misil);
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, jarvisfriend[i].cohete.misil);
						//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, rocket->objective);
						jarvisfriend[i].cohete.objective = NULL;
						jarvisfriend[i].cohete.timecreated = 0;
						jarvisfriend[i].cohete.misil = NULL;
						if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, misilpos.x, misilpos.y, misilpos.z, 10.0f, 10.0f, 10.0f, 0) == true)
						{
							CVector2D explhead;

							explhead.x = player->GetPosition().x - misilpos.x;
							explhead.y = player->GetPosition().y - misilpos.y;

							plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &boolvars.anglecohete);


							boolvars.cohetebotexplotado = true;

							static float hadin;
							plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

							if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
								boolvars.vuelaatras = true;
							else
								boolvars.vuelaatras = false;

							boolvars.landgetup = 1;
						}

					}
					CVector origin;
					plugin::scripting::CallCommandById(COMMAND_REMOVE_BLIP, jarvisfriend[i].blip);
					StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, jarvisfriend[i].actorchar, &origin.x, &origin.y, &origin.z);
					plugin::scripting::CallCommandById(COMMAND_ADD_EXPLOSION, origin.x, origin.y, origin.z, 10);
					plugin::scripting::CallCommandById(COMMAND_DELETE_CHAR, jarvisfriend[i].actorchar);
					plugin::scripting::CallCommandById(COMMAND_DELETE_OBJECT, jarvisfriend[i].baseobj);
					plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, jarvisfriend[i].baseobj);
					plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisfriend[i].actorchar);
					if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, player, origin.x, origin.y, origin.z, 10.0f, 10.0f, 10.0f, 0) == true)
					{
						CVector2D explhead;

						explhead.x = player->GetPosition().x - origin.x;
						explhead.y = player->GetPosition().y - origin.y;

						plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &boolvars.anglecohete);


						boolvars.cohetebotexplotado = true;

						static float hadin;
						plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

						if (boolvars.anglecohete>hadin - 90.0f && boolvars.anglecohete<hadin + 90.0f)
							boolvars.vuelaatras = true;
						else
							boolvars.vuelaatras = false;

						boolvars.landgetup = 1;
					}
					jarvisfriend[i].target.actorchar = NULL;
					jarvisfriend[i].actorchar = NULL;
					jarvisfriend[i].actorid = 0;
					jarvisfriend[i].isfrozen = false;		


					int b = i;
					while (b < dimfriends - 1)
					{
						pagebelong[b] = pagebelong[b + 1];
						jarvisfriend[b].baseobj = jarvisfriend[b + 1].baseobj;
						jarvisfriend[b].thrusterscode = jarvisfriend[b + 1].thrusterscode;
						jarvisfriend[b].velx = jarvisfriend[b + 1].velx;
						jarvisfriend[b].vely = jarvisfriend[b + 1].vely;
						jarvisfriend[b].velz = jarvisfriend[b + 1].velz;
						jarvisfriend[b].audios[0] = jarvisfriend[b + 1].audios[0];
						jarvisfriend[b].audios[1] = jarvisfriend[b + 1].audios[1];
						jarvisfriend[b].audios[2] = jarvisfriend[b + 1].audios[2];
						jarvisfriend[b].audios[3] = jarvisfriend[b + 1].audios[3];
						jarvisfriend[b].audios[4] = jarvisfriend[b + 1].audios[4];
						jarvisfriend[b].audios[5] = jarvisfriend[b + 1].audios[5];
						jarvisfriend[b].audios[6] = jarvisfriend[b + 1].audios[6];
						jarvisfriend[b].botstream[0] = jarvisfriend[b + 1].botstream[0];
						jarvisfriend[b].botstream[1] = jarvisfriend[b + 1].botstream[1];
						jarvisfriend[b].botstream[2] = jarvisfriend[b + 1].botstream[2];
						jarvisfriend[b].mark = jarvisfriend[b + 1].mark;
						jarvisfriend[b].raystime = jarvisfriend[b + 1].raystime;
						jarvisfriend[b].yolohice = jarvisfriend[b + 1].yolohice;
						jarvisfriend[b].ibeam[0] = jarvisfriend[b + 1].ibeam[0];
						jarvisfriend[b].ibeam[1] = jarvisfriend[b + 1].ibeam[1];
						jarvisfriend[b].ibeam[2] = jarvisfriend[b + 1].ibeam[2];
						jarvisfriend[b].hasacar = jarvisfriend[b + 1].hasacar;
						jarvisfriend[b].blip = jarvisfriend[b + 1].blip;
						jarvisfriend[b].raysactive = jarvisfriend[b + 1].raysactive;
						jarvisfriend[b].taskindex = jarvisfriend[b + 1].taskindex;
						jarvisfriend[b].hp = jarvisfriend[b + 1].hp;
						jarvisfriend[b].wpn = jarvisfriend[b + 1].wpn;
						jarvisfriend[b].actorid = jarvisfriend[b + 1].actorid;
						jarvisfriend[b].actorchar = jarvisfriend[b + 1].actorchar;
						jarvisfriend[b].timetoact = jarvisfriend[b + 1].timetoact;
						jarvisfriend[b].jarvisprop.manoizqfx = jarvisfriend[b + 1].jarvisprop.manoizqfx;
						jarvisfriend[b].jarvisprop.manoderfx = jarvisfriend[b + 1].jarvisprop.manoderfx;
						jarvisfriend[b].jarvisprop.pieizqfx = jarvisfriend[b + 1].jarvisprop.pieizqfx;
						jarvisfriend[b].jarvisprop.piederfx = jarvisfriend[b + 1].jarvisprop.piederfx;
						jarvisfriend[b].jarvisprop.enviroment_ID = jarvisfriend[b + 1].jarvisprop.enviroment_ID;
						jarvisfriend[b].isfrozen = jarvisfriend[b + 1].isfrozen;
						jarvisfriend[b].target.actorchar = jarvisfriend[b + 1].target.actorchar;
						jarvisfriend[b].target.actorid = jarvisfriend[b + 1].target.actorid;
						jarvisfriend[b].target.timer = jarvisfriend[b + 1].target.timer;
						jarvisfriend[b].attacker.actorchar = jarvisfriend[b + 1].attacker.actorchar;
						jarvisfriend[b].attacker.actorid = jarvisfriend[b + 1].attacker.actorid;
						jarvisfriend[b].attacker.timer = jarvisfriend[b + 1].attacker.timer;
						jarvisfriend[b].cohete = jarvisfriend[b + 1].cohete;
						jarvisfriend[b].complete = jarvisfriend[b + 1].complete;
						jarvisfriend[b].obstaclex = jarvisfriend[b + 1].obstaclex;
						jarvisfriend[b].obstacley = jarvisfriend[b + 1].obstacley;
						jarvisfriend[b].obstaclez = jarvisfriend[b + 1].obstaclez;
						jarvisfriend[b].contadordist = jarvisfriend[b + 1].contadordist;
						jarvisfriend[b].damagetimer = jarvisfriend[b + 1].damagetimer;
						b++;
					}
					dimfriends -= 1;
				}
				else
				{
					if (jarvisfriend[i].damagetimer + 500 < CTimer::m_snTimeInMillisecondsNonClipped)
					{
						CVector dist, dist2, dist3, origin;
						float distanceftsk = 0.0f;
						CVector distanciabots;
						CVector distfortask = jarvisfriend[i].actorchar->GetPosition();
						CVector origin2 = player->GetPosition();
						distanciabots.x = origin2.x - distfortask.x;
						distanciabots.y = origin2.y - distfortask.y;
						distanciabots.z = origin2.z - distfortask.z;
						if (distanciabots.Magnitude() >= 20.0f)
						{
							isfaraway = true;
							jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
							jarvisfriend[i].taskindex = 0;
						}
						else
						{
							isfaraway = false;
							plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level1);
							if (level1 > 0 || cana > 0)
							{
								if ((jarvisfriend[i].target.timer + 500 < CTimer::m_snTimeInMillisecondsNonClipped &&
									jarvisfriend[i].taskindex == 0 && 
									jarvisfriend[i].timetoact + 50 < CTimer::m_snTimeInMillisecondsNonClipped) || 
									jarvisfriend[i].taskindex == 1 &&
									(jarvisfriend[i].timetoact + 5000 < CTimer::m_snTimeInMillisecondsNonClipped))
								{
									jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
									jarvisfriend[i].taskindex = 1;
									jarvisfriend[i].wpn = 1 + rand() % 3;
									if (dimenemy > 0)
									{
										int copindex = rand() % dimenemy;
										//char*shad;
										//shad = new char[20];
										//sprintf(shad, "%d", copindex);
										//WritePrivateProfileString("CONFIG", "ERROR", shad, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
										//delete[] shad;

										if ((int)jarvisenemy[copindex].actorchar != 0 && (int)jarvisfriend[i].target.actorchar != (int)jarvisenemy[copindex].actorchar)
										{
											jarvisfriend[i].target.actorchar = jarvisenemy[copindex].actorchar;
											jarvisfriend[i].target.actorid = jarvisenemy[copindex].actorid;
											jarvisfriend[i].target.timer = NULL;
											CVector distanciabots, posbot1, posbot2;
											posbot1 = jarvisfriend[i].target.actorchar->GetPosition();
											posbot2 = jarvisfriend[i].actorchar->GetPosition();
											distanciabots.x = posbot2.x - posbot1.x;
											distanciabots.y = posbot2.y - posbot1.y;
											distanciabots.z = posbot2.z - posbot1.z;
											if (distanciabots.Magnitude() >= 20.0f)
											{
												jarvisfriend[i].target.actorchar = cana;
												jarvisfriend[i].target.actorid = int(cana);
												jarvisfriend[i].target.timer = NULL;
											}
											else
											{
												jarvisenemy[copindex].attacker.actorchar = jarvisfriend[i].actorchar;
												jarvisenemy[copindex].attacker.actorid = jarvisfriend[i].actorid;
												CVector poss, poss1, poss2;
												plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[copindex].actorchar, 0.0f, 0.0f, 0.0f, &poss.x, &poss.y, &poss.z);
												plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[i].actorchar, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);

												poss2.x = (poss.x - poss1.x)*2.1f;
												poss2.y = (poss.y - poss1.y)*2.1f;
												poss2.z = (poss.z - poss1.z)*2.1f;
												jarvisenemy[copindex].velx = poss2.x;
												jarvisenemy[copindex].vely = poss2.y;
												jarvisenemy[copindex].velz = poss2.z;
											}
										}
										else
										{
											jarvisfriend[i].target.actorchar = cana;
											jarvisfriend[i].target.actorid = int(cana);
											jarvisfriend[i].target.timer = NULL;
										}

									}
									else
									{
										jarvisfriend[i].target.actorchar = cana;
										jarvisfriend[i].target.actorid = int(cana);
										jarvisfriend[i].target.timer = NULL;
									}

								}

								if ((int)jarvisfriend[i].target.actorchar == NULL)
								{
									jarvisfriend[i].target.actorchar = cana;
									jarvisfriend[i].target.actorid = int(cana);
									jarvisfriend[i].target.timer = NULL;
								}
							}
							else
							{
								jarvisfriend[i].taskindex = 0;
								jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
							}
						}


						if (jarvisfriend[i].taskindex == 1)
						{
							if (jarvisfriend[i].target.actorchar == NULL)
							{
								jarvisfriend[i].taskindex = 0;
							}
							else
							{
								if (jarvisfriend[i].target.actorchar->m_fHealth <= 1)
								{
									jarvisfriend[i].taskindex = 0;
									//WELL DONE SIR
								}
								else
								{
									CVector distanciabots;
									CVector posbot1, posbot2;
									posbot1 = jarvisfriend[i].target.actorchar->GetPosition();
									posbot2 = jarvisfriend[i].actorchar->GetPosition();
									distanciabots.x = posbot2.x - posbot1.x;
									distanciabots.y = posbot2.y - posbot1.y;
									distanciabots.z = posbot2.z - posbot1.z;
									if (distanciabots.Magnitude() >= 20.0f)
									{
										jarvisfriend[i].taskindex = 0;
										jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
									}
									else
									{
										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, &dist.x, &dist.y, &dist.z);
										plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, jarvisfriend[i].target.actorchar, &dist2.x, &dist2.y, &dist2.z);
										plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[i].target.actorchar, 0.0f, 10.0f, 0.0f, &origin.x, &origin.y, &origin.z);

										dist3.x = dist2.x - dist.x;
										dist3.y = dist2.y - dist.y;
										dist3.z = dist2.z - dist.z;
										if (dist3.Magnitude() < 20.0f && plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, origin.x, origin.y, origin.z, dist.x, dist.y, dist.z, 1, 1, 0, 0, 0) == true)
										{
											if (jarvisfriend[i].isfrozen == true)
											{
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
												plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
												jarvisfriend[i].isfrozen = false;
											}
											jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
											//atacar ally con arma .wpn (no nucleo) y hacer anim de apuntar o tirarle autos

											codigopropulsor = apuntarbot;
											if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
											{
												jarvisfriend[i].thrusterscode = codigopropulsor;
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
												}
												JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

											}
											else
											{
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_botAimIdle") == false)
												{
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_botAimIdle", "ironman", 10.0f, 1, 1, 1, 0, -2);

												}
												else
												{
													useweaponwithtarget(&jarvisfriend[i]);
												}
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
													JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


												}
											}
											float angle;
											plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - dist2.x, dist.y - dist2.y, &angle);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, 0.0f, 0.0f, dist2.z - dist.z);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, 0.0f, 0.0f, angle - 180.0f);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, angle - 180.0f);
										}
										else
										{
											//moverse hacia ally con anim adelante (o autoadelante si lleva un auto)
											if (jarvisfriend[i].isfrozen == true)
											{
												plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
												plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
												plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
												jarvisfriend[i].isfrozen = false;
											}
											CVector velocidad;
											float angle;
											velocidad.x = (origin.x - dist.x)*1.0f;
											velocidad.y = (origin.y - dist.y)*1.0f;
											velocidad.z = (origin.z - dist.z)*1.0f;

											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, velocidad.x, velocidad.y, velocidad.z);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisfriend[i].baseobj, 0.0f, 0.0f, 0.0);
											plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, &dist.x, &dist.y, &dist.z);

											plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - dist2.x, dist.y - dist2.y, &angle);


											codigopropulsor = adelante;
											if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
											{
												jarvisfriend[i].thrusterscode = codigopropulsor;
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
												}
												JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

											}
											else
											{
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveFWD") == false)
												{
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveFWD", "ironman", 10.0f, 1, 1, 1, 0, -2);

												}
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
													JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


												}
											}
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, angle - 180.0f);
										}
									}
								}
							}
						}
						if (jarvisfriend[i].taskindex == 0)
						{
							CVector origin, origin1, dist, distch;
							static float distance, angle;
							plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, &dist.x, &dist.y, &dist.z);

							static float ang;
							ang = -90.0f + 18.0f *(float)i;
							CVector2D pedcoords;
							plugin::scripting::CallCommandById(COMMAND_SIN, ang, &pedcoords.y);
							plugin::scripting::CallCommandById(COMMAND_COS, ang, &pedcoords.x);
							pedcoords.y *= 2.0f;
							pedcoords.x *= 2.0f;

							CVector velocidad;
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, pedcoords.x, pedcoords.y, 2.0f, &origin.x, &origin.y, &origin.z);
							plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &origin1.x, &origin1.y, &origin1.z);

							plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, origin1.x, origin1.y, origin1.z, dist.x, dist.y, dist.z, &distance);

							plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);
							if (distance > 5.0f)
							{
								if (distance >= 20.0f)
								{
									isfaraway = true;

								}
								else
								{
									isfaraway = false;
								}
								CVector velocidadplay;
								if (jarvisfriend[i].isfrozen == true)
								{
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
									plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
									jarvisfriend[i].isfrozen = false;
								}
								velocidad.x = (origin.x - dist.x)*1.1f;
								velocidad.y = (origin.y - dist.y)*1.1f;
								velocidad.z = (origin.z - dist.z)*1.1f;

								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, velocidad.x, velocidad.y, velocidad.z);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisfriend[i].baseobj, 0.0f, 0.0f, 0.0);
								plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, &dist.x, &dist.y, &dist.z);

								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);

								plugin::scripting::CallCommandById(COMMAND_GET_CHAR_VELOCITY, player, &velocidadplay.x, &velocidadplay.y, &velocidadplay.z);

								if (velocidadplay.x != 0.0f || velocidadplay.y != 0.0f || velocidadplay.z != 0.0f || plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, player, "fly_Hover") == false)
								{
									plugin::scripting::CallCommandById(COMMAND_GET_CHAR_VELOCITY, player, &velocidadplay.x, &velocidadplay.y, &velocidadplay.z);
									if (boolvars.iscjfrozen == false)
									{
										static float ang1, ang2, charhdn, distancia;

										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_HEADING, jarvisfriend[i].baseobj, &charhdn);

										plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, velocidadplay.x, velocidadplay.y, &ang1);

										plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, velocidadplay.x, velocidadplay.y, dist.x, dist.y, &distancia);

										float fastorientx = sqrt((velocidadplay.x * velocidadplay.x) + (velocidadplay.y * velocidadplay.y));


										plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, fastorientx, velocidadplay.z, &ang2);
										if (ang2 + 90.0f < 280 && ang2 + 90.0f > 80.0f)
										{
											ang1 = angle - 180.0f;
										}
										if (velocidadplay.Magnitude() > 90.0f)
										{
											//es rapido
											codigopropulsor = im_flyfast;
											if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
											{
												jarvisfriend[i].thrusterscode = codigopropulsor;
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
												}
												JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

											}
											else
											{
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_Fast") == false)
												{
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_Fast", "ironman", 10.0f, 1, 1, 1, 0, -2);

												}
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
													JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


												}
											}

											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, ang2 + 90.0f, 0.0f, ang1);
										}
										else
										{
											plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);
											if ((ang2 + 90.0f < 280 && ang2 + 90.0f > 80.0f) || (ang2 + 90.0f > -280 && ang2 + 90.0f < -80.0f))
											{
												if ((ang2 + 90.0f < 180) && (ang2 + 90.0f > -180))
												{
													codigopropulsor = arriba;
													if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
													{
														jarvisfriend[i].thrusterscode = codigopropulsor;
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
														}
														JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

													}
													else
													{
														if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveUp") == false)
														{
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
															plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveUp", "ironman", 10.0f, 1, 1, 1, 0, -2);

														}
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
															JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


														}
													}
												}
												else
												{
													//es hacia arriba o abajo
													codigopropulsor = abajo;
													if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
													{
														jarvisfriend[i].thrusterscode = codigopropulsor;
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
														}
														JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

													}
													else
													{
														if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveDown") == false)
														{
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
															plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveDown", "ironman", 10.0f, 1, 1, 1, 0, -2);

														}
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
															JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


														}
													}
												}
											}
											else
											{

												//es cualquier otro
												codigopropulsor = adelante;
												if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
												{
													jarvisfriend[i].thrusterscode = codigopropulsor;
													if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
														JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
														JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
													}
													JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

												}
												else
												{
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveFWD") == false)
													{
														JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
														JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveFWD", "ironman", 10.0f, 1, 1, 1, 0, -2);

													}
													if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
														JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


													}
												}

											}
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, 0.0f, 0.0f, ang1);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, ang1);
										}

										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, velocidadplay.x, velocidadplay.y, velocidadplay.z);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisfriend[i].baseobj, 0.0f, 0.0f, 0.0);
									}
								}
								else
								{
									if (velocidad.x != 0 || velocidad.y != 0 || velocidad.z != 0)
									{
										static float ang1, ang2, charhdn, distancia;
										charhdn = angle - 180.0f;

										plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, velocidad.x, velocidad.y, &ang1);

										plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_2D, velocidad.x, velocidad.y, dist.x, dist.y, &distancia);

										float fastorientx = sqrt((velocidad.x * velocidad.x) + (velocidad.y * velocidad.y));


										plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, fastorientx, velocidad.z, &ang2);
										ang1 = angle - 180.0f;
										if (velocidadplay.Magnitude() > 90.0f)
										{
											//es rapido
											codigopropulsor = im_flyfast;
											if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
											{
												jarvisfriend[i].thrusterscode = codigopropulsor;
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
												}
												JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

											}
											else
											{
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_Fast") == false)
												{
													JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
													JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
													plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_Fast", "ironman", 10.0f, 1, 1, 1, 0, -2);

												}
												if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
													JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


												}
											}

											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, ang2 + 90.0f, 0.0f, ang1);
										}
										else
										{
											plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);
											if ((ang2 + 90.0f < 280 && ang2 + 90.0f > 80.0f) || (ang2 + 90.0f > -280 && ang2 + 90.0f < -80.0f))
											{
												if ((ang2 + 90.0f < 180) && (ang2 + 90.0f > -180))
												{
													codigopropulsor = arriba;
													if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
													{
														jarvisfriend[i].thrusterscode = codigopropulsor;
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
														}
														JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

													}
													else
													{
														if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveUp") == false)
														{
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
															plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveUp", "ironman", 10.0f, 1, 1, 1, 0, -2);

														}
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
															JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


														}
													}
												}
												else
												{
													//es hacia arriba o abajo
													codigopropulsor = abajo;
													if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
													{
														jarvisfriend[i].thrusterscode = codigopropulsor;
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
														}
														JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

													}
													else
													{
														if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveDown") == false)
														{
															JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
															JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
															plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
															plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveDown", "ironman", 10.0f, 1, 1, 1, 0, -2);

														}
														if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
															JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


														}
													}
												}
											}
											else
											{

												//es cualquier otro
												codigopropulsor = adelante;
												if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
												{
													jarvisfriend[i].thrusterscode = codigopropulsor;
													if (isplayingfly(true, jarvisfriend[i].botstream[0], 0, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
														JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
														JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
													}
													JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

												}
												else
												{
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_MoveFWD") == false)
													{
														JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
														JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
														plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_MoveFWD", "ironman", 10.0f, 1, 1, 1, 0, -2);

													}
													if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == false && jarvisfriend[i].jarvisprop.manoizqfx == 0) {
														JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 1, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);


													}
												}

											}
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, 0.0f, 0.0f, ang1);
											plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, ang1);
										}

										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, velocidad.x, velocidad.y, velocidad.z);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisfriend[i].baseobj, 0.0f, 0.0f, 0.0);
									}
									else
									{
										plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, &velocidadplay.x, &velocidadplay.y, &velocidadplay.z);

										codigopropulsor = flotar;
										jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
										if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
										{
											jarvisfriend[i].thrusterscode = codigopropulsor;
											if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
												JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
												JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
											}
											JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

										}
										else
										{
											if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_Hover") == false)
											{
												JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
												JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
												plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_Hover", "ironman", 10.0f, 1, 1, 1, 0, -2);

											}
											JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);
										}
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, 0.0f, 0.0f, angle - 180.0f);
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, angle - 180.0f);
									}
								}
							}
							else
							{
								velocidad.x = (dist.x - dist.x)*1.0f;
								velocidad.y = (dist.y - dist.y)*1.0f;
								velocidad.z = (origin.z - dist.z)*1.0f;

								if (velocidad.z != 0.0f)
								{
									if (jarvisfriend[i].isfrozen == true)
									{
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
										jarvisfriend[i].isfrozen = false;
									}
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, velocidad.x, velocidad.y, velocidad.z);
									plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION_VELOCITY, jarvisfriend[i].baseobj, 0.0f, 0.0f, 0.0);
									plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_COORDINATES, jarvisfriend[i].baseobj, &dist.x, &dist.y, &dist.z);

									plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);
								}
								else
								{
									if (jarvisfriend[i].isfrozen == true)
									{
										plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
										plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
										jarvisfriend[i].isfrozen = false;
									}
									jarvisfriend[i].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
								}
								codigopropulsor = flotar;
								if (jarvisfriend[i].thrusterscode != codigopropulsor || jarvisvoice.Has_not_switched_enviroment(&jarvisfriend[i].jarvisprop) == false)
								{
									jarvisfriend[i].thrusterscode = codigopropulsor;
									if (isplayingfly(true, jarvisfriend[i].botstream[0], 1, jarvisfriend[i].jarvisprop.enviroment_ID, 1, i) == true || jarvisfriend[i].jarvisprop.manoizqfx != 0) {
										JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
										JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
									}
									JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);

								}
								else
								{
									if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_Hover") == false)
									{
										JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
										JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
										plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
										plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_Hover", "ironman", 10.0f, 1, 1, 1, 0, -2);

									}
									JarvisVoice::PlayThrustersIDLE(1, i, &jarvisfriend[i].botstream[0], (int)jarvisfriend[i].actorid, settings.folderdirs[pagebelong[i]].suits[jarvisfriend[i].mark], 0, jarvisfriend[i].thrusterscode, &jarvisfriend[i].jarvisprop);
								}
								plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, dist.x - origin1.x, dist.y - origin1.y, &angle);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_ROTATION, jarvisfriend[i].baseobj, 0.0f, 0.0f, angle - 180.0f);
								plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEADING, jarvisfriend[i].baseobj, angle - 180.0f);
							}
						}
						
					}
					else
					{
						if (jarvisfriend[i].jarvisprop.manoizqfx != 0) {
							JarvisVoice::StopThrustersIDLE(&jarvisfriend[i].botstream[0], &jarvisfriend[i].jarvisprop);
							JarvisVoice::storeenviroment(&jarvisfriend[i].jarvisprop.enviroment_ID);
							kill_thrusters(&jarvisfriend[i].jarvisprop);
						}
						if (jarvisfriend[i].isfrozen == true)
						{
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_DYNAMIC, jarvisfriend[i].baseobj, 1);
							plugin::scripting::CallCommandById(COMMAND_FREEZE_OBJECT_POSITION, jarvisfriend[i].baseobj, 0);
							plugin::scripting::CallCommandById(COMMAND_FREEZE_CHAR_POSITION, jarvisfriend[i].actorchar, 0);
							jarvisfriend[i].isfrozen = false;
						}
						if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, jarvisfriend[i].actorchar, "fly_Stop") == false) {
							plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, jarvisfriend[i].actorchar);
							plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, jarvisfriend[i].actorchar, "fly_Stop", "ironman", 10.0f, 0, 0, 0, 0, 500);
						}
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VELOCITY, jarvisfriend[i].baseobj, jarvisfriend[i].velx, jarvisfriend[i].vely, jarvisfriend[i].velz);
					}
				}
			}
			else
			{
				int b = i;
				while (b < dimfriends - 1)
				{
					pagebelong[b] = pagebelong[b + 1];
					jarvisfriend[b].baseobj = jarvisfriend[b + 1].baseobj;
					jarvisfriend[b].thrusterscode = jarvisfriend[b + 1].thrusterscode;
					jarvisfriend[b].velx = jarvisfriend[b + 1].velx;
					jarvisfriend[b].vely = jarvisfriend[b + 1].vely;
					jarvisfriend[b].velz = jarvisfriend[b + 1].velz;
					jarvisfriend[b].audios[0] = jarvisfriend[b + 1].audios[0];
					jarvisfriend[b].audios[1] = jarvisfriend[b + 1].audios[1];
					jarvisfriend[b].audios[2] = jarvisfriend[b + 1].audios[2];
					jarvisfriend[b].audios[3] = jarvisfriend[b + 1].audios[3];
					jarvisfriend[b].audios[4] = jarvisfriend[b + 1].audios[4];
					jarvisfriend[b].audios[5] = jarvisfriend[b + 1].audios[5];
					jarvisfriend[b].audios[6] = jarvisfriend[b + 1].audios[6];
					jarvisfriend[b].botstream[0] = jarvisfriend[b + 1].botstream[0];
					jarvisfriend[b].botstream[1] = jarvisfriend[b + 1].botstream[1];
					jarvisfriend[b].botstream[2] = jarvisfriend[b + 1].botstream[2];
					jarvisfriend[b].mark = jarvisfriend[b + 1].mark;
					jarvisfriend[b].raystime = jarvisfriend[b + 1].raystime;
					jarvisfriend[b].yolohice = jarvisfriend[b + 1].yolohice;
					jarvisfriend[b].ibeam[0] = jarvisfriend[b + 1].ibeam[0];
					jarvisfriend[b].ibeam[1] = jarvisfriend[b + 1].ibeam[1];
					jarvisfriend[b].ibeam[2] = jarvisfriend[b + 1].ibeam[2];
					jarvisfriend[b].hasacar = jarvisfriend[b + 1].hasacar;
					jarvisfriend[b].blip = jarvisfriend[b + 1].blip;
					jarvisfriend[b].raysactive = jarvisfriend[b + 1].raysactive;
					jarvisfriend[b].taskindex = jarvisfriend[b + 1].taskindex;
					jarvisfriend[b].hp = jarvisfriend[b + 1].hp;
					jarvisfriend[b].wpn = jarvisfriend[b + 1].wpn;
					jarvisfriend[b].actorid = jarvisfriend[b + 1].actorid;
					jarvisfriend[b].actorchar = jarvisfriend[b + 1].actorchar;
					jarvisfriend[b].timetoact = jarvisfriend[b + 1].timetoact;
					jarvisfriend[b].jarvisprop.manoizqfx = jarvisfriend[b + 1].jarvisprop.manoizqfx;
					jarvisfriend[b].jarvisprop.manoderfx = jarvisfriend[b + 1].jarvisprop.manoderfx;
					jarvisfriend[b].jarvisprop.pieizqfx = jarvisfriend[b + 1].jarvisprop.pieizqfx;
					jarvisfriend[b].jarvisprop.piederfx = jarvisfriend[b + 1].jarvisprop.piederfx;
					jarvisfriend[b].jarvisprop.enviroment_ID = jarvisfriend[b + 1].jarvisprop.enviroment_ID;
					jarvisfriend[b].isfrozen = jarvisfriend[b + 1].isfrozen;
					jarvisfriend[b].target.actorchar = jarvisfriend[b + 1].target.actorchar;
					jarvisfriend[b].target.actorid = jarvisfriend[b + 1].target.actorid;
					jarvisfriend[b].target.timer = jarvisfriend[b + 1].target.timer;
					jarvisfriend[b].attacker.actorchar = jarvisfriend[b + 1].attacker.actorchar;
					jarvisfriend[b].attacker.actorid = jarvisfriend[b + 1].attacker.actorid;
					jarvisfriend[b].attacker.timer = jarvisfriend[b + 1].attacker.timer;
					jarvisfriend[b].cohete = jarvisfriend[b + 1].cohete;
					jarvisfriend[b].complete = jarvisfriend[b + 1].complete;
					jarvisfriend[b].obstaclex = jarvisfriend[b + 1].obstaclex;
					jarvisfriend[b].obstacley = jarvisfriend[b + 1].obstacley;
					jarvisfriend[b].obstaclez = jarvisfriend[b + 1].obstaclez;
					jarvisfriend[b].contadordist = jarvisfriend[b + 1].contadordist;
					jarvisfriend[b].damagetimer = jarvisfriend[b + 1].damagetimer;
					b++;
				}
				dimfriends -= 1;
			}
			i++;
		}
		goto ending;
}

ending:
{
return true;
}
	
}

bool JarvisVoice::Punchped(int punchedped)
{
	int ped = punchedped;	
	CPed *player = FindPlayerPed(0);
	if (player)
	{
		if (ped > 0)
		{
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_LAST_WEAPON_DAMAGE, ped);
			CVector poss, poss1, poss2;
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 4.0f, 0.0f, &poss.x, &poss.y, &poss.z);
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);
			poss.x -= poss1.x;
			poss.y -= poss1.y;
			poss.z -= poss1.z;
			poss.x *= 10.0f;
			poss.y *= 10.0f;
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, ped, 0.0f, 0.0f, 0.0f, &poss2.x, &poss2.y, &poss2.z);
			plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COORDINATES_DONT_WARP_GANG, ped, poss2.x, poss2.y, poss2.z);
			plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, ped, poss.x, poss.y, poss.z);
			plugin::scripting::CallCommandById(COMMAND_EXPLODE_CHAR_HEAD, ped);
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, ped);
			plugin::scripting::CallCommandById(COMMAND_EXPLODE_CHAR_HEAD, ped);
			punchedped = NULL;
		}
	}
	return true;
}

bool JarvisVoice::Punchcar(int punchedcar)
{
	CPed *player = FindPlayerPed(0);
	if (player)
	{
		if (punchedcar > 0)
		{
			plugin::scripting::CallCommandById(COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE, punchedcar);
			CVector poss, poss1, poss2;
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 4.0f, 0.0f, &poss.x, &poss.y, &poss.z);
			plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);
			poss.x -= poss1.x;
			poss.y -= poss1.y;
			poss.x *= 0.1f;
			poss.y *= 0.1f;
			plugin::scripting::CallCommandById(COMMAND_APPLY_FORCE_TO_CAR, punchedcar, poss.x, poss.y, 0.0f, 0.0f, 0.0f, 0.5f);
			punchedcar = NULL;
		}
	}
	return true;
}

void JarvisVoice::Actionsforpeds(CPed *punchedped) {
	CPed *player = FindPlayerPed(0);
	if (!boolvars.menuisactive)
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, punchedped) == 1)
		{
			static int salud;
			if (punchedped > 0 && (int)punchedped != (int)player && (int)punchedped != boolvars.punchedvictim)
			{
				plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, punchedped, &salud);
				if (salud > 1)
				{


					int a = 0;
					plugin::scripting::CallCommandById(COMMAND_GET_PED_TYPE, punchedped, &a);
					if (a != 0)
					{
						if (punchedped == (CPed *)player->m_pLastEntityDamage)
						{
							cana = punchedped;
						}
						else
						{
							static int level1;
							plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level1);
							if (a == 6 && level1 > 0)
							{
								CVector randomcoppos = punchedped->GetPosition();
								CVector playerpo = player->GetPosition();
								CVector distanciapedcop;
								distanciapedcop.x = randomcoppos.x - playerpo.x;
								distanciapedcop.y = randomcoppos.y - playerpo.y;
								distanciapedcop.z = randomcoppos.z - playerpo.z;
								if (distanciapedcop.Magnitude() < 20.0f && plugin::scripting::CallCommandById(COMMAND_IS_LINE_OF_SIGHT_CLEAR, randomcoppos.x, randomcoppos.y, randomcoppos.z, playerpo.x, playerpo.y, playerpo.z, 1, 1, 0, 1, 0) == true)
								{
									cana = punchedped;
								}
							}
							else
							{
								cana = NULL;
							}
						}
						if (punchedped->m_nPedState != PEDSTATE_DRIVING)
						{
							if (dimenemy > 0)
							{
								int flypedindex = -1;
								int fly2ped = -1;
								for (int u = 0; u < dimenemy; u++)
								{
									if (int(jarvisenemy[u].actorchar) == int(punchedped))
									{
										//boolvars.action = 1;
										flypedindex = u;
										break;
									}
									else
									{
										if (int(jarvisfriend[u].actorchar) == int(punchedped))
										{
											//boolvars.action = 1;
											fly2ped = u;
											break;
										}
									}
								}
								if (flypedindex != -1 || fly2ped != -1)
								{
									if (flypedindex != -1)
									{
										bool y = false;
										int u1 = -1;
										CPed *shooter = NULL;
										for (int k = 0; k < dimfriends; k++)
										{
											if ((int)jarvisfriend[k].actorchar != NULL)
											{
												if ((int)punchedped == (int)jarvisfriend[k].target.actorchar &&
													jarvisfriend[k].yolohice == true &&
													plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, punchedped, jarvisfriend[k].obstaclex, jarvisfriend[k].obstacley, jarvisfriend[k].obstaclez, 2.0f, 2.0f, 2.0f, 0) == true)
												{
													u1 = k;
													shooter = jarvisfriend[k].actorchar;
													y = true;
													break;
												}
											}
										}

										if (shooter != NULL)
										{
											//if (y == true)
											//{
											plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_LAST_WEAPON_DAMAGE, jarvisenemy[flypedindex].actorchar);
											CVector poss, poss1, poss2;
											plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[u1].actorchar, 0.0f, 0.0f, 0.0f, &poss.x, &poss.y, &poss.z);
											plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[flypedindex].actorchar, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);

											poss2.x = (poss1.x - poss.x)*2.1f;
											poss2.y = (poss1.y - poss.y)*2.1f;
											poss2.z = (poss1.z - poss.z)*2.1f;
											//plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[flypedindex].actorchar, 0.0f, 0.0f, 0.0f, &poss2.x, &poss2.y, &poss2.z);

											if (jarvisfriend[u1].yolohice == true &&
												jarvisfriend[u1].target.timer + 500 < CTimer::m_snTimeInMillisecondsNonClipped &&
												jarvisenemy[flypedindex].damagetimer + 500 < CTimer::m_snTimeInMillisecondsNonClipped)
											{
												jarvisfriend[u1].yolohice = false;
												jarvisfriend[u1].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
												jarvisfriend[u1].taskindex = 0;
												jarvisfriend[u1].target.timer = CTimer::m_snTimeInMillisecondsNonClipped;
												jarvisenemy[flypedindex].damagetimer = CTimer::m_snTimeInMillisecondsNonClipped;
												//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, punchedped);
												//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COORDINATES_DONT_WARP_GANG, punchedped, poss2.x, poss2.y, poss2.z);
												//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, punchedped, poss.x, poss.y, poss.z);
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisenemy[flypedindex].actorchar);
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, shooter);
												shooter = NULL;
											}
											else
											{
												jarvisenemy[flypedindex].velx = poss2.x;
												jarvisenemy[flypedindex].vely = poss2.y;
												jarvisenemy[flypedindex].velz = poss2.z;
												float salud = jarvisenemy[flypedindex].actorchar->m_fHealth;
												if (salud >= 1)
												{
													jarvisenemy[flypedindex].actorchar->m_fHealth = salud - 0.01f;
												}
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisenemy[flypedindex].actorchar);
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, shooter);
												shooter = NULL;

											}
										}
										else
										{
											if ((boolvars.wpn == 2 || boolvars.wpn == 4) &&
												boolvars.indx == 22 &&
												boolvars.escudoactivo == false &&
												(CMouseControllerState().lmb == 1 ||
												(GetKeyState(VK_LBUTTON) & 0x8000) == 1 ||
													plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) == true))
											{
												if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, punchedped, boolvars.target.x, boolvars.target.y, boolvars.target.z, 2.0f, 2.0f, 2.0f, 0) == true)
												{
													shooter = FindPlayerPed(0);
													//if (y == true)
													//{
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_LAST_WEAPON_DAMAGE, jarvisenemy[flypedindex].actorchar);
													CVector poss, poss1, poss2;
													plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, shooter, 0.0f, 0.0f, 0.0f, &poss.x, &poss.y, &poss.z);
													plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[flypedindex].actorchar, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);

													poss2.x = (poss1.x - poss.x)*2.0f;
													poss2.y = (poss1.y - poss.y)*2.0f;
													poss2.z = (poss1.z - poss.z)*2.0f;

													//plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[flypedindex].actorchar, 0.0f, 0.0f, 0.0f, &poss2.x, &poss2.y, &poss2.z);

													if (jarvisenemy[flypedindex].damagetimer + 500 < CTimer::m_snTimeInMillisecondsNonClipped)
													{
														jarvisenemy[flypedindex].damagetimer = CTimer::m_snTimeInMillisecondsNonClipped;
														float salud = jarvisenemy[flypedindex].actorchar->m_fHealth;
														if (salud >= 1)
														{
															jarvisenemy[flypedindex].actorchar->m_fHealth = salud - 10.0f;
														}
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisenemy[flypedindex].actorchar);
														//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, punchedped);
														//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COORDINATES_DONT_WARP_GANG, punchedped, poss2.x, poss2.y, poss2.z);
														//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, punchedped, poss.x, poss.y, poss.z);
													}
													else
													{
														jarvisenemy[flypedindex].velx = poss2.x;
														jarvisenemy[flypedindex].vely = poss2.y;
														jarvisenemy[flypedindex].velz = poss2.z;


														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisenemy[flypedindex].actorchar);
													}
												}
												shooter = NULL;
											}/*
											else
											{
												if (JarvisVoice::IsAudioStreamPlayed(IMStream[0], audiostream[15]) == true)
												{
													AudioLib.ChannelStop(IMStream[0].audio);
													IMStream[0].audio = NULL;
												}
											}*/
										}
										//punchedped = NULL;
									}
									else
									{
										bool y = false;
										int u1 = -1;
										CPed *shooter = NULL;
										for (int k = 0; k < dimenemy; k++)
										{
											if ((int)jarvisenemy[k].actorchar != NULL)
											{
												if ((int)punchedped == (int)jarvisenemy[k].target.actorchar &&
													jarvisenemy[k].yolohice == true &&
													plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, punchedped, jarvisenemy[k].obstaclex, jarvisenemy[k].obstacley, jarvisenemy[k].obstaclez, 2.0f, 2.0f, 2.0f, 0) == true)
												{
													u1 = k;
													shooter = jarvisenemy[k].actorchar;
													y = true;
													break;
												}
											}
										}

										if (shooter != NULL)
										{
											//if (y == true)
											//{
											plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_LAST_WEAPON_DAMAGE, jarvisfriend[fly2ped].actorchar);
											CVector poss, poss1, poss2;
											plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisenemy[u1].actorchar, 0.0f, 0.0f, 0.0f, &poss.x, &poss.y, &poss.z);
											plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[fly2ped].actorchar, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);

											poss2.x = (poss1.x - poss.x)*2.1f;
											poss2.y = (poss1.y - poss.y)*2.1f;
											poss2.z = (poss1.z - poss.z)*2.1f;
											//plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[fly2ped].actorchar, 0.0f, 0.0f, 0.0f, &poss2.x, &poss2.y, &poss2.z);

											if (jarvisenemy[u1].yolohice == true &&
												jarvisenemy[u1].target.timer + 500 < CTimer::m_snTimeInMillisecondsNonClipped &&
												jarvisfriend[fly2ped].damagetimer + 500 < CTimer::m_snTimeInMillisecondsNonClipped)
											{
												jarvisenemy[u1].yolohice = false;
												jarvisenemy[u1].timetoact = CTimer::m_snTimeInMillisecondsNonClipped;
												jarvisenemy[u1].taskindex = 0;
												jarvisenemy[u1].target.timer = CTimer::m_snTimeInMillisecondsNonClipped;
												jarvisfriend[fly2ped].damagetimer = CTimer::m_snTimeInMillisecondsNonClipped;
												//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, punchedped);
												//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COORDINATES_DONT_WARP_GANG, punchedped, poss2.x, poss2.y, poss2.z);
												//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, punchedped, poss.x, poss.y, poss.z);
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisfriend[fly2ped].actorchar);
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, shooter);
												shooter = NULL;
											}
											else
											{
												jarvisfriend[fly2ped].velx = poss2.x;
												jarvisfriend[fly2ped].vely = poss2.y;
												jarvisfriend[fly2ped].velz = poss2.z;
												float salud = jarvisfriend[fly2ped].actorchar->m_fHealth;
												if (salud >= 1)
												{
													jarvisfriend[fly2ped].actorchar->m_fHealth = salud - 0.01f;
												}
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisfriend[fly2ped].actorchar);
												//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, shooter);
												shooter = NULL;

											}
										}
										else
										{
											if ((boolvars.wpn == 2 || boolvars.wpn == 4) &&
												boolvars.indx == 22 &&
												boolvars.escudoactivo == false &&
												(CMouseControllerState().lmb == 1 ||
												(GetKeyState(VK_LBUTTON) & 0x8000) == 1 ||
													plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) == true))
											{
												if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, punchedped, boolvars.target.x, boolvars.target.y, boolvars.target.z, 2.0f, 2.0f, 2.0f, 0) == true)
												{
													shooter = FindPlayerPed(0);
													//if (y == true)
													//{
													plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_LAST_WEAPON_DAMAGE, jarvisfriend[fly2ped].actorchar);
													CVector poss, poss1, poss2;
													plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, shooter, 0.0f, 0.0f, 0.0f, &poss.x, &poss.y, &poss.z);
													plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[fly2ped].actorchar, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);

													poss2.x = (poss1.x - poss.x)*2.0f;
													poss2.y = (poss1.y - poss.y)*2.0f;
													poss2.z = (poss1.z - poss.z)*2.0f;

													//plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, jarvisfriend[fly2ped].actorchar, 0.0f, 0.0f, 0.0f, &poss2.x, &poss2.y, &poss2.z);

													if (jarvisfriend[fly2ped].damagetimer + 500 < CTimer::m_snTimeInMillisecondsNonClipped)
													{
														jarvisfriend[fly2ped].damagetimer = CTimer::m_snTimeInMillisecondsNonClipped;
														float salud = jarvisfriend[fly2ped].actorchar->m_fHealth;
														if (salud >= 1)
														{
															jarvisfriend[fly2ped].actorchar->m_fHealth = salud - 10.0f;
														}
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisfriend[fly2ped].actorchar);
														//plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, punchedped);
														//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_COORDINATES_DONT_WARP_GANG, punchedped, poss2.x, poss2.y, poss2.z);
														//plugin::scripting::CallCommandById(COMMAND_SET_CHAR_VELOCITY, punchedped, poss.x, poss.y, poss.z);
													}
													else
													{
														jarvisfriend[fly2ped].velx = poss2.x;
														jarvisfriend[fly2ped].vely = poss2.y;
														jarvisfriend[fly2ped].velz = poss2.z;


														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, jarvisfriend[fly2ped].actorchar);
													}
												}
												shooter = NULL;
											}/*
											 else
											 {
											 if (JarvisVoice::IsAudioStreamPlayed(IMStream[0], audiostream[15]) == true)
											 {
											 AudioLib.ChannelStop(IMStream[0].audio);
											 IMStream[0].audio = NULL;
											 }
											 }*/
										}
									}
								}
							}
							if (punchedped != NULL)
							{
								static int act;

								//0: golpes
								//1: target scape
								//2: acumular target
								//default: golpes
								if (boolvars.punchedtargetsexist == true)
								{
									act = 4;
								}
								else
								{
									if (boolvars.indx != 22)
									{
										if (boolvars.indx != 18 && boolvars.indx != 19)
										{
											act = 0;
										}
										else
										{
											act = 2;
										}
									}
									else
									{
										if (boolvars.wpn == 5 || boolvars.wpn == 0)
										{
											if (act == 0)
											{
												act = 2;
											}
											else
											{
												act = 0;
											}
										}
										else
										{
											act = 0;
										}
									}
								}/*
								if (boolvars.wpn != 4 ||
									plugin::scripting::CallCommandById(COMMAND_IS_BUTTON_PRESSED, 0, 17) == false ||
									boolvars.indx!=22)
								{
									if (JarvisVoice::IsAudioStreamPlayed(IMStream[0], audiostream[15]) == true)
									{
										AudioLib.ChannelStop(IMStream[0].audio);
										IMStream[0].audio = NULL;
									}
								}*/
								switch (act) {
								case 0:
								{
									if (plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_CHAR, punchedped, player) != NULL)
									{
										if (boolvars.suit[3] == true)
										{
											static int wpon, wpon2;
											wpon = punchedped->m_nLastWeaponDamage;
											plugin::scripting::CallCommandById(COMMAND_GET_CURRENT_CHAR_WEAPON, player, wpon2);

											if (wpon2 == wpon &&
												(wpon == 0 ||
													wpon == 1 ||
													wpon == 2 ||
													wpon == 3 ||
													wpon == 4 ||
													wpon == 5 ||
													wpon == 6 ||
													wpon == 7 ||
													wpon == 10 ||
													wpon == 11 ||
													wpon == 12 ||
													wpon == 13 ||
													wpon == 14 ||
													wpon == 15
													))
											{
												boolvars.punchedvictim = CPools::GetPedRef(punchedped);
											}
										}
									}
									break;
								}
								case 2:
								{
									//target ped heads

									float salud;
									int pedfound = CPools::GetPedRef(punchedped);

									//if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, pedfound) == true)
									//{
									if (pedfound == boolvars.ped ||
										plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, punchedped, boolvars.target.x, boolvars.target.y, boolvars.target.z, 1.0f, 1.0f, 1.0f, 0) == 1)
									{
										if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SITTING_IN_ANY_CAR, punchedped) == 0 &&
											plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, punchedped) == 0 &&
											punchedped->m_nPedState != PEDSTATE_DRIVING)
										{
											plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, punchedped, &salud);
											if (salud > 0) {
												boolvars.currenttarg = CPools::GetPedRef(punchedped);
											}
										}
									}
									break;
								}
								case 3:
								{
									//targetted ped scape
									static int tipo, salud, salud1, salud2, weap, c;
									CVector coords = FindPlayerCoors(0);
									float dist;
									boolvars.victim = CPools::GetPedRef(punchedped);
									CVector vicpos;
									if (boolvars.ped > 0) {
										boolvars.victim1 = boolvars.ped;
										boolvars.ped = 0;
									}
									//la vble victim frecuentemente guarda nuevos peds candidatos a levantar las manos 
									//la vble ped sirve para guardar la victima apuntada para checkear cuanto tiempo lleva levantando las manos
									//es frecuentemente comparada con la vble victim1, la cual almacena definitivamente una victima asustada

									if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CHAR_ANY_MEANS_3D, boolvars.victim, boolvars.target.x, boolvars.target.y, boolvars.target.z, 1.0f, 1.0f, 1.0f, 0) == true)
									{
										plugin::scripting::CallCommandById(COMMAND_GET_CHAR_COORDINATES, boolvars.victim, &vicpos.x, &vicpos.y, &vicpos.z);
										plugin::scripting::CallCommandById(COMMAND_GET_DISTANCE_BETWEEN_COORDS_3D, vicpos.x, vicpos.y, vicpos.z, coords.x, coords.y, coords.z, &dist);
										plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, boolvars.victim, &salud);
										if (salud == 0 || dist > 50.0f) {
											if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, boolvars.victim, "handsup") == true)
											{
												plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim);
											}
											boolvars.victim = 0;
											boolvars.ped = 0;
										}
										else
										{
											if (boolvars.victim1 == 0)
											{
												//si no hay nadie levantando las manos, guardemos el objetivo
												boolvars.victim1 = boolvars.victim;
												boolvars.ped = boolvars.victim;
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SITTING_IN_ANY_CAR, boolvars.victim) == true)
												{
													//esta en un auto o muerto
													boolvars.victim = 0;
													boolvars.victim1 = 0;
													boolvars.ped = 0;
												}
												else
												{
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, boolvars.victim) == true)
													{
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim);
														boolvars.victim = 0;
														boolvars.victim1 = 0;
														boolvars.ped = 0;
													}
													else
													{
														plugin::scripting::CallCommandById(COMMAND_GET_PED_TYPE, boolvars.victim, &tipo);
														if (tipo != 3) {
															if (tipo == 4 || tipo == 5 || tipo == 18 || tipo == 19 || tipo == 22)
															{
																if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, boolvars.victim, "handsup") == false)
																{
																	plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, boolvars.victim, &salud2);
																	if (salud2 == 0)
																	{
																		plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim);
																		boolvars.victim1 = 0;
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																	else
																	{
																		if (plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_SPOTTED_CHAR_IN_FRONT, boolvars.victim, player) == true)
																		{
																			c = 0;
																			plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, boolvars.victim, "handsup", "ped", 4.0f, 0, 0, 0, 1, -1);
																			boolvars.victim1 = boolvars.victim;
																			boolvars.ped = boolvars.victim;
																		}
																		else
																		{
																			plugin::scripting::CallCommandById(COMMAND_TASK_SMART_FLEE_CHAR, boolvars.victim, player, 150.0f, -1);
																			boolvars.victim1 = 0;
																			boolvars.victim = 0;
																			boolvars.ped = 0;
																		}
																	}
																}
																else
																{
																	if (salud == salud2 && c < 150)
																	{
																		c++;
																		boolvars.victim1 = boolvars.victim;
																		boolvars.ped = boolvars.victim;
																	}
																	else {
																		plugin::scripting::CallCommandById(COMMAND_TASK_SMART_FLEE_CHAR, boolvars.victim, player, 150.0f, -1);
																		boolvars.victim1 = 0;
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																}
															}
															else
															{
																boolvars.victim1 = 0;
																if (tipo == 6) {
																	int neverwanted = plugin::patch::GetInt(0x969171, 0);

																	if (neverwanted != 1)
																	{
																		int level = 0;
																		plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level);
																		if (level < 3) {
																			level += 1;
																			plugin::scripting::CallCommandById(COMMAND_ALTER_WANTED_LEVEL_NO_DROP, 0, level);
																		}
																	}

																	boolvars.ped = 0;
																	boolvars.victim = 0;
																}
																else
																{
																	plugin::scripting::CallCommandById(COMMAND_GET_CURRENT_CHAR_WEAPON, boolvars.victim, &weap);
																	if (weap != 0 && weap != 1)
																	{
																		plugin::scripting::CallCommandById(COMMAND_TASK_KILL_CHAR_ON_FOOT, boolvars.victim, player);
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																	else
																	{
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																}
															}
														}
														else
														{
															if (tipo != 3)
															{
																//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim);
															}
															//si es tipo player, lo dejo intacto, solo vuelvo la vble cero
															boolvars.victim = 0;
															boolvars.ped = 0;
														}
													}
												}
											}
											if (boolvars.victim1 != boolvars.victim)
											{
												//si alguien levanta las manos y estan apuntando a otro, hacerlo correr en caso de estar vivo

												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, boolvars.victim1, "handsup") == true)
												{
													plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, boolvars.victim1, &salud2);
													if (salud2 == 0)
													{
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim1);
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim1);
														boolvars.victim1 = boolvars.victim;
														boolvars.ped = boolvars.victim;
													}
													else
													{
														plugin::scripting::CallCommandById(COMMAND_TASK_SMART_FLEE_CHAR, boolvars.victim1, player, 150.0f, -1);
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim1);
														boolvars.victim1 = boolvars.victim;
														boolvars.ped = boolvars.victim;
													}
												}
												else
												{
													//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim1);
													boolvars.victim1 = boolvars.victim;
													boolvars.ped = boolvars.victim;
												}
											}
											else
											{
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_SITTING_IN_ANY_CAR, boolvars.victim) == true)
												{
													//esta en un auto o muerto
													//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim);
													boolvars.victim = 0;
													boolvars.victim1 = 0;
													boolvars.ped = 0;
												}
												else
												{
													if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_DEAD, boolvars.victim) == true)
													{
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim);
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim);
														boolvars.victim = 0;
														boolvars.victim1 = 0;
														boolvars.ped = 0;
													}
													else
													{
														plugin::scripting::CallCommandById(COMMAND_GET_PED_TYPE, boolvars.victim, &tipo);
														if (tipo != 3) {
															if (tipo == 4 || tipo == 5 || tipo == 18 || tipo == 19 || tipo == 22)
															{
																if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, boolvars.victim, "handsup") == false)
																{
																	plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, boolvars.victim, &salud2);
																	if (salud2 == 0)
																	{
																		plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim);
																		boolvars.victim1 = 0;
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																	else
																	{
																		if (plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_SPOTTED_CHAR_IN_FRONT, boolvars.victim, player) == true)
																		{
																			c = 0;
																			plugin::scripting::CallCommandById(COMMAND_TASK_PLAY_ANIM_NON_INTERRUPTABLE, boolvars.victim, "handsup", "ped", 4.0f, 0, 0, 0, 1, -1);
																			boolvars.victim1 = boolvars.victim;
																			boolvars.ped = boolvars.victim;
																		}
																		else
																		{
																			plugin::scripting::CallCommandById(COMMAND_TASK_SMART_FLEE_CHAR, boolvars.victim, player, 150.0f, -1);
																			boolvars.victim1 = 0;
																			boolvars.victim = 0;
																			boolvars.ped = 0;
																		}
																	}
																}
																else
																{
																	if (salud == salud2 && c < 150)
																	{
																		c++;
																		boolvars.victim1 = boolvars.victim;
																		boolvars.ped = boolvars.victim;
																	}
																	else {
																		plugin::scripting::CallCommandById(COMMAND_TASK_SMART_FLEE_CHAR, boolvars.victim, player, 150.0f, -1);
																		boolvars.victim1 = 0;
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																}
															}
															else
															{
																boolvars.victim1 = 0;
																if (tipo == 6) {
																	int neverwanted = plugin::patch::GetInt(0x969171, 0);

																	if (neverwanted != 1)
																	{
																		int level = 0;
																		plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level);
																		if (level < 3) {
																			level += 1;
																			plugin::scripting::CallCommandById(COMMAND_ALTER_WANTED_LEVEL_NO_DROP, 0, level);
																		}
																	}

																	boolvars.ped = 0;
																	boolvars.victim = 0;
																}
																else
																{
																	plugin::scripting::CallCommandById(COMMAND_GET_CURRENT_CHAR_WEAPON, boolvars.victim, &weap);
																	if (weap != 0 && weap != 1)
																	{
																		plugin::scripting::CallCommandById(COMMAND_TASK_KILL_CHAR_ON_FOOT, boolvars.victim, player);
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																	else
																	{
																		boolvars.victim = 0;
																		boolvars.ped = 0;
																	}
																}
															}
														}
														else
														{
															if (tipo != 3)
															{
																//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim);
															}
															//si es tipo player, lo dejo intacto, solo vuelvo la vble cero
															boolvars.victim = 0;
															boolvars.ped = 0;
														}
													}
												}
											}
										}
									}
									else
									{
										if (boolvars.victim1 > 0)
										{
											if (plugin::scripting::CallCommandById(COMMAND_DOES_CHAR_EXIST, boolvars.victim1) == true)
											{
												if (plugin::scripting::CallCommandById(COMMAND_IS_CHAR_PLAYING_ANIM, boolvars.victim1, "handsup") == true)
												{
													plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, boolvars.victim1, &salud2);
													if (salud2 == 0) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim1);
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim1);
														boolvars.victim1 = 0;
													}
													else {
														plugin::scripting::CallCommandById(COMMAND_TASK_SMART_FLEE_CHAR, boolvars.victim1, player, 150.0f, -1);
														boolvars.victim1 = 0;
													}
												}
												else
												{
													plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEALTH, boolvars.victim1, &salud2);
													if (salud2 == 0) {
														plugin::scripting::CallCommandById(COMMAND_CLEAR_CHAR_TASKS_IMMEDIATELY, boolvars.victim1);
														//plugin::scripting::CallCommandById(COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED, boolvars.victim1);
														boolvars.victim1 = 0;
													}
													else
													{
														boolvars.victim1 = 0;
													}
												}
											}


											int neverwanted = plugin::patch::GetInt(0x969171, 0);
											{
												if (neverwanted != 1)
												{
													int level = 0;
													plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level);
													if (level < 3) {
														level += 1;
														plugin::scripting::CallCommandById(COMMAND_ALTER_WANTED_LEVEL_NO_DROP, 0, level);
													}
												}
											}
										}
									}
									break;
								}
								default:
								{
									if (plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_CHAR, punchedped, player) != NULL)
									{
										if (boolvars.suit[3] == true)
										{
											static int wpon, wpon2;
											wpon = punchedped->m_nLastWeaponDamage;
											plugin::scripting::CallCommandById(COMMAND_GET_CURRENT_CHAR_WEAPON, player, wpon2);
											if (wpon2 == wpon &&
												(wpon == 0 ||
													wpon == 1 ||
													wpon == 2 ||
													wpon == 3 ||
													wpon == 4 ||
													wpon == 5 ||
													wpon == 6 ||
													wpon == 7 ||
													wpon == 10 ||
													wpon == 11 ||
													wpon == 12 ||
													wpon == 13 ||
													wpon == 14 ||
													wpon == 15
													))
											{/*if (plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 0) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 1) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 2) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 3) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 4) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 5) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 6) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 7) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 10) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 11) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 12) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 13) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 14) != NULL ||
												plugin::scripting::CallCommandById(COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON, punchedped, 15) != NULL)
											{*/
												boolvars.punchedvictim = CPools::GetPedRef(punchedped);
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
}

void JarvisVoice::Actionsforcars(CVehicle *punchedcar)
{
	CPed *player = FindPlayerPed(0);
	if (!boolvars.menuisactive)
	{
		int punchmodl, level = 0;

		if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, punchedcar) == 1)
		{

			plugin::scripting::CallCommandById(COMMAND_GET_CAR_MODEL, punchedcar, &punchmodl);
			plugin::scripting::CallCommandById(COMMAND_STORE_WANTED_LEVEL, 0, &level);

			//if (boolvars.punchedtargetsexist == false)
			//{
			if (level > 0 && punchmodl == 497)
			{
				bool c = false;
				bool vand = false;
				for (int i = 0; i < 10; i++)
				{
					if (punchedcar == polmaver[i] && c == false)
					{
						if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_IN_WATER, polmaver[i]) == false)
						{
							if (polmaver[i]->m_fHealth >= 1)
							{
								c = true;
								break;
							}
							else
							{
								polmaver[i] = NULL;
								vand = true;
							}
						}
						else
						{
							polmaver[i] = NULL;
							vand = true;
						}
					}
				}
				bool d = c;
				bool e = false;
				if (c == false)
				{
					if (punchedcar->m_fHealth < 1 || vand == true)
					{
						bool vande = false;
						int i = 0;
						while (i < 10)
						{
							if (punchedcar == polmaver[i])
							{
								polmaver[i] = NULL;
								vand = true;
							}
							if (NULL != polmissile[i])
							{
								vande = true;
							}
							i++;
						}
						if (vand == true && vande == false)
						{
							int i = 0;
							while (i < dimenemy)
							{
								if ((int)jarvisenemy[i].actorchar != NULL)
								{
									if (jarvisenemy[i].actorchar->m_fHealth >= 1)
									{
										d = true;
										break;
									}
								}
								i++;
							}
							if (d == false && i >= dimenemy)
							{
								//JarvisVoice::PlayVoiceEvent(22);
								//plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, punchedcar);
							}
						}
					}
					else
					{
						if (vand == false)
						{
							AudioLib.PlayVoiceEvent(21);
							for (int i = 0; i < 10; i++)
							{
								if (NULL == polmaver[i])
								{
									polmaver[i] = punchedcar;
									break;
								}
								else
								{
									if (plugin::scripting::CallCommandById(COMMAND_DOES_VEHICLE_EXIST, polmaver[i]) == 1) {
										if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_DEAD, polmaver[i]) == 1)
										{
											plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, polmaver[i]);
											polmaver[i] = punchedcar;
											break;
										}
										else
										{
											if (plugin::scripting::CallCommandById(COMMAND_IS_CAR_IN_WATER, polmaver[i]) == 1)
											{
												plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, polmaver[i]);
												polmaver[i] = punchedcar;
												break;
											}
										}
									}
									else
									{
										polmaver[i] = punchedcar;
										break;
									}
								}
							}
						}
					}
				}
			}
			int act = 0;
			if (boolvars.indx == 22 &&
				boolvars.escudoactivo == false)
			{
				if (boolvars.wpn == 2 || boolvars.wpn == 4)
				{
					if (boolvars.isshootin == true)
					{
						if (dimfriends > 0)
						{
							if (act == 4)
							{
								act = 5;
							}
							else
							{
								act = 4;
							}
						}
						else
						{
							act = 4;
						}
					}
					else
					{
						if (dimfriends > 0)
						{
							act = 5;
						}
						else
						{
							act = 1;
						}
					}
				}
				else
				{
					if (!boolvars.killedtargetsexist)
					{
						if (boolvars.wpn == 5)
						{
							if (dimfriends > 0)
							{
								act = 5;//iniciar bots
							}
							else
							{
								act = 1;//golpes
							}
						}

						if (boolvars.wpn == 3 || boolvars.wpn == 0) {
							if (dimfriends > 0)
							{
								if (act == 3)//una vez concluida la accion de guardado del objetivo
								{
									act = 5;//iniciar bots
								}
								else
								{
									act = 3;//inicio el guardado del objetivo (cars)
								}
							}
							else
							{
								act = 3;//inicio el guardado del objetivo (cars)
							}
						}
					}
				}

			}
			else
			{
				if (dimfriends > 0)
				{
					if (act == 1)
					{
						act = 5;
					}
					else
					{
						act = 1;
					}
				}
			}

			//0: golpes
			//1: golpes
			//2: golpes
			//3: mira target
			//4: empujar con repulsor
			//5: for bots
			switch (act)
			{
			case 0: {
				//plugin::scripting::CallCommandById(COMMAND_SET_CAR_CAN_BE_DAMAGED, punchedcar, 1);
				if (plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_CHAR, punchedcar, player) != NULL)
				{
					if (boolvars.suit[3] == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 0) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 1) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 2) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 3) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 4) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 5) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 6) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 7) != NULL)
						{
							boolvars.punchedcar = CPools::GetVehicleRef(punchedcar);
						}
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE, punchedcar);
					}
				}
				break;
			}
			case 1: {
				//plugin::scripting::CallCommandById(COMMAND_SET_CAR_CAN_BE_DAMAGED, punchedcar, 1);
				if (plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_CHAR, punchedcar, player) != NULL)
				{
					if (boolvars.suit[3] == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 0) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 1) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 2) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 3) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 4) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 5) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 6) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 7) != NULL)
						{
							boolvars.punchedcar = CPools::GetVehicleRef(punchedcar);
						}
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE, punchedcar);
					}
				}
				break;
			}
			case 2: {
				//plugin::scripting::CallCommandById(COMMAND_SET_CAR_CAN_BE_DAMAGED, punchedcar, 1);
				if (plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_CHAR, punchedcar, player) != NULL)
				{
					if (boolvars.suit[3] == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 0) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 1) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 2) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 3) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 4) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 5) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 6) != NULL ||
							plugin::scripting::CallCommandById(COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON, punchedcar, 7) != NULL)
						{
							boolvars.punchedcar = CPools::GetVehicleRef(punchedcar);
						}
					}
					else
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE, punchedcar);
					}
				}
				break;
			}
			case 3:
			{
				if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CAR_3D, punchedcar, boolvars.target.x, boolvars.target.y, boolvars.target.z, 5.0f, 5.0f, 5.0f, 0) == true)
				{
					/*static int salud;
					plugin::scripting::CallCommandById(COMMAND_GET_CAR_HEALTH, punchedcar, &salud);
					if (salud >= 0) {*/
					boolvars.currenttarg = CPools::GetVehicleRef(punchedcar);
					//}
				}
				break;
			}
			case 4:
			{
				if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CAR_3D, punchedcar, boolvars.target.x, boolvars.target.y, boolvars.target.z, 5.0f, 5.0f, 5.0f, 0) == true)
				{

					plugin::scripting::CallCommandById(COMMAND_SET_CAR_CAN_BE_DAMAGED, punchedcar, 1);
					if (boolvars.suit[3] == true)
					{
						plugin::scripting::CallCommandById(COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE, punchedcar);
						CVector poss, poss1, poss2;
						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 2.0f, 0.0f, &poss.x, &poss.y, &poss.z);
						plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, player, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);
						poss.x -= poss1.x;
						poss.y -= poss1.y;
						poss.x *= 0.1f;
						poss.y *= 0.1f;
						plugin::scripting::CallCommandById(COMMAND_APPLY_FORCE_TO_CAR, punchedcar, poss.x, poss.y, 0.0f, 0.0f, 0.0f, 0.2f);
						//plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, punchedcar);
					}
				}
				break;
			}
			case 5:
			{

				CPed *shooter = NULL;
				for (int u = 0; u < dimfriends; u++)
				{
					if (jarvisfriend[u].yolohice == true)
					{
						if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CAR_3D, punchedcar, jarvisfriend[u].obstaclex, jarvisfriend[u].obstacley, jarvisfriend[u].obstaclez, 5.0f, 5.0f, 5.0f, 0) == true)
						{
							jarvisfriend[u].yolohice = false;
							boolvars.action = 1;
							if ((int)jarvisfriend[u].actorchar != NULL)
							{
								shooter = jarvisfriend[u].actorchar;
								break;
							}
						}
					}
				}
				if (shooter != NULL)
				{
					plugin::scripting::CallCommandById(COMMAND_SET_CAR_CAN_BE_DAMAGED, punchedcar, 1);
					plugin::scripting::CallCommandById(COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE, punchedcar);
					CVector poss, poss1, poss2;
					plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, shooter, 0.0f, 2.0f, 0.0f, &poss.x, &poss.y, &poss.z);
					plugin::scripting::CallCommandById(COMMAND_GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS, shooter, 0.0f, 0.0f, 0.0f, &poss1.x, &poss1.y, &poss1.z);
					poss.x -= poss1.x;
					poss.y -= poss1.y;
					poss.x *= 0.1f;
					poss.y *= 0.1f;
					plugin::scripting::CallCommandById(COMMAND_APPLY_FORCE_TO_CAR, punchedcar, poss.x, poss.y, 0.0f, 0.0f, 0.0f, 0.2f);
					//plugin::scripting::CallCommandById(COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED, punchedcar);
				}
				break;
			}
			default:
			{
				if (plugin::scripting::CallCommandById(COMMAND_LOCATE_CAR_3D, punchedcar, boolvars.target.x, boolvars.target.y, boolvars.target.z, 5.0f, 5.0f, 5.0f, 0) == true)
				{
					boolvars.currenttarg = CPools::GetVehicleRef(punchedcar);
				}
				break;
			}
			}
		}
	}
}

static CObject *parachute;

bool JarvisVoice::processparachute()
{
	static int timr;
	if (parachute != NULL)
	{
		if (plugin::scripting::CallCommandById(COMMAND_IS_OBJECT_PLAYING_ANIM, parachute, "PARA_OPEN_O") == true)
		{
			static float timeopen;
			plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_ANIM_CURRENT_TIME, parachute, "PARA_OPEN_O", &timeopen);
			/*
			number = new char[10];
			sprintf(number, "%f", timeopen);
			CFont::SetBackground(0, 0);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetProportional(true);
			CFont::SetJustify(false);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetEdge(2);
			CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
			CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
			CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), number);
			delete[] number;*/
			if (timeopen < 0.150)
			{
				if (parachute->IsVisible() == true)
				{
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 0);
					timr = CTimer::m_snTimeInMillisecondsNonClipped;
				}/*
				else
				{
					if (CTimer::m_snTimeInMillisecondsNonClipped > timr + 200)
					{
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 1);
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, parachute);
						parachute = NULL;
						return true;
					}
				}*/
			}
			else
			{
				plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 1);
				plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, parachute);
				parachute = NULL;
			}
			return true;
		}
		else
		{
			plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 0);
			//plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, parachute);
			//parachute = NULL;
			return true;
		}
	}
	return true;
}

bool JarvisVoice::Actionsforobjs(CObject *objt) {
	int mdl = 0;
	if ((int)objt > 0 && (int)objt <= 89344)
	{
		if (plugin::scripting::CallCommandById(COMMAND_DOES_OBJECT_EXIST, objt) == 1)
		{
			plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_MODEL, objt, &mdl);
			//mdl = objt->m_nModelIndex;
			//int objt = CPools::GetObjectRef(obj);
			if (mdl != 3131)
			{
				if (plugin::scripting::CallCommandById(COMMAND_HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON, objt, 0) == 1 ||
					plugin::scripting::CallCommandById(COMMAND_HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON, objt, 1) == 1)
				{
					if (boolvars.suit[3] == true)
					{
						//plugin::scripting::CallCommandById(COMMAND_CLEAR_OBJECT_LAST_WEAPON_DAMAGE, objt);
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_HEALTH, objt, 0);
						plugin::scripting::CallCommandById(COMMAND_BREAK_OBJECT, objt, 5);
						plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, objt);

						return true;
					}
					else
					{
						//plugin::scripting::CallCommandById(COMMAND_CLEAR_OBJECT_LAST_WEAPON_DAMAGE, objt);
						//plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, objt);
						return true;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				parachute = objt;
				return true;
			}
		}
	}
}
/*
	{
		if (parachute != NULL)
		{
			if (plugin::scripting::CallCommandById(COMMAND_IS_OBJECT_PLAYING_ANIM, parachute, "PARA_OPEN_O") == true)
			{
				static float timeopen;
				plugin::scripting::CallCommandById(COMMAND_GET_OBJECT_ANIM_CURRENT_TIME, parachute, "PARA_OPEN_O", &timeopen);
				number = new char[10];
				sprintf(number, "%f", timeopen);
				CFont::SetBackground(0, 0);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetProportional(true);
				CFont::SetJustify(false);
				CFont::SetColor(CRGBA(255, 255, 255, 255));
				CFont::SetFontStyle(FONT_SUBTITLES);
				CFont::SetEdge(2);
				CFont::SetCentreSize(SCREEN_WIDTH + SCREEN_COORD(-350));
				CFont::SetScale(SCREEN_MULTIPLIER(settings.vecSubtitlesScale.x), SCREEN_MULTIPLIER(settings.vecSubtitlesScale.y));
				CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(settings.fSubtitlesWidePosnY), number);
				delete[] number;
				if (timeopen < 0.200)
				{
					if (parachute->IsVisible() == true)
					{
						plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 0);
						timr = CTimer::m_snTimeInMillisecondsNonClipped;
					}
					else
					{
						if (CTimer::m_snTimeInMillisecondsNonClipped > timr + 200)
						{
							plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 1);
							plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, parachute);
							parachute = NULL;
							goto endofobjtask1;
						}
					}
					//plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, obj);
					//parax = NULL;
				}
				else
				{
					plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, parachute, 1);
					plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, parachute);
					parachute = NULL;
				}
				goto endofobjtask1;
				//}
			}
			else
			{
				//plugin::scripting::CallCommandById(COMMAND_SET_OBJECT_VISIBLE, obj, 0);
				//plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, obj);

				//parax = NULL;
				plugin::scripting::CallCommandById(COMMAND_MARK_OBJECT_AS_NO_LONGER_NEEDED, parachute);
				parachute = NULL;
				goto endofobjtask1;
			}
		}
		else
		{
			goto endofobjtask1;
		}
	}


	endofobjtask1:*/