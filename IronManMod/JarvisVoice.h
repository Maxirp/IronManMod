#pragma once
#include "game_sa\CVector2D.h"
#include "game_sa\CSprite2d.h"
#include "MobileTextures.h"
#include "Settings.h"
#include <plugin.h>
#include <iostream>
#include <vector>
#include "BASS_funcs.h"

using namespace std;

static CPed *cana = NULL;
static int objectlist[50];
static int dim;
static CVehicle *polmaver[10] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static int polmissile[20] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };


typedef struct thrusters Tthrusters;
struct thrusters
{
	int manoizqfx = 0;
	int manoderfx = 0;
	int pieizqfx = 0;
	int piederfx = 0;
	int enviroment_ID;
};

typedef struct targete Ttargete;
struct targete
{
public:
	int timer;
	int actorid = 0;
	CPed *actorchar = NULL;
};

typedef struct botrockets Tbotrockets;
struct botrockets
{
	int misil = NULL;
	CPed *objective = NULL;
	float velprevx = NULL;
	float velprevy = NULL;
	float velprevz = NULL;
	int timecreated = NULL;
	int recordcol=NULL;
};

typedef struct jarvisbot Tjarvisbot;
struct jarvisbot
{
public:
	int baseobj = NULL;
	int thrusterscode = NULL;
	float velx = NULL;
	float vely = NULL;
	float velz = NULL;
	Taudiofile audios[7];
	Taudiofile botstream[3];
	int mark=-1;
	int raystime = NULL;
	bool yolohice = NULL;
	int ibeam[3] = { NULL,NULL,NULL };
	bool hasacar = NULL;
	int blip = NULL;
	bool raysactive = NULL;
	int taskindex = 0;
	int hp = NULL;
	int wpn;
	int actorid = NULL;
	CPed *actorchar=NULL;
	int timetoact = NULL;
	thrusters jarvisprop;
	bool isfrozen = NULL;
	Ttargete target;
	Ttargete attacker;
	Tbotrockets cohete;
	bool complete = NULL;
	float obstaclex = NULL;
	float obstacley = NULL;
	float obstaclez = NULL;
	float contadordist = NULL;
	bool contadorinicial = false;
	int damagetimer=NULL;
	int timecreated=NULL;
};
static int dimenemy = 0;
static int dimfriends = 0;
static int pagebelong[20];
static int pageenemy[2];
static Tjarvisbot jarvisfriend[20];

static Tjarvisbot jarvisenemy[2];


enum weaponid {
	unibeam,
	repulsor,
	dualrepulsor,
	missile,
	dart,
	bullets,
	armlaser,
};

enum streamid {
	darts,
	repulsorstart,
	repulsorblast,
	repStartbkupmod,
	fireBullets,
	launchM,
	thrustersfx,
	underwater,
	flying_thrusters,
	takeoff,
	flyfast,
	targetSet,
	clank,
	flystop,
	unibeamsfx,
	beamloop,
	alarm,		//ve
	dcareful,	//ve
	dwarning,	//ve
	loopalarm,//
	randomalarm,//
};

enum animtothruster {
	flotar,
	im_flyfast,
	arriba,
	abajo,
	izquierda,
	derecha,
	adelante,
	atras,
	apuntar,
	apuntarbot,
	stopcaida,
};

class JarvisVoice {
public:
	static void PlayBeep(Taudiofile *MP3Stream);
	float GetYAngle();
	static bool aimironman();
	static void Actionsforpeds(CPed *);
	static void Actionsforcars(CVehicle *);
	static bool Actionsforobjs(CObject *);
	static bool isvulnerablecharinarea(CPed *, float, float, float, float);
	static void Loadsuiticons();
	static void changeoutfitonair();
	static void Updateaudiopositions();
	static void ManageMaxirpBass();
	static void superpowersironman();
	static bool ironmanpowers();
	static bool jarvisbotpowers();
	static bool notwastednotbusted();
	static bool Punchped(int punchedped);
	static bool Punchcar(int punchedped);
	static float GetXAngleforopcodes();
	static bool got_car(int *car);
	static bool LoadAudios();
	static void unloadaudios();
	static void PlayShot(Taudiofile *MP3Stream);
	static void PlayAudiostream(Taudiofile audiostreams, int user, Taudiofile *MP3Stream);
	static bool IsAudioVoicePlayed(int id);
	static bool IsAudioStreamPlayed(Taudiofile MP3Stream, Taudiofile audiostreams);
	static bool PlayThrustersIDLE(int user, int index, Taudiofile *, int pedid, Ttraje suit, int id, int code, Tthrusters *jets);
	static void DrawSuitClickIconAtCoords(bool available, bool noicon, int id, CRect rect, float mouseposx, float mouseposy, bool *ishovered);
	static bool DrawClickIconAtCoords(bool bn, bool chang, bool setingcheck, CSprite2d *sprite, CSprite2d *spriteh, CRect rect, float mouseposx, float mouseposy);

	static void StopThrustersIDLE(Taudiofile *, Tthrusters *jets);
	static void storeenviroment(int *);
	//static void IronManTasks();
	static void PauseAllStreams();
	static void ResumeAllStreams();
	float newangle2();
	void IronManPatch();
	static bool has_not_switched_weapon(int wid);
	void Display_targets_on_screen(int wid, int storedtargets[], CSprite2d *target, CSprite2d *blocked);
	int *Neutralize_targets(int wid, int storedtargets[], vector<int>*rocket, bool *completed);
	int *Disable_targets(int storedtargets[]);
	int *Mark_targets(int wid, int current, int storedtargets[]);
	void aimcar(int cartype, float *x, float *y, float *z, float *x1, float *y1, float *z1);
	static void aim(float *x, float *y, float *z);
	static void AimedPedReaction(CVector posn, int *vict);
	static void AimedPedReactiontwo(int *vict);
	void coordstoscreen(float x, float y, float z, float *dx, float *dy, float *wid, float *hei);
	bool Va_adelante();
	bool Va_atras();
	bool Va_izquierda();
	bool Va_derecha();
	float newanglepos();
	float newangleposforflight();
	static bool is_on_foot();
	void setvelocityindirection(CPed *player, float velocx, float velocy, float velocz);
	float GetXAngle();
	bool has_obstacles(CPed *player, float offsetx, float offsety, float offsetz, int solid, int car, int actor, int object, int particle);
	void Set_XZ_cam_angle_to_Actor(CPed *player);
	void SetVelocityUnlimited(bool id);
	void writeProtected(int addr, uchar value, int size);// , HANDLE pHandle);
	static bool is_on_air_or_water();
	int activesuitwearing();
	bool is_wearing_suit(int bp);
	bool is_wearing_suit_and_not_driving(int bp);
	bool is_over_ground(float distance);
	bool is_aiming();
	bool nosemueve();
	bool nosemueve2();
	static bool processparachute();
	bool Has_not_switched_enviroment(Tthrusters *jets);
	static void create_thrusters(int pedid, Ttraje *suit, int code, Tthrusters *jets);
	static void kill_thrusters(Tthrusters *jets);
	//static void getthrusterslocation(CVector IDLErightfoot, CVector IDLEleftfoot, CVector IDLErighthand, CVector IDLElefthand, CVector FLYrightfoot, CVector FLYleftfoot, CVector FLYrighthand, CVector FLYlefthand);
	
	
};
extern JarvisVoice jarvisvoice;
static Tthrusters propellers;
extern bool was_player_damaged(float *damage, float *damage1);
extern int contador;
