#include <iostream>
#include <napi.h>
#include <codecvt>
#include "Windows.h"

using namespace std;

std::string wstringToUtf8(const std::wstring &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> strCnv;
    return strCnv.to_bytes(str);
}

Napi::String Method(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Napi::String::New(env, "Hello.");
}

BOOL CALLBACK EnumWindowsTitleProc(HWND hwnd, LPARAM lParam)
{

    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    wstring title(&windowTitle[0]);

    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager")
    {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::wstring> &titles =
        *reinterpret_cast<std::vector<std::wstring> *>(lParam);
    titles.push_back(title);

    return TRUE;
}

Napi::Array MgrGetWindowTitleList(const Napi::CallbackInfo &info)
{
    // locale loc("chs");
    // wcout.imbue(loc);

    Napi::Env env = info.Env();

    std::vector<std::wstring> titles;
    EnumWindows(EnumWindowsTitleProc, reinterpret_cast<LPARAM>(&titles));
    
    // At this point, titles if fully populated and could be displayed, e.g.:
    
    // int size = titles.size();
    Napi::Array arr = Napi::Array::New(env);
    uint32_t index = 0;
    // for (size_t i = 0; i < size; i++)
    // {
    //     Napi::String str = Napi::String::New(env, wstringToUtf8(titles[i]));
    //     arr.Set(i, str);
    // }

    for (const auto &title : titles)
    {
        // wcout << title << endl;
        Napi::String str = Napi::String::New(env, wstringToUtf8(title));

        arr.Set(index, str);
        index++;
    }
    
    return arr;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    // exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Method));
    exports.Set(Napi::String::New(env, "getTitleList"), Napi::Function::New(env, MgrGetWindowTitleList));
    return exports;
}

NODE_API_MODULE(winmgr, Init);