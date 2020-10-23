# GTAIronMan
![GitHub Logo](https://media.moddb.com/images/mods/1/32/31542/iron_man3.1.png)
GTA IronMan is a modification for the game Grand theft auto: San Andreas that changes the game in the style of that superhero. 
Written with plugin sdk compatibility, in cplusplus, this ".dll" renamed to ".asi" features different changes to the game

In order to install the mod, you must download an installer of the pack,
disable your antivirus before installing
run it as administrator
select your gta folder during installation to apply the mod

Link of installer: https://www.moddb.com/mods/gta-iron-man-mod/downloads/gta-iron-man-mod-stable-build-v23-setup

- **Changelog:**

- **IronManMod**
File that loads most of the dependencies
- **Main**
File that replaces just the font file
- **MobileAreaName**
It just disables eventual game area show on screen
- **MobileColors**
Changes the red markers with light blue ones
- **MobileMenuSystem**
Unimplemented. File that changes the menu system of gta sa pc with the style of mobile. Causes bugs so you were warned
- **AreaAroundRadar**
File that always shows the clock and place data around the radar
Shows altimeter and compass when armored
- **Bassfuncs:**
Sound system using the bass library
- Script loads the library maxirp.dll to make the sounds available for the game
- It loads all the functions of maxirp.dll so the game can manage the sound
- **MobilePlayerInfo:**
Animated interface of Jarvis that changes when changing outfit
- **MobileRadar**
File that adds an animated radar of Jarvis when armored
Replaces the direction arrow of radar and the radardisk
Adds customized textures with animations
Animation will stop everytime player health changes
- **MobileRadioName**
File that changes the position of radio name in the game
- **MobileVehicleName**
File that changes the position of vehicle name in the game
- **MobileSubtitles**
File that changes the font of the subtitles
changes the parachute lines of activation
- **MobileTextures**
File that loads the textures of the menu and the loading screens
- **MobileProgressBar**
File that draws a large loading bar
- **MobileLoadingScreens**
Iron Man loading screens
Replacement of font files with new files through the script
- **GPS**
File that draws a GPS System
Obtained from dk22pac code
- **MobileStatsBox**
Stats box in new location
- **Settings**
File that loads config files to change some of the values in the code, the interface, radar positions and more
- **MovTextures**
File that does the texture drawing of most of the script
- **Shapehelper**
File that draws customized text boxes with different sizes, fonts, colors and textures
- **JarvisVoice**
This file name is tricky, it does load the jarvis voice and the rest of the audio, but has more than that...
Jarvis voice alerts, alarms, warnings and more
Updates the audio panning/mp3 location when the camera and skins change their position
Implements animations to stand, walk, run, sprint based on the Tony Stark pose
Implements ingame menu that:
- loads png icons from the folder of a skin (path: ROOT/modloader/skins/category/skinfolder)
- loads iron man skins to wear (lmb)
- loads iron man skins to spawn (rmb)
- loads config menu to activate/deactivate settings
- type cheat 'IRON' to display (may experience bugs because of the beta status of the project)
Loads Jarvis bots/sentries/Iron Legion)
- they are created when right clicking the suits in the menu
- they have 2 weapons: repulsors and rockets
- they do not attack the player, but they do attack bots and cops when wanted
- they only way to delete bots is killing them
- they may experience bugs because of the beta status of the project
Loads Ultron bots:
- they are created when reaching wanted stars
- they have 2 weapons: repulsors and rockets
- they attack playey and sentries
- they are deleted by being killed or arriving to the ls police station when unwanted
- they may experience bugs because of the beta status of the project
Implements super destructive punches on simple clics
- punching peds makes the head explode and ped fly away
- punching cars makes the car fly away
- punching objects makes it break
Implements aiming iron man weapons and firing with all mouse buttons
1 fists to fly to target and punch
2 repulsor hand
3 explosive heatseek rockets for vehicles
4 unibeam
5 darts as projectiles for peds heads (heatseek guided)
- 'right clic' or aim key: aim iron man weapon
- 'left clic' to fire: shoot or attack
- aim key + jump key: hold shield instead of weapon
- scrolling weapons key to switch between Iron Man weapons and also change shield skin
Implements flying system with the movement controls:
- 'up': fly frontward
- 'down': fly backward
- 'left': fly left
- 'right': fly right
- crouch key: fly down
- jump key: fly up
- sprint key: fly forward very fast
- 'x': impulse from ground
Creates and destroys flying thrusters:
- they are created once
- they are distroyed when switching movement or stopping flight
- they have two effects, one for air (thrusters.fxp), one for underwater (thruster_under.fxp)
Performs lifting, holding and throwing vehicles with keys:
- key 'E' to lift it
- movement keys to walk or fly while holding it
- clic to throw vehicle
Removes the armor when being killed or arrested
Makes the unused parachute invisible
Swiches status with indexes:
0: ciclobasico (in spanish means basic cycle)
- checks if the player is armored, driving, aiming, over ground, jumping, on foot, pressing 'e' in front of a car (to lift it), pressing 'x' (to impulse), or position frozen
7: levantarauto (in spanish means lift car)
- makes the char perform the car lift anim
8: origcareliminate (delete original car)
- deletes the car to lift (but stores the model and color to create a new similar one)
9: loadmodels
- loads the vehicle that will be created as a lifted car
10: createandlift
- creates the car to be lifted
- creates a helper invisible object. It will be used as the 'glue' between the car and the player hands
- attaches the car to the helper
11: animate
- makes the player play the 'holdcar' animation
12: attachnewcar
- attaches the player to the helper
- makes the camera go away
13: throwcar
- while not clicking, you can aim wherever you want to throw it
- hit the clic to make the car detach and fly to target
- hit the get inside vehicle ('f' or 'intro') key to drop the car
- hit the sprint key to drop vehicle too
14: animthrowcar
- makes the animation of throwing car
- gives the vehicle velocity
15: carfly
- makes the car fly away and disappear once you stopped throwing it
16: abort_throw_car
- makes the car detach and applies world collision to it
17: unload_car
- deletes the vehicle
18: volarbajo
- has controls for the player to fly close to the ground
19: controlesdevuelo
- makes the player control the air/water flight
- changes the animation
- changes the sound effect
- changes the thruster effect/rotation
20: make_a_clank
- makes the player hit the ground with animation when descending
21: impulsarse
- makes the player impulse from the ground when holding the 'x' key
22: aimin
- makes the player aim weapons
- creates a shield when pressing rmb+jump key
23: volarsolo
- makes the player fly up without keypressing according to the speed of the impulse chosen

- _Important_
to make the script wait some time i use:
boolvars.waiter = CTimer::m_snTimeInMillisecondsNonClipped;
boolvars.timetowait = 200; //for example 200 milliseconds = Wait(200) in cleo scripting

- **Known Issues**
- Bugs in the suit menu that may cause the game crash
- Bugs in the ultron attack system that makes the game freeze and then continue
- Bugs in the superpowers when getting killed, sometimes they just don't work until you restart the game
- Bugs in the armor bar that makes it look like a full circle
- Bugs when reading the ini files that uses much memory and may cause the game crash too
- Bugs, bugs everywhere because this is just a beta
