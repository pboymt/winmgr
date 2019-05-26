#include "screen.h"

Napi::Number MgrScreenGetWidth(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    int width = GetSystemMetrics(SM_CXSCREEN);

    return Napi::Number::New(env, (double)width);
}

Napi::Number MgrScreenGetHeight(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    int width = GetSystemMetrics(SM_CYSCREEN);

    return Napi::Number::New(env, (double)width);
}

Napi::Object MgrScreenGetSize(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::Object obj = Napi::Object::New(env);

    obj.Set("width", Napi::Number::New(env, (double)GetSystemMetrics(SM_CXSCREEN)));
    obj.Set("height", Napi::Number::New(env, (double)GetSystemMetrics(SM_CYSCREEN)));

    return obj;
}

// Napi::Number MgrScreenGetVWidth(const Napi::CallbackInfo &info)
// {
//     Napi::Env env = info.Env();

//     int width = GetSystemMetrics(SM_CXFULLSCREEN);

//     return Napi::Number::New(env, (double)width);
// }

// Napi::Number MgrScreenGetVHeight(const Napi::CallbackInfo &info)
// {
//     Napi::Env env = info.Env();

//     int width = GetSystemMetrics(SM_CYFULLSCREEN);

//     return Napi::Number::New(env, (double)width);
// }