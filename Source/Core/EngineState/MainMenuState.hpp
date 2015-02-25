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
// File: MainMenuState.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines MainMenuState class.
// ========================================================================= //

#ifndef __MAINMENUSTATE_HPP__
#define __MAINMENUSTATE_HPP__

// ========================================================================= //

#include "EngineState.hpp"

// ========================================================================= //
// Main menu of engine, provides access to other engine states.
class MainMenuState : public EngineState
{
public:
    // Empty constructor.
    explicit MainMenuState(void);

    // Empty destructor.
    virtual ~MainMenuState(void) override;

    // Creates world, UI.
    virtual void enter(void) override;

    // Destroys world.
    virtual void exit(void) override;

    // Processes UI interaction.
    virtual void update(void) override;
};

// ========================================================================= //

#endif

// ========================================================================= //