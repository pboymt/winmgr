#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <Windows.h>

using namespace std;


struct MgrWindowInfo
{
    DWORD pid;
    string title;
    RECT rect;
};

struct MgrWindowFinder
{
    DWORD pid;
    HWND hwnd;
};

// wstring 转码到 UTF-8
string wstringToUtf8(const wstring &str);

// 获取窗口标题
string getWindowTitle(HWND hwnd);

// 获取窗口PID
DWORD getWindowPid(HWND hwnd);

/** 
 * 获取窗口是否可用的状态。
 * 
 * 有一些Bug，能够获取到一些非活跃的UWP窗口，不知道如何解决。
 */
bool isWindowDisabled(HWND hwnd);

#endif