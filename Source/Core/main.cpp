// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: main.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements entry point of program.
// ========================================================================= //

#include "Engine.hpp"

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// ========================================================================= //
// Entry point.
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR args, INT)
#else
int main(int argc, char** argv)
#endif
{
	Engine engine;

	try{
		if (engine.init() == false){
			return 1;
		}

		// Start the engine!
		engine.start(0);
	}
	catch (std::exception& e){
		// Report any exceptions.
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(GetForegroundWindow(), e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#else
		fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
	}

	return 0;
}

// ========================================================================= //