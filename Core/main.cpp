// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //

#include <Ogre.h>

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// ========================================================================= //

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR args, INT)
#else
int main(int argc, char** argv)
#endif
{

	return 0;
}

// ========================================================================= //