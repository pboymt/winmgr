#include <iostream>
#include <codecvt>
#include <Windows.h>
#include "utils.h"

using namespace std;

string wstringToUtf8(const wstring &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> strCnv;
    return strCnv.to_bytes(str);
}

string getWindowTitle(HWND hwnd)
{
    const int allowLength = 1024;
    wchar_t titleRaw[allowLength];

    GetWindowTextW(hwnd, titleRaw, allowLength);

    wstring title(&titleRaw[0]);

    return wstringToUtf8(title);
}

DWORD getWindowPid(HWND hwnd)
{
    DWORD pid = 0;

    GetWindowThreadProcessId(hwnd, &pid);

    return pid;
}

bool isWindowDisabled(HWND hwnd)
{
    int length = ::GetWindowTextLengthW(hwnd);
    string title = getWindowTitle(hwnd);
    string str("Program Manager");
    if (IsWindowEnabled(hwnd) && IsWindowVisible(hwnd) && length > 0 && title != str)
    {
        return false;
    }
    return true;
}