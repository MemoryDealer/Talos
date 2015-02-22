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
// File: StartupState.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements StartupState class.
// ========================================================================= //

#include "Core/EngineNotifications.hpp"
#include "Core/EngineState/EngineStateID.hpp"
#include "Core/Resources.hpp"
#include "StartupState.hpp"

// ========================================================================= //

StartupState::StartupState(void) :
m_loaded(false)
{

}

// ========================================================================= //

StartupState::~StartupState(void)
{

}

// ========================================================================= //

void StartupState::enter(void)
{
    // Create thread for loading engine resources.
    std::thread t(&StartupState::loadResources, this);
    t.detach();
}

// ========================================================================= //

void StartupState::exit(void)
{

}

// ========================================================================= //

void StartupState::update(void)
{
    if (m_active == true){

        // Render something simple...
        // ...

        if (m_loaded == true){
            printf("Startup done...\n");
            // Done loading, notify engine to start main menu.
            m_subject.notify(EngineNotification::PopAndPush, 
                             EngineStateID::MainMenu);
            printf("Notified engine\n");
        }
    }
}

// ========================================================================= //

void StartupState::loadResources(void)
{
    // Load resources for Ogre (from Resources.hpp).
    printf("Loading Ogre resources...\n");
    loadOgreResources();
    printf("Loading meshes...\n");
    loadMeshes();
    printf("Loading CEGUI resources...\n");
    loadCEGUIResources();
    printf("Done loading...\n");

    m_loaded = true;
    printf("loaded = true\n");
}

// ========================================================================= //