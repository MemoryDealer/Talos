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
    m_layers.resize(Layer::NumLayers);
}

// ========================================================================= //

MainMenuUI::~MainMenuUI(void)
{

}

// ========================================================================= //

void MainMenuUI::init(void)
{
    // Load each layer from layout files.
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

    // Root layer.
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

    // Campaign layer.
    m_layers[Layer::Campaign] = wmgr.loadLayoutFromFile(
        "MainMenu/campaign.layout");
    m_layers[Layer::Campaign]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Multiplayer layer.
    m_layers[Layer::Multiplayer] = wmgr.loadLayoutFromFile(
        "MainMenu/multiplayer.layout");
    m_layers[Layer::Multiplayer]->getChild("ButtonHost")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::multiplayer_HostPressed, this));
    m_layers[Layer::Multiplayer]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Host layer.
    m_layers[Layer::Host] = wmgr.loadLayoutFromFile("MainMenu/host.layout");
    m_layers[Layer::Host]->getChild("ButtonHost")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::host_HostPressed, this));
    m_layers[Layer::Host]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Join layer.
    m_layers[Layer::Join] = wmgr.loadLayoutFromFile("MainMenu/join.layout");
    m_layers[Layer::Join]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Options layer.
    m_layers[Layer::Options] = wmgr.loadLayoutFromFile("MainMenu/options.layout");
    m_layers[Layer::Options]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));


    // Add each layer to root window.
    for (int i = 0; i < Layer::NumLayers; ++i){
        CEGUI::System::getSingleton().getDefaultGUIContext().
            getRootWindow()->addChild(m_layers[i]);
             m_layers[i]->setVisible(false);
    }  

    // Push first layer onto stack.
    this->pushLayer(Layer::Root);
}

// ========================================================================= //

void MainMenuUI::destroy(void)
{
    
}

// ========================================================================= //

bool MainMenuUI::update(void)
{
    CEGUI::System::getSingleton().
        injectTimePulse(1.f / 16.f);

    return (m_events.empty() == false);
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
    this->pushLayer(Layer::Multiplayer);
    return true;
}

// ========================================================================= //

bool MainMenuUI::root_ExitPressed(const CEGUI::EventArgs& e)
{
    m_events.push(Event::Exit);
    return true;
}

// ========================================================================= //

bool MainMenuUI::multiplayer_HostPressed(const CEGUI::EventArgs& e)
{
    this->pushLayer(Layer::Host);
    return true;
}

// ========================================================================= //

bool MainMenuUI::host_HostPressed(const CEGUI::EventArgs& e)
{
    m_events.push(Event::HostGame);
    return true;
}

// ========================================================================= //

bool MainMenuUI::BackPressed(const CEGUI::EventArgs& e)
{
    this->popLayer();

    return true;
}

// ========================================================================= //