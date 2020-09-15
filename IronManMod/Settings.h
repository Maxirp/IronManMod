#pragma once
// plugin-sdk
#include <plugin.h>
#include "game_sa\CSprite2d.h"
#include "game_sa\RenderWare.h"
// mobile-hud
using namespace std;
using namespace plugin;

typedef struct traje Ttraje;
struct traje
{
public:
	void deletealldata();
	bool hasini;
	bool iconexists;
	bool mdlexists;
	bool texexists;
	int id;
	char name[20];
	char pngname[20];
	CVector rightfoot;
	CVector leftfoot;
	CVector righthand;
	CVector lefthand;
	CVector flyrightfoot;
	CVector flyleftfoot;
	CVector flyrighthand;
	CVector flylefthand;
};

typedef struct elementoencarpeta Telemento;
struct elementoencarpeta {
public:
	char nombre[20];
	bool existe;
	int tipo; //1=data 2=dff 3=png 4=txd
};

typedef struct SuitFiles Tsuitfiles;
struct SuitFiles {
public:
	char nombre[20];
	char nombrecarpeta[20];
	bool hastexture;
	bool hasini;
	bool hasmodel;
	bool hasicon;
};

typedef struct folderdir Tfolderdir;
struct folderdir
{
public:
	char name[20];
	int subcontentamnt;
	
	Ttraje suits[85];
};

class Settings {
public:
	int skinid;
	bool healthdisplayed;
	int marktraje;
	int		  numtextures[85];
	bool	  bWEnablealkstyle;
    bool      bEnableColors;
    bool      bEnablePlayerInfo;
    bool      bEnableRadar;
	bool      bEnableRadar1;
	bool	  bDonewithradar;
    bool      bEnableSubtitles;
    bool      bEnableRadioNames;
    bool      bEnableAreaNames;
    bool      bEnableVehicleNames;
    bool      bEnableStatsBox;
    bool      bEnableTextBox;
    bool      bEnableTimers;
    bool      bEnableProgressBars;
    bool      bEnableMenuSystem;
    bool      bEnableLoadingScreens;
    bool      bEnableMenuPages;
    bool      bEnableFrontends;
	bool	  gpsshow;
    bool      bRadarTop;
	bool	  bwpalignver;
	bool	  bwpalignhor;
	bool	  bhudflight;
	bool      bReloadWKey;
	bool	  standardhud;
	bool	  ironmanhud;
	bool	  areadata;
	bool	  clockdata;
	int hud;
	RwTexDictionary *HUDTexDictionary;
	CVector2D vecUnusedWeaponIconOffst;
	int vecUnusedWeaponIconAlpha;
	CVector2D vecUnusedWeaponIconScale;
    float vecWeaponIconPosny;
    CVector2D vecWeaponIconScale;
    float vecWeaponAmmoPosny;
    CVector2D vecWeaponAmmoScale;
    float vecClockPosny;
    CVector2D vecClockScale;
    float vecMoneyPosny;
    CVector2D vecMoneyScale;
    CVector2D vecBreathPosn;
    CVector2D vecBreathScale;
    CVector2D vecWantedLevelPosn;
    CVector2D vecWantedLevelStarScale;
    float     fWantedLevelStarSpace;
    float     fSecondPlayerOffsetY;
    CVector2D vecRadarPosn;
    float     fRadarWidthHalf;
    float     fRadarHeightHalf;
    float     fRadarBorderWidthHalf;
    float     fRadarBorderHeightHalf;
    CVector2D vecPlaneRadarPosn;
    CVector2D vecPlaneRadarSize;
    CVector2D vecPlaneRadarLightPartPosn;
    CVector2D vecPlaneRadarLightPartSize;
    bool      bRadioNameTop;
    float     fRadioNameTopPosnY;
    CVector2D vecRadioNamePosn;
    CVector2D vecRadioNameScale;
    CVector2D vecTextBoxPosn;
    float     fTextBoxPosnXWithRadar;
    float     fTextBoxPosnXWithRadarAndPlane;
    CVector2D vecTextBoxFontScale;
    float     fTextBoxWidth;
    float     fTextBoxBorderSize;
    CVector2D vecTextBoxProgressBarSize;
    float     fTextBoxProgressBarYShift;
    float     fTextBoxWidthWithProgressBar;
    float     fTextBoxPlusMinTextXShift;
    CVector2D vecTextBoxTextPosnWhenMenuEnabled;
    float     fTextBoxHeightWhenMenuEnabled;
    bool      bMissionTimersTop;
    float     fMissionTimersTopPosnY;
    float     fMissionTimersBoxPosnX;
    float     fMissionTimersBoxPosnMinY;
    float     fMissionTimersBoxPosnMinY2P;
    CVector2D vecMissionTimersBoxSize;
    CVector2D vecMissionTimersTextScale;
    float     fMissionTimersTextMaxWidth;
    float     fMissionTimersTextOffsetY;
    float     fMissionTimersTextDescriptionOffsetX;
    float     fMissionTimersTextValueOffsetX;
    CVector2D vecMissionTimersProgressBarOffset;
    CVector2D vecMissionTimersProgressBarSize;

    float     fSubtitlesPosnY;
    float     fSubtitlesWidePosnY;
    CVector2D vecSubtitlesScale;
    CVector2D vecAreaNamePosn;
    CVector2D vecAreaNameScale;
    CVector2D vecVehicleNamePosn;
    CVector2D vecVehicleNameScale;

    CVector2D vecStatsBoxPosn;

    bool      IsStatsBoxOpen;

    int       iReloadKey;
	int radarmode;
	int amount;
	CRGBA coloraltimeter;
	CRGBA coloraltimeterbar;
	CRGBA coloraltimetermark;
	CRGBA colorradar;
	CRGBA colorcompassbg;
	CRGBA colorcompassbar;
	CRGBA colorcompassmark;
	CRGBA blueregular;
	CRGBA redcodecolour;
	CRGBA areacolor;
	CRGBA clockcolor;
	folderdir folderdirs[20];
	static bool storesuits();
    void Read();
};
extern Settings settings;

class BoolVars {
public:
	bool menuerror;
	bool immune;
	int shield12;
	bool updateshieldnow;
	bool escudoactivo;
	int mark;
	int hudindex;
	int helperobj;
	int landgetup;
	int timehud;
	float ofstappearx;
	bool hudactive;
	bool hudsetting;
	float maxarmor;
	int pageofsuit;
	int timrmenu;
	int alphafad;
	int waiter;
	int timetowait;
	int posironskinsfold;
	int yndex;
	int hud;
	float gamevolume;
	bool found;
	std::string para_text;
	std::string helptext1;
	std::string helptext2;
	std::string helptext3;
	std::string helptext4;
	std::string helptext5;
	std::string helptext6;
	std::string helptext7;
	std::string helptext8;
	std::string online;
	std::string impulso;
	std::string suitarmormenu;
	std::string menupag1;
	std::string menupag2;
	std::string Tonyplay;
	std::string Notplay;
	std::string Suitplay;
	std::string Options;
	std::string Prevmenu;
	std::string Nextmenu;
	std::string Closesets;
	std::string Clicoptions;
	std::string Closemenu;
	std::string distan;
	std::string dronfar;
	bool iscalculating;
	bool settingisactive;
	bool isinvincible;
	bool vueloconmouse;
	int flytime;
	bool landedpara;
	bool paralineactive;
	float altitude;
	float range;
	bool errortext;
	bool spdmouse;
	bool iscjfrozen;
	bool isshootin;
	bool bassinit;
	bool wpmenuisactive;
	int indx;
	int unfadetime;
	int fadetime;
	bool startfade;
	float bottomtextboxcoord;
	float toptextboxcoord;
	float horizonfloor;
	float pith;
	bool cannothitcar;
	bool thrusterskilled;
	bool murio;
	//bool isclanking;
	int car1;
	int wpn;
	int aimedpeds[5];
	int currenttarg;
	int ped;
	CVector explosionpos;
	CVector stordexplpos;
	CVector pricel;
	int expltype;
	CVector target;
	bool fartarget;
	int timecreatedped;
	int action;
	int victim1, victim;
	//bool isfastflying;
	bool menuisactive;
	//bool isattaching;
	bool killedtargetsexist;
	int punchedvictim;
	int punchedcar;
	bool punchedtargetsexist;
	bool mousewheelhacktrigger;
	//bool vuelasolo;
	//bool hastoloadcar;
	//bool zkeystate;
	//bool is_lifting;
	//bool is_throwing_car;
	//bool volaba;
	bool cohetebotexplotado;
	float anglecohete;
	bool vuelaatras;
	bool systemerror;
	bool has_a_car;
	//bool is_not_holding_anymore;
	bool aims;
	bool radarisactive;
	bool suit[7];
	int activesuit;
	//bool radarisinactive;
	//bool readytomenu;
	//bool playerwantsstandard;
};

extern BoolVars boolvars;