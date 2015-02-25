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
// File: MainMenuUI.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements MainMenuUI class.
// ========================================================================= //

#include "MainMenuUI.hpp"

// ========================================================================= //

MainMenuUI::MainMenuUI(void)
{

}

// ========================================================================= //

MainMenuUI::~MainMenuUI(void)
{

}

// ========================================================================= //

void MainMenuUI::init(void)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

    m_layers[Layer::Root] = wmgr.loadLayoutFromFile("MainMenu/root.layout");
    m_layers[Layer::Root]->getChild("ButtonCampaign")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::root_CampaignPressed, this));
    m_layers[Layer::Root]->getChild("ButtonMultiplayer")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::root_MultiplayerPressed, this));
    m_layers[Layer::Root]->getChild("ButtonExit")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::root_ExitPressed, this));

    // Add each layer to root window.
    for (int i = 0; i < 1; ++i){
        CEGUI::System::getSingleton().getDefaultGUIContext().
            getRootWindow()->addChild(m_layers[i]);

        if (i > Layer::Root){
            m_layers[i]->setVisible(false);
        }
    }  
}

// ========================================================================= //

void MainMenuUI::destroy(void)
{

}

// ========================================================================= //

void MainMenuUI::update(void)
{
    CEGUI::System::getSingleton().
        injectTimePulse(1.f / 16.f);
}

// ========================================================================= //

// Event handlers:

// ========================================================================= //

bool MainMenuUI::root_CampaignPressed(const CEGUI::EventArgs& e)
{
    printf("Campaign pressed!");

    return true;
}

// ========================================================================= //

bool MainMenuUI::root_MultiplayerPressed(const CEGUI::EventArgs& e)
{

    return true;
}

// ========================================================================= //

bool MainMenuUI::root_ExitPressed(const CEGUI::EventArgs& e)
{

    return true;
}

// ========================================================================= //