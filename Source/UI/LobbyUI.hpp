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
// File: LobbyUI.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines LobbyUI class.
// ========================================================================= //

#ifndef __LOBBYUI_HPP__
#define __LOBBYUI_HPP__

// ========================================================================= //

#include "UI.hpp"

// ========================================================================= //

class LobbyUI : public UI
{
public:
    // Default initializes member data.
    explicit LobbyUI(void);

    // Empty destructor.
    virtual ~LobbyUI(void) override;

    // Creates layouts for lobby.
    virtual void init(void) override;

    // Destroys layouts for lobby.
    virtual void destroy(void) override;

    // Steps CEGUI system.
    virtual bool update(void) override;

    // Event handlers:

    bool root_SendPressed(const CEGUI::EventArgs& e);
    bool root_StartPressed(const CEGUI::EventArgs& e);

    enum Layer{
        Root = 0,

        NumLayers
    };

    enum Event{
        Exit = 1,
        Chat,
        Start
    };
};

// ========================================================================= //

#endif

// ========================================================================= //