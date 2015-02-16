// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// ========================================================================= //
// File: main.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements entry point of program.
// ========================================================================= //

#include "Engine.hpp"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// ========================================================================= //
// Entry point.
#ifdef _DEBUG
int main(int argc, char** argv)
#else
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR args, INT)
#endif
#else
int main(int argc, char** argv)
#endif
{
#ifndef _DEBUG
    // Allocate debug console for testing in release mode.
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitle("Talos Engine Release Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
#endif

    Engine engine;

    try{
        if (engine.init() == false){
            return 1;
        }

        // Start the engine!
        engine.start(Engine::StateID::STATE_INTRO);
    }
    catch (std::exception& e){
        // Report any exceptions.
#ifdef WIN32
        MessageBox(GetForegroundWindow(), e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#else
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
    }

    return 0;
}

// ========================================================================= //