#include "BASS_funcs.h"
#include "CDebug.h"

AUDIOLIB AudioLib;

void AUDIOLIB::Readfromdll() {
	library = LoadLibrary(GAME_PATH("maxirp.dll"));
	GetVersion = (TBASS_GetVersion)GetProcAddress(library, "BASS_GetVersion");
	//Start = (TBASS_Start)GetProcAddress(library, "BASS_Start");
	BassInit = (TBASS_Init)GetProcAddress(library, "BASS_Init");

	GetDeviceInfo = (TBASS_GetDeviceInfo)GetProcAddress(library, "BASS_GetDeviceInfo");
	GetInfo = (TBASS_GetInfo)GetProcAddress(library, "BASS_GetInfo");
	StreamFree = (TBASS_StreamFree)GetProcAddress(library, "BASS_StreamFree");

	StreamCreateFile = (TBASS_StreamCreateFile)GetProcAddress(library, "BASS_StreamCreateFile");
	ChannelPlay = (TBASS_ChannelPlay)GetProcAddress(library, "BASS_ChannelPlay");
	ChannelPause = (TBASS_ChannelPause)GetProcAddress(library, "BASS_ChannelPause");
	ChannelStop = (TBASS_ChannelStop)GetProcAddress(library, "BASS_ChannelStop");
	ChannelIsActive = (TBASS_ChannelIsActive)GetProcAddress(library, "BASS_ChannelIsActive");
	ChannelSet3DPosition = (TBASS_ChannelSet3DPosition)GetProcAddress(library, "BASS_ChannelSet3DPosition");
	Set3DPosition = (TBASS_Set3DPosition)GetProcAddress(library, "BASS_Set3DPosition");
	Set3DFactors = (TBASS_Set3DFactors)GetProcAddress(library, "BASS_Set3DFactors");
	ChannelSetAttribute = (TBASS_ChannelSetAttribute)GetProcAddress(library, "BASS_ChannelSetAttribute");
	ChannelSet3DAttributes = (TBASS_ChannelSet3DAttributes)GetProcAddress(library, "BASS_ChannelSet3DAttributes");
	Apply3D = (TBASS_Apply3D)GetProcAddress(library, "BASS_Apply3D");
	ErrorGetCode = (TBASS_ErrorGetCode)GetProcAddress(library, "BASS_ErrorGetCode");
}

void AUDIOLIB::EnumerateBassDevices(int& total, int& enabled, int& default_device)
{
	BASS_DEVICEINFO info;
	for (default_device = -1, enabled = 0, total = 0; AudioLib.GetDeviceInfo(total, &info); ++total)
	{
		if (info.flags & BASS_DEVICE_ENABLED) ++enabled;
		if (info.flags & BASS_DEVICE_DEFAULT) default_device = total;
		sprintf(gString, "Found sound device %d%s: %s", total, default_device == total ?
			" (default)" : "", info.name);
		Logear(gString);
	}
}

bool AUDIOLIB::Init()
{
	int default_device, total_devices, enabled_devices;
	BASS_DEVICEINFO info = { nullptr, nullptr, 0 };
	EnumerateBassDevices(total_devices, enabled_devices, default_device);
	if (forceDevice != -1 && AudioLib.GetDeviceInfo(forceDevice, &info) &&
		info.flags & BASS_DEVICE_ENABLED)
		default_device = forceDevice;

	sprintf(gString, "On system found %d devices, %d enabled devices, assuming device to use: %d (%s)",
		total_devices, enabled_devices, default_device, AudioLib.GetDeviceInfo(default_device, &info) ?
		info.name : "Unknown device");
	Logear(gString);

	BASS_3DVECTOR pos(0, 0, 0), vel(0, 0, 0), front(0, -1.0, 0), top(0, 0, 1.0);

	if (AudioLib.BassInit(default_device, 44100, BASS_DEVICE_3D | BASS_DEVICE_DEFAULT, 0, 0) &&
		AudioLib.Set3DFactors(1.0, 0.3, 1.0) &&
		AudioLib.Set3DPosition(&pos, &vel, &front, &top))
	{
		Logear("SoundSystem initialized");

		// Can we use floating-point (HQ) audio streams?

		BASS_INFO SoundDevice;
		// 
		if (AudioLib.GetInfo(&SoundDevice))
		{
			if (SoundDevice.flags & DSCAPS_EMULDRIVER)
				Logear("Audio drivers not installed - using DirectSound emulation");
			if (!SoundDevice.eax)
				Logear("Audio hardware acceleration disabled (no EAX)");
		}
		initialized = true;
		//this->hwnd = hwnd;
		AudioLib.Apply3D();
		return true;
	}
	Warning("Could not initialize BASS sound system");
	return false;
}
