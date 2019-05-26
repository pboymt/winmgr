#include <iostream>
#include <vector>
#include <Windows.h>
#include "utils.h"
#include "callbacks.h"

using namespace std;

BOOL CALLBACK EnumWindowsTitleProc(HWND hwnd, LPARAM lParam)
{
    if (isWindowDisabled(hwnd))
    {
        return TRUE;
    }

    vector<string> &titles =
        *reinterpret_cast<vector<string> *>(lParam);
    string title = getWindowTitle(hwnd);
    titles.push_back(title);

    return TRUE;
}

BOOL CALLBACK EnumWindowsPidProc(HWND hwnd, LPARAM lParam)
{

    if (isWindowDisabled(hwnd))
    {
        return TRUE;
    }

    std::vector<DWORD> &wininfos =
        *reinterpret_cast<std::vector<DWORD> *>(lParam);

    DWORD pid = getWindowPid(hwnd);
    wininfos.push_back(pid);

    return TRUE;
}

BOOL CALLBACK EnumWindowInfoProc(HWND hwnd, LPARAM lParam)
{
    if (isWindowDisabled(hwnd))
    {
        return TRUE;
    }
    MgrWindowInfo info;
    info.pid = getWindowPid(hwnd);
    info.title = getWindowTitle(hwnd);
    WINDOWINFO winfo;
    GetWindowInfo(hwnd, &winfo);
    info.rect = winfo.rcClient;
    std::vector<MgrWindowInfo> &infos =
        *reinterpret_cast<std::vector<MgrWindowInfo> *>(lParam);
    infos.push_back(info);

    return TRUE;
}

BOOL CALLBACK EnumWindowsFindByPidProc(HWND hwnd, LPARAM lParam)
{

    if (isWindowDisabled(hwnd))
    {
        return TRUE;
    }

    MgrWindowFinder &finder = *reinterpret_cast<MgrWindowFinder *>(lParam);

    DWORD pid = getWindowPid(hwnd);

    if (pid == finder.pid)
    {
        finder.hwnd = hwnd;
        return FALSE;
    }

    return TRUE;
}