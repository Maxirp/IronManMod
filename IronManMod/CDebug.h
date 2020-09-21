#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <plugin.h>

using namespace plugin;

#define LOGEAR __noop

#ifdef DEBUGIT
#undef LOGEAR
#define LOGEAR(a,...) {Debug.Trace(a, __VA_ARGS__);}
#endif

class CDebug
{
#ifdef DEBUGIT
    std::ofstream m_hFile;
#endif

public:
#ifdef DEBUGIT

    CDebug() : m_hFile(PLUGIN_PATH("IronMan.log"))
    {
        Trace("Log started.");
    }

    ~CDebug()
    {
        Trace("Log finished.");
    }

    void Trace(const char *format, ...)
    {
        SYSTEMTIME			t;
        static char			szBuf[1024];

        GetLocalTime(&t);
        sprintf(szBuf, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        va_list arg;
        va_start(arg, format);
        vsprintf(szBuf + strlen(szBuf), format, arg);
        va_end(arg);
        m_hFile << szBuf << std::endl;
        OutputDebugString(szBuf);
        OutputDebugString("\n");
    }
#endif
};

extern CDebug Debug;
void Logear(const char *);
void Warning(const char *);
void Error(const char *);
bool ErrorMax(const char *);
bool Welcome(const char *);
void MaxirpLinks(const char *);
