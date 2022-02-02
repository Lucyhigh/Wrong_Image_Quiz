#pragma once

#define WIN32_LEAN_AND_MEAN

#include<sdkddkver.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

#include <tchar.h>
#include <time.h>

#include <iostream>
#include <random>
#include <vector>
#include <map>
#include<bitset> 
#include <cassert>
using namespace std;

#define RND RandomFunction::getSingleton()
#define KEYMANAGER KeyManager::getSingleton()

#include "CommonMacroFunction.h"
#include "RandomFunction.h"
#include "KeyManager.h"

#define WINNAME				(LPSTR)(TEXT("WindowsAPI"))
#define WINSTART_X			400	
#define WINSTART_Y			100
#define WINSIZE_X			800
#define WINSIZE_Y			800
#define WINSTYLE			WS_CAPTION | WS_SYSMENU


#define SAFE_DELETE(p)				{if(p){delete (p); (p) = nullptr;}}	
#define SAFE_DELETE_ARRAY(p)		{if(p){delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)				{if(p){(p)->release(); (p) = nullptr;}}

extern HINSTANCE _hInstance;
extern HWND _hWnd;			
extern POINT _ptMouse;		
