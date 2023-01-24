#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#pragma warning(push, 0)
#include <cocos2d.h>
#include <cocos-ext.h>
#pragma warning(pop)
#include <MinHook.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;
using std::cout, std::endl, std::string, std::vector;

// #define INDEV // comment out to remove console

#include "utils.h"

// include your files right here