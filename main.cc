#include <iostream>
#include <napi.h>
#include <codecvt>
#include "Windows.h"

using namespace std;

struct MgrWindowInfo
{
    DWORD pid;
    string title;
};

struct MgrWindowFinder
{
    DWORD pid;
    HWND hwnd;
};

// wstring 转码到 UTF-8
string wstringToUtf8(const wstring &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> strCnv;
    return strCnv.to_bytes(str);
}

// 获取窗口标题
string getWindowTitle(HWND hwnd)
{
    const int allowLength = 1024;
    wchar_t titleRaw[allowLength];

    GetWindowTextW(hwnd, titleRaw, allowLength);

    wstring title(&titleRaw[0]);

    return wstringToUtf8(title);
}

// 获取窗口PID
DWORD getWindowPid(HWND hwnd)
{
    DWORD pid = 0;

    GetWindowThreadProcessId(hwnd, &pid);

    return pid;
}

/** 
 * 获取窗口是否可用的状态。
 * 
 * 有一些Bug，能够获取到一些非活跃的UWP窗口，不知道如何解决。
 */
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

// 示例方法，没用
Napi::String Method(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Napi::String::New(env, "Hello.");
}

// 获取窗口标题列表

/**
 * 获取窗口标题的遍历程序
 */
BOOL CALLBACK EnumWindowsTitleProc(HWND hwnd, LPARAM lParam)
{
    if (isWindowDisabled(hwnd))
    {
        return TRUE;
    }

    std::vector<string> &titles =
        *reinterpret_cast<std::vector<string> *>(lParam);
    string title = getWindowTitle(hwnd);
    titles.push_back(title);

    return TRUE;
}

/**
 * 获取窗口标题列表
 */
Napi::Array MgrGetWindowTitleList(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    std::vector<string> titles;
    EnumWindows(EnumWindowsTitleProc, reinterpret_cast<LPARAM>(&titles));

    Napi::Array arr = Napi::Array::New(env);
    uint32_t index = 0;

    for (const auto &title : titles)
    {
        Napi::String str = Napi::String::New(env, title);

        arr.Set(index, str);
        index++;
    }

    return arr;
}

// 获取窗口进程id列表

/**
 * 获取窗口pid的遍历程序
 */
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

/**
 * 获取窗口pid列表
 */
Napi::Object MgrGetWindowPidList(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::vector<DWORD> pids;

    EnumWindows(EnumWindowsPidProc, reinterpret_cast<LPARAM>(&pids));

    Napi::Array arr = Napi::Array::New(env);
    uint32_t index = 0;

    for (const auto pid : pids)
    {
        Napi::Number num = Napi::Number::New(env, pid);

        arr.Set(index, num);
        index++;
    }

    return arr;
}

// 获取窗口详情列表

/**
 * 获取窗口详情的遍历程序
 */
BOOL CALLBACK EnumWindowInfoProc(HWND hwnd, LPARAM lParam)
{
    if (isWindowDisabled(hwnd))
    {
        return TRUE;
    }
    MgrWindowInfo info;
    info.pid = getWindowPid(hwnd);
    info.title = getWindowTitle(hwnd);
    std::vector<MgrWindowInfo> &infos =
        *reinterpret_cast<std::vector<MgrWindowInfo> *>(lParam);
    infos.push_back(info);

    return TRUE;
}

/**
 * 获取窗口详情列表
 */
Napi::Array MgrGetWindowInfoList(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    std::vector<MgrWindowInfo> wininfos;

    EnumWindows(EnumWindowInfoProc, reinterpret_cast<LPARAM>(&wininfos));

    Napi::Array arr = Napi::Array::New(env);
    uint32_t index = 0;

    for (const auto wininfo : wininfos)
    {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set(Napi::String::New(env, "pid"), Napi::Number::New(env, wininfo.pid));
        obj.Set(Napi::String::New(env, "title"), Napi::String::New(env, wininfo.title));
        arr.Set(index, obj);
        index++;
    }

    return arr;
}

// 通过PID聚焦窗口

/**
 * 获取窗口句柄的遍历
 */
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

Napi::Value MgrFocusWindowByPid(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments Type").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    MgrWindowFinder finder;
    finder.hwnd = NULL;
    finder.pid = info[0].As<Napi::Number>().Uint32Value();

    EnumWindows(EnumWindowsFindByPidProc, reinterpret_cast<LPARAM>(&finder));

    if (finder.hwnd)
    {
        SwitchToThisWindow(finder.hwnd, true);
        return Napi::Boolean::New(env, true);
    }
    return Napi::Boolean::New(env, false);
}

Napi::Value MgrGetWindowRectByPid(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments Type").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    MgrWindowFinder finder;
    finder.hwnd = NULL;
    finder.pid = info[0].As<Napi::Number>().Uint32Value();

    EnumWindows(EnumWindowsFindByPidProc, reinterpret_cast<LPARAM>(&finder));

    if (finder.hwnd)
    {
        // SwitchToThisWindow(finder.hwnd, true);
        WINDOWINFO winfo;
        GetWindowInfo(finder.hwnd, &winfo);
        RECT rect = winfo.rcClient;
        // cout << "Bottom\t" << rect.bottom << endl;
        // cout << "Top\t" << rect.top << endl;
        // cout << "Left\t" << rect.left << endl;
        // cout << "Right\t" << rect.right << endl;
        Napi::Object obj = Napi::Object::New(env);
        obj.Set(Napi::String::New(env, "x"), Napi::Number::New(env, (double)rect.top));
        obj.Set(Napi::String::New(env, "y"), Napi::Number::New(env, (double)rect.left));
        obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, (double)(rect.right - rect.left)));
        obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, (double)(rect.bottom - rect.top)));

        return obj;
    }
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getTitleList"), Napi::Function::New(env, MgrGetWindowTitleList));
    exports.Set(Napi::String::New(env, "getProcessIdList"), Napi::Function::New(env, MgrGetWindowPidList));
    exports.Set(Napi::String::New(env, "getWindowInfoList"), Napi::Function::New(env, MgrGetWindowInfoList));
    exports.Set(Napi::String::New(env, "focusWindowByPid"), Napi::Function::New(env, MgrFocusWindowByPid));
    exports.Set(Napi::String::New(env, "getWindowRectByPid"), Napi::Function::New(env, MgrGetWindowRectByPid));
    return exports;
}

NODE_API_MODULE(winmgr, Init);