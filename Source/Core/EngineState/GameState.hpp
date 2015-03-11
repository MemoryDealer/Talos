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
// File: GameState.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines GameState class.
// ========================================================================= //

#ifndef __GAMESTATE_HPP__
#define __GAMESTATE_HPP__

// ========================================================================= //

#include "EngineState.hpp"

// ========================================================================= //
// Gameplay, processes player-world interaction, multiplayer, etc.
class GameState : public EngineState
{
public:
    explicit GameState(void);

    virtual ~GameState(void) override;

    // Creates world, UI.
    virtual void enter(void) override;

    // Destroys world.
    virtual void exit(void) override;

    // Hides UI.
    virtual void pause(void) override;

    // Shows UI.
    virtual void resume(void) override;

    // Processes player/UI interaction.
    virtual void update(void) override;

    // Processes network events.
    void handleNetEvents(void);

    // Processes UI events.
    void handleUIEvents(void);

    // Creates new Entities for players in Network.
    void addNetworkPlayers(void);
};

// ========================================================================= //

#endif

// ========================================================================= //