#pragma once

#include <plugin.h>
#include "game_sa\CSprite2d.h"
#include "game_sa\RenderWare.h"
#include "game_sa\CVector2D.h"
#include "MobileTextures.h"
#include "Settings.h"
#include <iostream>
// mobile-hud
using namespace plugin;

class Movtextures {
public:
	void readallhudtextures();
	void MyDrawWeaponIcon1();
	void Loadmenuicons();
	void InstallPatches();
	static void cargartexturas();
	void drawindexedtexture(int index);
	void drawsizedtexture(CRect siz, CSprite2d *tex, int index);
};

extern Movtextures movtextures;