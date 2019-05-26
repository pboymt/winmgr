#include <iostream>
#include "utils.h"
#include "callbacks.h"
#include "winmgr.h"

using namespace std;

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
        // Napi::Object obj = Napi::Object::New(env);
        RECT rect = wininfo.rect;
        obj.Set(Napi::String::New(env, "x"), Napi::Number::New(env, (double)rect.top));
        obj.Set(Napi::String::New(env, "y"), Napi::Number::New(env, (double)rect.left));
        obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, (double)(rect.right - rect.left)));
        obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, (double)(rect.bottom - rect.top)));

        arr.Set(index, obj);
        index++;
    }

    return arr;
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

    if (info[0].ToNumber().Uint32Value() > 65535)
    {
        Napi::TypeError::New(env, "Out of Range").ThrowAsJavaScriptException();
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

    if (info[0].ToNumber().Uint32Value() > 65535)
    {
        Napi::TypeError::New(env, "Out of Range").ThrowAsJavaScriptException();
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

Napi::Value MgrGetWindowInfoByPid(const Napi::CallbackInfo &info)
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

    if (info[0].ToNumber().Uint32Value() > 65535)
    {
        Napi::TypeError::New(env, "Out of Range").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    MgrWindowFinder finder;
    finder.hwnd = NULL;
    finder.pid = info[0].As<Napi::Number>().Uint32Value();

    EnumWindows(EnumWindowsFindByPidProc, reinterpret_cast<LPARAM>(&finder));

    if (finder.hwnd)
    {
        Napi::Object obj = Napi::Object::New(env);

        obj.Set(Napi::String::New(env, "pid"), Napi::Number::New(env, getWindowPid(finder.hwnd)));
        obj.Set(Napi::String::New(env, "title"), Napi::String::New(env, getWindowTitle(finder.hwnd)));

        WINDOWINFO winfo;
        GetWindowInfo(finder.hwnd, &winfo);
        RECT rect = winfo.rcClient;

        obj.Set(Napi::String::New(env, "x"), Napi::Number::New(env, (double)rect.top));
        obj.Set(Napi::String::New(env, "y"), Napi::Number::New(env, (double)rect.left));
        obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, (double)(rect.right - rect.left)));
        obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, (double)(rect.bottom - rect.top)));

        return obj;
    }
    return env.Undefined();
}