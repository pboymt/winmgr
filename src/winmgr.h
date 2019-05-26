#ifndef _WINMGR_H_
#define _WINMGR_H_

#include <napi.h>

// 获取窗口标题列表
Napi::Array MgrGetWindowTitleList(const Napi::CallbackInfo &info);

// 获取窗口pid列表
Napi::Object MgrGetWindowPidList(const Napi::CallbackInfo &info);

// 获取窗口详情列表
Napi::Array MgrGetWindowInfoList(const Napi::CallbackInfo &info);

// 通过PID获取窗口详情
Napi::Value MgrGetWindowInfoByPid(const Napi::CallbackInfo &info);

// 通过PID聚焦窗口
Napi::Value MgrFocusWindowByPid(const Napi::CallbackInfo &info);

// 通过PID获取窗口位置与尺寸
Napi::Value MgrGetWindowRectByPid(const Napi::CallbackInfo &info);

#endif