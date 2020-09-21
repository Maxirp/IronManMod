#include "MobileLoadingScreen.h"
#include <plugin.h>
#include "extensions\Screen.h"
#include "Settings.h"

MobileLoadingScreen MobileLoad;

void MobileLoadingScreen::InstallPatches() {
    static float f_255 = 0.0f;
    plugin::patch::SetPointer(0x590BA7 + 2, &f_255);
    plugin::patch::SetInt(0x748B0E, 5);
    plugin::patch::RedirectJump(0x5900B0, LoadSplashes);
    plugin::patch::RedirectJump(0x58FF60, RenderSplash);
    plugin::patch::RedirectCall(0x590480, RenderLoadingBar);
}
char *pText;
char *pText2;
int logoAlpha = 0;
int backgroundAlpha = 0;
float divider = 6.0f;

void MobileLoadingScreen::LoadSplashes(char bStarting, char bNvidia) {

	if (KeyPressed(118))
	{
		exit(1);
	}
	static bool onceawhile;
	if (!onceawhile)
	{
		onceawhile = true;
	}
    LARGE_INTEGER PerformanceCount;
    int splashNumber = 0;

    QueryPerformanceCounter(&PerformanceCount);
    srand(PerformanceCount.LowPart);

    for (int screenId = 0; screenId < 7; ++screenId) {
        if (bStarting) {
            if (bNvidia == 1) {
                pText = legal_2;
                pText2 = "";
                logoAlpha = 0;
                backgroundAlpha = 255;
            }
            else {
                pText = legal_2;
                pText2 = "";
                logoAlpha = 0;
                backgroundAlpha = 255;
            }
        }
        else {
            if (screenId)
                splashNumber = rand() % 6;
            else {
                pText = "";
                pText2 = "";
                logoAlpha = 255;
				divider = 6.0f;
                backgroundAlpha = 0;
            }
        }
        mobileTex.m_aSplashes[screenId].m_pTexture = mobileTex.m_aSplashesTxd.GetTexture(splashNumber);
    }
    mobileTex.m_aSplashes[6].m_pTexture = nullptr;
}

void MobileLoadingScreen::RenderSplash() {

	if (KeyPressed(118))
	{
		exit(1);
	}
    CSprite2d::InitPerFrame();
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, reinterpret_cast<void*>(rwTEXTUREADDRESSCLAMP));
	static bool initial;
	CRect dimsplash= CRect(NULL, NULL, NULL, NULL);
	static float fPosX;
	static float fPosY;
	static float fPosX1;
	static float fPosY1;
	static float size1;
	static float size2;
	static float alpha;
    CFont::SetProportional(true);
    CFont::SetBackground(0, 0);
    CFont::SetJustify(false);
    CFont::SetEdge(1);
    CFont::SetOrientation(ALIGN_CENTER);
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetEdge(1);
	CFont::SetColor(CRGBA(255, 255, 255, 255));

    //if (fPosX > 0) fPosX -= 200;

	if (plugin::IsPluginInstalled("fastloader.asi") == false && plugin::IsPluginInstalled("imfast.asi") == false)
	{
		if (!initial) {
			size1 = 0.841f;
			size2 = 1.83f;
			alpha = 255;
			initial = true;
		}
		else
		{
			size1 -= 0.001f;
			size2 -= 0.002f;
			if (size1 < 0.5) {
				if (alpha >= 1) {
					alpha -= 1;
				}
				else {
					alpha = 0;
				}
			}
			if (divider > 1.0f)
			{
				divider -= 0.25f;
			}
			else
			{
				divider = 1.0f;
			}
		}
		if (fPosX < 0.0f || fPosY < 0.0f)
		{
			fPosX = BilinearOffset(0.0f);
			fPosY = BilinearOffset(0.0f);
			fPosX1 = BilinearOffset(0.0f + SCREEN_WIDTH);
			fPosY1 = BilinearOffset(0.0f + SCREEN_HEIGHT);
		}
		else
		{
			fPosX = BilinearOffset(SCREEN_COORD_CENTER_X - ((SCREEN_WIDTH / 2.01f) / divider));
			fPosY = BilinearOffset(SCREEN_COORD_CENTER_Y - ((SCREEN_HEIGHT / 2.01f) / divider));
			fPosX1 = BilinearOffset(SCREEN_COORD_CENTER_X + ((SCREEN_WIDTH / 2.01f) / divider));
			fPosY1 = BilinearOffset(SCREEN_COORD_CENTER_Y + ((SCREEN_HEIGHT / 2.01f) / divider));
		}
	}
	else
	{
		fPosX = BilinearOffset(0.0f);
		fPosY = BilinearOffset(0.0f);
		fPosX1 = BilinearOffset(0.0f + SCREEN_WIDTH);
		fPosY1 = BilinearOffset(0.0f + SCREEN_HEIGHT);
	}
	dimsplash.left = (fPosX);
	dimsplash.right = (fPosX1);
	dimsplash.top = (fPosY);
	dimsplash.bottom = (fPosY1);

    if (MobileLoad.m_bFading) {
        CSprite2d::DrawRect(CRect(BilinearOffset(0.0f), BilinearOffset(0.0f), BilinearOffset(0.0f + SCREEN_WIDTH), BilinearOffset(0.0f + SCREEN_HEIGHT)), CRGBA(0, 0, 0, 255));

        if (!MobileLoad.m_currDisplayedSplash) {
            mobileTex.m_nBackgroundSprite.m_pTexture = mobileTex.m_aSplashesTxd.GetTexture(LEGALSCREEN);
			
			mobileTex.m_nBackgroundSprite.Draw(dimsplash, CRGBA(backgroundAlpha, backgroundAlpha, backgroundAlpha, alpha));
			
			CFont::SetColor(CRGBA(255, 255, 255, alpha));
            // Legal screen
            if (RsGlobal.maximumWidth >= 0 && RsGlobal.maximumWidth <= 1280) {
                CFont::SetScale(SCREEN_MULTIPLIER(size1), SCREEN_MULTIPLIER(size2));
                CFont::SetWrapx(fPosX + SCREEN_COORD_RIGHT(62.0f));
				CFont::PrintString(SCREEN_COORD_CENTER_X, SCREEN_COORD_CENTER_Y, pText);
				CFont::PrintString(SCREEN_COORD_CENTER_X, SCREEN_COORD_CENTER_Y, pText2);
            }
            else {
                CFont::SetScale(SCREEN_MULTIPLIER(size1), SCREEN_MULTIPLIER(size2));
				CFont::SetWrapx(fPosX + SCREEN_COORD_RIGHT(62.0f)); //+ SCREEN_COORD_CENTER_LEFT(-725.0f));
                CFont::PrintString(SCREEN_COORD_CENTER_X, SCREEN_COORD_CENTER_Y, pText);
                CFont::PrintString(SCREEN_COORD_CENTER_X, SCREEN_COORD_CENTER_Y, pText2);
            }

            mobileTex.m_nBackgroundSprite.m_pTexture = mobileTex.m_aSplashesTxd.GetTexture(ROCKSTAR);
            mobileTex.m_nBackgroundSprite.Draw(dimsplash, CRGBA(logoAlpha, logoAlpha, logoAlpha, logoAlpha));

            mobileTex.m_nBackgroundSprite.m_pTexture = nullptr;
        }
        else { // Loading screen
            if (RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840)
                mobileTex.m_aSplashes[MobileLoad.m_currDisplayedSplash].Draw(CRect(SCREEN_COORD_CENTER_X - SCREEN_COORD((1920.0f * 900 / 1080) / 2), SCREEN_COORD(0.0f), SCREEN_COORD_CENTER_X - SCREEN_COORD((1920.0f * 900 / 1080) / 2) + SCREEN_COORD(1920.0f * 900 / 1080), SCREEN_COORD(0.0f) + SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
            else
                mobileTex.m_aSplashes[MobileLoad.m_currDisplayedSplash].Draw(CRect(SCREEN_COORD(0.0f), SCREEN_COORD(0.0f), SCREEN_COORD(0.0f) + SCREEN_WIDTH, SCREEN_COORD(0.0f) + SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));
        }
    }
}

void MobileLoadingScreen::DisplaySplash() {
	if (KeyPressed(118))
	{
		exit(1);
	}
}

void MobileLoadingScreen::RenderLoadingBar(float x, float y, unsigned short width, unsigned char height, float progress, signed char progressAdd, unsigned char drawPercentage, unsigned char drawBlackBorder, CRGBA color, CRGBA addColor) {
	if (KeyPressed(118))
	{
		exit(1);
	}
	float distance = 18.0f;
    if (RsGlobal.maximumWidth == 2560 || RsGlobal.maximumWidth == 3840)
        CSprite2d::DrawBarChart(SCREEN_COORD_CENTER_X - SCREEN_COORD((1920.0f * 900 / 1080) / 2 - distance), SCREEN_COORD_BOTTOM(30.0f), SCREEN_COORD(1920.0f * 900 / 1080 - distance * 2), SCREEN_MULTIPLIER(20.0f), progress, progressAdd, drawPercentage, drawBlackBorder, color, addColor);
    else
        CSprite2d::DrawBarChart(SCREEN_COORD_LEFT(distance), SCREEN_COORD_BOTTOM(30.0f), SCREEN_COORD_MAX_X - SCREEN_COORD(distance * 2), SCREEN_MULTIPLIER(20.0f), progress, progressAdd, drawPercentage, drawBlackBorder, color, addColor);
}