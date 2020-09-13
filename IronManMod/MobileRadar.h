#pragma once
#include "game_sa\CVector2D.h"
#include "game_sa\CSprite2d.h"
#include <plugin.h>

using namespace plugin;
class MobileRadarTextures {
public:
	RwTexture *textura;
	CSprite2d *bugdrop;
	CSprite2d *waterdrop;
	CSprite2d *radarbug;
	CSprite2d *radarglow;

	CSprite2d *center;
	//static CSprite2d *damcenter = NULL;
	CSprite2d *center1;
	CSprite2d *center2;
	RwTexture *textur;
	CSprite2d *texfail;
	CSprite2d *altcut;
	CSprite2d *altend;
	CSprite2d *altpos;
};

class MobileRadar {
public:
    static void InstallPatches();
    static void MyTransformRadarPointToScreenSpace(CVector2D *out, CVector2D *in);
    static void __fastcall MyDrawRadarCircle(CSprite2d *sprite, int, CRect const &rect, CRGBA const &color);
    static void __fastcall MyDrawRadarPlane(CSprite2d *sprite, int, float x1, float y1, float x2, float y2, float x3, float y3, float x4,
        float y4, CRGBA const &color);
    static void MyDrawPlaneHeightBorder(CRect const& rect, CRGBA const& color);
    static void MyDrawPlaneHeight(CRect const& rect, CRGBA const& color);
    static void DrawRadarRectangle(CSprite2d *sprite, CRect const& rect, CRGBA const& color); 
	static void DrawRadarRectangle1(CSprite2d *sprite, CRect const& rect, CRGBA const& color);
	static void DrawRadarCenter();
	static void DrawFailReport(CSprite2d *fail);
};


extern MobileRadarTextures mobileradar;