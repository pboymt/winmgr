#include <iostream>
#include <napi.h>
#include <codecvt>
#include <Windows.h>
#include "utils.h"
#include "callbacks.h"
#include "winmgr.h"
#include "screen.h"

using namespace std;

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "getTitleList"), Napi::Function::New(env, MgrGetWindowTitleList));
    exports.Set(Napi::String::New(env, "getProcessIdList"), Napi::Function::New(env, MgrGetWindowPidList));
    exports.Set(Napi::String::New(env, "getWindowInfoList"), Napi::Function::New(env, MgrGetWindowInfoList));
    exports.Set(Napi::String::New(env, "getWindowInfoByPid"), Napi::Function::New(env, MgrGetWindowInfoByPid));
    exports.Set(Napi::String::New(env, "focusWindowByPid"), Napi::Function::New(env, MgrFocusWindowByPid));
    exports.Set(Napi::String::New(env, "getWindowRectByPid"), Napi::Function::New(env, MgrGetWindowRectByPid));
    exports.Set(Napi::String::New(env, "getScreenWidth"), Napi::Function::New(env, MgrScreenGetWidth));
    exports.Set(Napi::String::New(env, "getScreenHeight"), Napi::Function::New(env, MgrScreenGetHeight));
    // exports.Set(Napi::String::New(env, "getScreenVWidth"), Napi::Function::New(env, MgrScreenGetVWidth));
    // exports.Set(Napi::String::New(env, "getScreenVHeight"), Napi::Function::New(env, MgrScreenGetVHeight));
    exports.Set(Napi::String::New(env, "getScreenSize"), Napi::Function::New(env, MgrScreenGetSize));
    return exports;
}

NODE_API_MODULE(winmgr, Init);