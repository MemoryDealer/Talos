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
// File: MainMenuUI.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines MainMenuUI class.
// ========================================================================= //

#ifndef __MAINMENUUI_HPP__
#define __MAINMENUUI_HPP__

// ========================================================================= //

#include "UI.hpp"

// ========================================================================= //
// UI handler for MainMenuState.
class MainMenuUI : public UI
{
public:
    // Default initializes member data.
    explicit MainMenuUI(void);

    // Empty destructor.
    virtual ~MainMenuUI(void) override;

    // Creates layouts for main menu.
    virtual void init(void) override;

    // Destroys layouts for main menu.
    virtual void destroy(void) override;

    // Steps CEGUI system.
    virtual bool update(void) override;

    // Event handlers:

    bool root_CampaignPressed(const CEGUI::EventArgs& e);
    bool root_MultiplayerPressed(const CEGUI::EventArgs& e);
    bool root_ExitPressed(const CEGUI::EventArgs& e);

    bool multiplayer_HostPressed(const CEGUI::EventArgs& e);

    bool host_HostPressed(const CEGUI::EventArgs& e);

    bool BackPressed(const CEGUI::EventArgs& e);

    // Window layers.
    enum Layer{
        Root = 0,
            Campaign,
            Multiplayer,
                Host,
                Join,
            Options,

        NumLayers
    };

    enum Event{
        Exit = 1,
        HostGame
    };

private:
   
};

// ========================================================================= //

#endif

// ========================================================================= //