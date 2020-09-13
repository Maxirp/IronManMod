#pragma once

#define g_pHelpMessage ((char *)0xBAA7A0)
#define g_HelpMessageState (*(__int32 *)0xBAA474)
#define g_HelpMessageTimer (*(signed __int32 *)0xBAA47C)
#define g_HelpMessageFadeTimer (*(signed __int32 *)0xBAA478)
#define g_pHelpMessageToPrint ((char *)0xBAA480)
#define g_fTextBoxNumLines (*(float *)0xBAA460)
#define PLAYONESHOT(id, volume) ((void (__thiscall *)(unsigned int, unsigned short, float, float))0x506EA0)(0xB6BC90, id, volume, 1.0)
#define Camera_WidescreenOn (*(unsigned __int8 *)0xB6F065)
#define bTextBoxPermanent2 (*(__int8 *)0xBAA464)
#define m_HelpMessageQuick (*(unsigned __int8 *)0xBAA472)
#define Cutscene_Running (*(unsigned __int8 *)0xB5F851)
#define g_HelpMessageStatId (*(unsigned __int16 *)0xBAA470)
#define g_filenameBuffer ((char *)0xB71670)
#define flt_8D0938 (*(float *)0x8D0938)
#define GetGroupMembersCount(groupId) ((unsigned int (__thiscall *)(unsigned int))0x5F6AA0)(groupId * 0x2D4 + 0xC09928)
#define GetPlayerStat(statid) ((float (__cdecl *)(unsigned short))0x558E40)(statid)
#define dword_BAA468 (*(float *)0xBAA468)
#define g_fScriptTextBoxesWidth (*(float *)0x8D0934)
#define StatMax (*(unsigned __int16 *)0xBAA46C)
#define g_BigMessage0 ((char *)0xBAACC0)
#define g_BigMessage4 ((char *)0xBAAEC0)
#define byte_96C014 ((char *)0x96C014)
#define Menu_WidescreenOn (*(unsigned __int8 *)0xBA6793)
#define ZoneToPrint ((char *)0xBAB1D0)

// Plugin-SDK
#include <plugin.h>
#include "game_sa\CClock.h"
#include "game_sa\CFont.h"
#include "game_sa\CFileMgr.h"
#include "game_sa\CHud.h"
#include "game_sa\CMenuManager.h"
#include "game_sa\CMessages.h"
#include "game_sa\CPad.h"
#include "game_sa\CPed.h"
#include "game_sa\CRadar.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CSprite.h"
#include "game_sa\CStats.h"
#include "game_sa\CText.h"
#include "game_sa\CTimer.h"
#include "game_sa\CTxdStore.h"
#include "game_sa\CWeaponInfo.h"
#include "game_sa\CWorld.h"

#include "game_sa\common.h"
#include "game_sa\RenderWare.h"

static short HUD_AREA_FONT;

static short HUD_VEHICLE_FONT;

static const float fTextBoxPosnX = 50.0f;
static const float fTextBoxPosnXWithRadar = 50.0f;
static const float fTextBoxPosnXWithRadarAndPlane = 50.0f;
static const float fTextBoxPosnY = 50.0;
static const float fTextBoxFontScaleX = 0.7;
static const float fTextBoxFontScaleY = 1.9;
static const float fTextBoxWidth = 421.0;
static const float fTextBoxBorderSize = 6.0;

static const char *hudfilename;
static const char *fontfilename;
static const char *fontdatafilename;

static void Ini()
{
	fontfilename = ".\\IronMan\\Fonts\\fonts_IM.TXD";
	fontdatafilename = ".\\IronMan\\Fonts\\fonts_IM.DAT";
}
