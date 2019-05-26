#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <Windows.h>
#include <napi.h>

Napi::Number MgrScreenGetWidth(const Napi::CallbackInfo &info);

Napi::Number MgrScreenGetHeight(const Napi::CallbackInfo &info);

Napi::Object MgrScreenGetSize(const Napi::CallbackInfo &info);

// Napi::Number MgrScreenGetVWidth(const Napi::CallbackInfo &info);

// Napi::Number MgrScreenGetVHeight(const Napi::CallbackInfo &info);

#endif