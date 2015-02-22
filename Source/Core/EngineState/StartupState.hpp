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
// File: StartupState.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines StartupState class.
// ========================================================================= //

#ifndef __STARTUPSTATE_HPP__
#define __STARTUPSTATE_HPP__

// ========================================================================= //

#include "EngineState.hpp"

// ========================================================================= //

class StartupState final : public EngineState
{
public:
    // Calls EngineState constructor.
    explicit StartupState(void);

    // Empty destructor.
    virtual ~StartupState(void) override;

    // Set up basic stuff.
    virtual void enter(void) override;

    // Free basic stuff.
    virtual void exit(void) override;

    // Test.
    virtual void update(void) override;

    // Thread to load engine resources.
    void loadResources(void);

private:
    bool m_loaded;
};

// ========================================================================= //

#endif

// ========================================================================= //