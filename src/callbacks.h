#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include <iostream>
#include <Windows.h>

using namespace std;

// 获取窗口标题的遍历程序
BOOL CALLBACK EnumWindowsTitleProc(HWND hwnd, LPARAM lParam);

// 获取窗口pid的遍历程序
BOOL CALLBACK EnumWindowsPidProc(HWND hwnd, LPARAM lParam);

// 获取窗口详情的遍历程序
BOOL CALLBACK EnumWindowInfoProc(HWND hwnd, LPARAM lParam);

// 获取窗口句柄的遍历
BOOL CALLBACK EnumWindowsFindByPidProc(HWND hwnd, LPARAM lParam);

#endif