#pragma once

#ifdef __dll__
#define IMPEXP __declspec(dllexport)
#else
#define IMPEXP __declspec(dllimport)
#endif 	// __dll__
#include "framework.h"
#include "MapLibrary.h"

extern "C" IMPEXP void* CreateMapClassInstance();