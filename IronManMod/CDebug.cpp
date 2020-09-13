#include "stdafx.h"
#include "CDebug.h"
#include "shellapi.h"
#include "Winuser.h"

CDebug Debug;

void Error(const char *szStr)
{
    MessageBox(nullptr, szStr, "Iron-Man Mod error", MB_ICONERROR | MB_OK);
	LOGEAR("[Error] Exiting with error: %s", szStr);
    //exit(1);
}

void Logear(const char *szStr)
{
	LOGEAR(szStr);
}

void Warning(const char *szStr)
{
    MessageBox(nullptr, szStr, "Iron-Man Mod warning", MB_ICONWARNING | MB_OK);
	LOGEAR("[Warning] %s", szStr);
    //exit(1);
}

bool Welcome(const char *szStr)
{
	//MessageBox(nullptr, szStr, "Maxirp's Iron-Man Mod", MB_ICONINFORMATION | MB_OK);
	//exit(1);
	static int chius;
	static std::string chau;
	static HWND pointr;
	if (MessageBox(pointr, szStr, "Maxirp's Iron-Man Mod", MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL)
	{
		ShellExecute(0, 0, "http://www.patreon.com/Maxirp", 0, 0, SW_SHOW);
		exit(1);
		return false;
	}
	else
	{
		if (pointr == nullptr)
		{
			return true;
		}
	}
	return false;
}

void MaxirpLinks(const char *szStr)
{
	//if (MessageBox(nullptr, szStr, "Maxirp's Iron-Man Mod", MB_ICONQUESTION | MB_YESNO) == IDYES)
	
	if (MessageBox(nullptr, szStr, "Maxirp's Iron-Man Mod", MB_ICONQUESTION | MB_HELP) == WM_HELP)
	{
		ShellExecute(0, 0, "http://www.patreon.com/Maxirp", 0, 0, SW_SHOW);
		exit(1);
	}
}