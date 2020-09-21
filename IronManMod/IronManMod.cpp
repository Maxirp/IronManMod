// GTA RWTEX file loader example by The_GTA.
// Contains useful utilities if you want to load RenderWare textures natively.
// Uses DK22Pac's plugin sdk.

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define WINVER _WIN32_WINNT_WINXP
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define NTDDI_VERSION NTDDI_WINXP
#include <Windows.h>

#include "Main.hpp"

#include <game_sa/CSprite2d.h>
#include <game_sa/RenderWare.h>
#include <game_sa/CFileMgr.h>
// plugin-sdk

#include <plugin.h>

// mobile-hud
#include "Movtextures.h"
#include "MobilePlayerInfo.h"
#include "MobileRadar.h"
#include "MobileColors.h"
#include "MobileSubtitles.h"
#include "MobileRadioName.h"
#include "MobileTextBox.h"
#include "MobileMissionTimers.h"
#include "MobileProgressBar.h"
#include "MobileAreaName.h"
#include "MobileVehicleName.h"
#include "MobileMenuSystem.h"
#include "MobileLoadingScreen.h"
#include "MobileTextures.h"
#include "MobileStatsBox.h"
#include "JarvisVoice.h"
#include "Settings.h"
#include "CFont.h"
#include "extensions\ScriptCommands.h"
#include "CPad.h"
#include <math.h>
#include "game_sa\CRunningScript.h"
#include "game_sa\common.h"
#include "game_sa\CTimer.h"
#include "Utility.h"
#include "CPools.h"
#include "CPed.h"
#include "game_sa\CFileLoader.h"
#include "CDebug.h"

static float RADAR_RANGE = 150.0f;
static int RADAR_COLOUR_A = 170;

using namespace plugin;

void __declspec(naked) RadarAlpha() {
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
	_asm {
		mov     ecx, 586420h
		mov     al, [esp + 140h - 12Dh]
		test    al, al
		jmp     ecx
	}
}

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

static RwTexture* loadRWTEXFromDisc(const char *path)
{
	RwTexture *texHandle = NULL;

	RwStream *imgStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, (void*)path);

	if (imgStream)
	{
		texHandle = RwTextureStreamReadRockstar(imgStream);

		RwStreamClose(imgStream, NULL);
	}

	return texHandle;
}


/*=========================================================
RwFindTexture

Arguments:
name - primary name of the texture you want to find
maskName - name of the alpha mask (unused shit)
Purpose:
Scans the global and the current TXD containers for a
matching texture. If the TXD is found in the current TXD
(m_findInstanceRef), it is referenced and returned to the
caller. If not, the global texture container is researched.
Textures in the global environment are not referenced upon
return but are put into the current texture container if it
is set. The global texture environment is either like a
temporary storage for textures or a routine to dynamically
create them and give to models.
Note:
GTA:SA does not use the global texture routine.
Binary offsets:
(1.0 US): 0x007F3AC0
(1.0 EU): 0x007F3B00
=========================================================*/
typedef RwTexture* (__cdecl *RwReadGlobalTexture_t) (const char *name, const char *secName);
typedef RwTexture* (__cdecl *RwReadLocalTexture_t) (const char *name);

struct rwTextureManager
{
	RwLinkList                      globalTxd;                              // 0
	void*                           pad;                                    // 8
	RwFreeList*                     txdStruct;                              // 12
	RwTexDictionary*                current;                                // 16
	RwReadGlobalTexture_t           readGlobal;                             // 20
	RwReadLocalTexture_t            readLocal;                              // 24
};

inline int GetRWTextureManagerOffset(void)
{
	return *(int*)0x00C97B4C;
}

// Macro to get a plugin struct from a RenderWare object.
template <typename structType, typename rwobjType>
structType* RW_PLUGINSTRUCT(rwobjType *obj, size_t pluginOffset)
{
	return (structType*)((char*)obj + pluginOffset);
}

RwTexture* RwFindTexture(const char *name, const char *maskName)
{
	void *rwInterface = RwEngineInstance;

	// First try to find thing in our textures :)
	{
		if (strcmp(name, "radar_centre") == 0)
		{
			std::string texPath = CFileMgr::ms_rootDirName;
			texPath += "IronMan/textures/";
			texPath += name;
			texPath += ".rwtex";

			if (RwTexture *texHandle = loadRWTEXFromDisc(texPath.c_str()))
			{
				// We can just return it.
				// We do not care about caching it.
				return texHandle;
			}
		}
		
	}

	rwTextureManager *rwTexMan = RW_PLUGINSTRUCT <rwTextureManager>(rwInterface, GetRWTextureManagerOffset());

	// The local store will reference textures
	if (RwTexture *tex = rwTexMan->readLocal(name))
	{
		tex->refCount++;
		return tex;
	}

	// We could also get a texture from the global store.
	if (RwTexture *tex = rwTexMan->readGlobal(name, maskName))
	{
		if (RwTexDictionary *txd = rwTexMan->current)
		{
			// This is a caching feature.
			RwTexDictionaryRemoveTexture(tex);
			RwTexDictionaryAddTexture(txd, tex);
		}

		return tex;
	}

	// If we have not found anything, we tell the system about an error
	RwError err;
	err.pluginID = 0x01;
	err.errorCode = 0x16;

	// Actually, there is a missing texture handler; it is void though
	RwErrorSet(&err);
	return NULL;
}




//static void threadFunc();
/*
static HMODULE GetThisDllHandle()
{
MEMORY_BASIC_INFORMATION info;
size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetThisDllHandle, &info, sizeof(info));
assert(len == sizeof(info));
return len ? (HMODULE)info.AllocationBase : NULL;
}

class InitializerClass {
public:
InitializerClass() {
// some code to initialize your plugin (will be executed at game start)
HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)threadFunc, GetThisDllHandle(), NULL, NULL);
SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
}

~InitializerClass() {
// some code to deinitialize your plugin (will be executed at game closing)
}
};*/
#include "CExplosion.h"

#include "game_sa\FxManager_c.h"
//static FxSystem_c *__fastcall MyCreateExplosionFx(CExplosion *explosion, int, char *name, RwV3d *point, RwMatrixTag *mat, unsigned char flag);

//extern ThiscallEvent <AddressList<0x73723E, H_CALL>, PRIORITY_BEFORE, ArgPickN<CExplosion*, 0>, void(CExplosion*)>  renderexplosion;
/*
BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{*/

bool AUDIOLIB::isset() {
	if (library != 0)
	{
		if (!boolvars.cannothitcar)
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
/*
HWND OnCreateMainWindow(HINSTANCE hinst)
{
	if (HIWORD(AudioLib.GetVersion()) != BASSVERSION) Error("An incorrect version of bass.dll has been loaded");
	LOGEAR("Creating main window...");
	HWND wnd = CreateMainWindow(hinst);
	if (!AudioLib.Init(wnd)) LOGEAR("CSoundSystem::Init() failed. Error code: %d", AudioLib.ErrorGetCode());
	return wnd;
}

void AUDIOLIB::Inject()
{
	LOGEAR("Injecting SoundSystem...");

	CreateMainWindow = (HWND(__cdecl *)(HINSTANCE hinst))0x00745560;
	plugin::patch::RedirectCall(0x007487A8, OnCreateMainWindow);
	
	auto addr = 0x00748454;
	static const auto pWindowProcHook = &HOOK_DefWindowProc;
	DWORD ptr;
	inj.MemoryRead(addr, ptr);
	inj.MemoryRead(ptr, imp_DefWindowProc);
	inj.MemoryWrite(addr, (DWORD)&pWindowProcHook);
}*/

#include "../game_sa/rw/skeleton.h"
#include "WinUser.h"

void pauseresume()
{
	AudioLib.userPaused = (bool *)0x00B7CB49;
	AudioLib.codePaused = (bool *)0x00B7CB48;
	MSG *msg;
		if (GetMessageA(msg, psGlobalType().window, 0, 0))
		{			
			if (AudioLib.isset())
			{
				// pause streams if the window loses focus, or if SA found any other reason to pause
				if (AudioLib.codePaused || AudioLib.userPaused) JarvisVoice::PauseAllStreams();
				else
				{
					switch (msg->message)
					{
					case WM_ACTIVATE:
						JarvisVoice::ResumeAllStreams();
						break;
					case WM_KILLFOCUS:
						JarvisVoice::PauseAllStreams();
						break;
					}
				}
			}
		}
	
}

class MobileHud {
	injector::scoped_jmp findTexture_hook;

public:

	static void FontTextures(int index) {
		CTxdStore::LoadTxd(index, fontfilename);
	}

	static void FontData() {
		CFileMgr::OpenFile(fontdatafilename, "rb");
	}
	
	static void threadFunc() {
		jarvisvoice.superpowersironman();
	}

	static CRGBA * __fastcall SetRadarDisc(CRGBA *rgba, int, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		rgba->r = 255;
		rgba->g = 255;
		rgba->b = 255;
		rgba->a = 255;
		return rgba;
	}

	static void ProcessExplosions()
	{
		CPed *player = FindPlayerPed(0);
		if (player)
		{
			//CExplosion lastexpl;
			CVector ppos = player->GetPosition();
			CVector epos;
			static float distlastex = 10.0f;
			
			//plugin::patch::GetRaw(13142352, &lastexpl, 0x7C, 0);
			CExplosion *lastexpl = (CExplosion *)0xC88950;
			static int timeexploded;
			for (int i = 0; i < 16; i++)
			{
				epos = lastexpl[i].m_vecPosition;
				float distnewexpl = DistanceBetweenPoints(epos, ppos);
				if (distnewexpl < distlastex
					&& lastexpl[i].m_nType != 8
					&& lastexpl[i].m_nType != 9
					&& lastexpl[i].m_nFuelTimer < 100)
				{
					CVector2D explhead;
					distlastex = distnewexpl;
					explhead.x = ppos.x - epos.x;
					explhead.y = ppos.y - epos.y;

					static float explhd;
					plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, explhead.x, explhead.y, &explhd);
					
					static float hadin;
					plugin::scripting::CallCommandById(COMMAND_GET_CHAR_HEADING, player, &hadin);

					if (explhd>hadin - 90.0f && explhd<hadin + 90.0f)
						boolvars.vuelaatras = true;
					else
						boolvars.vuelaatras = false;

					boolvars.cohetebotexplotado = true;
					plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, ppos.x - epos.x, ppos.y - epos.y, &boolvars.anglecohete);
					break;
				}
			}
		}
	}

	static FxSystem_c *__fastcall MyCreateExplosionFx(CExplosion *explosion, int, char *name, RwV3d *point, RwMatrixTag *mat, unsigned char flag) {
		CVector ppos = FindPlayerCoors(0);
		CVector epos;
		epos.x = point->x;
		epos.y = point->y;
		epos.z = point->z;
		if (DistanceBetweenPoints(epos, ppos) < 10.0f
			&& explosion->m_nType != 8
			&& explosion->m_nType != 9)
		{
			//boolvars.explosionpos.x = epos.x;
			//boolvars.explosionpos.y = epos.y;
			//boolvars.explosionpos.z = epos.z;
			//boolvars.expltype = explosion->m_nType;
			int error= rand() % 2;
			if (error == 1)
				boolvars.systemerror = true;

			boolvars.cohetebotexplotado = true;
			plugin::scripting::CallCommandById(COMMAND_GET_HEADING_FROM_VECTOR_2D, ppos.x - epos.x, ppos.y - epos.y, &boolvars.anglecohete);
		}
		return g_fxMan.CreateFxSystem(name, point, mat, flag);
	}
	//scriptRegisterAdditionalThread(HMODULE hModule, threadFunc);

		

	MobileHud() {
		static bool in;
		if (!in)
		{
			MainPath = CFileMgr::ms_rootDirName;
			in = true;
		}

		Ini();
		patch::RedirectCall(0x5BA6A4, MobileHud::FontTextures);
		patch::RedirectCall(0x7187DB, MobileHud::FontData);
		//FILE* _f = fopen("CLEO//test_script.cc", "r");
		//fseek(_f, 0, SEEK_END);
		//int fsize = ftell(_f);
		//fseek(_f, 0, SEEK_SET);
		//script_buffer = malloc(1024 + 4);
		//memset(script_buffer, 0, fsize + 4);
		//fread(script_buffer, fsize, 1, _f);
		//plugin::Events::gameProcessEvent.before -= threadFunc;

		plugin::patch::RedirectCall(0x53E981, threadFunc);

		if (KeyPressed(118))
		{
			exit(1);
		}

		// Set Radar Range
		patch::SetFloat(0x586C9B, RADAR_RANGE);
		patch::Set<const void*>(0x586C66, &RADAR_RANGE);

		// Transparent Radar
		patch::Set<DWORD>(0x5864BD, RADAR_COLOUR_A);
		plugin::patch::RedirectJump(0x58641A, RadarAlpha);

		//Radar Disc RGBA
		patch::RedirectCall(0x58A8BD, &SetRadarDisc);
		patch::RedirectCall(0x58A813, &SetRadarDisc);
		patch::RedirectCall(0x58A967, &SetRadarDisc);
		patch::RedirectCall(0x58AA15, &SetRadarDisc);

		findTexture_hook = injector::scoped_jmp(0x007F3AC0, RwFindTexture, true);

		static bool once;
		if (!once)
		{
			Events::attachRwPluginsEvent.before += [] {
				bool versionfina0 = false;
				bool versionfina1 = false;
				bool versionfina2 = false;
				bool versionfina3 = false;
				bool versionfina4 = false;
				bool versionfina5 = false;
				bool versionfina6 = false;
				bool versionfina7 = false;
				bool versionfina8 = false;
				bool versionfina9 = false;

				bool versionfina10 = false;
				bool versionfina11 = false;
				bool versionfina12 = false;
				bool versionfina13 = false;
				bool versionfina14 = false;
				bool versionfina15 = false;
				bool versionfina16 = false;
				bool versionfina17 = false;
				bool versionfina18 = false;
				bool versionfina19 = false;

				bool versionfina20 = false;
				bool versionfina21 = false;
				bool versionfina22 = false;
				bool versionfina23 = false;
				bool versionfina24 = false;
				bool versionfina25 = false;
				bool versionfina26 = false;
				bool versionfina27 = false;
				FILE *file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\mansion.ide"), "r");
				bool versionisold = false;
				if (file != NULL) {
					versionisold = true;
					fclose(file);
				}
				else
				{
					file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\mansion.ipl"), "r");
					if (file != NULL) {
						versionisold = true;
						fclose(file);
					}
					else
					{
						file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\suit_pickup.cs"), "r");
						if (file != NULL) {
							versionisold = true;
							fclose(file);
						}
						else
						{
							file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\suit_pickup.cs"), "r");
							if (file != NULL) {
								versionisold = true;
								fclose(file);
							}
							else
							{
								file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\starksave.cs"), "r");
								if (file != NULL) {
									versionisold = true;
									fclose(file);
								}
								else
								{
									file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\Mansion.cs"), "r");
									if (file != NULL) {
										versionisold = true;
										fclose(file);
									}
									else
									{
										file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\juke_music.cs"), "r");
										if (file != NULL) {
											versionisold = true;
											fclose(file);
										}
										else
										{
											file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\parachute_pickup.cs"), "r");
											if (file != NULL) {
												versionisold = true;
												fclose(file);
											}
											else
											{
												file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\IMCars.cs"), "r");
												if (file != NULL) {
													versionisold = true;
													fclose(file);
												}
												else
												{
													file = fopen(GAME_PATH("modloader\\IronManMod\\Mansion stark\\CLEO\\juke_music.cs"), "r");
													if (file != NULL) {
														versionisold = true;
														fclose(file);
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
				if (versionisold == false)
				{
					file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\ironman.ifp"), "r");
					if (file != NULL) {
						fclose(file);
						file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\mkq.ifp"), "r");
						if (file != NULL) {
							fclose(file);
							file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\mkq2.ifp"), "r");
							if (file != NULL) {
								fclose(file);
								file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\mkq3.ifp"), "r");
								if (file != NULL) {
									fclose(file);
									file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\mkq4.ifp"), "r");
									if (file != NULL) {
										fclose(file);
										file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\parachute.ifp"), "r");
										if (file != NULL) {
											fclose(file);
											file = fopen(GAME_PATH("modloader\\IronManMod\\Animation pack\\sword.ifp"), "r");
											if (file != NULL) {
												fclose(file);
												versionfina0 = true;
											}
										}
									}
								}
							}
						}
					}
					if (versionfina0 == true)
					{
						file = fopen(GAME_PATH("modloader\\IronManMod\\Avengers tower\\fighotbase_LAn.dff"), "r");
						if (file != NULL) {
							fclose(file);
							file = fopen(GAME_PATH("modloader\\IronManMod\\Avengers tower\\LODhotbase_LAn.dff"), "r");
							if (file != NULL) {
								fclose(file);
								file = fopen(GAME_PATH("modloader\\IronManMod\\Avengers tower\\lan_3.col"), "r");
								if (file != NULL) {
									fclose(file);
									file = fopen(GAME_PATH("modloader\\IronManMod\\Avengers tower\\lanlod.txd"), "r");
									if (file != NULL) {
										fclose(file);
										file = fopen(GAME_PATH("modloader\\IronManMod\\Avengers tower\\pershingsq.txd"), "r");
										if (file != NULL) {
											fclose(file);
											versionfina1 = true;
										}
									}
								}
							}
						}
						if (versionfina1 == true)
						{
							file = fopen(GAME_PATH("modloader\\IronManMod\\Cars\\rccam1.txd"), "r");
							if (file != NULL) {
								fclose(file);
								file = fopen(GAME_PATH("modloader\\IronManMod\\Cars\\rccam.txd"), "r");
								if (file != NULL) {
									fclose(file);
									file = fopen(GAME_PATH("modloader\\IronManMod\\Cars\\rccam.dff"), "r");
									if (file != NULL) {
										fclose(file);
										versionfina2 = true;
									}
								}
							}
							if (versionfina2 == true)
							{
								file = fopen(GAME_PATH("modloader\\IronManMod\\CLEO\\IMCars.cs"), "r");
								if (file != NULL) {
									fclose(file);
									file = fopen(GAME_PATH("modloader\\IronManMod\\CLEO\\IMMansion.cs"), "r");
									if (file != NULL) {
										fclose(file);
										file = fopen(GAME_PATH("modloader\\IronManMod\\CLEO\\IMMusic.cs"), "r");
										if (file != NULL) {
											fclose(file);
											file = fopen(GAME_PATH("modloader\\IronManMod\\CLEO\\IMPickups.cs"), "r");
											if (file != NULL) {
												fclose(file);
												file = fopen(GAME_PATH("modloader\\IronManMod\\CLEO\\IMSave.cs"), "r");
												if (file != NULL) {
													fclose(file);
													file = fopen(GAME_PATH("modloader\\IronManMod\\CLEO\\IMSuits.cs"), "r");
													if (file != NULL) {
														fclose(file);
														versionfina3 = true;
													}
												}
											}
										}
									}
								}
								if (versionfina3 == true)
								{
									file = fopen(GAME_PATH("modloader\\IronManMod\\data\\Mansion.ide"), "r");
									if (file != NULL) {
										fclose(file);
										file = fopen(GAME_PATH("modloader\\IronManMod\\data\\Mansion.ipl"), "r");
										if (file != NULL) {
											fclose(file);
											file = fopen(GAME_PATH("modloader\\IronManMod\\data\\animgrp.dat"), "r");
											if (file != NULL) {
												fclose(file);
												versionfina4 = true;
											}
										}
									}
									if (versionfina4 == true)
									{
										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\IronMan_Mod.ini"), "r");
										if (file != NULL) {
											fclose(file);
											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\hudcolor.dat"), "r");
											if (file != NULL) {
												fclose(file);
												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\maxirpHud.dat"), "r");
												if (file != NULL) {
													fclose(file);
													versionfina5 = true;
												}
											}
										}
										if (versionfina5 == true)
										{
											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\maxirpscs.txd"), "r");
											if (file != NULL) {
												fclose(file);
												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\radar_centre.rwtex"), "r");
												if (file != NULL) {
													fclose(file);
													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\menu_background.txd"), "r");
													if (file != NULL) {
														fclose(file);
														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\menu_slider.txd"), "r");
														if (file != NULL) {
															fclose(file);
															versionfina6 = true;
														}
													}
												}
											}
											if (versionfina6 == true)
											{
												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\beating.png"), "r");
												if (file != NULL) {
													fclose(file);
													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\beating.png"), "r");
													if (file != NULL) {
														fclose(file);
														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\DAMAGE.png"), "r");
														if (file != NULL) {
															fclose(file);
															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\FLIGHT.png"), "r");
															if (file != NULL) {
																fclose(file);
																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\horizon.png"), "r");
																if (file != NULL) {
																	fclose(file);
																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_bottom.png"), "r");
																	if (file != NULL) {
																		fclose(file);
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_righ1.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_right.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_right01.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_right02.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_spin1.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_spin2.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_static3.png"), "r");
																								if (file != NULL) {
																									fclose(file);
																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_static4.png"), "r");
																									if (file != NULL) {
																										fclose(file);
																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\HUD_static5.png"), "r");
																										if (file != NULL) {
																											fclose(file);
																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\ONLINE.png"), "r");
																											if (file != NULL) {
																												fclose(file);
																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\targetfly.png"), "r");
																												if (file != NULL) {
																													fclose(file);
																													versionfina7 = true;
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
												if (versionfina7 == true)
												{
													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\HUD_health.png"), "r");
													if (file != NULL) {
														fclose(file);
														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\HUD_Tony.png"), "r");
														if (file != NULL) {
															fclose(file);
															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\HUD_Pepper.png"), "r");
															if (file != NULL) {
																fclose(file);
																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\HUD_Rhodey.png"), "r");
																if (file != NULL) {
																	fclose(file);
																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\HUD_righ2.png"), "r");
																	if (file != NULL) {
																		fclose(file);
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\HUD_righ3.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\statbars_armed.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\statbars_unarmed.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\stats\\statbars_null.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						versionfina8 = true;
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
													if (versionfina8 == true)
													{
														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\bullets.png"), "r");
														if (file != NULL) {
															fclose(file);
															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\darts.png"), "r");
															if (file != NULL) {
																fclose(file);
																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\flameT.png"), "r");
																if (file != NULL) {
																	fclose(file);
																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\melee.png"), "r");
																	if (file != NULL) {
																		fclose(file);
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\rep_chest.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\rep_hand.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\rockets.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD1\\weapons\\wpns.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						versionfina9 = true;
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
														if (versionfina9 == true)
														{
															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_bg00.png"), "r");
															if (file != NULL) {
																fclose(file);
																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_bg01.png"), "r");
																if (file != NULL) {
																	fclose(file);
																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_comunic.png"), "r");
																	if (file != NULL) {
																		fclose(file);
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_data.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_diag.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_flight_mode.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_flight_speed.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_heading.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\HUD0\\rs_wanted.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								versionfina10 = true;
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
															if (versionfina10 == true)
															{
																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Altimeter\\alt01.png"), "r");
																if (file != NULL) {
																	fclose(file);
																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Altimeter\\alt02.png"), "r");
																	if (file != NULL) {
																		fclose(file);
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Altimeter\\alt03.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			versionfina11 = true;
																		}
																	}
																}
																if (versionfina11 == true)
																{
																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Background\\bg_flash.png"), "r");
																	if (file != NULL) {
																		fclose(file);
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Background\\bg_tps_border.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Background\\bg_tps_frozen.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Background\\bg_tps_middle.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Background\\visor0.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Background\\visor1.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							versionfina12 = true;
																						}
																					}
																				}
																			}
																		}
																	}
																	if (versionfina12 == true)
																	{
																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair.png"), "r");
																		if (file != NULL) {
																			fclose(file);
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair_dam.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair_r.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Crosshair\\crosshair_target.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						versionfina13 = true;
																					}
																				}
																			}
																		}
																		if (versionfina13 == true)
																		{
																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\HelpText\\help_bg.png"), "r");
																			if (file != NULL) {
																				fclose(file);
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\PlayerInfo\\hpcircle.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\PlayerInfo\\hpcircle1.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\PlayerInfo\\hpcircle2.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\PlayerInfo\\hpcircle3.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\PlayerInfo\\malarm.png"), "r");
																								if (file != NULL) {
																									fclose(file);
																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\PlayerInfo\\wpnarrow.png"), "r");
																									if (file != NULL) {
																										fclose(file);
																										versionfina14 = true;
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																			if (versionfina14 == true)
																			{
																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radar\\bugcoloreddisk.png"), "r");
																				if (file != NULL) {
																					fclose(file);
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radar\\bugradarwaves.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radar\\hudcoloreddisk.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radar\\hudradarwaves.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								versionfina15 = true;
																							}
																						}
																					}
																				}
																				if (versionfina15 == true)
																				{
																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radarblips\\hudbigarrow.png"), "r");
																					if (file != NULL) {
																						fclose(file);
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radarblips\\hudfail.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radarblips\\hudsmallarrow.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Radarblips\\hudtriang.png"), "r");
																								if (file != NULL) {
																									fclose(file);
																									versionfina16 = true;
																								}
																							}
																						}
																					}
																				}
																				if (versionfina16 == true)
																				{
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\chosen.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\chosenh.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\closebutn.png"), "r");
																								if (file != NULL) {
																									fclose(file);
																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\menuarrow.png"), "r");
																									if (file != NULL) {
																										fclose(file);
																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\miniarrow.png"), "r");
																										if (file != NULL) {
																											fclose(file);
																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\nextarrows.png"), "r");
																											if (file != NULL) {
																												fclose(file);
																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\nextarrowsh.png"), "r");
																												if (file != NULL) {
																													fclose(file);
																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\off.png"), "r");
																													if (file != NULL) {
																														fclose(file);
																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\on.png"), "r");
																														if (file != NULL) {
																															fclose(file);
																															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\selector.png"), "r");
																															if (file != NULL) {
																																fclose(file);
																																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\settings.png"), "r");
																																if (file != NULL) {
																																	fclose(file);
																																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\spin_left_top.png"), "r");
																																	if (file != NULL) {
																																		fclose(file);
																																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\Tony.png"), "r");
																																		if (file != NULL) {
																																			fclose(file);
																																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\unk0.png"), "r");
																																			if (file != NULL) {
																																				fclose(file);
																																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Suitmenu\\unk1.png"), "r");
																																				if (file != NULL) {
																																					fclose(file);
																																					versionfina17 = true;
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
																					if (versionfina17 == true)
																					{
																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Target\\Target01.png"), "r");
																						if (file != NULL) {
																							fclose(file);
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Target\\Target02.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								versionfina18 = true;
																							}
																						}
																						if (versionfina18 == true)
																						{
																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\bullets.png"), "r");
																							if (file != NULL) {
																								fclose(file);
																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\darts.png"), "r");
																								if (file != NULL) {
																									fclose(file);
																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\flameT.png"), "r");
																									if (file != NULL) {
																										fclose(file);
																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\melee.png"), "r");
																										if (file != NULL) {
																											fclose(file);
																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\rep_chest.png"), "r");
																											if (file != NULL) {
																												fclose(file);
																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\rep_hand.png"), "r");
																												if (file != NULL) {
																													fclose(file);
																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\textures\\ALLHUDS\\Weapons\\rockets.png"), "r");
																													if (file != NULL) {
																														fclose(file);
																														versionfina19 = true;
																													}
																												}
																											}
																										}
																									}
																								}
																							}
																							if (versionfina19 == true)
																							{
																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\text\\american.dat"), "r");
																								if (file != NULL) {
																									fclose(file);
																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\text\\spanish.dat"), "r");
																									if (file != NULL) {
																										fclose(file);
																										versionfina20 = true;
																									}
																								}
																								if (versionfina20 == true)
																								{
																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Dialogues\\suit_selected.mp3"), "r");
																									if (file != NULL) {
																										fclose(file);
																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Dialogues\\no_jarvis.mp3"), "r");
																										if (file != NULL) {
																											fclose(file);
																											versionfina21 = true;
																										}
																									}
																									if (versionfina21 == true)
																									{
																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\at_your_service.mp3"), "r");
																										if (file != NULL) {
																											fclose(file);
																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\dcareful.mp3"), "r");
																											if (file != NULL) {
																												fclose(file);
																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\dwarning.mp3"), "r");
																												if (file != NULL) {
																													fclose(file);
																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\enemyAl.mp3"), "r");
																													if (file != NULL) {
																														fclose(file);
																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\online.mp3"), "r");
																														if (file != NULL) {
																															fclose(file);
																															versionfina22 = true;
																														}
																													}
																												}
																											}
																										}
																										if (versionfina22 == true)
																										{
																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\online1.mp3"), "r");
																											if (file != NULL) {
																												fclose(file);
																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\Brilliant.wav"), "r");
																												if (file != NULL) {
																													fclose(file);
																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\Impecable.wav"), "r");
																													if (file != NULL) {
																														fclose(file);
																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\j_warning2.wav"), "r");
																														if (file != NULL) {
																															fclose(file);
																															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\Nicely_done.wav"), "r");
																															if (file != NULL) {
																																fclose(file);
																																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\Jarvis\\Well_done.wav"), "r");
																																if (file != NULL) {
																																	fclose(file);
																																	versionfina23 = true;
																																}
																															}
																														}
																													}
																												}
																											}
																											if (versionfina23 == true)
																											{
																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\alarm.mp3"), "r");
																												if (file != NULL) {
																													fclose(file);
																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\beep.wav"), "r");
																													if (file != NULL) {
																														fclose(file);
																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose00.mp3"), "r");
																														if (file != NULL) {
																															fclose(file);
																															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose01.mp3"), "r");
																															if (file != NULL) {
																																fclose(file);
																																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose02.mp3"), "r");
																																if (file != NULL) {
																																	fclose(file);
																																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose03.mp3"), "r");
																																	if (file != NULL) {
																																		fclose(file);
																																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose04.mp3"), "r");
																																		if (file != NULL) {
																																			fclose(file);
																																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose05.mp3"), "r");
																																			if (file != NULL) {
																																				fclose(file);
																																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\choose06.mp3"), "r");
																																				if (file != NULL) {
																																					fclose(file);
																																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\clic.mp3"), "r");
																																					if (file != NULL) {
																																						fclose(file);
																																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\dart.mp3"), "r");
																																						if (file != NULL) {
																																							fclose(file);
																																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\fallstop.mp3"), "r");
																																							if (file != NULL) {
																																								fclose(file);
																																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\firebullets.mp3"), "r");
																																								if (file != NULL) {
																																									fclose(file);
																																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\flyfast.mp3"), "r");
																																									if (file != NULL) {
																																										fclose(file);
																																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\flying_thrusters.mp3"), "r");
																																										if (file != NULL) {
																																											fclose(file);
																																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\flystop.mp3"), "r");
																																											if (file != NULL) {
																																												fclose(file);
																																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\hover.mp3"), "r");
																																												if (file != NULL) {
																																													fclose(file);
																																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\hover_thrusters.mp3"), "r");
																																													if (file != NULL) {
																																														fclose(file);
																																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\immsel.mp3"), "r");
																																														if (file != NULL) {
																																															fclose(file);
																																															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\jet_soundFlying.mp3"), "r");
																																															if (file != NULL) {
																																																fclose(file);
																																																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\landing.mp3"), "r");
																																																if (file != NULL) {
																																																	fclose(file);
																																																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\launchM.mp3"), "r");
																																																	if (file != NULL) {
																																																		fclose(file);
																																																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\parachute_fall_stop.mp3"), "r");
																																																		if (file != NULL) {
																																																			fclose(file);
																																																			file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\repStartbkupmod.mp3"), "r");
																																																			if (file != NULL) {
																																																				fclose(file);
																																																				file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\Repulsor_aim.mp3"), "r");
																																																				if (file != NULL) {
																																																					fclose(file);
																																																					file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\Repulsor_blasts.mp3"), "r");
																																																					if (file != NULL) {
																																																						fclose(file);
																																																						file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\repulsorBlast.mp3"), "r");
																																																						if (file != NULL) {
																																																							fclose(file);
																																																							file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\repulsorBlastbk.mp3"), "r");
																																																							if (file != NULL) {
																																																								fclose(file);
																																																								file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\repulsorStart.mp3"), "r");
																																																								if (file != NULL) {
																																																									fclose(file);
																																																									file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\STEP.mp3"), "r");
																																																									if (file != NULL) {
																																																										fclose(file);
																																																										file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\stepIm.mp3"), "r");
																																																										if (file != NULL) {
																																																											fclose(file);
																																																											file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\suit_selected.mp3"), "r");
																																																											if (file != NULL) {
																																																												fclose(file);
																																																												file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\takeoff.mp3"), "r");
																																																												if (file != NULL) {
																																																													fclose(file);
																																																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\targetSet.wav"), "r");
																																																													if (file != NULL) {
																																																														fclose(file);
																																																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\thrusters_off.mp3"), "r");
																																																														if (file != NULL) {
																																																															fclose(file);
																																																															file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\underwater_thrusters.mp3"), "r");
																																																															if (file != NULL) {
																																																																fclose(file);
																																																																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\uniaim.mp3"), "r");
																																																																if (file != NULL) {
																																																																	fclose(file);
																																																																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\unibeam.mp3"), "r");
																																																																	if (file != NULL) {
																																																																		fclose(file);
																																																																		file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\sound\\SFX\\uniloop.mp3"), "r");
																																																																		if (file != NULL) {
																																																																			fclose(file);
																																																																			versionfina24 = true;
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
																														}
																													}
																												}
																												if (versionfina24 == true)
																												{
																													file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\Fonts\\fonts_IM.dat"), "r");
																													if (file != NULL) {
																														fclose(file);
																														file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan\\Fonts\\fonts_IM.txd"), "r");
																														if (file != NULL) {
																															fclose(file);
																															versionfina25 = true;
																														}
																													}
																													if (versionfina25 == true)
																													{
																														file = fopen(GAME_PATH("modloader\\IronManMod\\weapons\\paracx.txd"), "r");
																														if (file != NULL) {
																															fclose(file);
																															file = fopen(GAME_PATH("modloader\\IronManMod\\weapons\\gun_para.txd"), "r");
																															if (file != NULL) {
																																fclose(file);
																																file = fopen(GAME_PATH("modloader\\IronManMod\\weapons\\gun_para.dff"), "r");
																																if (file != NULL) {
																																	fclose(file);
																																	file = fopen(GAME_PATH("modloader\\IronManMod\\weapons\\parachute.dff"), "r");
																																	if (file != NULL) {
																																		fclose(file);
																																		versionfina26 = true;
																																	}
																																}
																															}
																														}
																														if (versionfina26 == true)
																														{
																															file = fopen(GAME_PATH("modloader\\IronManMod\\skins\\GenericArmor.dat"), "r");
																															if (file != NULL) {
																																fclose(file);
																																file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan.img"), "r");
																																if (file != NULL) {
																																	fclose(file);
																																	file = fopen(GAME_PATH("modloader\\IronManMod\\IronMan_by_Maxirp.asi"), "r");
																																	if (file != NULL) {
																																		fclose(file);
																																		file = fopen(GAME_PATH("modloader\\IronManMod\\loader.txt"), "r");
																																		if (file != NULL) {
																																			fclose(file);
																																			versionfina27 = true;
																																		}
																																	}
																																}
																																if(versionfina27==true)
																																{
																																	if (Welcome("Welcome to GTA: Iron Man Mod by Maxirp93.\n \n Click OK to start, or press Cancel to contact the mod author") == true)
																																	{
																																		AudioLib.Readfromdll();
																																		JarvisVoice::ManageMaxirpBass();
																																		if (!AudioLib.Init())
																																		{
																																			sprintf(gString, "CSoundSystem::Init() failed. Error code: %d", AudioLib.ErrorGetCode());
																																			Logear(gString);
																																		}
																																		else
																																		{
																																			JarvisVoice::LoadAudios();
																																		}
																																	}
																																}
																																else
																																{
																																	Error("One or more files missing at the folder: modloader/IronManMod/");
																																}
																															}
																															else
																															{
																																Error("One or more files missing at the folder: modloader/IronManMod/skins");
																															}
																														}
																														else
																														{
																															Error("One or more files missing at the folder: modloader/IronManMod/weapons");
																														}
																													}
																													else
																													{
																														Error("One or more files missing at the folder: modloader/IronManMod/IronMan/Fonts");
																													}
																												}
																												else
																												{
																													Error("One or more files missing at the folder: modloader/IronManMod/IronMan/sound/SFX");
																												}
																											}
																											else
																											{
																												Error("One or more files missing at the folder: modloader/IronManMod/IronMan/sound/Jarvis");
																											}
																										}
																										else
																										{
																											Error("One or more files missing at the folder: modloader/IronManMod/IronMan/sound/Jarvis");
																										}
																									}
																									else
																									{
																										Error("One or more files missing at the folder: modloader/IronManMod/sound/Dialogues");
																									}
																								}
																								else
																								{
																									Error("One or more files missing at the folder: modloader/IronManMod/IronMan/text");
																								}
																							}
																							else
																							{
																								Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Weapons");
																							}
																						}
																						else
																						{
																							Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Target");
																						}
																					}
																					else
																					{
																						Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Suitmenu");
																					}
																				}
																				else
																				{
																					Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Radarblips");
																				}
																			}
																			else
																			{
																				Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Radar");
																			}
																		}
																		else
																		{
																			Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/HelpText");
																		}
																	}
																	else
																	{
																		Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Crosshair");
																	}
																}
																else
																{
																	Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/ALLHUDS/Altimeter");
																}
															}
															else
															{
																Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/HUD0");
															}
														}
														else
														{
															Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/HUD1/weapons");
														}
													}
													else
													{
														Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/HUD1/stats");
													}
												}
												else
												{
													Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures/HUD1");
												}
											}
											else
											{
												Error("One or more files missing at the folder: modloader/IronManMod/IronMan/textures");
											}
										}
										else
										{
											Error("One or more files missing at the folder: modloader/IronManMod/IronMan");
										}
									}
									else
									{
										Error("One or more files missing at the folder: modloader/IronManMod/data");
									}
								}
								else
								{
									Error("One or more files missing at the folder: modloader/IronManMod/CLEO");
								}
							}
							else
							{
								Error("One or more files missing at the folder: modloader/IronManMod/Cars");
							}
						}
						else
						{
							Error("One or more files missing at the folder: modloader/IronManMod/Avengers tower");
						}
					}
					else
					{
						Error("One or more files missing at the folder: modloader/IronManMod/Animation pack");
					}
				}
				else
				{
					if (ErrorMax("ERROR: Your version of Iron Man mod is outdated.\n \n You will need to replace your IronManMod folder\n \n Click OK to download the latest version of it.") == true)
					{
						Logear("ERROR. Download latest version of Iron Man mod");
					}
				}
			};
			once = true;
		}

		Events::initRwEvent += [] {


			if (KeyPressed(118))
			{
				exit(1);
			}
			//screen::SetBaseResolution(900.0f);
			settings.Read();
			MobileTextures::Setup();
			settings.storesuits();
			boolvars.yndex = boolvars.posironskinsfold;
			JarvisVoice::Loadsuiticons();

			
			//settings.HUDTexDictionary = CFileLoader::LoadTexDictionary(PLUGIN_PATH("IronMan\\textures\\maxirphud.txd"));
			//MobilePlayerInfo::cargartexturas();
			boolvars.wpn = GetPrivateProfileInt("CONFIG", "WEAPON", 0, PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));

			mobileradar.textura = new(RwTexture);
			mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\hudradarwaves.png"));
			mobileradar.waterdrop = new CSprite2d();
			mobileradar.waterdrop->m_pTexture = mobileradar.textura;
			mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\bugradarwaves.png"));
			mobileradar.bugdrop = new CSprite2d();
			mobileradar.bugdrop->m_pTexture = mobileradar.textura;
			mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\hudcoloreddisk.png"));
			mobileradar.radarglow = new CSprite2d();
			mobileradar.radarglow->m_pTexture = mobileradar.textura;
			mobileradar.textura = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\Radar\\bugcoloreddisk.png"));
			mobileradar.radarbug = new CSprite2d();
			mobileradar.radarbug->m_pTexture = mobileradar.textura;
			mobileradar.textura = NULL;
			mobileradar.altpos = new CSprite2d();
			mobileradar.altpos->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\altimeter\\alt01.png"));

			mobileradar.altcut = new CSprite2d();
			mobileradar.altcut->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\altimeter\\alt02.png"));

			mobileradar.altend = new CSprite2d();
			mobileradar.altend->m_pTexture = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\altimeter\\alt03.png"));

			mobileradar.textur = new RwTexture();
			mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudtriang.png"));
			mobileradar.center = new CSprite2d();
			mobileradar.center->m_pTexture = mobileradar.textur;
			mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudbigarrow.png"));
			mobileradar.center2 = new CSprite2d();
			mobileradar.center2->m_pTexture = mobileradar.textur;
			mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudsmallarrow.png"));
			mobileradar.center1 = new CSprite2d();
			mobileradar.center1->m_pTexture = mobileradar.textur;

			mobileradar.textur = mobileTex.LoadTexture(PLUGIN_PATH("IronMan\\textures\\ALLHUDS\\RadarBlips\\hudfail.png"));
			mobileradar.texfail = new CSprite2d();
			mobileradar.texfail->m_pTexture = mobileradar.textur;
			mobileradar.textur = NULL;
			movtextures.Loadmenuicons();

			{

				if (KeyPressed(118))
				{
					exit(1);
				}
				pauseresume();
				if (settings.bEnableColors)         MobileColors::Read();
				if (settings.bEnablePlayerInfo)     MobilePlayerInfo::InstallPatches();
				if (settings.bEnableRadar) 			MobileRadar::InstallPatches();
				if (settings.bEnableSubtitles)      MobileSubtitles::InstallPatches();
				if (settings.bEnableRadioNames)     MobileRadioName::InstallPatches();
				if (settings.bEnableAreaNames)      MobileAreaName::InstallPatches();
				if (settings.bEnableVehicleNames)   MobileVehicleName::InstallPatches();
				if (settings.bEnableStatsBox)       MobileStatsBox::InstallPatches();
				if (settings.bEnableTextBox)        MobileTextBox::InstallPatches(), movtextures.InstallPatches();
				if (settings.bEnableTimers)         MobileMissionTimers::InstallPatches();
				if (settings.bEnableProgressBars)   MobileProgressBar::InstallPatches();
				if (settings.bEnableMenuSystem)     MobileMenuSystem::InstallPatches();
				if (settings.bEnableLoadingScreens) MobileLoadingScreen::InstallPatches();


				//if (settings.bEnableMenuPages)      MobileMenuPage::InstallPatches();
				//if (settings.bEnableFrontends)      MobileFrontEnd::InstallPatches();
			}

			if (settings.bReloadWKey) {
				Events::drawingEvent += [] {
					if (settings.iReloadKey != 0) {
						if (settings.iReloadKey == -1 || KeyPressed(settings.iReloadKey))
							settings.Read();
					}
				};
			}
		};


		//patch::RedirectCall(0x73723E, MyCreateExplosionFx);

		Events::pedRenderEvent += [](CPed *punchedped) {
			JarvisVoice::Actionsforpeds(punchedped);
		};

		Events::vehicleRenderEvent.after += [](CVehicle *punchedcar) {
			JarvisVoice::Actionsforcars(punchedcar);
		};

		Events::objectRenderEvent += [](CObject *objeto) {
			JarvisVoice::Actionsforobjs(objeto);
			
		};

		Events::drawingEvent.before += [] {
			if (KeyPressed(118))
			{
				exit(1);
			}
			//JarvisVoice::ManageMaxirpBass(); //AGREGADO
			ProcessExplosions();
			boolvars.cannothitcar = false;
			//MobilePlayerInfo::getcharalt();
			JarvisVoice::processparachute();

			JarvisVoice::aimironman();

			JarvisVoice::ironmanpowers();

			JarvisVoice::jarvisbotpowers();

			JarvisVoice::Punchped(boolvars.punchedvictim);
			boolvars.punchedvictim = 0;
			
			JarvisVoice::Punchcar(boolvars.punchedcar);
			boolvars.punchedcar = 0;
		};


		Events::drawBlipsEvent += [] {
			MobileRadar::DrawRadarCenter();
		};
		
		Events::drawHudEvent.before += [] {
			JarvisVoice::changeoutfitonair(); 
		};
		/*
		Events::drawHudEvent += [] {
			MobilePlayerInfo::MyDrawWeaponIcon1();
		};*/
		
		Events::onPauseAllSounds += [] {
			//JarvisVoice::ManageMaxirpBass(); //AGREGADO
			JarvisVoice::PauseAllStreams();
			boolvars.cannothitcar = true;
		};

		Events::onResumeAllSounds += [] {
			//JarvisVoice::ManageMaxirpBass(); //AGREGADO
			boolvars.cannothitcar = false;
		};

		Events::drawMenuBackgroundEvent += [] {
			static bool once;
			if (!once)
			{
				boolvars.para_text = TheText.tkeyMain.GetTextByLabel("PARA_01", &boolvars.found);
				once = true;
			}
			if (KeyPressed(118))
			{
				exit(1);
			}
			//JarvisVoice::ManageMaxirpBass(); //AGREGADO
			boolvars.menuisactive = false;
			//JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
			JarvisVoice::PauseAllStreams();
			//JarvisVoice::PauseBass(); //AGREGADO
			boolvars.cannothitcar = true;
		};
		ThiscallEvent<AddressList<0x53C69C, H_CALL>, PRIORITY_AFTER, ArgPickNone, void(void *self, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a)> InitialiseWhenRestarting;

		InitialiseWhenRestarting += [] {
			if (KeyPressed(118))
			{
				exit(1);
			}
			boolvars.waiter = 0;
			boolvars.timetowait = 0;
			//JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
			JarvisVoice::PauseAllStreams();
			boolvars.cannothitcar = true;
			//JarvisVoice::ManageMaxirpBass();
			boolvars.menuisactive = false;
			//boolvars.isattaching = false;
			boolvars.killedtargetsexist = false;
			boolvars.mousewheelhacktrigger = false;
			//boolvars.vuelasolo = false;
			//boolvars.hastoloadcar = false;
			//boolvars.zkeystate = false;
			//boolvars.is_lifting = false;
			//boolvars.is_throwing_car = false;
			//boolvars.volaba = false;
			boolvars.has_a_car = false;
			//boolvars.is_not_holding_anymore = false;
			boolvars.aims = false;
			MobileLoadingScreen::DisplaySplash();
		};

		Events::shutdownRwEvent += [] {

				mobileradar.altcut = NULL;
				mobileradar.altend = NULL;
				mobileradar.altpos = NULL;
				mobileradar.bugdrop = NULL;
				mobileradar.center = NULL;
				mobileradar.center1 = NULL;
				mobileradar.center2 = NULL;
				mobileradar.radarbug = NULL;
				mobileradar.radarglow = NULL;
				mobileradar.texfail = NULL;
				mobileradar.textur = NULL;
				mobileradar.textura = NULL;
				mobileradar.waterdrop = NULL;
			
			WritePrivateProfileString("CONFIG", "MARK", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			WritePrivateProfileString("CONFIG", "WEAPON", "0", PLUGIN_PATH("IronMan\\IronMan_Mod.ini"));
			/*if (settings.HUDTexDictionary) {
				RwTexDictionaryDestroy(settings.HUDTexDictionary);
				settings.HUDTexDictionary = nullptr;
			}*/
			MobileTxdStorage::Instance().Shutdown();
			//JarvisVoice::StopThrustersIDLE(&IMStream[1], &propellers);
			JarvisVoice::PauseAllStreams();
			JarvisVoice::unloadaudios();
			boolvars.cannothitcar = true;
			//JarvisVoice::ManageMaxirpBass();
			boolvars.menuisactive = false;
			//boolvars.isattaching = false;
			boolvars.killedtargetsexist = false;
			boolvars.mousewheelhacktrigger = false;
			//boolvars.vuelasolo = false;
			//boolvars.hastoloadcar = false;
			//boolvars.zkeystate = false;
			//boolvars.is_lifting = false;
			//boolvars.is_throwing_car = false;
			//boolvars.volaba = false;
			boolvars.has_a_car = false;
			//boolvars.is_not_holding_anymore = false;
			boolvars.aims = false;
			//MobileTextures::Unload();
		};

		CdeclEvent<AddressList<0x71982E, H_JUMP>, PRIORITY_BEFORE, ArgPickNone, void()> myProcessPerFrameEvent;

		myProcessPerFrameEvent += [] {
			MobileTxdStorage::Instance().Process();
		};
	}
} mobileHud;

